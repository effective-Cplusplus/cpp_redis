#ifndef cpp_define_h__
#define cpp_define_h__

#include <vector>

namespace cpp_redis {
	const std::string g_crlf                        = "\r\n";
	const std::string g_nil                         = "nil";

	constexpr std::uint16_t g_send_times            = 500; //毫秒单位
	/******************常规操作***********************************/
	const std::string g_select_cmd                  = "SELECT";
	const std::string g_auth_cmd                    = "AUTH";
	const std::string g_del_cmd                     = "DEL";
	const std::string g_exists_cmd                  = "EXISTS";
	const std::string g_pexpire_cmd                 = "EXPIRE";
	const std::string g_expire_cmd                  = "PEXPIRE";
	const std::string g_expire_at_cmd               = "EXPIREAT";
	const std::string g_pexpire_at_cmd              = "PEXPIREAT";
	const std::string g_remove_expire_cmd           = "PERSIST";
	const std::string g_ttl_cmd                     = "TTL";
	const std::string g_rename_cmd                  = "RENAME";
	const std::string g_renamenx_cmd                = "RENAMENX";
	/****************字符串操作***********************************/
	const std::string g_setx_cmd                    = "SETEX";
	const std::string g_psetx_cmd                   = "PSETEX";
	const std::string g_set_cmd                     = "SET";
	const std::string g_strsub_cmd                  = "GETRANGE";//2.0之前叫substr
	const std::string g_incrby_cmd                  = "INCRBY";
	const std::string g_incr_cmd                    = "INCR";
	const std::string g_decr_cmd                    = "DECR";
	const std::string g_decrby_cmd                  = "DECRBY";
	const std::string g_incr_by_float_cmd           = "INCRBYFLOAT";
	const std::string g_get_cmd                     = "GET";
	const std::string g_get_set_cmd                 = "GETSET";
	const std::string g_substr_cmd                  = "SUBSTR";
	const std::string g_mget_cmd                    = "MGET";
	const std::string g_mset_cmd                    = "MSET";
	const std::string g_msetnx_cmd                  = "MSETNX";
	const std::string g_append_cmd                  = "APPEND";
	/******************list操作*************************************/
	const std::string g_rpush_cmd                   = "RPUSH";
	const std::string g_lpush_cmd                   = "LPUSH";
	const std::string g_llen_cmd                    = "LLEN";
	const std::string g_lrange_cmd                  = "LRANGE";
	const std::string g_rpop_cmd                    = "RPOP";
	const std::string g_lpop_cmd                    = "LPOP";
	const std::string g_brpop_cmd                   = "BRPOP";
	const std::string g_blpop_cmd                   = "BLPOP";
	const std::string g_trim_cmd                    = "LTRIM";
	const std::string g_lindex_cmd                  = "LINDEX";
	const std::string g_lset_cmd                    = "LSET";
	const std::string g_lrem_cmd                    = "LREM";
	const std::string g_rpoplpush_cmd               = "RPOPLPUSH";
	const std::string g_lpushx_cmd                  = "LPUSHX";
	const std::string g_rpushx_cmd                  = "RPUSHX";
	const std::string g_list_insert_cmd             = "LINSERT";
	const std::string g_brpoplpush_cmd              = "BRPOPLPUSH";
	/*******************set操作*************************************/
	const std::string g_sadd_cmd                    = "SADD";
	const std::string g_srem_cmd                    = "SREM";
	const std::string g_sismember_cmd               = "SISMEMBER";
	const std::string g_spop_elem_cmd               = "SPOP";
	const std::string g_srandmember_cmd             = "SRANDMEMBER";
	const std::string g_smove_cmd                   = "SMOVE";
	const std::string g_ssize_cmd                   = "SCARD"; //奇葩名字
	const std::string g_smembers_cmd                = "SMEMBERS";
	const std::string g_sinter_cmd                  = "SINTER";
	const std::string  g_sinter_store_cmd           = "SINTERSTORE";
	const std::string g_sunion_cmd                  = "SUNION";//
	const std::string  g_sunion_store_cmd           = "SUNIONSTORE";
	const std::string g_sdiff_cmd                   = "SDIFF";
	const std::string g_sdiff_store_cmd             = "SDIFFSTORE";
	/*********************zset操作**************************************/
	const std::string g_zset_aggsum                 = "SUM";
	const std::string g_zset_aggmin                 = "MIN";
	const std::string g_zset_aggmax                 = "MAX";
	const std::string g_zset_add_cmd                = "ZADD";
	const std::string g_zset_score_cmd              = "ZSCORE";
	const std::string g_zset_incrby_cmd             = "ZINCRBY";
	const std::string g_zset_card_cmd               = "ZCARD";
	const std::string g_zset_count_cmd              = "ZCOUNT";
	const std::string g_zset_range_cmd              = "ZRANGE";
	const std::string g_zset_revrane_cmd            = "ZREVRANGE";
	const std::string g_zset_range_score_cmd        = "ZRANGEBYSCORE";
	const std::string g_zset_rerange_score_cmd      = "ZREVRANGEBYSCORE";
	const std::string g_zset_rank_cmd               = "ZRANK";
	const std::string g_zset_revrank_cmd            = "ZREVRANK";
	const std::string g_zset_rem_cmd                = "ZREM";
	const std::string g_zset_remrangeby_rank_cmd    = "ZREMRANGEBYRANK";
	const std::string g_zset_remrangebyscore_cmd    = "ZREMRANGEBYSCORE";
	const std::string g_zset_rangebylex_cmd         = "ZRANGEBYLEX";
	const std::string g_zset_lexcount_cmd           = "ZLEXCOUNT";
	const std::string g_zset_remrangebylex_cmd      = "ZREMRANGEBYLEX";
	const std::string g_zset_union_store_mcd        = "ZUNIONSTORE";
	const std::string g_zset_inter_store_cmd        = "ZINTERSTORE";
	/***********************hash操作************************************/
	const std::string g_hash_set_cmd                = "HSET";
	const std::string g_hash_setx_cmd               = "HSETNX";
	const std::string g_hash_exists_cmd             = "HEXISTS";
	const std::string g_hash_get_cmd                = "HGET";
	const std::string g_hash_del_cmd                = "HDEL";
	const std::string g_hash_len_cmd                = "HLEN";
	const std::string g_hash_strlen_cmd             = "HSTRLEN";
	const std::string g_hash_incrby_cmd             = "HINCRBY";
	const std::string g_hash_mset_cmd               = "HMSET";
	const std::string g_hash_mget_cmd               = "HMGET";
	const std::string g_hash_keys_cmd               = "HKEYS";
	const std::string g_hash_vals_cmd               = "HVALS";
	const std::string g_hash_get_all_cmd            = "HGETALL";
	const std::string g_hash_incrby_float_cmd       = "HINCRBYFLOAT";
	/*******************************************************************/

	enum status
	{
		unconnected_                                = -1,
		results_                                    = 0,
		int_result_                                 = 2,
		errors_                                     = 4,
		status_                                     = 8
	};

	enum redis_cmd
	{
		auth                                        = 0,
		select                                      = 1,
		set                                         = 2,
		setex                                       = 3,
		psetex                                      = 4,
		strsub                                      = 5,
		incr                                        = 6,
		incrby                                      = 7,
		incr_by_float                               = 8,
		decr                                        = 9,
		decyby                                      = 10,
		del                                         = 11,
		exists                                      = 12,
		get                                         = 13,
		expire                                      = 14,
		pexpire                                     = 15,
		expire_at                                   = 16,
		pexpire_at                                  = 17,
		remove_expire                               = 18,
		ttl                                         = 19,
		rename                                      = 20,
		renamenx                                    = 21,
		get_set                                     = 22,
		substr                                      = 23,
		mget                                        = 24,
		mset                                        = 25,
		msetnx                                      = 26,
		append                                      = 27,
		rpush                                       = 28,
		lpush                                       = 29,
		llen                                        = 30,
		lrange                                      = 31,
		rpop                                        = 32,
		lpop                                        = 33,
		brpop                                       = 34,
		blpop                                       = 35,
		ltrim                                       = 36,
		lindex                                      = 37,
		lset                                        = 38,
		lrem                                        = 39,
		rpoplpush                                   = 40,
		lpushx                                      = 41,
		rpushx                                      = 42,
		list_insert                                 = 43,
		brpoplpush                                  = 44,
		sadd                                        = 45,
		srem                                        = 46,
		sismember                                   = 47,
		spop_elem                                   = 48,
		srandmember                                 = 49,
		smove                                       = 50,
		ssize                                       = 51,
		smembers                                    = 52,
		sinter                                      = 53,
		ssinter_store                               = 54,
		sunion                                      = 55,
		ssunion_store                               = 56,
		sdiff                                       = 57,
		sdiff_store                                 = 58,
		zset_add                                    = 59,
		zset_score                                  = 60,
		zset_incrby                                 = 61,
		zset_card                                   = 62,
		zset_count                                  = 63,
		zset_range                                  = 64,
		zset_rank                                   = 65,
		zset_rem                                    = 66,
		zset_revrank                                = 67,
		zset_revrange                               = 68,
		zset_lexcount                               = 69,
		zset_rangebylex                             = 70,
		zset_union_store                            = 71,
		zset_inter_store                            = 72,
		zset_range_score                            = 73,
		zset_remrangebylex                          = 74,
		zset_rerange_score                          = 75,
		zset_remrangebyscore                        = 76,
		zset_remrangeby_rank                        = 77,
		hash_set                                    = 78,
		hash_setx                                   = 79,
		hash_exists                                 = 80,
		hash_get                                    = 81,
		hash_del                                    = 82,
		hash_len                                    = 83,
		hash_mset                                   = 84,
		hash_mget                                   = 85,
		hash_vals                                   = 86,
		hash_keys                                   = 87,
		hash_strlen                                 = 88,
		hash_incrby                                 = 89,
		hash_get_all                                = 90,
		hash_incrby_float                           = 91,
	};

	enum request_type
	{
		none                                        = -1,
		string_request                              = 0,
		list_request                                = 1,
		set_request                                 = 2,
		zset_request                                = 3,
		hash_request                                = 4,
	};

	enum aggregate_mothod
	{
		agg_none                                    = -1,
		agg_sum                                     = 0,
		agg_min                                     = 1,
		agg_max                                     = 2,
	};

	struct String {};
	struct List {};
	struct Set {};
	struct ZSet {};
	struct Hash {};
}

#endif // cpp_define_h__
