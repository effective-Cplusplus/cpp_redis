#include <boost/asio.hpp>
#include <iostream>
#include <tuple>
#include <atomic>
#include <mutex>
#include "traits.hpp"
#include "redis_unit.hpp"
#include "cpp_redis_response.hpp"

namespace cpp_redis {
	class cpp_redis_net
	{
	public:
		cpp_redis_net()
		{
			try
			{
				ios_ = cpp_redis::traits::make_unique<boost::asio::io_service>();
				socket_ = cpp_redis::traits::make_unique<boost::asio::ip::tcp::socket>(*ios_);
				timer_ = cpp_redis::traits::make_unique<boost::asio::steady_timer>(*ios_);
				response_ = std::make_shared<cpp_redis_response>();
			}
			catch (const std::exception & ex)
			{
				std::cout << ex.what() << std::endl;
				ios_ = nullptr;
				socket_ = nullptr;
				response_ = nullptr;
			}
		}

		~cpp_redis_net() {
			close();

			ios_->stop();
		}

		/********ip,port,password,db_num********************/
		template<typename...Args>
		bool connect_to(std::string&& ip, Args&&...args)
		{
			if (socket_ == nullptr || ios_ == nullptr) {
				return false;
			}

			constexpr auto size = sizeof...(args);
			if (size > 3) {
				return false;
			}

			if (!cpp_redis::unit::ip_v6_check(ip) && !cpp_redis::unit::ip_addr_check(ip)) {
				return false;
			}

			host_ = std::move(ip);
			auto tp = std::make_tuple<Args...>(std::forward<Args>(args)...);
#if (_MSC_VER >=1500 && _MSC_VER<=1900)
			if (constexpr (sizeof...(args) == 1)) {
				port_ = std::move(std::get<0>(tp));
			}
			else if (constexpr (sizeof...(args) == 2)) {
				port_ = std::move(std::get<0>(tp));
				password_ = std::move(std::get<1>(tp));
			}
			else if (constexpr (sizeof...(args) == 3)) {
				port_ = std::move(std::get<0>(tp));
				password_ = std::move(std::get<1>(tp));
				db_num_ = std::move(std::get<2>(tp));
			}

#else
			if constexpr (sizeof...(args) == 1) {
				port_ = std::move(std::get<0>(tp));
			}
			else if constexpr (sizeof...(args) == 2) {
				port_ = std::move(std::get<0>(tp));
				password_ = std::move(std::get<1>(tp));
			}
			else if constexpr (sizeof...(args) == 3) {
				port_ = std::move(std::get<0>(tp));
				password_ = std::move(std::get<1>(tp));
				db_num_ = std::move(std::get<2>(tp));
			}
#endif
			boost::system::error_code ec;
			socket_->connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(host_), port_), ec);
			if (ec) {
				return false;
			}

			is_connect_ = true;
			init_socket();
			if (!password_.empty()) {
				bool is_sucess = send_auth();
				if (!is_sucess) {
					return is_sucess;
				}
			}

			if (db_num_ >= 0) {
				return select_db();
			}

			return true;
		}

		bool send_msg(std::string&& buffer) {
			std::unique_lock<std::mutex>lock(mutex_);

			boost::system::error_code ec;
			std::string msg = std::move(buffer);
			boost::asio::write(*socket_, boost::asio::buffer(msg), boost::asio::transfer_exactly(msg.size()), ec);
			if (ec) {
				is_connect_ = false;
				return false;
			}

			lock.unlock();
			read_msg();
			return true;
		}

		const std::shared_ptr<cpp_redis_response>& get_responese() const {
			return response_;
		}

		void close()
		{
			boost::system::error_code ec;
			socket_->close(ec);
		}
	private:
		bool select_db()
		{
			if (db_num_ < 0) {
				return false;
			}

			std::string data = cpp_redis::unit::int_to_string(db_num_);
			data.append(g_crlf);
			std::string msg = "select ";
			msg.append(data);
			if (!send_msg(std::move(msg))){
				return false;
			}

			if (response_->get_result_code() == status::unconnected_ ||
				response_->get_result_code() == status::errors_) {
				return false;
			}

			return true;
		}

		bool read_msg() {
			boost::asio::streambuf response;
			int loop_times = -1;

			response_->reset();

			do
			{
				std::string data;
				read_bytes(response, data);
				if (data.empty()) {
					return false;
				}

				switch (data[0])
				{
				case '*':
				{
					int len = unit::string_to_int(data.c_str() + 1);
					loop_times = len;
					if (len == -1)
					{
						set_nil();
					}
					break;
				}
				case '$':
				{
					int cmd_len = unit::string_to_int(data.c_str() + 1);
					if (cmd_len > 0)
					{
						read_bytes(response);
					}
					else if (cmd_len == -1 ||cmd_len == 0)
					{
						set_nil();
					}
					break;
				}
				case ':':
				{
					int l = unit::string_to_int(data.c_str() + 1);
					set_int_result(l);
					break;
				}
				case '+':
				{
					read_status_bytes(&data[0] + 1);
					break;
				}
				case '-':
				{
					read_error_bytes(&data[0] + 1);
					break;
				}
				default:
					return false;
				}
			} while (--loop_times > -1);
			return true;
		}

		void read_bytes(boost::asio::streambuf& response, std::string& data)
		{
			if (!is_connect_) {
				response_->set_result_code(status::unconnected_);
				return;
			}

			boost::system::error_code e;
			size_t bytes_transferred = boost::asio::read_until(*socket_, response, g_crlf, e);
			if (e.value() != 0) {
				response_->set_result_code(status::unconnected_);
				return;
			}

			if (bytes_transferred > 0) {
				data.assign(boost::asio::buffers_begin(response.data()), boost::asio::buffers_begin(response.data()) + bytes_transferred);
				auto pos = data.find(g_crlf);
				if (pos != std::string::npos) {
					data = data.substr(0, pos);
				}
			}
			else {
				data = "-ERR read bytes is error";
			}

			// Consume bytes_transferred bytes from the input sequence.  The input sequence is
			// now empty.
			response.consume(bytes_transferred);
			//ÄáÂêÓÐ¿Ó£¬Óöµ½\t½Ø¶ÏÁË
			//std::istream response_stream(&response);
			//response_stream >> data;
		}

		void read_bytes(boost::asio::streambuf& response)
		{
			if (!is_connect_){
				response_->set_result_code(status::unconnected_);
				return;
			}

			boost::system::error_code e;
			size_t bytes_transferred = boost::asio::read_until(*socket_, response, g_crlf, e);

			if (e.value() != 0) {
				response_->set_result_code(status::unconnected_);
				return;
			}

			std::string data;

			if (bytes_transferred > 0) {
				data.assign(boost::asio::buffers_begin(response.data()), boost::asio::buffers_begin(response.data()) + bytes_transferred);
				auto pos = data.find(g_crlf);
				if (pos != std::string::npos) {
					data = data.substr(0, pos);
				}
			}
			else {
				data = "-ERR read bytes is error";
			}

			// Consume bytes_transferred bytes from the input sequence.  The input sequence is
			// now empty.
			response.consume(bytes_transferred);
			response_->set_results(std::move(data));
			//read_size += 2;

			//std::istreambuf_iterator<char> i(&response);
			//std::istreambuf_iterator<char> eos;
			//std::string v;

			//int index = 0;
			//while (i != eos && index++ < read_size)
			//{
			//	char c = *i++;
			//	if ((c != '\r') && (c != '\n'))
			//	{
			//		v.push_back(c);
			//	}
			//}

			//response_->set_results(std::move(v));
		}


		void read_ping_bytes(std::string& data)
		{
			if (!is_connect_) {
				response_->set_result_code(status::unconnected_);
				return;
			}

			boost::system::error_code e;
			size_t bytes_transferred = socket_->read_some(boost::asio::buffer(&data[0], data.size()),e);
			if (e.value() != 0) {
				response_->set_result_code(status::unconnected_);
				return;
			}

			if (bytes_transferred > 0) {
				auto pos = data.find(g_crlf);
				if (pos != std::string::npos) {
					data = data.substr(0, pos);
				}
			}
			else {
				data = "-ERR read bytes is error";
			}
		}

		void set_nil()
		{
			std::string data = g_nil;
			response_->set_results(std::move(data));
		}

		void set_int_result(int value)
		{
			response_->set_int_results(value);
			response_->set_result_code(status::int_result_);
		}

		void read_error_bytes(const char* r)
		{
			response_->set_error(r);
			response_->set_result_code(status::errors_);
		}

		void read_status_bytes(const char* r)
		{
			response_->set_status(r);
			response_->set_result_code(status::status_);
		}

		bool send_auth()
		{
			password_.append(g_crlf);
			std::string msg = "auth ";
			msg.append(password_);
			if (!send_msg(std::move(msg))) {
				return false;
			}

			if (response_->get_result_code() == status::errors_)
			{
				return false;
			}

			return true;
		}

		void init_socket() {
			boost::system::error_code ec;
			socket_->set_option(boost::asio::ip::tcp::no_delay(true), ec);
		}
	private:
		std::string host_;
		std::uint16_t port_ = 6379;
		int db_num_ = -1;
		std::string password_;
		std::string data_;
		std::atomic<bool>is_connect_ = false;
		std::mutex mutex_;
		std::shared_ptr<cpp_redis_response> response_;
		std::unique_ptr<boost::asio::steady_timer>timer_;
		std::unique_ptr<boost::asio::io_service>ios_{};
		std::unique_ptr<boost::asio::ip::tcp::socket>socket_{};
	};
}//cpp_redis