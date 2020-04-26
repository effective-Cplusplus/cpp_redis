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

		template<typename T>
		bool delete_key(T&& key)
		{
			if (client_ == nullptr) {
				return false;
			}

			reset();
			any_type_to_string(key);

			if (keys_.empty()){
				return false;
			}

			return client_->delete_key(std::move(keys_[0]));
		}

		template<typename T>
		bool is_key_exist(T&& key)
		{
			if (client_  == nullptr) {
				return false;
			}

			reset();
			any_type_to_string(key);

			if (keys_.empty()) {
				return false;
			}

			return client_->is_key_exist(std::move(keys_[0]));
		}

		template<typename T>
		bool expire(std::string&& key, std::size_t seconds)
		{
			if (client_ == nullptr) {
				return false;
			}

			reset();
			any_type_to_string(key);

			if (keys_.empty()) {
				return false;
			}

			return client_->expire(std::move(keys_[0]), seconds);
		}

		template<typename T>
		bool pexpire(std::string&& key, std::size_t milliseconds)
		{
			if (client_ == nullptr) {
				return false;
			}

			reset();
			any_type_to_string(key);

			if (keys_.empty()) {
				return false;
			}

			return client_->pexpire(std::move(keys_[0]), milliseconds);
		}


		template<typename T>
		bool expire_at(T&& key, std::size_t unix_timestamp)
		{
			if (client_ == nullptr) {
				return false;
			}

			reset();
			any_type_to_string(key);

			if (keys_.empty()) {
				return false;
			}

			return client_->expire_at(std::move(keys_), unix_timestamp);
		}

		template<typename T>
		bool pexpire_at(T&& key, int64_t unix_milli_timestamp)
		{
			if (client_ == nullptr) {
				return false;
			}

			reset();
			any_type_to_string(key);

			if (keys_.empty()) {
				return false;
			}

			return client_->pexpire_at(std::move(keys_), unix_milli_timestamp);
		}

		template<typename T>
		bool remove_expire(T&& key)
		{
			if (client_ == nullptr) {
				return false;
			}

			reset();
			any_type_to_string(key);

			if (keys_.empty()) {
				return false;
			}

			return client_->remove_expire(std::move(keys_[0]));
		}


		template<typename T>
		int  remainder_ttl(T&& key)
		{
			if (client_  == nullptr) {
				return -1;
			}

			reset();
			any_type_to_string(key);

			if (keys_.empty()) {
				return -1;
			}

			return client_->remainder_ttl(std::move(keys_[0]));
		}

		template<typename T1,typename T2>
		bool rename_key(T1&& src_key, T2&&new_key){
			if (client_ == nullptr) {
				return false;
			}

			reset();
			any_type_to_string(src_key);
			any_type_to_string(new_key);
			if (keys_.size() !=2) {
				return false;
			}

			client_->rename_key(std::move(keys_[0]), std::move(keys_[1]));
		}

		//会判断new_key是否存在 
		template<typename T1, typename T2>
		bool renamenx_key(T1&& src_key, T2&& new_key) {
			if (client_ == nullptr) {
				return false;
			}

			reset();
			any_type_to_string(src_key);
			any_type_to_string(new_key);
			if (keys_.size() != 2) {
				return false;
			}

			client_->renamenx_key(std::move(keys_[0]), std::move(keys_[1]));
		}

		template<typename T1,typename T2>
		bool setex(T1&& key,T2 &&value, size_t seconds)
		{
			static_assert(is_sting_, "This API Support String Request");

			if (client_ == nullptr) {
				return false;
			}

			reset();
			any_type_to_string(key);
			any_type_to_string(value);

			if (keys_.size()!=2){
				return false;
			}

			return client_->setex(std::move(keys_[0]),std::move(keys_[1]),unit::int_to_string(seconds));
		}

		template<typename T1, typename T2>
		bool psetex(T1&& key, T2&& value,size_t milliseconds)
		{
			static_assert(is_sting_, "This API Support String Request");

			if (client_ == nullptr) {
				return false;
			}

			reset();
			any_type_to_string(key);
			any_type_to_string(value);

			if (keys_.size() != 2) {
				return false;
			}

			return client_->psetex(std::move(keys_[0]), std::move(keys_[1]),unit::int_to_string(milliseconds));
		}

		//提高这以下四个接口，主要防止，有些版本不支持setex的接口
		template<typename T1, typename T2>
		bool set_has_seconds(T1&& key, T2&& value,std::size_t seconds)
		{
			static_assert(is_sting_, "This API Support String Request");

			if (client_ == nullptr) {
				return false;
			}

			reset();
			any_type_to_string(key);
			any_type_to_string(value);

			if (keys_.size() != 2) {
				return false;
			}

			return client_->set_has_seconds(std::move(keys_[0]), std::move(keys_[1]), unit::int_to_string(seconds));
		}

		//此接口相当于setnx 加上秒，is_exist:true(NX) false(XX)
		template<typename T1,typename T2>
		bool set_has_seconds_if(T1&& key,T2&& value, std::size_t seconds,bool is_exist)
		{
			static_assert(is_sting_, "This API Support String Request");

			if (client_ == nullptr) {
				return false;
			}

			reset();
			any_type_to_string(key);
			any_type_to_string(value);

			if (keys_.size() != 2) {
				return false;
			}

			return client_->set_has_seconds_if(std::move(keys_[0]), std::move(keys_[1]),unit::int_to_string(seconds),is_exist);
		}
	
		template<typename T1,typename T2>
		bool set_has_milliseconds(T1&& key,T2&& value, std::size_t milliseconds)
		{
			static_assert(is_sting_, "This API Support String Request");

			if (client_ == nullptr) {
				return false;
			}

			reset();
			any_type_to_string(key);
			any_type_to_string(value);

			if (keys_.size() != 2) {
				return false;
			}

			return client_->set_has_milliseconds(std::move(keys_[0]), std::move(keys_[1]),unit::int_to_string(milliseconds));
		}

		//is_exist:true(NX)false(XX)
		template<typename T1, typename T2>
		bool set_has_milliseconds_if(T1&& key, T2&& value,std::size_t milliseconds,bool is_exist)
		{
			static_assert(is_sting_, "This API Support String Request");

			if (client_ == nullptr) {
				return false;
			}

			reset();
			any_type_to_string(key);
			any_type_to_string(value);

			if (keys_.size() != 2) {
				return false;
			}

			return client_->set_has_milliseconds_if(std::move(keys_[0]), std::move(keys_[1]), unit::int_to_string(milliseconds), is_exist);
		}

		template<typename T1,typename T2>
		bool set(T1&& key,T2&& value)
		{
			static_assert(is_sting_, "This API Support String Request");
			if (client_ == nullptr) {
				return false;
			}

			reset();
			any_type_to_string(key);
			any_type_to_string(value);

			if (keys_.size() != 2) {
				return false;
			}

			return client_->set(std::move(keys_[0]), std::move(keys_[1]));
		}

		template<typename T>
		std::string get_range(T&& key,int start,int end)
		{
			static_assert(is_sting_, "This API Support String Request");
			if (client_ == nullptr) {
				return "";
			}

			reset();
			any_type_to_string(key);
			
			if (keys_.empty()){
				return "";
			}

			return client_->get_range(std::move(keys_[0]),unit::int_to_string(start),unit::int_to_string(end));
		}

		template<typename T>
		int incr(T&& key)
		{
			static_assert(is_sting_, "This API Support String Request");
			if (client_ == nullptr) {
				return -1;
			}

			reset();
			any_type_to_string(key);

			if (keys_.empty()) {
				return -1;
			}

			return client_->incr(std::move(keys_[0]));
		}

		//若key不存在直接创建，并执行增加
		template<typename T>
		int  incr_by_increment(T&& key, int increment)
		{
			static_assert(is_sting_, "This API Support String Request");
			if (client_ == nullptr) {
				return -1;
			}

			reset();
			any_type_to_string(key);

			if (keys_.empty()) {
				return -1;
			}

			return client_->incr_by_increment(std::move(keys_[0]),unit::int_to_string(increment));
		}

		template<typename T>
		std::string incr_by_float(T&& key,float increment)
		{
			static_assert(is_sting_, "This API Support String Request");
			if (client_ == nullptr) {
				return "";
			}

			reset();
			any_type_to_string(key);

			if (keys_.empty()) {
				return "";
			}

			return client_->incr_by_float(std::move(keys_[0]), unit::float_to_string(increment));
		}

		template<typename T>
		int decr(std::string&& key)
		{
			static_assert(is_sting_, "This API Support String Request");
			if (client_ == nullptr) {
				return -1;
			}

			reset();
			any_type_to_string(key);

			if (keys_.empty()) {
				return -1;
			}

			return client_->decr(std::move(keys_[0]));
		}

		template<typename T>
		int decr_increment(T&& key,int increment)
		{
			static_assert(is_sting_, "This API Support String Request");
			if (client_ == nullptr) {
				return -1 ;
			}

			reset();
			any_type_to_string(key);

			if (keys_.empty()) {
				return -1;
			}

			return client_->decr_increment(std::move(keys_[0]),unit::int_to_string(increment));
		}

		template<typename T>
		std::string get_reflect_value(T&& key)
		{
			static_assert(is_sting_, "This API Support String Request");
			if (client_  == nullptr) {
				return "";
			}

			reset();
			any_type_to_string(key);

			if (keys_.empty()) {
				return "";
			}

			return client_->get_reflect_value(std::move(keys_[0]));
		}

		template<typename T1,typename T2>
		std::string get_set_key(T1&& key, T2&& value)
		{
			static_assert(is_sting_, "This API Support String Request");
			if (client_ == nullptr ) {
				return "";
			}

			reset();
			any_type_to_string(key);
			any_type_to_string(value);
			if (keys_.size()!=2) {
				return "";
			}

			return client_->get_set_key(std::move(keys_[0]), std::move(keys_[1]));
		}

		template<typename T>
		std::string substr_reflect_value(T&& key, int start, int end)
		{
			static_assert(is_sting_, "This API Support String Request");
			if (client_ == nullptr) {
				return "";
			}

			reset();
			any_type_to_string(key);
			if (keys_.empty()) {
				return "";
			}


			return client_->substr_reflect_value(std::move(keys_[0]), start, end);
		}

		template<typename...Args>
		std::vector<std::string> multi_get_keys(Args&&...key)
		{
			constexpr auto Size = sizeof...(key);
			static_assert(is_sting_, "This API Support String Request");
			if (Size ==0){
				return {};
			}

			reset();
			for_each_args(std::forward<Args>(key)...);
			if (keys_.empty()){
				return {};
			}

			return client_->multi_get_keys(std::move(keys_));
		}

		template<typename...Args>
		bool multi_set_keys(Args&&...key_value)
		{
			constexpr auto Size = sizeof...(key_value);
			static_assert(is_sting_, "This API Support String Request");

			if (Size ==0) {
				return false;
			}

			reset();
			for_each_args(std::forward<Args>(key_value)...);
			if (keys_.empty()) {
				return false;
			}

			return client_->multi_set_keys(std::move(keys_));
		}

		template<typename...Args>
		int multi_set_if_not_set(Args...key_value)
		{
			constexpr auto Size = sizeof...(key_value);
			static_assert(is_sting_, "This API Support String Request");
			if (Size == 0) {
				return 0;
			}

			reset();
			for_each_args(std::forward<Args>(key_value)...);
			if (keys_.empty()) {
				return 0;
			}

			return client_->multi_set_if_not_set(std::move(keys_));
		}

		//key不存，会直接创建key
		template<typename T1,typename T2>
		int append_value(T1&& key, T2&& new_value)
		{
			static_assert(is_sting_, "This API Support String Request");
			if (client_ == nullptr) {
				return 1;
			}

			reset();
			any_type_to_string(key);
			any_type_to_string(new_value);

			if (keys_.size() != 2) {
				return 0;
			}

			return client_->append_value(std::move(keys_[0]),std::move(keys_[1]));
		}

		template<typename T1,typename T2>
		int list_rpush(T1&& key, T2&& value)
		{
			static_assert(is_list_, "This API Support List Request");

			if (client_ == nullptr) {
				return 0;
			}

			reset();
			any_type_to_string(key);
			any_type_to_string(value);

			if (keys_.size()!=2){
				return 0;
			}

			return client_->list_rpush(std::move(keys_[0]),std::move(keys_[1]));
		}

		template<typename T1, typename T2>
		int list_rpush_if(T1&& key, T2&& value)
		{
			static_assert(is_list_, "This API Support List Request");

			if (client_ == nullptr) {
				return 0;
			}

			reset();
			any_type_to_string(key);
			any_type_to_string(value);

			if (keys_.size() != 2) {
				return 0;
			}

			return client_->list_rpush_if(std::move(keys_[0]),std::move(keys_[1]));
		}

		template<typename T1, typename T2>
		int list_lpush(T1&& key,T2&& value)
		{
			static_assert(is_list_, "This API Support List Request");

			if (client_ == nullptr) {
				return 0;
			}

			reset();
			any_type_to_string(key);
			any_type_to_string(value);

			if (keys_.size() != 2) {
				return 0;
			}

			return client_->list_lpush(std::move(keys_[0]),std::move(keys_[1]));
		}

		template<typename T1,typename T2>
		int list_lpush_if(T1&& key,T2&& value)
		{
			static_assert(is_list_, "This API Support List Request");

			if (client_ == nullptr) {
				return 0;
			}

			reset();
			any_type_to_string(key);
			any_type_to_string(value);

			if (keys_.size() != 2) {
				return 0;
			}

			return client_->list_lpush_if(std::move(keys_[0]),std::move(keys_[1]));
		}

		template<typename T>
		int32_t list_size(T&& key)
		{
			static_assert(is_list_, "This API Support List Request");

			if (client_== nullptr) {
				return 0;
			}

			reset();
			any_type_to_string(key);
			if (keys_.empty()){
				return 0;
			}

			return client_->list_size(std::move(key));
		}

		template<typename T>
		std::vector <std::string> list_range(T&& key, int start, int end)
		{
			static_assert(is_list_, "This API Support List Request");

			if (client_ == nullptr) {
				return {};
			}

			reset();
			any_type_to_string(key);
			if (keys_.empty()){
				return{};
			}

			return client_->list_range(std::move(keys_[0]),start, end);
		}

		template<typename T>
		std::string list_lpop(T&& key)
		{
			static_assert(is_list_, "This API Support List Request");

			if (client_  == nullptr) {
				return "";
			}

			reset();
			any_type_to_string(key);
			if (keys_.empty()) {
				return"";
			}

			return client_->list_lpop(std::move(keys_[0]));
		}

		template<typename T>
		std::string list_rpop(T&& key)
		{
			static_assert(is_list_, "This API Support List Request");

			if (client_  == nullptr) {
				return "";
			}

			reset();
			any_type_to_string(key);
			if (keys_.empty()) {
				return"";
			}

			return client_->list_rpop(std::move(keys_[0]));
		}

		template<typename T>
		std::string list_brpop(T&& key, size_t timeout = 0)
		{
			static_assert(is_list_, "This API Support List Request");

			if (client_ == nullptr) {
				return "";
			}

			reset();
			any_type_to_string(key);
			if (keys_.empty()) {
				return"";
			}

			return client_->list_brpop(std::move(keys_[0]), timeout);
		}

		template<typename T>
		std::string list_blpop(T&& key, size_t timeout = 0)
		{
			static_assert(is_list_, "This API Support List Request");

			if (client_== nullptr) {
				return "";
			}

			reset();
			any_type_to_string(key);
			if (keys_.empty()) {
				return"";
			}

			return  client_->list_blpop(std::move(keys_[0]), timeout);
		}

		template<typename T>
		bool list_trim(T&& key, int start, int end)
		{
			static_assert(is_list_, "This API Support List Request");

			if (client_  == nullptr) {
				return "";
			}

			reset();
			any_type_to_string(key);
			if (keys_.empty()) {
				return"";
			}

			return client_->list_trim(std::move(keys_[0]),start, end);
		}

		template<typename T>
		std::string list_index(T&& key, int index)
		{
			static_assert(is_list_, "This API Support List Request");

			if (client_ == nullptr) {
				return "";
			}

			reset();
			any_type_to_string(key);
			if (keys_.empty()) {
				return"";
			}

			return client_->list_index(std::move(keys_[0]), index);
		}

		template<typename T1,typename T2>
		bool list_set(T1&& key, T2&& value, int index)
		{
			static_assert(is_list_, "This API Support List Request");

			if (client_  == nullptr) {
				return false;
			}

			reset();
			any_type_to_string(key);
			any_type_to_string(value);
			if (keys_.size() !=2){
				return false;
			}

			return client_->list_set(std::move(keys_[0]),std::move(keys_[1]),unit::int_to_string(index));
		}


		//没有移除就为0 ，有移除就大于0，count表示list的起始位置,一般从0开始删除,-1表示最后一个元素删除
		//如果从0开始删除，有多少删除多少
		//如果从-1开始删除,就只会删除一个元素
		//数量为>=|count|
		template<typename T1,typename T2>
		int list_del_elem(T1&& key,T2&& value, int count = 0)
		{
			static_assert(is_list_, "This API Support List Request");

			if (client_  == nullptr) {
				return 0;
			}

			reset();
			any_type_to_string(key);
			any_type_to_string(value);
			if (keys_.size()!=2) {
				return 0;
			}

			return client_->list_del_elem(std::move(keys_[0]),std::move(keys_[1]),unit::int_to_string(count));
		}

		template<typename T1, typename T2>
		std::string list_rpoplpush(T1&& src_key,T2&& dst_key)
		{
			static_assert(is_list_, "This API Support List Request");

			if (client_ == nullptr) {
				return "";
			}

			reset();
			any_type_to_string(src_key);
			any_type_to_string(dst_key);

			if (keys_.size() !=2){
				return "";
			}

			return client_->list_rpoplpush(std::move(keys_[0]),std::move(keys_[1]));
		}

		template<typename T1,typename T2>
		std::string list_brpoplpush(T1&& src_key,T2&& dst_key, int timeout = 0)
		{
			static_assert(is_list_, "This API Support List Request");

			if (client_ == nullptr) {
				return "";
			}

			reset();
			any_type_to_string(src_key);
			any_type_to_string(dst_key);

			if (keys_.size() !=2){
				return "";
			}

			return client_->list_brpoplpush(std::move(keys_[0]),std::move(keys_[1]), timeout);
		}

		template<typename T1, typename T2,typename T3>
		int list_insert_before(T1&& key,T2&& dst_value, T3&& insert_value)
		{
			static_assert(is_list_, "This API Support List Request");

			if (client_ == nullptr || key.empty()) {
				return 0;
			}

			reset();
			any_type_to_string(key);
			any_type_to_string(dst_value);
			any_type_to_string(insert_value);

			if (keys_.size()!=3) {
				return 0;
			}

			return client_->list_insert_before(std::move(keys_[0]), std::move(keys_[1]),std::move(keys_[2]));
		}

		template<typename T1,typename T2,typename T3>
		int list_insert_after(T1&& key,T2&& dst_value,T3&& insert_value)
		{
			static_assert(is_list_, "This API Support List Request");

			if (client_== nullptr || key.empty()) {
				return 0;
			}

			reset();
			any_type_to_string(key);
			any_type_to_string(dst_value);
			any_type_to_string(insert_value);

			if (keys_.size()!=3){
				return 0;
			}

			return client_->list_insert_after(std::move(keys_[0]),std::move(keys_[1]),std::move(keys_[2]));
		}

		template<typename T,typename...Args>
		int set_add(T&& key, Args&&...args)
		{
			constexpr auto Size = sizeof...(args)+1;
			static_assert(is_set_, "This API Support Set Request");
			reset();
			any_type_to_string(key);

			for_each_args(std::forward<Args>(args)...);
			if (keys_.size() != Size){
				return 0;
			}

			return client_->set_add(std::move(keys_));
		}

		template<typename T,typename...Args>
		int set_delete_elem(T&& key, Args&&...args)
		{
			constexpr auto Size = sizeof...(args)+1;
			static_assert(is_set_, "This API Support Set Request");
			reset();
			any_type_to_string(key);

			for_each_args(std::forward<Args>(args)...);
			if (keys_.size() != Size) {
				return 0;
			}

			return client_->set_delete_elem(std::move(keys_));
		}

		template<typename T1,typename T2>
		bool set_is_member(T1&& key,T2&& value)
		{
			static_assert(is_set_, "This API Support Set Request");
			if (client_  == nullptr) {
				return false;
			}

			reset();
			any_type_to_string(key);
			any_type_to_string(value);

			if (keys_.size() !=2){
				return false;
			}

			return client_->set_is_member(std::move(keys_[0]),std::move(keys_[1]));
		}

		template<typename T>
		std::string set_rdel_elem(T&& key)
		{
			static_assert(is_set_, "This API Support Set Request");
			if (client_ == nullptr) {
				return "";
			}

			reset();
			any_type_to_string(key);
			if (keys_.empty()) {
				return "";
			}

			return client_->set_rdel_elem(std::move(keys_[0]));
		}

		//只是随机，不会发生删除
		template<typename T>
		std::vector <std::string> set_rand_elem(T&& key, int count)
		{
			static_assert(is_set_, "This API Support Set Request");
			if (client_  == nullptr) {
				return {};
			}

			reset();
			any_type_to_string(key);
			if (keys_.empty()) {
				return "";
			}

			return client_->set_rand_elem(std::move(keys_[0]), count);
		}

		template<typename T1,typename T2,typename T3>
		bool set_move_elem(T1&& src_key,T2&& dst_key,T3&& member)
		{
			static_assert(is_set_, "This API Support Set Request");
			if (client_== nullptr) {
				return false;
			}

			reset();
			any_type_to_string(src_key);
			any_type_to_string(dst_key);
			any_type_to_string(member);

			if (keys_.size() != 3){
				return false;
			}

			return client_->set_move_elem(std::move(keys_[0]),std::move(keys_[1]),std::move(keys_[2]));
		}

		template<typename T>
		size_t set_get_size(T&& key)
		{
			static_assert(is_set_, "This API Support Set Request");
			if (client_ == nullptr) {
				return 0;
			}

			reset();
			any_type_to_string(key);

			if (keys_.empty()){
				return 0;
			}
			 
			return  client_->set_get_size(std::move(keys_[0]));
		}

		template<typename T>
		std::vector <std::string> set_get_all_member(T&& key)
		{
			static_assert(is_set_, "This API Support Set Request");
			if (client_== nullptr) {
				return {};

			}

			reset();
			any_type_to_string(key);

			if (keys_.empty()) {
				return 0;
			}

			return  client_->set_get_all_member(std::move(keys_[0]));
		}

		//求集合的交集，如果一个为空，就返回空
		template<typename T,typename...Args>
		std::vector <std::string> set_sinter(T&&key,Args&&...keys)
		{
			constexpr auto Size = sizeof...(keys)+1;
			static_assert(is_set_, "This API Support Set Request");
			reset();
			any_type_to_string(key);

			for_each_args(std::forward<Args>(keys)...);

			if (keys_.size() !=Size){
				return {};
			}

			return client_->set_sinter(std::move(keys_));
		}

		//求集合的交集，如果一个为空，就返回空,并保存另外一个地方
		template<typename T,typename...Args>
		int set_inter_store(T&& dst_key, Args&&...keys)
		{
			constexpr auto Size = sizeof...(keys)+1;
			static_assert(is_set_, "This API Support Set Request");

			reset();
			any_type_to_string(dst_key);
			for_each_args(std::forward<Args>(keys)...);

			if (keys_.size() != Size) {
				return {};
			}

			return client_->set_inter_store(std::move(keys_));
		}

		//求集合的并集合,不存在key就视为空 
		//(返回一个集合的全部成员，该集合是所有给定集合的并集)
		template<typename...Args>
		std::vector <std::string> set_union(Args&&...key)
		{
			constexpr auto Size = sizeof...(key);
			static_assert(is_set_, "This API Support Set Request");
			if (client_ == nullptr || Size ==0) {
				return {};
			}

			reset();
			for_each_args(std::forward<Args>(key)...);
			if (keys_.size() != Size){
				return {};
			}

			return client_->set_union(std::move(keys_));
		}
		//求集合的并集，如果一个为空，就返回空,并保存另外一个地方
		//(返回一个集合的全部成员，该集合是所有给定集合的并集)
		template<typename T,typename...Args>
		int set_union_store(T&& dst_key, Args&&...keys)
		{
			constexpr auto Size = sizeof...(keys)+1;
			static_assert(is_set_, "This API Support Set Request");
			if (client_ == nullptr) {
				return -1;
			}

			reset();
			any_type_to_string(dst_key);
			for_each_args(std::forward<Args>(keys)...);

			if (keys_.size() != Size){
				return -1;
			}

			return client_->set_union_store(std::move(keys_));
		}

		//返回一个集合的全部成员，该集合是所有给定集合之间的差集。
		//不存在的 key 被视为空集。
		template<typename...Args>
		std::vector <std::string> set_diff(Args&&...key)
		{
			constexpr auto Size = sizeof...(key);
			static_assert(is_set_, "This API Support Set Request");
			if (client_ == nullptr) {
				return{};
			}

			reset();
			for_each_args(std::forward<Args>(key)...);

			if (keys_.size() != Size){
				return {};
			}

			return client_->set_diff(std::move(keys_));
		}

		template<typename T,typename...Args>
		int set_diff_store(T&& dst_key, Args&&...key)
		{
			constexpr auto Size = sizeof...(key)+1;
			static_assert(is_set_, "This API Support Set Request");
			if (client_ == nullptr) {
				return 0;
			}

			reset();
			any_type_to_string(dst_key);
			for_each_args(std::forward<Args>(key)...);
			if (keys_.size() != Size){
				return 0;
			}

			return client_->set_diff_store(std::move(keys_));
		}

		//ZADD key score member [[score member] [score member] …]
		template<typename T,typename...Args>
		int zset_add(T&& key, Args&&...args) {
			constexpr auto Size = sizeof...(args)+1;
			static_assert(is_zset, "This API Support ZSet Request");

			reset();
			any_type_to_string(key);
			for_each_zset_args(std::forward<Args>(args)...);

			if (client_ == nullptr|| keys_.size()!= Size) {
				return -1;
			}

			return client_->zset_add(std::move(keys_));
		}

		//获取指定zset成员的值
		template<typename T1,typename T2>
		std::string zset_score(T1&& key, T2&& member)
		{
			static_assert(is_zset, "This API Support ZSet Request");

			if (client_== nullptr){
				return "";
			}

			reset();
			any_type_to_string(key);
			any_type_to_string(member);

			if (keys_.size() !=2){
				return "";
			}

			return client_->zset_score(std::move(keys_[0]),std::move(keys_[1]));
		}

		//获取指定zset成员增加值
		//member:就是数据库中的value字段
		template<typename T1,typename T2>
		std::string zset_incrby(T1&& key, int increment, T2&& member)
		{
			static_assert(is_zset, "This API Support ZSet Request");

			if (client_ == nullptr){
				return "";
			}

			reset();
			any_type_to_string(key);
			any_type_to_string(member);

			if (keys_.size() !=2){
				return "";
			}

			return client_->zset_incrby(std::move(keys_[0]), unit::int_to_string(increment), std::move(keys_[1]));

		}

		//返回有序集 key的总数
		template<typename T>
		int zset_counts(T&& key)
		{
			static_assert(is_zset, "This API Support ZSet Request");
			if (client_ == nullptr){
				return 0;
			}

			reset();
			any_type_to_string(key);
			if (keys_.empty()) {
				return 0;
			}

			return client_->zset_card(std::move(keys_[0]));
		}

		//返回有序集 key的 min和max区间总数
		template<typename T>
		int zset_range_counts(T&& key, int min, int max)
		{
			static_assert(is_zset, "This API Support ZSet Request");
			if (client_  == nullptr){
				return 0;
			}

			reset();
			any_type_to_string(key);
			if (keys_.empty()){
				return 0;
			}

			return client_->zset_count(std::move(keys_[0]), unit::int_to_string(min), unit::int_to_string(max));
		}

		//返回列表以 value1, score1, ..., valueN, scoreN 的格式表示。
		//with_scores:false不带score返回
		//递增排序
		template<typename T>
		std::vector <std::string> zset_range(T&& key, int begin, int end, bool with_scores = true)
		{
			static_assert(is_zset, "This API Support ZSet Request");

			if (client_ == nullptr){
				return { {} };
			}

			reset();
			any_type_to_string(key);
			if (keys_.empty()){
				return {};
			}

			return client_->zset_range(std::move(keys_[0]), unit::int_to_string(begin), unit::int_to_string(end), with_scores);
		}

		//其中成员的位置按 score 值递减(从大到小)来排列,命令和zrange相似
		//递减排序
		//字典序的逆序
		template<typename T>
		std::vector <std::string> zset_rerange(T&& key, int begin, int end, bool with_scores = true)
		{
			static_assert(is_zset, "This API Support ZSet Request");
			if (client_ == nullptr){
				return { {} };
			}

			reset();
			any_type_to_string(key);
			if (keys_.empty()) {
				return {};
			}

			return client_->zset_revrange(std::move(keys_[0]), unit::int_to_string(begin), unit::int_to_string(end), with_scores);
		}

		//求区间内的score排序
		template<typename T>
		std::vector <std::string> zset_range_score(T&& key, int min, int max,
			bool with_scores = true, bool limit = false, int limit_min = 0, int limit_max = 1)
		{
			static_assert(is_zset, "This API Support ZSet Request");

			if (client_ == nullptr) {
				return {} ;
			}

			reset();
			any_type_to_string(key);

			if (keys_.empty()){
				return {};
			}

			return client_->zset_range_score(std::move(keys_[0]), unit::int_to_string(min),
				unit::int_to_string(max), with_scores, limit, unit::int_to_string(limit_min), unit::int_to_string(limit_max));
		}

		//求区间内的score从数据库中逆向取值并排序
		template<typename T>
		std::vector <std::string> zset_revrange_score(T&& key, int max, int min,
			bool with_scores = true, bool limit = false, int limit_min = 0, int limit_max = 1)
		{
			static_assert(is_zset, "This API Support ZSet Request");

			if (client_== nullptr) {
				return {} ;
			}

			reset();
			any_type_to_string(key);

			if (keys_.empty()) {
				return {};
			}

			return client_->zset_revrange_score(std::move(keys_[0]), unit::int_to_string(max),
				unit::int_to_string(min), with_scores, limit, unit::int_to_string(limit_min), unit::int_to_string(limit_max));
		}

		//升序，从0开始
		template<typename T1,typename T2>
		int zset_rank(T1&& key,T2&& member)
		{
			static_assert(is_zset, "This API Support ZSet Request");

			if (client_== nullptr) {
				return -1;
			}

			reset();
			any_type_to_string(key);
			any_type_to_string(member);

			if (keys_.size() !=2){
				return -1;
			}

			return client_->zset_rank(std::move(keys_[0]),std::move(keys_[1]));
		}

		//降序，从0开始，到从大的开始排
		template<typename T1, typename T2>
		int zset_revrank(T1&& key,T2&& member)
		{
			static_assert(is_zset, "This API Support ZSet Request");

			if (client_ == nullptr) {
				return -1;
			}

			reset();
			any_type_to_string(key);
			any_type_to_string(member);


			if (keys_.size() != 2) {
				return -1;
			}

			return client_->zset_revrank(std::move(keys_[0]), std::move(keys_[1]));
		}

		//ZREM key  member [[score member][score member] …]
		template<typename T,typename...Args>
		bool zset_rem(T&& key, Args&&...args)
		{
			constexpr auto Size = sizeof...(args) + 1;
			static_assert(is_zset, "This API Support ZSet Request");

			if (client_ == nullptr) {
				return false;
			}

			reset();
			any_type_to_string(key);
			for_each_args(std::forward<Args>(args)...);

			if (keys_.size()!=Size){
				return false;
			}

			return client_->zset_rem(std::move(keys_));
		}

		//移除有序集 key 中，指定排名(rank)区间内的所有成员
		template<typename T>
		int zset_remrangeby_rank(T&& key,int begin,int end)
		{
			static_assert(is_zset, "This API Support ZSet Request");

			if (client_== nullptr) {
				return -1;
			}

			reset();
			any_type_to_string(key);

			if (keys_.empty()){
				return -1;
			}

			return client_->zset_remrangeby_rank(std::move(keys_[0]), unit::int_to_string(begin), unit::int_to_string(end));
		}

		//移除有序集 key 中，所有 score 值介于 min 和 max 之间(包括等于 min 或 max )的成员。
		template<typename T>
		int zset_remrangebyscore(T&& key,int min,int max)
		{
			static_assert(is_zset, "This API Support ZSet Request");
			
			if (client_== nullptr) {
				return -1;
			}

			reset();
			any_type_to_string(key);

			if (keys_.empty()) {
				return -1;
			}

			return client_->zset_remrangebyscore(std::move(keys_[0]), unit::int_to_string(min), unit::int_to_string(max));
		}
		
		//合法的 min 和 max 参数必须包含(或者[， 其中(表示开区间（指定的值不会被包含在范围之内）， 而[则表示闭区间（指定的值会被包含在范围之内）。
		//特殊值 + 和 - 在 min 参数以及 max 参数中具有特殊的意义， 其中 + 表示正无限， 而 - 表示负无限。 
		//因此， 向一个所有成员的分值都相同的有序集合发送命令 ZRANGEBYLEX <zset> -+， 命令将返回有序集合中的所有元素。

		template<typename T>
		std::vector <std::string> zset_rangebylex(T&& key,std::string&&min,std::string&& max,bool limit=false,int limit_min=0,int limit_max=1)
		{
			static_assert(is_zset, "This API Support ZSet Request");

			if (client_ == nullptr){
				return {};
			}

			reset();
			any_type_to_string(key);

			if (keys_.empty()){
				return {};
			}

			return client_->zset_rangebylex(std::move(keys_[0]),std::forward<std::string>(min),std::forward<std::string>(max), 
				limit,unit::int_to_string(limit_max),unit::int_to_string(limit_max));
		}

		//此接口和上面接口一样，只是返回数量
		template<typename T>
		int zset_lexcount(T&& key, std::string&& min, std::string&& max)
		{
			static_assert(is_zset, "This API Support ZSet Request");

			if (client_== nullptr) {
				return -1;
			}

			reset();
			any_type_to_string(key);
			if (keys_.empty()){
				return -1;
			}

			return client_->zset_lexcount(std::move(keys_[0]), std::forward<std::string>(min), std::forward<std::string>(max));
		}

		//特殊符号，删除元素
		template<typename T>
		int zset_remrangebylex(T&& key, std::string&& min, std::string&& max)
		{
			static_assert(is_zset, "This API Support ZSet Request");

			if (client_ == nullptr) {
				return -1;
			}

			reset();
			any_type_to_string(key);
			if (keys_.empty()) {
				return -1;
			}

			return client_->zset_remrangebylex(std::move(keys_[0]),std::forward<std::string>(min), std::forward<std::string>(max));
		}
		
		//计算给定的一个或多个有序集的并集，其中给定 key 的数量必须以 num_keys 参数指定，
		//并将该并集(结果集)储存到 dst_store_key ,其中num_keys,参与计算key的个数
		//默认情况下，结果集中某个成员的 score 值是所有给定集下该成员 score 值之 和
		//函数使用介绍，参数1：目标存储的key,参数 2，求和的方式，参数3：N个key 跟上权重,具体使用可以看官方命令
		template<typename T,typename...Args>
		int zset_union_store(T&& dst_store_key,int num_keys,aggregate_mothod mothod,
			int weight_min,int weight_max,Args&&...args)
		{
			constexpr auto Size = sizeof...(args) + 2;
			static_assert(is_zset, "This API Support ZSet Request");

			reset();
			any_type_to_string(dst_store_key);
			keys_.emplace_back(unit::int_to_string(num_keys));

			for_each_args(std::forward<Args>(args)...);

			if (client_ == nullptr || keys_.size() != Size) {
				return -1;
			}

			keys_.emplace_back("WEIGHTS");
			keys_.emplace_back(unit::int_to_string(weight_min));
			keys_.emplace_back(unit::int_to_string(weight_max));
			return client_->zset_union_store(std::move(keys_), mothod);
		}

		//求交集，具体参数可以参考 zset_union_store接口
		template<typename T,typename...Args>
		int zset_inter_store(T&& dst_store_key, int num_keys, aggregate_mothod mothod,
			int weight_min, int weight_max, Args&&...args)
		{
			constexpr auto Size = sizeof...(args) + 2;
			static_assert(is_zset, "This API Support ZSet Request");

			reset();
			any_type_to_string(dst_store_key);
			keys_.emplace_back(unit::int_to_string(num_keys));

			for_each_args(std::forward<Args>(args)...);

			if (client_ == nullptr || keys_.size() != Size) {
				return -1;
			}

			keys_.emplace_back("WEIGHTS");
			keys_.emplace_back(unit::int_to_string(weight_min));
			keys_.emplace_back(unit::int_to_string(weight_max));
			return client_->zset_inter_store(std::move(keys_), mothod);
		}

		//当 HSET 命令在哈希表中新创建 field 域并成功为它设置值时， 命令返回 1 
		//如果域 field 已经存在于哈希表， 并且 HSET 命令成功使用新值覆盖了它的旧值， 那么命令返回 0
		template<typename T1,typename T2,typename T3>
		int hash_set(T1 &&key,T2 &&field, T3&& value)
		{
			static_assert(is_hash, "This API Support hash Request");

			if (client_ == nullptr){
				return -1;
			}

			reset();
			any_type_to_string(key);
			any_type_to_string(field);
			any_type_to_string(value);
			if (keys_.size()!=3){
				return -1;
			}

			return client_->hash_set(std::move(keys_[0]),std::move(keys_[1]),std::move(keys_[2]));
		}

		//HSETNX 命令在设置成功时返回 1 ， 在给定域已经存在而放弃执行设置操作时返回 0.
		//如果给定域已经存在于哈希表当中， 那么命令将放弃执行设置操作。
		template<typename T1, typename T2,typename T3>
		int hash_setx(T1&& key, T1&& field, T2&& value)
		{
			static_assert(is_hash, "This API Support hash Request");

			if (client_ == nullptr) {
				return -1;
			}

			reset();
			any_type_to_string(key);
			any_type_to_string(field);
			any_type_to_string(value);
			if (keys_.size() != 3) {
				return -1;
			}

			return client_->hash_setx(std::move(keys_[0]),std::move(keys_[1]),std::move(keys_[2]));
		}

		template<typename T1,typename T2>
		int hash_exists(T1&& key,T2&& field)
		{
			static_assert(is_hash, "This API Support hash Request");

			if (client_ == nullptr) {
				return -1;
			}

			reset();
			any_type_to_string(key);
			any_type_to_string(field);

			if (keys_.size() != 2) {
				return -1;
			}

			return client_->hash_exists(std::move(keys_[0]),std::move(keys_[1]));
		}

		template<typename T1,typename T2>
		std::string hash_get(T1&& key,T2&& field)
		{
			static_assert(is_hash, "This API Support hash Request");

			if (client_ == nullptr) {
				return "";
			}

			reset();
			any_type_to_string(key);
			any_type_to_string(field);

			if (keys_.size() != 2) {
				return "";
			}

			return client_->hash_get(std::move(keys_[0]),std::move(keys_[1]));
		}
		
		template<typename T,typename...Args>
		int hash_del(T&& key, Args&&...fields)
		{
			static_assert(is_hash, "This API Support hash Request");
			constexpr auto Size = sizeof...(fields)+1;

			if (client_ == nullptr || key.empty()){
				return -1;
			}

			reset();
			any_type_to_string(key);
			for_each_args(std::forward<Args>(fields)...);
			if ( keys_.empty() || keys_.size() != Size){
				return -1;
			}

			return client_->hash_del(std::move(keys_));
		}

		template<typename T>
		int hash_len(T&& key)
		{
			static_assert(is_hash, "This API Support hash Request");
			if (client_ == nullptr){
				return 0;
			}

			reset();
			any_type_to_string(key);

			if (keys_.empty()) {
				return 0;
			}

			return client_->hash_len(std::move(keys_[0]));
		}

		template<typename T1,typename T2>
		int hash_strlen(T1&& key,T2&& field)
		{
			static_assert(is_hash, "This API Support hash Request");
			if (client_== nullptr) {
				return 0;
			}

			reset();
			any_type_to_string(key);
			any_type_to_string(field);

			if (keys_.size() !=2){
				return 0;
			}

			return client_->hash_strlen(std::move(keys_[0]),std::move(keys_[1]));
		}

		//返回增加值
		//一个新的哈希表被创建并执行HINCRBY 命令(注意地方)
		template<typename T1,typename T2>
		int hash_incrby(T1&& key, T2&& field, int increment)
		{
			static_assert(is_hash, "This API Support hash Request");
			if (client_  == nullptr) {
				return 0;
			}

			reset();
			any_type_to_string(key);
			any_type_to_string(field);

			if (keys_.size() != 2) {
				return "";
			}

			return client_->hash_incrby(std::move(keys_[0]),std::move(keys_[1]),unit::int_to_string(increment));
		}

		template<typename T>
		std::string hash_incrby_float(std::string&& key, T&& field, double increment)
		{
			static_assert(is_hash, "This API Support hash Request");
			if (client_ == nullptr || key.empty()) {
				return "";
			}

			reset();
			any_type_to_string(key);
			any_type_to_string(field);

			if (keys_.size() != 2) {
				return "";
			}

			return client_->hash_incrby_float(std::move(keys_[0]),std::move(keys_[1]),unit::double_to_string(increment));
		}

		template<typename T1,typename T2>
		std::string hash_incrby_float(T1&& key, T2&& field, float increment)
		{
			static_assert(is_hash, "This API Support hash Request");
			if (client_ == nullptr || key.empty()) {
				return "";
			}

			reset();
			any_type_to_string(key);
			any_type_to_string(field);

			if (keys_.size() != 2) {
				return "";
			}

			return client_->hash_incrby_float(std::move(keys_[0]), std::move(keys_[1]),unit::float_to_string(increment));
		}

		template<typename T,typename...Args>
		bool hash_mset(T&& key, Args&&...keys)
		{
			const auto Size = sizeof...(keys) + 1;
			static_assert(is_hash, "This API Support hash Request");
			if (client_ == nullptr || key.empty()) {
				return false;
			}

			reset();
			any_type_to_string(key);
			for_each_args(std::forward<Args>(keys)...);

			if (keys_.size() != Size) {
				return false;
			}

			return client_->hash_mset(std::move(keys_));
		}

		template<typename T,typename...Args>
		std::vector <std::string> hash_mget(T&& key, Args&&...keys)
		{
			const auto Size = sizeof...(keys) + 1;
			static_assert(is_hash, "This API Support hash Request");
			if (client_ == nullptr || key.empty()) {
				return {} ;
			}

			reset();
			any_type_to_string(key);
			for_each_args(std::forward<Args>(keys)...);

			if (keys_.size() != Size) {
				return  {} ;
			}

			return client_->hash_mget(std::move(keys_));
		}

		//返回所有的keys
		template<typename T>
		std::vector <std::string> hash_keys(T&& key)
		{
			static_assert(is_hash, "This API Support hash Request");
			if (client_ == nullptr) {
				return {};
			}
			
			reset();
			any_type_to_string(key);

			if (keys_.empty()) {
				return {};
			}

			return client_->hash_keys(std::move(keys_[0]));
		}

		//返回key中的所有值
		template<typename T>
		std::vector <std::string> hash_vals(T&& key)
		{
			static_assert(is_hash, "This API Support hash Request");
			if (client_ == nullptr) {
				return {} ;
			}

			reset();
			any_type_to_string(key);

			if (keys_.empty()) {
				return {};
			}

			return client_->hash_vals(std::move(keys_[0]));
		}

		//返回key中的域和值
		template<typename T>
		std::vector <std::string> hash_get_all(T&& key)
		{
			static_assert(is_hash, "This API Support hash Request");
			if (client_ == nullptr) {
				return {} ;
			}

			reset();
			any_type_to_string(key);
			
			if (keys_.empty()){
				return {};
			}

			return client_->hash_get_all(std::move(keys_[0]));
		}
	private:
		template<typename T>
		void any_type_to_string(T&& value)
		{
			std::string str;
#if (_MSC_VER >= 1700 && _MSC_VER <= 1900) //vs2012-vs2015
#else
			if constexpr (std::is_same<typename std::decay<decltype(value)>::type, bool>::value) {
				str = value ? "true" : "false";
			}else {
				str = to_string(value);
			}
#endif
			if (!str.empty()){
				keys_.push_back(std::move(str));
			}
		}

		template<typename...Args>
		void for_each_args(Args&&...value)
		{
			int arr[] = { (any_type_to_string(value),0)... };
		}

		template<typename T>
		void zset_make_keys(T&&t)
		{
			std::string value = to_string(std::forward<T>(t));

			if (!value.empty()) {
				keys_.push_back(std::move(value));
			}
		}

		template<typename...Args>
		void for_each_zset_args(Args&&...args)
		{
			int arr[] = { (zset_make_keys(args),0)... };
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

		void reset() {
			keys_.clear();
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
