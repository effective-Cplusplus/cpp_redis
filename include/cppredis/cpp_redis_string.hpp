#ifndef cpp_redis_string_h__
#define cpp_redis_string_h__

#include "client_interface.hpp"

namespace cpp_redis {
	class string_client:public client {
	public:
		string_client() = default;

		virtual~string_client()
		{

		}

		virtual bool setex(std::string&& key, std::string&& value, size_t seconds)
		{
			check_args();

			auto p = make_pair(key, value);
			auto data = request_->req_key_value_has_senconds(request_->get_cmd(redis_cmd::setex), std::move(p), seconds);

			socket_->send_msg(std::move(data));
			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::status_)
			{
				return false;
			}

			return true;
		}

		virtual bool set(std::string&& key, std::string&& value)
		{
			std::string msg = request_->req_n_key_value(request_->get_cmd(redis_cmd::set), std::forward<std::string>(key), std::forward<std::string>(value));
			socket_->send_msg(std::move(msg));

			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::status_)
			{
				return false;
			}

			return true;
		}

		virtual std::tuple<bool, int> incr(std::string&& key, int increment = 1)
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::incrby), std::forward<std::string>(key), std::move(unit::int_to_string(increment)));
			socket_->send_msg(std::move(msg));
			const auto res = socket_->get_responese();

			if (res->get_result_code() != status::int_result_) {
				return std::make_tuple(false, -1);
			}

			const auto int_results = res->get_int_results();
			if (int_results.empty()) {
				return std::make_tuple(false, -1);
			}

			return std::make_tuple(true, int_results[0]);
		}

		virtual std::tuple<bool, int> decr(std::string&& key, int increment = 1)
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::incrby), std::forward<std::string>(key), std::move(unit::int_to_string(unit::turn(increment))));
			socket_->send_msg(std::move(msg));
			const auto res = socket_->get_responese();


			const auto int_results = res->get_int_results();
			if (int_results.empty()) {
				return std::make_tuple(false, -1);
			}

			return std::make_tuple(true, int_results[0]);
		}

		virtual std::string get_reflect_value(std::string&& key)
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::get), std::forward<std::string>(key));
			socket_->send_msg(std::move(msg));
			const auto res = socket_->get_responese();

			if (res->get_result_code() != status::results_) {
				return "";
			}

			const auto results = res->get_results();
			if (results.empty()) {
				return "";
			}

			return std::move(std::string(results[0].c_str(), strlen(results[0].c_str())));
		}

		virtual std::string get_set_key(std::string&& key, std::string&& value)
		{
			check_args();

			std::string msg = request_->req_n_key_value(request_->get_cmd(redis_cmd::get_set), std::forward<std::string>(key), std::forward <std::string>(value));
			socket_->send_msg(std::move(msg));

			const auto res = socket_->get_responese();

			const auto results = res->get_results();
			if (results.empty()) {
				return "";
			}

			return std::move(std::string(results[0].c_str(), strlen(results[0].c_str())));
		}

		virtual std::string substr_reflect_value(std::string&& key, int start, int end)
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::substr), std::forward<std::string>(key),
				std::move(unit::int_to_string(start)), std::move(unit::int_to_string(end)));
			socket_->send_msg(std::move(msg));

			const auto res = socket_->get_responese();

			const auto results = res->get_results();
			if (results.empty()) {
				return "";
			}

			return std::move(std::string(results[0].c_str(), strlen(results[0].c_str())));
		}

		template<typename...Args>
		VALUES multi_get_keys(Args&&...key)
		{
			check_args();

			VALUES values;
			std::string msg = request_->req_n_key(request_->get_cmd(cpp_redis::mget), std::forward<Args>(key)...);
			socket_->send_msg(std::move(msg));
			const auto res = socket_->get_responese();

			if (res->get_result_code() != status::results_) {
				return {};
			}

			return std::move(res->get_results());
		}

		/****************此接口需要传key,value,key,value....*************************/
		template<typename...Args>
		bool multi_set_keys(Args&&...key_value)
		{
			check_args();
			std::string msg = request_->req_n_key_value(request_->get_cmd(cpp_redis::mset), std::forward<Args>(key_value)...);
			socket_->send_msg(std::move(msg));
			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::status_) {
				return false;
			}

			return true;
		}

		/****************此接口需要传key,value,key,value....*************************/
		//当所有 key 都成功设置，返回 1 。 如果所有给定 key 都设置失败(至少有一个 key 已经存在)，那么返回 0,表示失败
		template<typename...Args>
		int multi_set_if_not_set(Args...key_value)
		{
			check_args();

			std::string msg = request_->req_n_key_value(request_->get_cmd(cpp_redis::msetnx), std::forward<Args>(key_value)...);
			socket_->send_msg(std::move(msg));
			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::int_result_) {
				return 0;
			}

			const auto result = res->get_int_results();
			return result[0];
		}

		//在指定key追加值
		virtual std::tuple<bool, int> append_value(std::string&& key, std::string&& new_value)
		{
			check_args();

			std::string msg = request_->req_n_key_value(request_->get_cmd(cpp_redis::append),
				std::forward<std::string>(key), std::forward<std::string>(new_value));
			socket_->send_msg(std::move(msg));
			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::int_result_) {
				return { false,0 };
			}

			const auto int_results = res->get_int_results();

			if (int_results.empty()) {
				return { false,0 };
			}

			return std::make_tuple(true, int_results[0]);
		}
	};
}
#endif // cpp_redis_string_h__
