#ifndef cpp_hash_client_h__
#define cpp_hash_client_h__

#include "client_interface.hpp"

namespace cpp_redis
{
	class hash_client :public client {
	public:
		hash_client() = default;
		virtual ~hash_client() {

		}

		//当 HSET 命令在哈希表中新创建 field 域并成功为它设置值时， 命令返回 1 
		//如果域 field 已经存在于哈希表， 并且 HSET 命令成功使用新值覆盖了它的旧值， 那么命令返回 0
		virtual int hash_set(std::string&& key, std::string&& field, std::string&& value)
		{
			check_args();
			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::hash_set),std::forward<std::string>(key),
				std::forward<std::string>(field), std::forward<std::string>(value));
		
			socket_->send_msg(std::move(msg));
			const auto res = socket_->get_responese();
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
			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::hash_setx), std::forward<std::string>(key),
				std::forward<std::string>(field), std::forward<std::string>(value));

			socket_->send_msg(std::move(msg));
			const auto res = socket_->get_responese();
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
			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::hash_exists), std::forward<std::string>(key),
				std::forward<std::string>(field));

			socket_->send_msg(std::move(msg));
			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::int_result_) {
				return 0;
			}

			const auto results = res->get_int_results();
			if (results.empty()) {
				return 0;
			}

			return results[0];
		}

	};
}

#endif // cpp_hash_client_h__
