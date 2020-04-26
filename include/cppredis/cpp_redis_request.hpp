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
			case cpp_redis::psetex:
				cmd_str = g_psetx_cmd;
				break;
			case cpp_redis::strsub:
				cmd_str = g_strsub_cmd;
				break;
			case cpp_redis::incrby:
				cmd_str = g_incrby_cmd;
				break;
			case cpp_redis::incr:
				cmd_str = g_incr_cmd;
				break;
			case cpp_redis::incr_by_float:
				cmd_str = g_incr_by_float_cmd;
				break;
			case cpp_redis::decr:
				cmd_str = g_decr_cmd;
				break;
			case redis_cmd::decyby:
				cmd_str = g_decrby_cmd;
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

			std::vector<std::string> cmds;
			cpp_redis::unit::for_each_tuple_front(tp, [this, &cmds](std::string str, const int& index) {
				cmds.push_back(std::move(str));
				});


			return build_respone(std::move(command), std::move(cmds));
		}

		std::string req_n_keys(std::string&& command, std::vector<std::string>&& keys)
		{
			return build_respone(std::move(command), std::forward<std::vector<std::string>>(keys));
		}
private:
		std::string build_respone(std::string&& command, std::vector<std::string>&& cmds)
		{
			std::vector<std::string> key_cmds = std::move(cmds);
			size_t cmd_size = key_cmds.size();
			
			std::string v;
			add_number(cmd_size+1, v, '*');
			add_value_with_size(std::forward<std::string>(command), v);
			
			for (size_t index =0;index < cmd_size;++index){
				add_pair(std::move(key_cmds[index]), v);
			}

			return std::move(v);
		}

		void add_value(std::string&& s, std::string& v)
		{
			std::string str = std::move(s);
			v.append(str).append(g_crlf);
		}

		void add_number(int k, std::string& v, char start = '$')
		{
			v.push_back(start);
			add_value(unit::int_to_string(k), v);
		}

		void add_value_with_size(std::string&& s, std::string& v)
		{
			add_number(s.size(), v);
			add_value(std::forward<std::string>(s),v);
		}

		void add_pair(std::string&& p, std::string& v)
		{
			add_value_with_size(std::forward<std::string>(p), v);
		}
	};
}//cpp_redis

#endif // cpp_redis_request_h__
