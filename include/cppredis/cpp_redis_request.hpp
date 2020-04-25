#ifndef cpp_redis_request_h__
#define cpp_redis_request_h__

#include <algorithm>
#include <map>
#include <vector>
#include "unit.hpp"
#include "cpp_define.h"

namespace cpp_redis {
	class cpp_redis_request
	{
	public:
		cpp_redis_request() = default;
		~cpp_redis_request() = default;

		std::string get_cmd(redis_cmd cmd) {
			std::string cmd_str;
			switch (cmd)
			{
			case cpp_redis::auth:
				cmd_str = g_auth_cmd;
				break;
			case cpp_redis::select:
				cmd_str = g_select_cmd;
				break;
			case cpp_redis::set:
				cmd_str = g_set_cmd;
				break;
			case cpp_redis::setex:
				cmd_str = g_setx_cmd;
				break;
			case cpp_redis::incrby:
				cmd_str = g_incrby_cmd;
				break;
			case cpp_redis::del:
				cmd_str = g_del_cmd;
				break;
			case cpp_redis::exists:
				cmd_str = g_exists_cmd;
				break;
			case cpp_redis::get:
				cmd_str = g_get_cmd;
				break;
			case cpp_redis::expire:
				cmd_str = g_expire_cmd;
				break;
			case cpp_redis::expire_at:
				cmd_str = g_expire_at_cmd;
				break;
			case cpp_redis::ttl:
				cmd_str = g_ttl_cmd;
				break;
			case cpp_redis::get_set:
				cmd_str = g_get_set_cmd;
				break;
			case cpp_redis::substr:
				cmd_str = g_substr_cmd;
				break;
			case cpp_redis::mget:
				cmd_str = g_mget_cmd;
				break;
			case cpp_redis::mset:
				cmd_str = g_mset_cmd;
				break;
			case cpp_redis::msetnx:
				cmd_str = g_msetnx_cmd;
				break;
			case cpp_redis::append:
				cmd_str = g_append_cmd;
				break;
			case cpp_redis::rpush:
				cmd_str = g_rpush_cmd;
				break;
			case cpp_redis::lpush:
				cmd_str = g_lpush_cmd;
				break;
			case cpp_redis::llen:
				cmd_str = g_llen_cmd;
				break;
			case cpp_redis::lrange:
				cmd_str = g_lrange_cmd;
				break;
			case cpp_redis::lpop:
				cmd_str = g_lpop_cmd;
				break;
			case cpp_redis::rpop:
				cmd_str = g_rpop_cmd;
				break;
			case cpp_redis::blpop:
				cmd_str = g_blpop_cmd;
				break;
			case cpp_redis::brpop:
				cmd_str = g_brpop_cmd;
				break;
			case cpp_redis::ltrim:
				cmd_str = g_trim_cmd;
				break;
			case cpp_redis::lindex:
				cmd_str = g_lindex_cmd;
				break;
			case cpp_redis::lset:
				cmd_str = g_lset_cmd;
				break;
			case cpp_redis::lrem:
				cmd_str = g_lrem_cmd;
				break;
			case cpp_redis::rpoplpush:
				cmd_str = g_rpoplpush_cmd;
				break;
			case cpp_redis::rpushx:
				cmd_str = g_rpushx_cmd;
				break;
			case cpp_redis::lpushx:
				cmd_str = g_lpushx_cmd;
				break;
			case cpp_redis::list_insert:
				cmd_str = g_list_insert_cmd;
				break;
			case cpp_redis::brpoplpush:
				cmd_str = g_brpoplpush_cmd;
				break;
			case cpp_redis::sadd:
				cmd_str = g_sadd_cmd;
				break;
			case cpp_redis::srem:
				cmd_str = g_srem_cmd;
				break;
			case cpp_redis::sismember:
				cmd_str = g_sismember_cmd;
				break;
			case cpp_redis::spop_elem:
				cmd_str = g_spop_elem_cmd;
				break;
			case cpp_redis::srandmember:
				cmd_str = g_srandmember_cmd;
				break;
			case cpp_redis::smove:
				cmd_str = g_smove_cmd;
				break;
			case cpp_redis::ssize:
				cmd_str = g_ssize_cmd;
				break;
			case cpp_redis::smembers:
				cmd_str = g_smembers_cmd;
				break;
			case cpp_redis::sinter:
				cmd_str = g_sinter_cmd;
			case cpp_redis::ssinter_store:
				cmd_str = g_sinter_store_cmd;
				break;
			case cpp_redis::sunion:
				cmd_str = g_sunion_cmd;
				break;
			case cpp_redis::ssunion_store:
				cmd_str = g_sunion_store_cmd;
				break;
			case cpp_redis::sdiff:
				cmd_str = g_sdiff_cmd;
				break;
			case cpp_redis::sdiff_store:
				cmd_str = g_sdiff_store_cmd;
				break;
			case cpp_redis::zset_add:
				cmd_str = g_zset_add_cmd;
				break;
			case cpp_redis::zset_score:
				cmd_str = g_zset_score_cmd;
				break;
			case cpp_redis::zset_incrby:
				cmd_str = g_zset_incrby_cmd;
				break;
			case cpp_redis::zset_card:
				cmd_str = g_zset_card_cmd;
				break;
			case cpp_redis::zset_count:
				cmd_str = g_zset_count_cmd;
				break;
			case cpp_redis::zset_range:
				cmd_str = g_zset_range_cmd;
				break;
			case cpp_redis::zset_revrange:
				cmd_str = g_zset_revrane_cmd;
				break;
			case cpp_redis::zset_range_score:
				cmd_str = g_zset_range_score_cmd;
				break;
			case cpp_redis::zset_rerange_score:
				cmd_str = g_zset_rerange_score_cmd;
				break;
			case cpp_redis::zset_rank:
				cmd_str = g_zset_rank_cmd;
				break;
			case cpp_redis::zset_revrank:
				cmd_str = g_zset_revrank_cmd;
				break;
			case cpp_redis::zset_rem:
				cmd_str = g_zset_rem_cmd;
				break;
			case cpp_redis::zset_remrangeby_rank:
				cmd_str = g_zset_remrangeby_rank_cmd;
				break;
			case cpp_redis::zset_remrangebyscore:
				cmd_str = g_zset_remrangebyscore_cmd;
				break;
			case cpp_redis::zset_rangebylex:
				cmd_str = g_zset_rangebylex_cmd;
				break;
			case cpp_redis::zset_lexcount:
				cmd_str = g_zset_lexcount_cmd;
				break;
			case cpp_redis::zset_remrangebylex:
				cmd_str = g_zset_remrangebylex_cmd;
				break;
			case cpp_redis::zset_union_store:
				cmd_str = g_zset_union_store_mcd;
				break;
			case cpp_redis::zset_inter_store:
				cmd_str = g_zset_inter_store_cmd;
				break;
			case cpp_redis::hash_set:
				cmd_str = g_hash_set_cmd;
				break;
			case cpp_redis::hash_setx:
				cmd_str = g_hash_setx_cmd;
				break;
			case cpp_redis::hash_exists:
				cmd_str = g_hash_exists_cmd;
				break;
			case cpp_redis::hash_get:
				cmd_str = g_hash_get_cmd;
				break;
			case cpp_redis::hash_del:
				cmd_str =g_hash_del_cmd;
				break;
			case cpp_redis::hash_len:
				cmd_str = g_hash_len_cmd;
				break;
			case cpp_redis::hash_mset:
				cmd_str = g_hash_mset_cmd;
				break;
			case cpp_redis::hash_mget:
				cmd_str = g_hash_mget_cmd;
				break;
			case  cpp_redis::hash_vals:
				cmd_str = g_hash_vals_cmd;
				break;
			case cpp_redis::hash_keys:
				cmd_str = g_hash_keys_cmd;
				break;
			case cpp_redis::hash_strlen:
				cmd_str = g_hash_strlen_cmd;
				break;
			case cpp_redis::hash_incrby:
				cmd_str = g_hash_incrby_cmd;
				break;
			case cpp_redis::hash_incrby_float:
				cmd_str =g_hash_incrby_float_cmd;
				break;
			case cpp_redis::hash_get_all:
				cmd_str = g_hash_get_all_cmd;
				break;
			default:
				break;
			}

			return std::move(cmd_str);
		}

		std::string get_aggregate_mothod(aggregate_mothod mothod)
		{
			std::string str;
			switch (mothod)
			{
			case aggregate_mothod::agg_sum:
				str = g_zset_aggsum;
				break;
			case aggregate_mothod::agg_min:
				str = g_zset_aggmin;
				break;
			case aggregate_mothod::agg_max:
				str = g_zset_aggmax;
			default:
				break;
			}

			return std::move(str);
		}

		template<typename...Args>
		std::string req_n_key(std::string&& command, Args&&...key)
		{
			auto tp = std::make_tuple<Args...>(std::forward<Args>(key)...);

			KEYS cmds;
			cpp_redis::unit::for_each_tuple_front(tp, [this, &cmds](std::string str, const int& index) {
				cmds.push_back(std::move(str));
				});


			return build_respone(std::move(command), std::move(cmds));
		}

		template<typename...Args>
		std::string req_n_key_value(std::string&& command, Args&&...key_value)
		{
			auto tuple = std::make_tuple<Args...>(std::forward<Args>(key_value)...);
			PAIRS pairs;

			int count = 0;
			PAIR pair;
			cpp_redis::unit::for_each_tuple_front(tuple, [this, &pairs, &count, &pair](std::string str, const int& index) {
				if (count < 2) {
					if (count == 0) {
						pair.first = std::move(str);
					}
					else
					{
						pair.second = std::move(str);
						pairs.push_back(std::move(pair));
						count = 0;
						return;
					}
				}

				++count;
				});

			return build_respone(std::move(command), std::move(pairs));
		}

		std::string req_n_keys(std::string&& command, KEYS&& keys)
		{
			return build_respone(std::move(command), std::forward<KEYS>(keys));
		}

		std::string req_key_value_has_senconds(std::string&& command, PAIR&& p, size_t seconds = 0)
		{
			std::vector<BYTES> cmds;
			cmds.push_back(std::move(p.first));
			if (seconds > 0) {
				cmds.push_back(std::move(unit::int_to_string(seconds)));
			}

			cmds.push_back(std::move(p.second));
			return build_respone(std::move(command), std::move(cmds));
		}

		std::string req_key_value_has_index(std::string&& command, PAIR&& p, std::string&& index)
		{
			std::vector<BYTES> cmds;
			cmds.push_back(std::move(p.first));
			cmds.push_back(std::move(index));
			cmds.push_back(std::move(p.second));
			return build_respone(std::move(command), std::move(cmds));
		}

		template<typename T>
		std::string build_respone(std::string&& command, T&& cmds)
		{
			auto key_cmds = std::move(cmds);
			BYTES v;
			add_number(get_command_size(key_cmds), v, '*');
			add_value_with_size(command, v);

			for (auto i = key_cmds.begin(); i != key_cmds.end(); ++i) {
				auto& p = *i;
				
				add_pair(p, v);
			}

			return std::move(v);
		}

		template <class T>
		PAIR make_pair(T&& key, std::string&& value)
		{
			PAIR u;
			u.first = std::move(key);
			u.second = std::move(value);
			return std::move(u);
		}

		template <class T>
		PAIR  make_pair(T&& key, const char* value)
		{
			PAIR u;
			u.first = std::move(key);
			u.second = std::move(std::string(value, strlen(value)));
			return std::move(u);
		}

		template <class T>
		PAIR  make_pair(T&& key, const unsigned char* value, size_t length)
		{
			PAIR u;
			u.first = std::move(key);
			u.second = std::move(std::string(value, length));
			return std::move(u);
		}

		template <class T>
		PAIR  make_pair(T&& key, const std::int32_t value)
		{
			std::stringstream str;
			str << value;
			return std::move(make_pair(std::move(key), str.str()));
		}

		template <class T>
		PAIR  make_pair(T&& key, const std::uint32_t value)
		{
			std::stringstream str;
			str << value;
			return std::move(make_pair(std::move(key), str.str()));
		}

		template<class T>
		void make_pairs(PAIR&& cmd, T& p)
		{
			if (cmd.first.empty()) {
				return;
			}

			p.push_back(std::move(cmd));
		}

		void makePairs(PAIR&& cmd, std::map<KEY, BYTES>& p)
		{
			if (cmd.first.empty()) {
				return;
			}

			p.insert(std::move(cmd));
		}

		template<typename T>
		int get_command_size(const T& cmds)
		{
			return cmds.size() + 1;
		}

		int get_command_size(const PAIRS& cmds)
		{
			return cmds.size() * 2 + 1;
		}

		template <class T>
		void add_value(T* x, size_t sz, BYTES& v)
		{
			std::copy(x, x + sz, std::back_inserter(v));
			std::copy(g_crlf.begin(), g_crlf.begin() + 2, std::back_inserter(v));
		}

		template<class T>
		void add_value(T& s, BYTES& v)
		{
			add_value(&s[0], s.size(), v);
		}

		void add_number(int k, BYTES& v, char start = '$')
		{
			std::string n = unit::int_to_string(k);
			v.push_back(start);
			add_value(n, v);
		}

		template<class T>
		void add_value_with_size(T& s, BYTES& v)
		{
			add_number(s.size(), v);
			add_value(&s[0], s.size(), v);
		}

		void add_pair(PAIR& p, BYTES& v)
		{
			add_value_with_size(p.first, v);
			add_value_with_size(p.second, v);
		}

		template <class T>
		void add_pair(T& p, BYTES& v)
		{
			add_value_with_size(p, v);
		}
	};
}//cpp_redis

#endif // cpp_redis_request_h__
