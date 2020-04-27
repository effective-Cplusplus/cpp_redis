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

		virtual bool setex(std::string&& key, std::string&& value,std::string && seconds)
		{
			check_args();
			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::setex),
				std::forward<std::string>(key),std::forward<std::string>(seconds),std::forward<std::string>(value));

			socket_->send_msg(std::move(msg));
			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::status_){
				return false;
			}

			return true;
		}

		virtual bool psetex(std::string&& key, std::string&& value, std::string&& milliseconds)
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::psetex),std::forward<std::string>(key),
				std::forward<std::string>(milliseconds), std::forward<std::string>(value));

			socket_->send_msg(std::move(msg));
			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::status_){
				return false;
			}

			return true;
		}

		virtual bool set(std::string&& key, std::string&& value)
		{
			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::set),
				std::forward<std::string>(key), std::forward<std::string>(value));
			socket_->send_msg(std::move(msg));

			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::status_){
				return false;
			}

			return true;
		}

		virtual bool set_has_seconds(std::string&& key, std::string&& value, std::string&& seconds)
		{
			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::set),
				std::forward<std::string>(key), std::forward<std::string>(value),"EX",std::forward<std::string>(seconds));
			socket_->send_msg(std::move(msg));

			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::status_){
				return false;
			}

			return true;
		}

		virtual bool set_has_seconds_if(std::string&& key, std::string&& value, std::string&& seconds, bool is_exist)
		{
			std::string msg;
			if (is_exist){
				msg = request_->req_n_key(request_->get_cmd(redis_cmd::set),
					std::forward<std::string>(key), std::forward<std::string>(value), "EX", std::forward<std::string>(seconds),"XX");
			}else {
				msg = request_->req_n_key(request_->get_cmd(redis_cmd::set),
					std::forward<std::string>(key), std::forward<std::string>(value), "EX", std::forward<std::string>(seconds), "NX");
			}

			socket_->send_msg(std::move(msg));
			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::status_){
				return false;
			}

			return true;
		}

		virtual bool set_has_milliseconds(std::string&& key, std::string&& value, std::string&& milliseconds)
		{
			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::set),
				std::forward<std::string>(key), std::forward<std::string>(value), "PX", std::forward<std::string>(milliseconds));
			socket_->send_msg(std::move(msg));

			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::status_){
				return false;
			}

			return true;
		}

		virtual bool set_has_milliseconds_if(std::string&& key, std::string&& value,
			std::string&& milliseconds, bool is_exist)
		{
			std::string msg;
			if (is_exist){
				msg = request_->req_n_key(request_->get_cmd(redis_cmd::set),
					std::forward<std::string>(key), std::forward<std::string>(value), "PX", std::forward<std::string>(milliseconds),"XX");
			}else {
				msg = request_->req_n_key(request_->get_cmd(redis_cmd::set),
					std::forward<std::string>(key), std::forward<std::string>(value), "PX", std::forward<std::string>(milliseconds),"NX");
			}
			
			socket_->send_msg(std::move(msg));
			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::status_){
				return false;
			}

			return true;
		}

		//sub_str;//2.0之前叫SUBSTR 
		virtual std::string get_range(std::string&& key, std::string&& start, std::string&& end)
		{
			check_args();
			
			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::strsub),std::forward<std::string>(key),
				std::forward<std::string>(start),std::forward<std::string>(end));

			socket_->send_msg(std::move(msg));

			const auto res = socket_->get_responese();

			if (res->get_result_code() != status::results_){
				return "";
			}

			const auto results = res->get_results();

			return ((results.empty() ||results[0] == g_nil) ? "": std::move(results[0]));
		}

		//若不存在,就创建key,然后再增加1
		virtual int incr(std::string&& key)
		{
			check_args();
			
			std::string  msg = request_->req_n_key(request_->get_cmd(cpp_redis::incr),
				std::forward<std::string>(key));

			socket_->send_msg(std::move(msg));
			const auto res = socket_->get_responese();

			if (res->get_result_code() != status::int_result_&&
				res->get_result_code() != status::status_){
				return -1;
			}

			const auto results = res->get_int_results();

			return ((!results.empty()) ? results[0] : -1);
		}

		//若不存在,就创建key,然后再增加
		virtual int incr_by_increment(std::string&& key,std::string&& increment)
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::incrby), std::forward<std::string>(key), 
				std::forward<std::string>(increment));

			socket_->send_msg(std::move(msg));
			const auto res = socket_->get_responese();

			if (res->get_result_code() != status::int_result_&&
				res->get_result_code() != status::status_) {
				return -1;
			}

			const auto results = res->get_int_results();

			return ((!results.empty()) ? results[0] : -1);
		}

		virtual std::string incr_by_float(std::string&& key, std::string&& increment)
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::incr_by_float), std::forward<std::string>(key),
				std::forward<std::string>(increment));

			socket_->send_msg(std::move(msg));
			const auto res = socket_->get_responese();

			if (res->get_result_code() != status::results_&&
				res->get_result_code() != status::status_){
				return "";
			}

			const auto results = res->get_results();

			return (!results.empty() && results[0] != g_nil ? std::move(results[0]) : "");
		}

		virtual int decr(std::string&& key)
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::decr),
				std::forward<std::string>(key));
			
			socket_->send_msg(std::move(msg));
			const auto res = socket_->get_responese();
			
			if (res->get_result_code() != status::int_result_&&
				res->get_result_code() != status::status_){
				return -1;
			}

			const auto int_results = res->get_int_results();

			return ((!int_results.empty()) ? int_results[0] : -1);
		}

		virtual int decr_increment(std::string&& key,std::string&& increment)
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::decyby),
				std::forward<std::string>(key),std::forward<std::string>(increment));
			socket_->send_msg(std::move(msg));
			const auto res = socket_->get_responese();

			const auto int_results = res->get_int_results();

			return ((!int_results.empty()) ? int_results[0]: -1);
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

			return ((!results.empty()) ? std::move(results[0]) : "");
		}

		virtual std::string get_set_key(std::string&& key, std::string&& value)
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::get_set), 
				std::forward<std::string>(key), std::forward <std::string>(value));
			socket_->send_msg(std::move(msg));

			const auto res = socket_->get_responese();

			const auto results = res->get_results();

			return ((!results.empty()) ? std::move(results[0]) : "");
		}

		virtual std::string substr_reflect_value(std::string&& key, int start, int end)
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::substr), std::forward<std::string>(key),
				std::move(unit::int_to_string(start)), std::move(unit::int_to_string(end)));
			socket_->send_msg(std::move(msg));

			const auto res = socket_->get_responese();

			const auto results = res->get_results();
			
			return ((!results.empty()) ? std::move(results[0]) : "");
		}

		virtual std::vector<std::string>  multi_get_keys(std::vector<std::string>&&keys)
		{
			check_args();

			std::string msg = request_->req_n_keys(request_->get_cmd(cpp_redis::mget), std::forward<std::vector<std::string>>(keys));
			socket_->send_msg(std::move(msg));
			const auto res = socket_->get_responese();

			if (res->get_result_code() != status::results_&&
				res->get_result_code() != status::status_) {
				return {};
			}

			return std::move(res->get_results());
		}

		/****************此接口需要传key,value,key,value....*************************/
		virtual bool multi_set_keys(std::vector<std::string>&& keys)
		{
			check_args();
			std::string msg = request_->req_n_keys(request_->get_cmd(cpp_redis::mset), std::forward<std::vector<std::string>>(keys));
			socket_->send_msg(std::move(msg));
			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::status_) {
				return false;
			}

			return true;
		}

		/****************此接口需要传key,value,key,value....*************************/
		//当所有 key 都成功设置，返回 1 。 如果所有给定 key 都设置失败(至少有一个 key 已经存在)，那么返回 0,表示失败
		virtual int multi_set_if_not_set(std::vector<std::string>&&keys)
		{
			check_args();

			std::string msg = request_->req_n_keys(request_->get_cmd(cpp_redis::msetnx), std::forward<std::vector<std::string>>(keys));
			socket_->send_msg(std::move(msg));
			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::int_result_&&
				res->get_result_code() != status::status_) {
				return 0;
			}

			const auto results = res->get_int_results();
			return ((!results.empty()) ? results[0] : -1);
		}

		//在指定key追加值
		virtual int append_value(std::string&& key, std::string&& appended_value)
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(cpp_redis::append),
				std::forward<std::string>(key), std::forward<std::string>(appended_value));
			socket_->send_msg(std::move(msg));
			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::int_result_&&
				res->get_result_code() != status::status_) {
				return -1;
			}

			const auto results = res->get_int_results();

			return ((!results.empty()) ? results[0]:-1);
		}
	};
}
#endif // cpp_redis_string_h__
