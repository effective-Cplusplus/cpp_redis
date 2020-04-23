#ifndef cpp_zset_client_h__
#define cpp_zset_client_h__

namespace cpp_redis {
	class zset_client :public client {
	public:
		zset_client() = default;
		virtual ~zset_client() {

		}

		//score Ö»ÄÜÎªdouble,int float
		template<typename...Args>
		int zset_add(std::string&& key, Args&&...args) {
			check_args();
			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::zset_add), std::forward<Args>(args)...);
			socket_->send_msg(std::move(msg));

			const auto res = socket_->get_responese();
			if (res->get_result_code()!= status::int_result_){
				return -1;
			}

			const auto result = res->get_int_results();
			if (result.empty()){
				return -1;
			}

			return result[0];
		}

		virtual std::string zset_score(std::string&& key,std::string&& member)
		{
			check_args();
			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::zset_score), std::forward<std::string>(key),std::forward<std::string>(member));
			socket_->send_msg(std::move(msg));

			const auto res = socket_->get_responese();
			if (res->get_result_code()!=status::results_){
				return "";
			}

			const auto results = res->get_results();
			if (results.empty()){
				return "";
			}

			return ((results[0] == g_nil) ? "" :results[0]);
		}

		virtual std::string zset_incrby(std::string&& key, std::string&& member)
		{
			check_args();
			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::zset_incrby), std::forward<std::string>(key),"increment ",std::forward<std::string>(member));
			socket_->send_msg(std::move(msg));

			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::results_) {
				return "";
			}

			const auto results = res->get_results();
			if (results.empty()) {
				return "";
			}

			return ((results[0] == g_nil) ? "" : results[0]);
		}
	};
}//cpp_redis

#endif // cpp_zset_client_h__
