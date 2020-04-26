#ifndef client_h__
#define client_h__

#include "cpp_redis_list.hpp"
#include "cpp_redis_string.hpp"
#include "cpp_set_client.hpp"
#include "cpp_hash_client.hpp"
#include "cpp_zset_client.hpp"

namespace cpp_redis {
	template<typename type >
	class redis_client
	{
	public:
		redis_client() {
#if (_MSC_VER >= 1700 && _MSC_VER <= 1900) //vs2012-vs2015
			if (constexpr (is_sting_)) {
				request_type_ = request_type::string_request;
			}
			else if (constexpr (is_list_)) {
				request_type_ = request_type::list_request;
			}
			else if (constexpr (is_set_)) {
				request_type_ = request_type::set_request;
			}
			else if (constexpr (is_zset)) {
				request_type_ = request_type::zset_request;
			}
			else if (constexpr (is_hash)) {
				request_type_ = request_type::hash_request;
			}
			else if (constexpr(is_none)) {
				request_type_ = request_type_::none;
			}
#else
			if constexpr (is_sting_) {
				request_type_ = request_type::string_request;
			}
			else if constexpr (is_list_) {
				request_type_ = request_type::list_request;
			}
			else if constexpr (is_set_) {
				request_type_ = request_type::set_request;
			}
			else if constexpr (is_zset) {
				request_type_ = request_type::zset_request;
			}
			else if constexpr (is_hash) {
				request_type_ = request_type::hash_request;
			}
			else if constexpr (is_none) {
				request_type_ = request_type_::none;
			}
#endif
			create_object();
		}

		~redis_client() = default;

		std::string get_current_error() {
			if (client_ == nullptr) {
				return "";
			}

			return client_->get_current_error();
		}

		bool set_db_num(int&& num)
		{
			if (client_ == nullptr) {
				return false;
			}

			return client_->set_db_num(std::forward<int>(num));
		}

		bool auth(std::string&& password)
		{
			if (client_== nullptr) {
				return false;
			}

			return client_->auth(std::forward<std::string>(password));
		}

		template<typename...Args>
		constexpr bool connect_to(std::string&& ip, Args&&...args) {
			if (client_ == nullptr) {
				return false;
			}

			return client_->connect_to(std::forward<std::string>(ip), std::forward<Args>(args)...);
		}

		bool delete_key(std::string&& key)
		{
			if (client_ == nullptr || key.empty()) {
				return false;
			}

			return client_->delete_key(std::forward<std::string>(key));
		}

		bool is_key_exist(std::string&& key)
		{
			if (client_  == nullptr || key.empty()) {
				return false;
			}

			return client_->is_key_exist(std::forward<std::string>(key));
		}

		bool expire(std::string&& key, std::size_t seconds)
		{
			if (client_ == nullptr || key.empty()) {
				return false;
			}

			return client_->expire(std::forward<std::string>(key), seconds);
		}

		bool expire_at(std::string&& key, std::size_t unix_timestamp)
		{
			if (client_ == nullptr || key.empty) {
				return false;
			}

			return client_->expire_at(std::forward<std::string>(key), unix_timestamp);
		}

		int  remainder_ttl(std::string&& key)
		{
			if (client_  == nullptr || key.empty()) {
				return INT32_MAX;
			}

			return client_->remainder_ttl(std::forward<std::string>(key));
		}

		bool setex(std::string&& key, std::string&& value, size_t seconds)
		{
			static_assert(is_sting_, "This API Support String Request");

			if (client_ == nullptr || key.empty()) {
				return false;
			}

			return client_->setex(std::forward<std::string>(key), 
				std::forward<std::string>(value),unit::int_to_string(seconds));
		}

		bool psetex(std::string&& key, std::string&& value, size_t milliseconds)
		{
			static_assert(is_sting_, "This API Support String Request");

			if (client_ == nullptr || key.empty()) {
				return false;
			}

			return client_->psetex(std::forward<std::string>(key), 
				std::forward<std::string>(value),unit::int_to_string(milliseconds));
		}

		//提高这以下四个接口，主要防止，有些版本不支持setex的接口
		bool set_has_seconds(std::string&& key, std::string&& value,std::size_t seconds)
		{
			static_assert(is_sting_, "This API Support String Request");

			if (client_ == nullptr || key.empty()) {
				return false;
			}

			return client_->set_has_seconds(std::forward<std::string>(key),
				std::forward<std::string>(value), unit::int_to_string(seconds));
		}

		//此接口相当于setnx 加上秒，is_exist:true(NX) false(XX)
		bool set_has_seconds_if(std::string&& key, std::string&& value, std::size_t seconds,bool is_exist)
		{
			static_assert(is_sting_, "This API Support String Request");

			if (client_ == nullptr || key.empty()) {
				return false;
			}

			return client_->set_has_seconds_if(std::forward<std::string>(key),
				std::forward<std::string>(value), unit::int_to_string(seconds),is_exist);
		}
	
		bool set_has_milliseconds(std::string&& key, std::string&& value, std::size_t milliseconds)
		{
			static_assert(is_sting_, "This API Support String Request");

			if (client_ == nullptr || key.empty()) {
				return false;
			}


			return client_->set_has_milliseconds(std::forward<std::string>(key),
				std::forward<std::string>(value), unit::int_to_string(milliseconds));
		}

		//is_exist:true(NX)false(XX)
		bool set_has_milliseconds_if(std::string&& key, std::string&& value, std::size_t milliseconds,bool is_exist)
		{
			static_assert(is_sting_, "This API Support String Request");

			if (client_ == nullptr || key.empty()) {
				return false;
			}


			return client_->set_has_milliseconds_if(std::forward<std::string>(key),
				std::forward<std::string>(value), unit::int_to_string(milliseconds), is_exist);
		}

		bool set(std::string&& key, std::string&& value)
		{
			static_assert(is_sting_, "This API Support String Request");
			if (client_ == nullptr || key.empty()) {
				return false;
			}

			return client_->set(std::forward<std::string>(key), std::forward<std::string>(value));
		}

		std::string get_range(std::string&& key,int start,int end)
		{
			static_assert(is_sting_, "This API Support String Request");
			if (client_ == nullptr || key.empty()) {
				return "";
			}

			return client_->get_range(std::forward<std::string>(key),
				unit::int_to_string(start),unit::int_to_string(end));
		}

		int incr(std::string&& key)
		{
			static_assert(is_sting_, "This API Support String Request");
			if (client_ == nullptr || key.empty()) {
				return -1;
			}

			return client_->incr(std::forward<std::string>(key));
		}

		//若key不存在直接创建，并执行增加
		int  incr_by_increment(std::string&& key, int increment)
		{
			static_assert(is_sting_, "This API Support String Request");
			if (client_ == nullptr || key.empty()) {
				return -1;
			}

			return client_->incr_by_increment(std::forward<std::string>(key),unit::int_to_string(increment));
		}

		std::string incr_by_float(std::string&& key,float increment)
		{
			static_assert(is_sting_, "This API Support String Request");
			if (client_ == nullptr || key.empty()) {
				return "";
			}

			return client_->incr_by_float(std::forward<std::string>(key), unit::float_to_string(increment));
		}

		int decr(std::string&& key)
		{
			static_assert(is_sting_, "This API Support String Request");
			if (client_ == nullptr || key.empty()) {
				return -1;
			}

			return client_->decr(std::forward<std::string>(key));
		}

		virtual int decr_increment(std::string&& key,int increment)
		{
			static_assert(is_sting_, "This API Support String Request");
			if (client_ == nullptr || key.empty()) {
				return -1 ;
			}

			return client_->decr_increment(std::forward<std::string>(key),unit::int_to_string(increment));
		}

		std::string get_reflect_value(std::string&& key)
		{
			static_assert(is_sting_, "This API Support String Request");
			if (client_  == nullptr || key.empty()) {
				return "";
			}

			return client_->get_reflect_value(std::forward<std::string>(key));
		}

		std::string get_set_key(std::string&& key, std::string&& value)
		{
			static_assert(is_sting_, "This API Support String Request");
			if (client_ == nullptr || key.empty()) {
				return "";
			}

			return client_->get_set_key(std::forward<std::string>(key), std::forward<std::string>(value));
		}

		std::string substr_reflect_value(std::string&& key, int start, int end)
		{
			static_assert(is_sting_, "This API Support String Request");
			if (client_ == nullptr || key.empty()) {
				return "";
			}

			return client_->substr_reflect_value(std::forward<std::string>(key), start, end);
		}

		template<typename...Args>
		VALUES multi_get_keys(Args&&...key)
		{
			constexpr auto Size = sizeof...(key);
			static_assert(is_sting_, "This API Support String Request");
			auto ptr = std::dynamic_pointer_cast<string_client>(client_);
			if (ptr == nullptr || Size ==0) {
				return{};
			}

			return ptr->multi_get_keys(std::forward<Args>(key)...);
		}

		template<typename...Args>
		bool multi_set_keys(Args&&...key_value)
		{
			constexpr auto Size = sizeof...(key_value);
			static_assert(is_sting_, "This API Support String Request");
			auto ptr = std::dynamic_pointer_cast<string_client>(client_);
			if (ptr == nullptr|| Size ==0) {
				return{};
			}

			return ptr->multi_set_keys(std::forward<Args>(key_value)...);
		}

		template<typename...Args>
		int multi_set_if_not_set(Args...key_value)
		{
			constexpr auto Size = sizeof...(key_value);
			static_assert(is_sting_, "This API Support String Request");
			auto ptr = std::dynamic_pointer_cast<string_client>(client_);
			if (ptr == nullptr || Size == 0) {
				return{};
			}

			return ptr->multi_set_if_not_set(std::forward<Args>(key_value)...);
		}

		//key不存，会直接创建key
		int append_value(std::string&& key, std::string&& append_value)
		{
			static_assert(is_sting_, "This API Support String Request");
			if (client_ == nullptr || key.empty() || append_value.empty()) {
				return 1 ;
			}

			return client_->append_value(std::forward<std::string>(key), 
				std::forward<std::string>(append_value));
		}

		std::tuple<bool, int> list_rpush(std::string&& key, std::string&& value)
		{
			static_assert(is_list_, "This API Support List Request");

			if (client_ == nullptr || 
				key.empty() || value.empty()) {
				return { false,-1 };
			}

			return client_->list_rpush(std::forward<std::string>(key), std::forward<std::string>(value));
		}

		std::tuple<bool, int> list_rpush_if(std::string&& key, std::string&& value)
		{
			static_assert(is_list_, "This API Support List Request");

			if (client_ == nullptr || 
				key.empty() || value.empty) {
				return { false,-1 };
			}

			return client_->list_rpush_if(std::forward<std::string>(key), std::forward<std::string>(value));
		}

		std::tuple<bool, int> list_lpush(std::string&& key, std::string&& value)
		{
			static_assert(is_list_, "This API Support List Request");

			if (client_ == nullptr || 
				key.empty() || value.empty()) {
				return { false,-1 };
			}

			return client_->list_lpush(std::forward<std::string>(key), std::forward<std::string>(value));
		}

		std::tuple<bool, int> list_lpush_if(std::string&& key, std::string&& value)
		{
			static_assert(is_list_, "This API Support List Request");

			if (client_ == nullptr || 
				key.empty() || value.empty()) {
				return { false,-1 };
			}

			return client_->list_lpush_if(std::forward<std::string>(key), std::forward<std::string>(value));
		}

		int32_t list_size(std::string&& key)
		{
			static_assert(is_list_, "This API Support List Request");

			if (client_== nullptr || key.empty()) {
				return 0;
			}

			return client_->list_size(std::forward<std::string>(key));
		}

		RESULTS_TYPE list_range(std::string&& key, int start, int end)
		{
			static_assert(is_list_, "This API Support List Request");

			if (client_ == nullptr || key.empty()) {
				return {};
			}

			return client_->list_range(std::forward<std::string>(key), start, end);
		}

		std::string list_lpop(std::string&& key)
		{
			static_assert(is_list_, "This API Support List Request");

			if (client_  == nullptr) {
				return "";
			}

			return client_->list_lpop(std::forward<std::string>(key));
		}

		std::string list_rpop(std::string&& key)
		{
			static_assert(is_list_, "This API Support List Request");

			if (client_  == nullptr || key.empty()) {
				return "";
			}

			return client_->list_rpop(std::forward<std::string>(key));
		}

		std::string list_brpop(std::string&& key, size_t timeout = 0)
		{
			static_assert(is_list_, "This API Support List Request");

			if (client_ == nullptr || key.empty()) {
				return "";
			}

			return client_->list_brpop(std::forward<std::string>(key), timeout);
		}

		std::string list_blpop(std::string&& key, size_t timeout = 0)
		{
			static_assert(is_list_, "This API Support List Request");

			if (client_== nullptr || key.empty()) {
				return "";
			}

			return  client_->list_blpop(std::forward<std::string>(key), timeout);
		}

		bool list_trim(std::string&& key, int start, int end)
		{
			static_assert(is_list_, "This API Support List Request");

			if (client_  == nullptr || key.empty()) {
				return "";
			}

			return client_->list_trim(std::forward<std::string>(key), start, end);
		}

		std::string list_index(std::string&& key, int index)
		{
			static_assert(is_list_, "This API Support List Request");

			if (client_ == nullptr || key.empty()) {
				return "";
			}

			return client_->list_index(std::forward<std::string>(key), index);
		}

		bool list_set(std::string&& key, std::string&& value, int index)
		{
			static_assert(is_list_, "This API Support List Request");

			if (client_  == nullptr || key.empty()) {
				return false;
			}

			return client_->list_set(std::forward<std::string>(key), 
				std::forward<std::string>(value),unit::int_to_string(index));
		}


		//没有移除就为0 ，有移除就大于0，count表示list的起始位置,一般从0开始删除,-1表示最后一个元素删除
		//如果从0开始删除，有多少删除多少
		//如果从-1开始删除,就只会删除一个元素
		//数量为>=|count|
		std::tuple<bool, int> list_del_elem(std::string&& key, std::string&& value, int count = 0)
		{
			static_assert(is_list_, "This API Support List Request");

			if (client_  == nullptr || key.empty()) {
				return { false,-1 };
			}

			return client_->list_del_elem(std::forward<std::string>(key), 
				std::forward<std::string>(value),unit::int_to_string(count));
		}

		std::string list_rpoplpush(std::string&& src_key, std::string&& dst_key)
		{
			static_assert(is_list_, "This API Support List Request");

			if (client_   == nullptr ||
				src_key.empty() || dst_key.empty()) {
				return "";
			}

			return client_->list_rpoplpush(std::forward<std::string>(src_key), std::forward<std::string>(dst_key));
		}

		std::string list_brpoplpush(std::string&& src_key, std::string&& dst_key, int timeout = 0)
		{
			static_assert(is_list_, "This API Support List Request");

			if (client_ == nullptr ||
				src_key.empty() || dst_key.empty()) {
				return "";
			}

			return client_->list_brpoplpush(std::forward<std::string>(src_key), std::forward<std::string>(dst_key), timeout);
		}

		int list_insert_before(std::string&& key, std::string&& dst_value, std::string&& insert_value)
		{
			static_assert(is_list_, "This API Support List Request");

			if (client_ == nullptr || key.empty()) {
				return -1;
			}

			return client_->list_insert_before(std::forward<std::string>(key),
				std::forward<std::string>(dst_value), std::forward<std::string>(insert_value));
		}

		int list_insert_after(std::string&& key, std::string&& dst_value, std::string&& insert_value)
		{
			static_assert(is_list_, "This API Support List Request");

			if (client_== nullptr || key.empty()) {
				return -1;
			}

			return client_->list_insert_after(std::forward<std::string>(key),
				std::forward<std::string>(dst_value), std::forward<std::string>(insert_value));
		}

		template<typename...Args>
		std::tuple<bool, int>set_add(std::string&& key, Args&&...args)
		{
			constexpr auto Size = sizeof...(args);
			static_assert(is_set_, "This API Support Set Request");
			auto ptr = std::dynamic_pointer_cast<set_client>(client_);
			if (ptr  == nullptr || 
				key.empty() || Size     ==0) {
				return { false,-1 };
			}

			return ptr->set_add(std::forward<std::string>(key), std::forward<Args>(args)...);
		}

		template<typename...Args>
		std::tuple<bool, int>set_delete_elem(std::string&& key, Args&&...args)
		{
			constexpr auto Size = sizeof...(args);
			static_assert(is_set_, "This API Support Set Request");
			auto ptr = std::dynamic_pointer_cast<set_client>(client_);
			if (ptr  == nullptr 
				|| key.empty() || Size  ==0) {
				return { false,-1 };
			}

			return ptr->set_delete_elem(std::forward<std::string>(key), std::forward<Args>(args)...);
		}

		bool set_is_member(std::string&& key, std::string&& value)
		{
			static_assert(is_set_, "This API Support Set Request");
			if (client_  == nullptr || key.empty() || value.empty()) {
				return false;
			}

			return client_->set_is_member(std::forward<std::string>(key), std::forward<std::string>(value));
		}

		std::string set_rdel_elem(std::string&& key)
		{
			static_assert(is_set_, "This API Support Set Request");
			if (client_ == nullptr || key.empty()) {
				return "";
			}

			return client_->set_rdel_elem(std::forward<std::string>(key));
		}

		//只是随机，不会发生删除
		RESULTS_TYPE set_rand_elem(std::string&& key, int count)
		{
			static_assert(is_set_, "This API Support Set Request");
			if (client_  == nullptr || key.empty()) {
				return {};
			}

			return client_->set_rand_elem(std::forward<std::string>(key), count);
		}

		bool set_move_elem(std::string&& src_key, std::string&& dst_key, std::string&& member)
		{
			static_assert(is_set_, "This API Support Set Request");
			if (client_== nullptr ||
				src_key.empty() || dst_key.empty() ||member.empty()) {
				return false;
			}

			return client_->set_move_elem(std::forward<std::string>(src_key),
				std::forward<std::string>(dst_key), std::forward<std::string>(member));
		}

		size_t set_get_size(std::string&& key)
		{
			static_assert(is_set_, "This API Support Set Request");
			if (client_ == nullptr || key.empty()) {
				return 0;

			}
			return  client_->set_get_size(std::forward<std::string>(key));
		}

		RESULTS_TYPE set_get_all_member(std::string&& key)
		{
			static_assert(is_set_, "This API Support Set Request");
			if (client_== nullptr || key.empty()) {
				return {};

			}

			return  client_->set_get_all_member(std::forward<std::string>(key));
		}

		//求集合的交集，如果一个为空，就返回空
		template<typename...Args>
		RESULTS_TYPE set_sinter(Args&&...keys)
		{
			constexpr auto Size = sizeof...(keys);
			static_assert(is_set_, "This API Support Set Request");
			auto ptr = std::dynamic_pointer_cast<set_client>(client_);
			if (ptr == nullptr || Size ==0) {
				return {};
			}

			return ptr->set_sinter(std::forward<Args>(keys)...);
		}

		//求集合的交集，如果一个为空，就返回空,并保存另外一个地方
		template<typename...Args>
		int set_inter_store(std::string&& dst_key, Args&&...keys)
		{
			constexpr auto Size = sizeof...(keys);
			static_assert(is_set_, "This API Support Set Request");
			auto ptr = std::dynamic_pointer_cast<set_client>(client_);
			if (ptr   == nullptr ||
				Size  ==0 || dst_key.empty()) {
				return -1;
			}

			return ptr->set_inter_store(std::forward<std::string>(dst_key), std::forward<Args>(keys)...);
		}

		//求集合的并集合,不存在key就视为空 
		//(返回一个集合的全部成员，该集合是所有给定集合的并集)
		template<typename...Args>
		RESULTS_TYPE set_union(Args&&...key)
		{
			constexpr auto Size = sizeof...(key);
			static_assert(is_set_, "This API Support Set Request");
			auto ptr = std::dynamic_pointer_cast<set_client>(client_);
			if (ptr  == nullptr || Size ==0) {
				return {};
			}

			return ptr->set_union(std::forward<Args>(key)...);
		}
		//求集合的并集，如果一个为空，就返回空,并保存另外一个地方
		//(返回一个集合的全部成员，该集合是所有给定集合的并集)
		template<typename...Args>
		int set_union_store(std::string&& dst_key, Args&&...keys)
		{
			constexpr auto Size = sizeof...(keys);
			static_assert(is_set_, "This API Support Set Request");
			auto ptr = std::dynamic_pointer_cast<set_client>(client_);
			if (ptr == nullptr || 
				dst_key.empty() || Size ==0) {
				return -1;
			}

			return ptr->set_union_store(std::forward<std::string>(dst_key), std::forward<Args>(keys)...);
		}

		//返回一个集合的全部成员，该集合是所有给定集合之间的差集。
		//不存在的 key 被视为空集。
		template<typename...Args>
		RESULTS_TYPE set_diff(Args&&...key)
		{
			constexpr auto Size = sizeof...(key);
			static_assert(is_set_, "This API Support Set Request");
			auto ptr = std::dynamic_pointer_cast<set_client>(client_);
			if (ptr == nullptr || Size ==0) {
				return{};
			}

			return ptr->set_diff(std::forward<Args>(key)...);
		}

		template<typename...Args>
		int set_diff_store(std::string&& dst_key, Args&&...key)
		{
			constexpr auto Size = sizeof...(key);
			static_assert(is_set_, "This API Support Set Request");
			auto ptr = std::dynamic_pointer_cast<set_client>(client_);
			if (ptr == nullptr || 
				dst_key.empty() || Size == 0) {
				return -1;
			}

			return ptr->set_diff_store(std::forward<std::string>(dst_key), std::forward<Args>(key)...);
		}

		//ZADD key score member [[score member] [score member] …]
		template<typename...Args>
		int zset_add(std::string&& key, Args&&...args) {
			constexpr auto Size = sizeof...(args)+1;
			static_assert(is_zset, "This API Support ZSet Request");

			keys_.push_back(std::forward<std::string>(key));
			make_keys(std::forward<Args>(args)...);

			if (client_ == nullptr ||
				keys_.size() == 1 || keys_.size()!= Size) {
				return -1;
			}

			return client_->zset_add(std::move(keys_));
		}

		//获取指定zset成员的值
		template<typename T>
		std::string zset_score(std::string&& key, T&& member)
		{
			check_pass_args(std::forward<T>(member));
			static_assert(is_zset, "This API Support ZSet Request");

			if (client_== nullptr){
				return "";
			}

			std::string value = to_string(member);

			if (value.empty()){
				return "";
			}

			return client_->zset_score(std::forward<std::string>(key), std::move(value));
		}

		//获取指定zset成员增加值
		//member:就是数据库中的value字段
		template<typename T>
		std::string zset_incrby(std::string&& key, int increment, T&& member)
		{
			check_pass_args(std::forward<T>(member));
			static_assert(is_zset, "This API Support ZSet Request");

			if (client_ == nullptr){
				return "";
			}

			std::string value = to_string(member);

			if (value.empty()){
				return "";
			}

			return client_->zset_incrby(std::forward<std::string>(key), unit::int_to_string(increment), std::move(value));

		}

		//返回有序集 key的总数
		int zset_counts(std::string&& key)
		{
			static_assert(is_zset, "This API Support ZSet Request");
			if (client_ == nullptr){
				return -1;
			}

			return client_->zset_card(std::forward<std::string>(key));
		}

		//返回有序集 key的 min和max区间总数
		int zset_range_counts(std::string&& key, int min, int max)
		{
			static_assert(is_zset, "This API Support ZSet Request");
			if (client_  == nullptr){
				return -1;
			}

			return client_->zset_count(std::forward<std::string>(key), unit::int_to_string(min), unit::int_to_string(max));
		}

		//返回列表以 value1, score1, ..., valueN, scoreN 的格式表示。
		//with_scores:false不带score返回
		//递增排序
		RESULTS_TYPE zset_range(std::string&& key, int begin, int end, bool with_scores = true)
		{
			static_assert(is_zset, "This API Support ZSet Request");

			if (client_ == nullptr){
				return { {} };
			}

			return client_->zset_range(std::forward<std::string>(key), unit::int_to_string(begin), unit::int_to_string(end), with_scores);
		}

		//其中成员的位置按 score 值递减(从大到小)来排列,命令和zrange相似
		//递减排序
		//字典序的逆序
		RESULTS_TYPE zset_rerange(std::string&& key, int begin, int end, bool with_scores = true)
		{
			static_assert(is_zset, "This API Support ZSet Request");
			if (client_ == nullptr){
				return { {} };
			}

			return client_->zset_revrange(std::forward<std::string>(key), unit::int_to_string(begin), unit::int_to_string(end), with_scores);
		}

		//求区间内的score排序
		RESULTS_TYPE zset_range_score(std::string&& key, int min, int max,
			bool with_scores = true, bool limit = false, int limit_min = 0, int limit_max = 1)
		{
			static_assert(is_zset, "This API Support ZSet Request");

			if (client_ == nullptr) {
				return { {} };
			}

			return client_->zset_range_score(std::forward<std::string>(key), unit::int_to_string(min),
				unit::int_to_string(max), with_scores, limit, unit::int_to_string(limit_min), unit::int_to_string(limit_max));
		}

		//求区间内的score从数据库中逆向取值并排序
		RESULTS_TYPE zset_revrange_score(std::string&& key, int max, int min,
			bool with_scores = true, bool limit = false, int limit_min = 0, int limit_max = 1)
		{
			static_assert(is_zset, "This API Support ZSet Request");

			if (client_== nullptr) {
				return { {} };
			}

			return client_->zset_revrange_score(std::forward<std::string>(key), unit::int_to_string(max),
				unit::int_to_string(min), with_scores, limit, unit::int_to_string(limit_min), unit::int_to_string(limit_max));
		}

		//升序，从0开始
		int zset_rank(std::string&& key, std::string&& member)
		{
			static_assert(is_zset, "This API Support ZSet Request");

			if (client_== nullptr) {
				return -1;
			}

			return client_->zset_rank(std::forward<std::string>(key), std::forward<std::string>(member));
		}

		//降序，从0开始，到从大的开始排
		int zset_revrank(std::string&& key, std::string&& member)
		{
			static_assert(is_zset, "This API Support ZSet Request");

			if (client_ == nullptr) {
				return -1;
			}

			return client_->zset_revrank(std::forward<std::string>(key), std::forward<std::string>(member));
		}

		//ZREM key score member [[score member][score member] …]
		template<typename...Args>
		bool zset_rem(std::string&& key, Args&&...args)
		{
			constexpr auto Size = sizeof...(args) + 1;
			static_assert(is_zset, "This API Support ZSet Request");
			auto ptr = std::dynamic_pointer_cast<zset_client>(client_);
			if (ptr == nullptr) {
				return false;
			}

			keys_.push_back(std::forward<std::string>(key));
			make_keys(std::forward<Args>(args)...);

			if (keys_.size() ==1 || keys_.size() !=Size){
				return false;
			}

			return ptr->zset_rem(std::move(keys_));
		}

		//移除有序集 key 中，指定排名(rank)区间内的所有成员
		int zset_remrangeby_rank(std::string&& key,int begin,int end)
		{
			static_assert(is_zset, "This API Support ZSet Request");

			if (client_== nullptr) {
				return -1;
			}

			return client_->zset_remrangeby_rank(std::forward<std::string>(key), unit::int_to_string(begin), unit::int_to_string(end));
		}

		//移除有序集 key 中，所有 score 值介于 min 和 max 之间(包括等于 min 或 max )的成员。
		int zset_remrangebyscore(std::string&& key,int min,int max)
		{
			static_assert(is_zset, "This API Support ZSet Request");
			
			if (client_== nullptr) {
				return -1;
			}

			return client_->zset_remrangebyscore(std::forward<std::string>(key), unit::int_to_string(min), unit::int_to_string(max));
		}
		
		//合法的 min 和 max 参数必须包含(或者[， 其中(表示开区间（指定的值不会被包含在范围之内）， 而[则表示闭区间（指定的值会被包含在范围之内）。
		//特殊值 + 和 - 在 min 参数以及 max 参数中具有特殊的意义， 其中 + 表示正无限， 而 - 表示负无限。 
		//因此， 向一个所有成员的分值都相同的有序集合发送命令 ZRANGEBYLEX <zset> -+， 命令将返回有序集合中的所有元素。

		RESULTS_TYPE zset_rangebylex(std::string&& key,std::string&&min,std::string&& max,bool limit=false,int limit_min=0,int limit_max=1)
		{
			static_assert(is_zset, "This API Support ZSet Request");

			if (client_ == nullptr){
				return { {}};
			}

			return client_->zset_rangebylex(std::forward<std::string>(key),std::forward<std::string>(min),std::forward<std::string>(max), 
				limit,unit::int_to_string(limit_max),unit::int_to_string(limit_max));
		}

		//此接口和上面接口一样，只是返回数量
		int zset_lexcount(std::string&& key, std::string&& min, std::string&& max)
		{
			static_assert(is_zset, "This API Support ZSet Request");

			if (client_== nullptr) {
				return -1;
			}

			return client_->zset_lexcount(std::forward<std::string>(key), std::forward<std::string>(min), std::forward<std::string>(max));
		}

		//特殊符号，删除元素
		int zset_remrangebylex(std::string&& key, std::string&& min, std::string&& max)
		{
			static_assert(is_zset, "This API Support ZSet Request");

			if (client_ == nullptr) {
				return -1;
			}

			return client_->zset_remrangebylex(std::forward<std::string>(key),std::forward<std::string>(min), std::forward<std::string>(max));
		}
		
		//计算给定的一个或多个有序集的并集，其中给定 key 的数量必须以 num_keys 参数指定，
		//并将该并集(结果集)储存到 dst_store_key ,其中num_keys,参与计算key的个数
		//默认情况下，结果集中某个成员的 score 值是所有给定集下该成员 score 值之 和
		//函数使用介绍，参数1：目标存储的key,参数 2，求和的方式，参数3：N个key 跟上权重,具体使用可以看官方命令
		template<typename...Args>
		int zset_union_store(std::string&& dst_store_key,int num_keys,aggregate_mothod mothod,
			int weight_min,int weight_max,Args&&...args)
		{
			constexpr auto Size = sizeof...(args) + 2;
			static_assert(is_zset, "This API Support ZSet Request");

			keys_.push_back(std::forward<std::string>(dst_store_key));
			keys_.emplace_back(unit::int_to_string(num_keys));

			make_keys(std::forward<Args>(args)...);

			if (client_ == nullptr || 
				keys_.size() == 1 || keys_.size() != Size) {
				return -1;
			}

			keys_.emplace_back("WEIGHTS");
			keys_.emplace_back(unit::int_to_string(weight_min));
			keys_.emplace_back(unit::int_to_string(weight_max));
			return client_->zset_union_store(std::move(keys_), mothod);
		}

		//求交集，具体参数可以参考 zset_union_store接口
		template<typename...Args>
		int zset_inter_store(std::string&& dst_store_key, int num_keys, aggregate_mothod mothod,
			int weight_min, int weight_max, Args&&...args)
		{
			constexpr auto Size = sizeof...(args) + 2;
			static_assert(is_zset, "This API Support ZSet Request");

			keys_.push_back(std::forward<std::string>(dst_store_key));
			keys_.emplace_back(unit::int_to_string(num_keys));

			make_keys(std::forward<Args>(args)...);

			if (client_ == nullptr ||
				keys_.size() == 1 || keys_.size() != Size) {
				return -1;
			}

			keys_.emplace_back("WEIGHTS");
			keys_.emplace_back(unit::int_to_string(weight_min));
			keys_.emplace_back(unit::int_to_string(weight_max));
			return client_->zset_inter_store(std::move(keys_), mothod);
		}

		//当 HSET 命令在哈希表中新创建 field 域并成功为它设置值时， 命令返回 1 
		//如果域 field 已经存在于哈希表， 并且 HSET 命令成功使用新值覆盖了它的旧值， 那么命令返回 0
		template<typename T1,typename T2>
		int hash_set(std::string&&key,T1 &&field, T2&& value)
		{
			static_assert(is_hash, "This API Support hash Request");

			if (client_ == nullptr || key.empty()){
				return -1;
			}

			return client_->hash_set(std::forward<std::string>(key), 
				hash_build_string(std::forward<T1>(field)), hash_build_string(std::forward<T2>(value)));
		}

		//HSETNX 命令在设置成功时返回 1 ， 在给定域已经存在而放弃执行设置操作时返回 0.
		//如果给定域已经存在于哈希表当中， 那么命令将放弃执行设置操作。
		template<typename T1, typename T2>
		int hash_setx(std::string&& key, T1&& field, T2&& value)
		{
			static_assert(is_hash, "This API Support hash Request");

			if (client_ == nullptr || key.empty()) {
				return -1;
			}

			return client_->hash_setx(std::forward<std::string>(key),
				hash_build_string(std::forward<T1>(field)), hash_build_string(std::forward<T2>(value)));
		}

		template<typename T>
		int hash_exists(std::string&& key,T&& field)
		{
			static_assert(is_hash, "This API Support hash Request");

			if (client_ == nullptr || key.empty()) {
				return -1;
			}

			return client_->hash_exists(std::forward<std::string>(key),hash_build_string(std::forward<T>(field)));
		}

		template<typename T>
		std::string hash_get(std::string&& key,T&& field)
		{
			static_assert(is_hash, "This API Support hash Request");

			if (client_ == nullptr && key.empty()) {
				return "";
			}

			return client_->hash_get(std::forward<std::string>(key),hash_build_string(std::forward<T>(field)));
		}
		
		template<typename...Args>
		int hash_del(std::string&& key, Args&&...fields)
		{
			static_assert(is_hash, "This API Support hash Request");
			constexpr auto Size = sizeof...(fields)+1;

			if (client_ == nullptr || key.empty()){
				return -1;
			}

			keys_.push_back(std::forward<std::string>(key));
			hash_make_keys(std::forward<Args>(fields)...);
			if ( keys_.empty() || keys_.size() != Size){
				return -1;
			}

			return client_->hash_del(std::move(keys_));
		}

		int hash_len(std::string&& key)
		{
			static_assert(is_hash, "This API Support hash Request");
			if (client_ == nullptr || key.empty()){
				return 0;
			}

			return client_->hash_len(std::forward<std::string>(key));
		}

		template<typename T>
		int hash_strlen(std::string&& key,T&& field)
		{
			static_assert(is_hash, "This API Support hash Request");
			if (client_== nullptr) {
				return 0;
			}

			return client_->hash_strlen(std::forward<std::string>(key), 
				hash_build_string(std::forward<T>(field)));
		}

		//返回增加值
		//一个新的哈希表被创建并执行HINCRBY 命令(注意地方)
		template<typename T>
		int hash_incrby(std::string&& key, T&& field, int increment)
		{
			static_assert(is_hash, "This API Support hash Request");
			if (client_  == nullptr || key.empty()) {
				return 0;
			}

			return client_->hash_incrby(std::forward<std::string>(key),hash_build_string(std::forward<T>(field)),
				unit::int_to_string(increment));
		}

		template<typename T>
		std::string hash_incrby_float(std::string&& key, T&& field, double increment)
		{
			static_assert(is_hash, "This API Support hash Request");
			if (client_ == nullptr || key.empty()) {
				return "";
			}

			return client_->hash_incrby_float(std::forward<std::string>(key), hash_build_string(std::forward<T>(field)),
				unit::double_to_string(increment));
		}

		template<typename T>
		std::string hash_incrby_float(std::string&& key, T&& field, float increment)
		{
			static_assert(is_hash, "This API Support hash Request");
			if (client_ == nullptr || key.empty()) {
				return "";
			}

			return client_->hash_incrby_float(std::forward<std::string>(key), hash_build_string(std::forward<T>(field)),
				unit::float_to_string(increment));
		}

		template<typename...Args>
		bool hash_mset(std::string&& key, Args&&...keys)
		{
			const auto Size = sizeof...(keys) + 1;
			static_assert(is_hash, "This API Support hash Request");
			if (client_ == nullptr || key.empty()) {
				return false;
			}

			keys_.push_back(std::forward<std::string>(key));
			hash_make_keys(std::forward<Args>(keys)...);

			if (keys_.size() != Size) {
				return false;
			}

			return client_->hash_mset(std::move(keys_));
		}

		template<typename...Args>
		RESULTS_TYPE hash_mget(std::string&& key, Args&&...keys)
		{
			const auto Size = sizeof...(keys) + 1;
			static_assert(is_hash, "This API Support hash Request");
			if (client_ == nullptr || key.empty()) {
				return { {} };
			}

			keys_.push_back(std::forward<std::string>(key));
			hash_make_keys(std::forward<Args>(keys)...);

			if (keys_.size() != Size) {
				return { {} };
			}

			return client_->hash_mget(std::move(keys_));
		}

		//返回所有的keys
		RESULTS_TYPE hash_keys(std::string&& key)
		{
			static_assert(is_hash, "This API Support hash Request");
			if (client_ == nullptr || key.empty()) {
				return { {} };
			}
			
			return client_->hash_keys(std::forward<std::string>(key));
		}

		//返回key中的所有值
		RESULTS_TYPE hash_vals(std::string&& key)
		{
			static_assert(is_hash, "This API Support hash Request");
			if (client_ == nullptr || key.empty()) {
				return { {} };
			}

			return client_->hash_vals(std::forward<std::string>(key));
		}

		//返回key中的域和值
		RESULTS_TYPE hash_get_all(std::string&& key)
		{
			static_assert(is_hash, "This API Support hash Request");
			if (client_ == nullptr || key.empty()) {
				return { {} };
			}

			return client_->hash_get_all(std::forward<std::string>(key));
		}
	private:
		void hash_make_keys()
		{

		}

		template<typename T, typename...Args>
		void hash_make_keys(T&& header, Args&&...args)
		{
			std::string value;
#if (_MSC_VER >= 1700 && _MSC_VER <= 1900) //vs2012-vs2015
#else
			if constexpr(std::is_same<T,bool>::value){
				value = header ? "true" : "false";
			}else{
				value = to_string(header);
			}
#endif
			keys_.push_back(std::move(value));
			hash_make_keys(std::forward<Args>(args)...);

		}

		void make_keys()
		{

		}

		template<typename T,typename...Args>
		void make_keys(T&&header,Args&&...args)
		{
			std::string value = to_string(std::forward<T>(header));

			if (!value.empty()){
				keys_.push_back(std::move(value));
			}

			make_keys(std::forward<Args>(args)...);
		}

		template<typename T>
		std::string to_string(T&& t)
		{
			std::string value;
#if (_MSC_VER >= 1700 && _MSC_VER <= 1900) //vs2012-vs2015
			if (constexpr (std::is_same< std::is_same<typename std::decay<decltype(t)>::type, float>::value)) {
				value = cpp_redis::unit::float_to_string(t);
			}
			else if (constexpr (std::is_same< std::is_same<typename std::decay<decltype(t)>::type, double>::value)) {
				value = cpp_redis::unit::double_to_string(t);
			}
			else if (constexpr (cpp_redis::traits::is_string<T>::value)) {
				value = build_string(t);
			}
			else if (constexpr (cpp_redis::traits::contains<typename std::decay<decltype(t)>::type, std::int16_t,
				std::uint16_t, std::uint32_t, std::int32_t, std::int64_t, std::uint64_t>::value)) {
				value = cpp_redis::unit::int_to_string(t);
			}
			else if (constexpr (std::is_enum<decltype(value)>::value)) {
				value = cpp_redis::unit::int_to_string(static_cast<int>(t));
			}else if (constexpr (cpp_redis::traits::contains<typename std::decay<decltype(t)>::type, char, unsigned char>::value)) {
				value.append("\'");
				value.append(cpp_redis::unit::char_to_str(t));
				value.append("\'");
		    }else if (constexpr (std::is_same<typename std::decay<decltype(t)>::type, std::uint8_t>::value)) {
				value.append("\'");
				value.append(cpp_redis::unit::char_to_str(t));
				value.append("\'");
			}else if (constexpr (std::is_same<typename std::decay<decltype(t)>::type, std::int8_t>::value)) {
				value.append("\'");
				value.append(cpp_redis::unit::char_to_str(t));
				value.append("\'");
			}
#else
			//加上std::decay之后，不管是常引用还是变量都可以变检索到
			if constexpr (std::is_same<typename std::decay<decltype(t)>::type, float>::value) {
				value = cpp_redis::unit::float_to_string(t);
			}else if constexpr (std::is_same<typename std::decay<decltype(t)>::type, double>::value) {
				value = cpp_redis::unit::double_to_string(t);
			}else if  constexpr (cpp_redis::traits::is_string<T>::value) {
				value = build_string(t);
			}else if constexpr (cpp_redis::traits::contains<typename std::decay<decltype(t)>::type,std::int16_t,
				std::uint16_t,std::uint32_t,std::int32_t,std::int64_t,std::uint64_t>::value) {
				value = cpp_redis::unit::int_to_string(t);
			}else if constexpr (std::is_enum<decltype(value)>::value) {
				value = cpp_redis::unit::int_to_string(static_cast<int>(t));
			}else if constexpr (cpp_redis::traits::contains<typename std::decay<decltype(t)>::type,char,unsigned char>::value) {
				value.append("\'");
				value.append(cpp_redis::unit::char_to_str(t));
				value.append("\'");
			}else if constexpr(std::is_same<typename std::decay<decltype(t)>::type,std::uint8_t>::value){
				value.append("\'");
				value.append(cpp_redis::unit::char_to_str(t));
				value.append("\'");
			}else if constexpr (std::is_same<typename std::decay<decltype(t)>::type, std::int8_t>::value) {
				value.append("\'");
				value.append(cpp_redis::unit::char_to_str(t));
				value.append("\'");
			}
#endif
			return std::move(value);
		}
		void create_object()
		{
			if (request_type_           == cpp_redis::string_request) {
				client_ = std::make_shared<string_client>();
			}
			else if (request_type_      == cpp_redis::list_request) {
				client_ = std::make_shared<list_client>();
			}
			else if (request_type_      == cpp_redis::set_request) {
				client_ = std::make_shared<set_client>();
			}
			else if (request_type_      == cpp_redis::zset_request) {
				client_ = std::make_shared<zset_client>();
			}
			else if (request_type_      == cpp_redis::hash_request) {
				client_ = std::make_shared<hash_client>();
			}
			else {
				static_assert(is_none, "Please pass String List set_request Set Hash");
			}
		}

		template<typename T>
		std::string hash_build_string(T&& value)
		{
			std::string str;
#if (_MSC_VER >= 1700 && _MSC_VER <= 1900) //vs2012-vs2015
			if (constexpr (std::is_same<T,bool>::value)){
				str = value ? "true" : "false";
			}
#else
			if constexpr (std::is_same<T,bool>::value){
				str = value ? "true" : "false";
			}else{
				str = to_string(value);
			}

			return std::move(str);
#endif
		}

		template<typename T>
		std::string build_string(T&& value)
		{
			std::string str;

#if (_MSC_VER >= 1700 && _MSC_VER <= 1900) //vs2012-vs2015
			if (constexpr (std::is_same<char*, typename std::decay<T>::type>::value)) {
				size_t size = strlen(value);
				str.resize(size);
				std::copy(value, value + size, str.begin());
			}else if (constexpr (std::is_same<const char*, typename std::decay<T>::type>::value)) {
				size_t size = strlen(value);
				str.resize(size);
				std::copy(value, value + size, str.begin());
			}else if (constexpr (std::is_same<std::string,typename std:decay<T>::type>::value)){
				str = std::move(value);
			}
#else
			if constexpr (std::is_same<char*, typename std::decay<T>::type>::value) {
				size_t size = strlen(value);
				str.resize(size);
				memcpy(&str[0], value, size);
			}
			else if constexpr (std::is_same<const char*, typename std::decay<T>::type>::value) {
				size_t size = strlen(value);
				str.resize(size);
				memcpy(&str[0], value, size);
			}else if constexpr (std::is_same<std::string, typename std::decay<T>::type>::value) {
				str = std::move(value);
			}
#endif
			return std::move(str);
		}

		template<typename T>
		void check_pass_args(T&& value)
		{
#if (_MSC_VER >= 1700 && _MSC_VER <= 1900) //vs2012-vs2015
			if (constexpr (cpp_redis::traits::is_string<T>::value))) {
			     constexpr bool is_string = cpp_redis::traits::is_string<T>::value;
			     static_assert(is_string, "T only support double string float int ");
			}
			else {
				constexpr bool is_none = cpp_redis::traits::contains<T, double, float, int>::value;
				static_assert(is_none, "T only support double string float int ");
			}
#else
			if constexpr (cpp_redis::traits::is_string<T>::value) {
				constexpr bool is_string = cpp_redis::traits::is_string<T>::value;
				static_assert(is_string, "T only support double string float int ");
			}
			else {
				constexpr bool is_none = cpp_redis::traits::contains<T, double, float, int>::value;
				static_assert(is_none, "T only support double string float int ");
			}
#endif
		}
	private:
		static constexpr bool is_sting_ = std::is_same<type, String>::value;
		static constexpr bool is_list_  = std::is_same<type, List>::value;
		static constexpr bool is_set_   = std::is_same<type, Set>::value;
		static constexpr bool is_zset   = std::is_same<type, ZSet>::value;
		static constexpr bool is_hash   = std::is_same<type, Hash>::value;
		static constexpr bool is_none   = cpp_redis::traits::contains<type, String, List, Set, ZSet, Hash>::value;
		int request_type_ = request_type::none;
		std::shared_ptr<client>client_;
		std::vector<std::string>keys_;
	};
}
#endif // client_h__
