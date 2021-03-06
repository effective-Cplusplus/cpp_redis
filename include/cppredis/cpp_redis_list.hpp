#ifndef cpp_redis_list_h__
#define cpp_redis_list_h__

#include "client_interface.hpp"

namespace cpp_redis {
	class list_client:public client {
	public:
		list_client() = default;

		virtual~list_client() {

		}

		//list插入时要选择方向, 返回tuple:返回行数
		//函数调用是右值引用
		//尾插入法
		virtual int list_rpush(std::string&& key, std::string&& value)
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(cpp_redis::rpush),
				std::forward<std::string>(key), std::forward<std::string>(value));

			if (!socket_->send_msg(std::move(msg))) {
				return 0;
			}

			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::int_result_ &&
				res->get_result_code() != status::status_) {
				return 0;
			}

			const auto results = res->get_int_results();

			return ((!results.empty()) ? results[0] :0);
		}

		//插入时判断key是否存在，存在插入不存在什么也不做
		virtual int list_rpush_if(std::string&& key, std::string&& value)
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(cpp_redis::rpushx),
				std::forward<std::string>(key), std::forward<std::string>(value));

			if (!socket_->send_msg(std::move(msg))) {
				return 0;
			}

			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::int_result_ &&
				res->get_result_code() != status::status_) {
				return 0;
			}

			const auto results = res->get_int_results();

			return ((!results.empty()) ? results[0] : 0);
		}

		//头插入法
		virtual int list_lpush(std::string&& key, std::string&& value)
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(cpp_redis::lpush),
				std::forward<std::string>(key), std::forward<std::string>(value));

			if (!socket_->send_msg(std::move(msg))) {
				return 0;
			}

			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::int_result_ 
				&& res->get_result_code()!=status::status_) {
				return 0;
			}

			const auto results = res->get_int_results();

			return ((!results.empty()) ? results[0] : 0);
		}

		//插入时判断key是否存在，存在插入不存在什么也不做
		virtual int  list_lpush_if(std::string&& key, std::string&& value)
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(cpp_redis::lpushx), 
				std::forward<std::string>(key), std::forward<std::string>(value));

			if (!socket_->send_msg(std::move(msg))) {
				return 0;
			}

			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::int_result_
				&&res->get_result_code() != status::status_) {
				return 0;
			}

			const auto results = res->get_int_results();

			return ((!results.empty()) ? results[0] :0);
		}

		virtual int32_t list_size(std::string&& key)
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(cpp_redis::llen), std::forward<std::string>(key));
			if (!socket_->send_msg(std::move(msg))) {
				return 0;
			}

			const auto res = socket_->get_responese();

			if (res->get_result_code() != status::int_result_
				&&res->get_result_code() != status::status_) {
				return 0;
			}

			const auto results = res->get_int_results();

			return ((!results.empty()) ? results[0] : 0);
		}

		/************和数组一样,下标从零开始**************************************/
		virtual  std::vector <std::string> list_range(std::string&& key, int start, int end)
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(cpp_redis::lrange), std::forward<std::string>(key),
				std::move(unit::int_to_string(start)), std::move(unit::int_to_string(end)));

			if (!socket_->send_msg(std::move(msg))) {
				return {};
			}

			const auto res = socket_->get_responese();

			if (res->get_result_code() != status::results_
				&&res->get_result_code() != status::status_) {
				return {} ;
			}

			//函数调用是右值
			return std::move(res->get_results());
		}

		//第一个开始弹,不管原始的插入方向
		virtual std::string list_lpop(std::string&& key)
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(cpp_redis::lpop), std::forward<std::string>(key));
			if (!socket_->send_msg(std::move(msg))) {
				return "";
			}

			const auto res = socket_->get_responese();

			if (res->get_result_code() != status::results_
				&&res->get_result_code() != status::status_) {
				return "";
			}

			const auto results = res->get_results();

			return ((!results.empty()) ? std::move(results[0]):"");
		}

		//从最后面弹出元素，元素弹完了，redis直接删除
		virtual std::string list_rpop(std::string&& key)
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(cpp_redis::rpop), std::forward<std::string>(key));
			if (!socket_->send_msg(std::move(msg))) {
				return "";
			}

			const auto res = socket_->get_responese();

			if (res->get_result_code() != status::results_
				&&res->get_result_code() != status::status_) {
				return "";
			}

			const auto results = res->get_results();

			return ((!results.empty()) ? std::move(results[0]) : "");
		}

		//弹出最后元素
		//返回的是键值对
		virtual std::string list_brpop(std::string&& key, size_t timeout = 0)
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(cpp_redis::brpop),
				std::forward<std::string>(key), std::move(unit::int_to_string(timeout)));
			if (!socket_->send_msg(std::move(msg))) {
				return "";
			}

			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::results_
				&&res->get_result_code() != status::status_) {
				return "";
			}

			const auto results = res->get_results();
			return ((!results.empty() && results.size() == 2) ? std::move(results[1]) : "");
		}

		//弹出最前元素
		//返回的是键值对
		virtual std::string list_blpop(std::string&& key, size_t timeout = 0)
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(cpp_redis::blpop), std::forward<std::string>(key), std::move(unit::int_to_string(timeout)));
			if (!socket_->send_msg(std::move(msg))) {
				return "";
			}

			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::results_
				&&res->get_result_code() != status::status_) {
				return "";
			}

			const auto results = res->get_results();
			return ((!results.empty() && results.size()==2) ? std::move(results[1]) : "");
		}

		//列表只保留指定区间内的元素，不在指定区间之内的元素都将被删除。
		virtual bool list_trim(std::string&& key, int start, int end)
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(cpp_redis::ltrim), std::forward<std::string>(key),
				std::move(unit::int_to_string(start)), std::move(unit::int_to_string(end)));
			if (!socket_->send_msg(std::move(msg))) {
				return false;
			}

			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::status_
				&&res->get_result_code() != status::status_) {
				return false;
			}

			return true;
		}
		//索引获取列表中的元素.以 -1 表示列表的最后一个元素，//-2 表示列表的倒数第二个元素，以此类推。
		virtual std::string list_index(std::string&& key, int index)
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(cpp_redis::lindex), std::forward<std::string>(key), std::move(unit::int_to_string(index)));
			if (!socket_->send_msg(std::move(msg))) {
				return "";
			}

			const auto res = socket_->get_responese();

			if (res->get_result_code() != status::results_
				&&res->get_result_code() != status::status_) {
				return "";
			}

			const auto  results = res->get_results();

			return ((!results.empty() && results[0]!= g_nil) ? std::move(results[0]) : "");
		}

		//指定索引上设置元素值
		virtual bool list_set(std::string&& key, std::string&& value,std::string&& index)
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(cpp_redis::lset),std::forward<std::string>(key),
				std::forward<std::string>(index),std::forward<std::string>(value));

			if (!socket_->send_msg(std::move(msg))) {
				return false;
			}

			const auto res = socket_->get_responese();

			if (res->get_result_code() == status::status_
				&&res->get_result_code() != status::status_) {
				return true;
			}

			return false;
		}

		//没有移除就为0 ，有移除就大于0，count表示list的起始位置,一般从0开始删除,-1表示最后一个元素删除
		//如果从0开始删除，有多少删除多少
		//如果从-1开始删除,就只会删除一个元素
		//数量为>=|count|
		virtual int list_del_elem(std::string&& key, std::string&& value,std::string &&count)
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(cpp_redis::lrem),std::forward<std::string>(key),
				std::forward<std::string>(count),std::forward<std::string>(value));

			if (!socket_->send_msg(std::move(msg))) {
				return 0;
			}

			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::int_result_
				&&res->get_result_code() != status::status_) {
				return 0;
			}

			const auto results = res->get_int_results();

			return ((!results.empty()) ? results[0]:0);
		}

		//最后一个元素移动到另外一个list去
		virtual std::string list_rpoplpush(std::string&& src_key, std::string&& dst_key)
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(cpp_redis::rpoplpush), std::forward<std::string>(src_key), std::forward<std::string>(dst_key));
			if (!socket_->send_msg(std::move(msg))) {
				return "";
			}

			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::results_
				&&res->get_result_code() != status::status_) {
				return "";
			}

			const auto results = res->get_results();

			return ((!results.empty()) ? std::move(results[0]) : "");
		}

		//最后一个元素移动到另外一个list去
		//timeout:0永远等待
		virtual std::string list_brpoplpush(std::string&& src_key, std::string&& dst_key, int timeout = 0)
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(cpp_redis::brpoplpush), std::forward<std::string>(src_key),
				std::forward<std::string>(dst_key), std::move(unit::int_to_string(timeout)));
			if (!socket_->send_msg(std::move(msg))) {
				return "";
			}

			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::results_
				&&res->get_result_code() != status::status_) {
				return "";
			}

			const auto results = res->get_results();

			return ((!results.empty()) ? std::move(results[0]) : "");
		}

		virtual int list_insert_before(std::string&& key, std::string&& dst_value, std::string&& insert_value)
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(cpp_redis::list_insert), std::forward<std::string>(key),"before",
				std::forward<std::string>(dst_value), std::forward<std::string>(insert_value));
			if (!socket_->send_msg(std::move(msg))) {
				return 0;
			}

			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::int_result_
				&&res->get_result_code() != status::status_)			{
				return 0;
			}

			const auto results = res->get_int_results();

			return ((!results.empty() && results[0]!=-1) ? results[0] : 0);
		}

		virtual int list_insert_after(std::string&& key, std::string&& dst_value, std::string&& insert_value)
		{
			check_args();

			std::string msg = request_->req_n_key(request_->get_cmd(cpp_redis::list_insert), std::forward<std::string>(key),"after",
				std::forward<std::string>(dst_value), std::forward<std::string>(insert_value));
			if (!socket_->send_msg(std::move(msg))) {
				return 0;
			}

			const auto res = socket_->get_responese();
			if (res->get_result_code() != status::int_result_
				&&res->get_result_code() != status::status_){
				return 0;
			}

			const auto results = res->get_int_results();

			return ((!results.empty()&&results[0]!=-1) ? results[0]:0);
		}
	};
}

#endif // cpp_redis_list_h__
