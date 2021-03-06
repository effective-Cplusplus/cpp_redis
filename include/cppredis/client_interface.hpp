#ifndef client_interface_h__
#define client_interface_h__

#include <assert.h>
#include "cpp_redis_request.hpp"
#include "cpp_redis_net.hpp"

namespace cpp_redis {
	class client {
	public:
		client() {
			try
			{
				socket_ = cpp_redis::traits::make_unique<cpp_redis_net>();
				request_ = cpp_redis::traits::make_unique<cpp_redis_request>();
			}
			catch (const std::exception & ex)
			{
				std::cout << ex.what() << std::endl;
				socket_ = nullptr;
				request_ = nullptr;
			}
		}

		virtual ~client() {
			close();
		}

		//从左边弹出元素，元素弹完了，redis直接删除
		std::string get_current_error()
		{
			check_args();
			const auto res = socket_->get_responese();
			return res->get_error();
		}

		std::string get_current_status()
		{
			check_args();
			const auto res = socket_->get_responese();
			return res->get_status();
		}
		/********ip,port,password,db_num********************/
		template<typename...Args>
		constexpr bool connect_to(std::string&& ip, Args&&...args) {
			check_args();
			return socket_->connect_to(std::move(ip), std::forward<Args>(args)...);
		}

		bool set_db_num(int&& num)
		{
			check_args();

			if (num < 0) {
				return false;
			}

			return select_db(std::move(num));
		}

		bool auth(std::string&& password)
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::auth), std::forward<std::string>(password));
			if (!socket_->send_msg(std::move(msg))){
				return false;
			}

			const auto res = socket_->get_responese();

			if (res->get_result_code() == status::unconnected_ ||
				res->get_result_code() == status::errors_) {
				return false;
			}

			return true;
		}

		bool delete_key(std::string&& key)
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::del), std::forward<std::string>(key));
			if (!socket_->send_msg(std::move(msg))){
				return false;
			}

			const auto res = socket_->get_responese();

			if (res->get_result_code() == status::errors_) {
				return false;
			}

			const auto int_results = res->get_int_results();
			if (int_results.empty()) {
				return false;
			}

			if (int_results[0] == 0) {
				return false;
			}

			return true;
		}

		bool is_key_exist(std::string&& key)
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::exists), std::forward<std::string>(key));
			if (!socket_->send_msg(std::move(msg))){
				return false;
			}

			const auto res = socket_->get_responese();

			if (res->get_result_code() == status::errors_) {
				return false;
			}

			const auto int_results = res->get_int_results();
			if (int_results.empty()) {
				return false;
			}

			if (int_results[0] == 0) {
				return false;
			}

			return true;
		}
		//给key加上时间
		bool expire(std::string&& key, std::size_t seconds)
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::expire), std::forward<std::string>(key), unit::int_to_string(seconds));
			if (!socket_->send_msg(std::move(msg))){
				return false;
			}

			const auto res = socket_->get_responese();

			if (res->get_result_code() != status::int_result_) {
				return false;
			}

			return true;
		}

		bool pexpire(std::string&& key, std::size_t milliseconds)
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::pexpire), 
				std::forward<std::string>(key), unit::int_to_string(milliseconds));

			if (!socket_->send_msg(std::move(msg))){
				return false;
			}

			const auto res = socket_->get_responese();

			if (res->get_result_code() != status::int_result_) {
				return false;
			}

			const auto results = res->get_int_results();
			return ((!results.empty())  ? (results[0] ==1):false);
		}

		//给key加上在什么时间过期
		bool expire_at(std::string&& key, std::size_t unix_timestamp)
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::expire_at), 
				std::forward<std::string>(key), unit::int_to_string(unix_timestamp));
			if (!socket_->send_msg(std::move(msg))){
				return false;
			}

			const auto res = socket_->get_responese();

			if (res->get_result_code() != status::int_result_) {
				return false;
			}

			const auto results = res->get_int_results();
			return ((!results.empty()) ? (results[0] == 1) : false);
		}

		//给key加上在什么时间过期
		bool pexpire_at(std::string&& key, std::size_t unix_milli_timestamp)
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::pexpire_at),
				std::forward<std::string>(key), unit::int_to_string(unix_milli_timestamp));

			if (!socket_->send_msg(std::move(msg))){
				return false;
			}

			const auto res = socket_->get_responese();

			if (res->get_result_code() != status::int_result_) {
				return false;
			}

			const auto results = res->get_int_results();
			return ((!results.empty()) ? (results[0] == 1) : false);
		}

		//key的剩余时间 -1:表示永久 -2:表示不存在
		int  remainder_ttl(std::string&& key)
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::ttl), std::forward<std::string>(key));
			if (!socket_->send_msg(std::move(msg))){
				return -2;
			}

			const auto res = socket_->get_responese();

			if (res->get_result_code() != status::int_result_) {
				return -2;
			}

			const auto int_results = res->get_int_results();
			if (int_results.empty()) {
				return -2;
			}

			return int_results[0];
		}

		//key的剩余时间 -1:表示永久 -2:表示不存在
		int64_t  remainder_pttl(std::string&& key)
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::pttl), 
				std::forward<std::string>(key));
			
			if (!socket_->send_msg(std::move(msg))){
				return -2;
			}

			const auto res = socket_->get_responese();

			if (res->get_result_code() != status::int_result_) {
				return -2;
			}

			const auto int_results = res->get_int_results();
			if (int_results.empty()) {
				return -2;
			}

			return int_results[0];
		}


		bool rename_key(std::string&& key,std::string &&new_key)
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::rename),
				std::forward<std::string>(key),std::forward<std::string>(new_key));

			if (!socket_->send_msg(std::move(msg))) {
				return false;
			}

			const auto res = socket_->get_responese();

			if (res->get_result_code() != status::status_){
				return false;
			}

			return true;
		}


		bool renamenx_key(std::string&& key, std::string&& new_key)
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::renamenx),
				std::forward<std::string>(key), std::forward<std::string>(new_key));

			if (!socket_->send_msg(std::move(msg))) {
				return false;
			}

			const auto res = socket_->get_responese();

			if (res->get_result_code() != status::status_) {
				return false;
			}

			return true;
		}
		
		bool start_multi()
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::multi));

			if (!socket_->send_msg(std::move(msg))) {
				return false;
			}

			const auto res = socket_->get_responese();

			if (res->get_result_code() != status::status_) {
				return false;
			}

			return true;
		}

		bool exec()
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::exec));

			if (!socket_->send_msg(std::move(msg))) {
				return false;
			}

			const auto res = socket_->get_responese();

			if (res->get_result_code() == status::errors_) {
				return false;
			}

			return true;
		}

		bool discard()
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::discard));

			if (!socket_->send_msg(std::move(msg))) {
				return false;
			}

			const auto res = socket_->get_responese();

			return true;
		}

		bool remove_expire(std::string&& key)
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::remove_expire),std::forward<std::string>(key));

			if (!socket_->send_msg(std::move(msg))) {
				return false;
			}

			const auto res = socket_->get_responese();

			if (res->get_result_code() !=status::int_result_){
				return false;
			}

			const auto results = res->get_int_results();

			return ((!results.empty()) ? results[0] ==1 :false);

		}

		virtual bool setex(std::string&& key, std::string&& value,std::string && seconds)
		{
			return false;
		}

		virtual bool psetex(std::string&& key, std::string&& value,std::string && milliseconds)
		{
			return false;
		}

		virtual bool set(std::string&& key, std::string&& value)
		{
			return false;
		}

		virtual int setnx(std::string&& key, std::string&& value)
		{
			return 0;
		}

		virtual std::vector<std::string> multi_get_keys(std::vector<std::string>&& keys)
		{
			return {};
		}

		virtual bool multi_set_keys(std::vector<std::string>&& keys)
		{
			return false;
		}

		virtual int multi_set_if_not_set(std::vector<std::string>&& keys)
		{
			return false;
		}

		virtual bool setnx_has_seconds(std::string&& key, std::string&& value,std::string&& seconds)
		{
			return false;
		}

		virtual bool setxx(std::string&& key, std::string&& value, std::string&& seconds)
		{
			return false;
		}

		virtual bool setnx_has_milliseconds(std::string&& key, std::string&& value,std::string&& milliseconds)
		{
			return false;
		}

		virtual bool setxx_has_milliseconds(std::string&& key, std::string&& value, std::string&& milliseconds)
		{
			return false;
		}

		virtual std::string get_range(std::string&&key, std::string&& start, std::string&& end)
		{
			return "";
		}

		virtual int incr_by_increment(std::string&& key,std::string&& increment)
		{
			return -1 ;
		}

		virtual std::string incr_by_float(std::string&& key, std::string&& increment)
		{
			return "";
		}

		virtual int incr(std::string&& key)
		{
			return -1;
		}

		virtual int decr(std::string&& key)
		{
			return -1;
		}

		virtual int decr_increment(std::string&& key,std::string && increment)
		{
			return -1;
		}

		virtual std::string get_reflect_value(std::string&& key)
		{
			return { false,-1 };
		}

		virtual std::string get_set_key(std::string&& key, std::string&& value)
		{
			return { false,-1 };
		}

		virtual std::string substr_reflect_value(std::string&& key, int start, int end)
		{
			return { false,-1 };
		}

		//在指定key追加值
		virtual int append_value(std::string&& key, std::string&& append_value)
		{
			return -1;
		}

		virtual int list_rpush(std::string&& key, std::string&& value)
		{
			return -1 ;
		}

		virtual int list_rpush_if(std::string&& key, std::string&& value)
		{
			return -1;
		}

		virtual int list_lpush(std::string&& key, std::string&& value)
		{
			return -1;
		}

		virtual int list_lpush_if(std::string&& key, std::string&& value)
		{
			return -1;
		}

		virtual int32_t list_size(std::string&& key)
		{
			return -1;
		}

		virtual std::vector<std::string> list_range(std::string&& key, int start, int end)
		{
			return {};
		}

		virtual std::string list_lpop(std::string&& key)
		{
			return "";
		}

		virtual std::string list_rpop(std::string&& key)
		{
			return "";
		}

		virtual std::string list_brpop(std::string&& key, size_t timeout = 0)
		{
			return "";
		}

		virtual std::string list_blpop(std::string&& key, size_t timeout = 0)
		{
			return "";
		}

		virtual bool list_trim(std::string&& key, int start, int end)
		{
			return "";
		}

		virtual std::string list_index(std::string&& key, int index)
		{
			return "";
		}

		virtual bool list_set(std::string&& key, std::string&& value,std::string && index)
		{
			return "";
		}

		virtual int list_del_elem(std::string&& key, std::string&& value,std::string&&count)
		{
			return 0;
		}

		virtual std::string list_rpoplpush(std::string&& src_key, std::string&& dst_key)
		{
			return "";
		}
		virtual std::string list_brpoplpush(std::string&& src_key, std::string&& dst_key, int timeout = 0)
		{
			return "";
		}

		virtual int list_insert_before(std::string&& key, std::string&& dst_value, std::string&& insert_value)
		{
			return INT32_MAX;
		}

		virtual int list_insert_after(std::string&& key, std::string&& dst_value, std::string&& insert_value)
		{
			return INT32_MAX;
		}

		virtual int set_add(std::vector<std::string>&& keys)
		{
			return 0;
		}

		virtual int set_delete_elem(std::vector<std::string>&& keys)
		{
			return 0;
		}

		virtual bool  set_is_member(std::string&& key, std::string&& value)
		{
			return false;
		}

		virtual std::string set_rdel_elem(std::string&& key)
		{
			return "";
		}

		virtual std::vector <std::string> set_sinter(std::vector<std::string>&& keys)
		{
			return {};
		}

		virtual int set_inter_store(std::vector<std::string>&& keys)
		{
			return 0;
		}

		virtual std::vector <std::string> set_union(std::vector<std::string>&& keys)
		{
			return {};
		}

		virtual int set_union_store(std::vector<std::string>&& keys)
		{
			return 0;
		}

		virtual std::vector <std::string> set_rand_elem(std::string&& key, int count)
		{
			return {};
		}

		virtual bool set_move_elem(std::string&& src_key, std::string&& dst_key, std::string&& member)
		{
			return false;
		}

		virtual std::vector <std::string> set_diff(std::vector<std::string>&& keys)
		{
			return {};
		}

		virtual size_t set_get_size(std::string&& key)
		{
			return 0;
		}
		virtual std::vector <std::string> set_get_all_member(std::string&& key)
		{
			return {};
		}
		
		virtual int set_diff_store(std::vector<std::string>&& keys)
		{
			return -1;
		}

		virtual int zset_add(std::vector<std::string>&& keys)
		{
			return -1;
		}
		virtual std::string zset_score(std::string&& key, std::string&& member)
		{
			return "";
		}
		virtual std::string zset_incrby(std::string&& key,std::string&& increment,std::string&& member)
		{
			return "";
		}

		virtual int zset_card(std::string&& key)
		{
			return 0;
		}

		virtual int zset_count(std::string&& key, std::string&& min, std::string&& max)
		{
			return 0;
		}

		virtual std::vector <std::string> zset_range(std::string&& key, std::string&& begin, std::string&& end,bool with_scores)
		{
			return { {} };
		}

		virtual std::vector <std::string> zset_revrange(std::string&& key, std::string&& begin, std::string&& end, bool with_scores)
		{
			return { {} };
		}

		virtual std::vector <std::string> zset_range_score(std::string&& key, std::string&& min, std::string&& max,
			bool with_scores, bool limit, std::string&& limit_min, std::string&& limit_max)
		{
			return { {} };
		}

		virtual std::vector <std::string> zset_revrange_score(std::string&& key, std::string&& max, std::string&& min,
			bool with_scores, bool limit, std::string&& limit_min,std::string&&limit_max)
		{
			return { {} };
		}
		
		virtual int zset_rank(std::string&& key, std::string&& member)
		{
			return -1;
		}

		virtual int zset_revrank(std::string&& key, std::string&& member)
		{
			return -1;
		}

		virtual bool zset_rem(std::vector<std::string>&& keys)
		{
			return false;
		}

		virtual int zset_remrangeby_rank(std::string&& key, std::string&& begin, std::string&& end)
		{
			return -1;
		}
		virtual int zset_remrangebyscore(std::string&& key, std::string&& min, std::string&& max)
		{
			return -1;
		}
		virtual std::vector <std::string> zset_rangebylex(std::string&& key, std::string&& min,
			std::string&& max, bool limit, std::string&& limit_min, std::string&& limit_max)
		{
			return { {} };
		}

		virtual int zset_lexcount(std::string&& key, std::string&& min, std::string&& max)
		{
			return -1;
		}

		virtual int zset_remrangebylex(std::string&& key, std::string&& min,std::string&& max)
		{
			return 0;
		}

		virtual int zset_union_store(std::vector<std::string>&& keys,aggregate_mothod mothod)
		{
			return -1;
		}

		virtual int zset_inter_store(std::vector<std::string>&& keys, aggregate_mothod mothod)
		{
			return -1;
		}

		virtual int hash_set(std::string&& key, std::string&& field, std::string&& value)
		{
			return 0;
		}

		virtual int hash_setx(std::string&& key, std::string&& field, std::string&& value)
		{
			return 0;
		}

		virtual int hash_exists(std::string&& key,std::string&& field)
		{
			return 0;
		}

		virtual std::string hash_get(std::string&& key, std::string&& field)
		{
			return "";
		}

		virtual int hash_del(std::vector<std::string>&& fields)
		{
			return -1;
		}

		virtual int hash_len(std::string&& key)
		{
			return 0;
		}

		virtual int hash_strlen(std::string&& key, std::string&& field)
		{
			return 0;
		}

		virtual int hash_incrby(std::string&& key, std::string && field, std::string&& increment)
		{
			return 0;
		}

		virtual std::string hash_incrby_float(std::string&& key, std::string&& field, std::string&& increment)
		{
			return "";
		}

		virtual bool hash_mset(std::vector<std::string>&& keys)
		{
			return false;
		}

		virtual std::vector <std::string> hash_mget(std::vector<std::string>&& keys)
		{
			return { {} };
		}

		virtual std::vector <std::string> hash_keys(std::string&& key)
		{
			return{ {} };
		}

		virtual std::vector <std::string> hash_vals(std::string&& key)
		{
			return{ {} };
		}

		virtual  std::vector <std::string> hash_get_all(std::string&& key)
		{
			return{ {} };
		}
private:
		bool select_db(int&& num)
		{
			check_args();

			if (num < 0) {
				return false;
			}

			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::select), std::move(unit::int_to_string(num)));
			if (!socket_->send_msg(std::move(msg))){
				return false;
			}

			const auto res = socket_->get_responese();

			if (res->get_result_code() == status::unconnected_ ||
				res->get_result_code() == status::errors_) {
				return false;
			}

			return true;
		}

		void close() {
			check_args();

			socket_->close();
		}
	protected:
		void check_args()
		{
			assert(socket_ != nullptr);
			assert(request_ != nullptr);
		}

		std::unique_ptr<cpp_redis_request>request_;
		std::unique_ptr<cpp_redis_net>socket_;
	};
}

#endif // client_interface_h__
