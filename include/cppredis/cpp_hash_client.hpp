#ifndef cpp_hash_client_h__
#define cpp_hash_client_h__

#include "client_interface.hpp"

namespace cpp_redis
{
	class hash_client :public client {
	public:
		hash_client()          = default;
		virtual ~hash_client() {

		}

		//当 HSET 命令在哈希表中新创建 field 域并成功为它设置值时， 命令返回 1 
		//如果域 field 已经存在于哈希表， 并且 HSET 命令成功使用新值覆盖了它的旧值， 那么命令返回 0
		virtual int hash_set(std::string&& key, std::string&& field, std::string&& value)
		{
			check_args();
			std::string msg    = request_->req_n_key(request_->get_cmd(redis_cmd::hash_set),std::forward<std::string>(key),
				std::forward<std::string>(field), std::forward<std::string>(value));
		
			socket_->send_msg(std::move(msg));
			const auto res     = socket_->get_responese();
			if (res->get_result_code() != status::int_result_){
				return 0;
			}

			const auto results = res->get_int_results();
			if (results.empty()){
				return 0;
			}

			return results[0];
		}

		//和string数据结构一样,可以实现分布式锁
		//HSETNX 命令在设置成功时返回 1 ， 在给定域已经存在而放弃执行设置操作时返回 0.
		//如果给定域已经存在于哈希表当中， 那么命令将放弃执行设置操作。
		virtual int hash_setx(std::string&& key, std::string&& field, std::string&& value)
		{
			check_args();
			std::string msg    = request_->req_n_key(request_->get_cmd(redis_cmd::hash_setx), std::forward<std::string>(key),
				std::forward<std::string>(field), std::forward<std::string>(value));

			socket_->send_msg(std::move(msg));
			const auto res     = socket_->get_responese();
			if (res->get_result_code() != status::int_result_) {
				return 0;
			}

			const auto results = res->get_int_results();
			if (results.empty()) {
				return 0;
			}

			return results[0];
		}

		virtual int hash_exists(std::string&& key, std::string&& field)
		{
			check_args();
			std::string msg    = request_->req_n_key(request_->get_cmd(redis_cmd::hash_exists), std::forward<std::string>(key),
				std::forward<std::string>(field));

			socket_->send_msg(std::move(msg));
			const auto res     = socket_->get_responese();
			if (res->get_result_code() != status::int_result_) {
				return 0;
			}

			const auto results = res->get_int_results();
			if (results.empty()) {
				return 0;
			}

			return results[0];
		}

		virtual std::string hash_get(std::string&& key, std::string&& field)
		{
			check_args();
			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::hash_get),std::forward<std::string>(key), std::forward<std::string>(field));

			socket_->send_msg(std::move(msg));
			const auto res = socket_->get_responese();
			if (res->get_result_code()!= status::results_){
				return "";
			}

			const auto results = res->get_results();
			if (results.empty()){
				return "";
			}

			return ((results[0] == g_nil) ? "" : results[0]);
		}

		virtual int hash_del(KEYS&& fields)
		{
			check_args();

			std::string msg = request_->req_n_keys(request_->get_cmd(redis_cmd::hash_del), std::forward<KEYS>(fields));

			socket_->send_msg(std::move(msg));

			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::int_result_){
				return -1;
			}

			const auto results = res->get_int_results();
			if (results.empty()){
				return -1;
			}

			return results[0];
		}

		virtual int hash_len(std::string&& key)
		{
			check_args();
			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::hash_len),std::forward<std::string>(key));

			socket_->send_msg(std::move(msg));

			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::int_result_) {
				return -1;
			}

			const auto results = res->get_int_results();
			if (results.empty()) {
				return -1;
			}

			return results[0];
		}

		virtual int hash_strlen(std::string&& key, std::string&& field)
		{
			check_args();
			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::hash_strlen), std::forward<std::string>(key),
				std::forward<std::string>(field));

			socket_->send_msg(std::move(msg));

			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::int_result_) {
				return -1;
			}

			const auto results = res->get_int_results();
			if (results.empty()) {
				return -1;
			}

			return results[0];
		}

		//返回增加值
		//一个新的哈希表被创建并执行HINCRBY 命令(注意地方)
		virtual int hash_incrby(std::string&& key, std::string&& field, std::string&& increment)
		{
			check_args();
			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::hash_incrby), std::forward<std::string>(key),
				std::forward<std::string>(field),std::forward<std::string>(increment));

			socket_->send_msg(std::move(msg));

			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::int_result_) {
				return -1;
			}

			const auto results = res->get_int_results();
			if (results.empty()) {
				return -1;
			}

			return results[0];
		}

		virtual std::string hash_incrby_float(std::string&& key, std::string&& field, std::string&& increment)
		{
			check_args();
			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::hash_incrby_float), std::forward<std::string>(key),
				std::forward<std::string>(field), std::forward<std::string>(increment));

			socket_->send_msg(std::move(msg));

			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::results_) {
				return"";
			}

			const auto results = res->get_results();
			if (results.empty()) {
				return"";
			}

			return results[0];
		}


		virtual bool hash_mset(KEYS&& keys)
		{
			std::string msg = request_->req_n_keys(request_->get_cmd(redis_cmd::hash_mset), std::forward<KEYS>(keys));
			socket_->send_msg(std::move(msg));

			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::status_)
			{
				return false;
			}

			return true;
		}

		virtual RESULTS_TYPE hash_mget(KEYS&& keys)
		{
			std::string msg = request_->req_n_keys(request_->get_cmd(redis_cmd::hash_mget), std::forward<KEYS>(keys));
			socket_->send_msg(std::move(msg));

			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::results_)
			{
				return { {} };
			}

			return std::move(res->get_results());
		}

		//返回所有的keys
		virtual RESULTS_TYPE hash_keys(std::string&& key)
		{
			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::hash_keys), std::forward<std::string>(key));
			socket_->send_msg(std::move(msg));

			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::results_)
			{
				return { {} };
			}

			return std::move(res->get_results());
		}


		//返回key中的所有值
		virtual RESULTS_TYPE hash_vals(std::string&& key)
		{
			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::hash_vals),
				std::forward<std::string>(key));
			socket_->send_msg(std::move(msg));

			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::results_)
			{
				return { {} };
			}

			return std::move(res->get_results());
		}
		
		//返回key中的域和值
		virtual  RESULTS_TYPE hash_get_all(std::string&& key)
		{
			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::hash_get_all), 
				std::forward<std::string>(key));
			socket_->send_msg(std::move(msg));

			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::results_)
			{
				return { {} };
			}

			return std::move(res->get_results());
		}

	};
}

#endif // cpp_hash_client_h__
