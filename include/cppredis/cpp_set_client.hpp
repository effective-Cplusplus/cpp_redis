#ifndef cpp_set_client_h__
#define cpp_set_client_h__

#include "client_interface.hpp"

namespace cpp_redis {
	class set_client :public client {
	public:
		set_client() = default;
		virtual ~set_client()
		{

		}

		//返回当前set的个数
		int set_add(std::vector<std::string>&& keys)
		{
			check_args();

			std::string msg = request_->req_n_keys(request_->get_cmd(redis_cmd::sadd),std::forward<std::vector<std::string>>(keys));

			socket_->send_msg(std::move(msg));

			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::int_result_) {
				return 0;
			}

			const auto results = res->get_int_results();

			return ((!results.empty()) ? results[0]:0);
		}

		//返回当前set的个数,删除失败直接返回-1
		int set_delete_elem(std::vector<std::string>&& keys)
		{
			check_args();

			std::string msg = request_->req_n_keys(request_->get_cmd(redis_cmd::srem),std::forward<std::vector<std::string>>(keys));

			socket_->send_msg(std::move(msg));

			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::int_result_) {
				return 0;
			}

			const auto results = res->get_int_results();
			return ((!results.empty()) ? results[0] : 0);
		}

		//是返回ture,其它一律为false
		virtual bool set_is_member(std::string&& key, std::string&& value)
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::sismember),
				std::forward<std::string>(key), std::forward<std::string>(value));

			socket_->send_msg(std::move(msg));

			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::int_result_) {
				return false;
			}

			const auto result = res->get_int_results();
			if (result.empty()) {
				return false;
			}

			return ((result[0] == 1) ? true : false);
		}

		//随机移除一个元素
		virtual std::string set_rdel_elem(std::string&& key)
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::spop_elem), std::forward<std::string>(key));

			socket_->send_msg(std::move(msg));

			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::results_) {
				return false;
			}

			const auto result = res->get_results();
			if (result.empty()) {
				return "";
			}

			return ((result[0] == g_nil) ? "" : result[0]);
		}

		//count >0表示元素不会重复, <0表示元素会重复
		virtual RESULTS_TYPE set_rand_elem(std::string&& key, int count)
		{
			check_args();
			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::srandmember), std::forward<std::string>(key), std::move(unit::int_to_string(count)));
			socket_->send_msg(std::move(msg));

			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::results_) {
				return{};
			}

			auto result = res->get_results();
			if (result.empty() || result[0] == g_nil) {
				return {};
			}

			return  std::move(result);
		}

		virtual bool set_move_elem(std::string&& src_key, std::string&& dst_key, std::string&& member)
		{
			check_args();
			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::smove), std::forward<std::string>(src_key),
				std::forward<std::string>(dst_key), std::forward<std::string>(member));

			socket_->send_msg(std::move(msg));

			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::int_result_) {
				return false;
			}

			const auto resulut = res->get_int_results();
			if (resulut.empty()) {
				return false;
			}

			return ((resulut[0] == 1) ? true : false);
		}

		virtual size_t set_get_size(std::string&& key)
		{
			check_args();
			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::ssize), std::forward<std::string>(key));

			socket_->send_msg(std::move(msg));

			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::int_result_) {
				return 0;
			}

			const auto resulut = res->get_int_results();
			if (resulut.empty()) {
				return 0;
			}

			return resulut[0];
		}

		virtual RESULTS_TYPE set_get_all_member(std::string&& key)
		{
			check_args();
			std::string msg = request_->req_n_key(request_->get_cmd(redis_cmd::smembers), std::forward<std::string>(key));
			socket_->send_msg(std::move(msg));

			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::results_) {
				return{};
			}

			auto result = res->get_results();
			if (result.empty() || result[0] == g_nil) {
				return {};
			}

			return  std::move(result);
		}

		//求集合的交集，如果一个为空，就返回空
		virtual RESULTS_TYPE set_sinter(std::vector<std::string>&& keys)
		{
			check_args();
			std::string msg = request_->req_n_keys(request_->get_cmd(redis_cmd::sinter),std::forward<std::vector<std::string>>(keys));
			socket_->send_msg(std::move(msg));

			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::results_) {
				return{};
			}

			auto result = res->get_results();
			return ((!result.empty()) ? std::move(result) : std::move(std::vector<std::string>{}));
		}

		//求集合的交集，如果一个为空，就返回空,并保存另外一个地方
		virtual int set_inter_store(std::vector<std::string>&& keys)
		{
			check_args();
			std::string msg = request_->req_n_keys(request_->get_cmd(redis_cmd::ssinter_store),std::forward<std::vector<std::string>>(keys));
			socket_->send_msg(std::move(msg));

			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::int_result_) {
				return 0;
			}

			auto result = res->get_int_results();

			return  ((!result.empty()) ? result[0] :0);
		}

		//求集合的并集合,不存在key就视为空 
		//(返回一个集合的全部成员，该集合是所有给定集合的并集)
		//A{1:1234,2:5678,37895,4:910245} B{1:1234,2:7895,3:78945}
		//A U B={1234,5678,7895,10245}
		virtual RESULTS_TYPE set_union(std::vector<std::string>&&keys)
		{
			check_args();
			std::string msg = request_->req_n_keys(request_->get_cmd(redis_cmd::sunion), std::forward<std::vector<std::string>>(keys));
			socket_->send_msg(std::move(msg));

			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::results_) {
				return{};
			}

			auto result = res->get_results();
			if (result.empty()) {
				return {};
			}

			return  std::move(result);
		}

		//求集合的并集，如果一个为空，就返回空,并保存另外一个地方
		//(返回一个集合的全部成员，该集合是所有给定集合的并集)
		//src_key可以是本身自己
		virtual int set_union_store(std::vector<std::string>&&keys)
		{
			check_args();
			std::string msg = request_->req_n_keys(request_->get_cmd(redis_cmd::ssunion_store),std::forward<std::vector<std::string>>(keys));
			socket_->send_msg(std::move(msg));

			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::int_result_) {
				return 0;
			}

			auto results = res->get_int_results();

			return ((!results.empty()) ? results[0] : 0);
		}

		//返回一个集合的全部成员，该集合是所有给定集合之间的差集。
	   //不存在的 key 被视为空集。

		virtual RESULTS_TYPE set_diff(std::vector<std::string>&& keys)
		{
			check_args();
			std::string msg = request_->req_n_keys(request_->get_cmd(redis_cmd::sdiff), std::forward<std::vector<std::string>>(keys));
			socket_->send_msg(std::move(msg));

			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::results_) {
				return{};
			}

			auto results = res->get_results();

			return  ((!results.empty()) ? std::move(results) : std::move(RESULTS_TYPE()));
		}

		//返回一个集合的全部成员，该集合是所有给定集合之间的差集。
		//不存在的 key 被视为空集。然后保存起来
		virtual int set_diff_store(std::vector<std::string>&&keys)
		{
			check_args();
			std::string msg = request_->req_n_keys(request_->get_cmd(redis_cmd::sdiff_store),std::forward<std::vector<std::string>>(keys));
			socket_->send_msg(std::move(msg));

			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::int_result_) {
				return 0;
			}

			auto results = res->get_int_results();


			return  ((!results.empty()) ? results[0]:0);
		}

	};
}

#endif // cpp_set_client_h__
