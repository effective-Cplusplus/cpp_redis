#ifndef cpp_define_h__
#define cpp_define_h__

#include <vector>

namespace cpp_redis {
	const std::string g_crlf = "\r\n";
	const std::string g_nil = "nil";

	constexpr std::uint16_t g_send_times = 500; //毫秒单位
	/******************常规操作***********************************/
	const std::string g_select_cmd = "SELECT";
	const std::string g_auth_cmd = "AUTH";
	const std::string g_del_cmd = "DEL";
	const std::string g_exists_cmd = "EXISTS";
	const std::string g_expire_cmd = "EXPIRE";
	const std::string g_expire_at_cmd = "EXPIREAT";
	const std::string g_ttl_cmd = "TTL";
	/****************字符串操作***********************************/
	const std::string g_setx_cmd = "SETEX";
	const std::string g_set_cmd = "SET";
	const std::string g_incrby_cmd = "INCRBY";
	const std::string g_get_cmd = "GET";
	const std::string g_get_set_cmd = "GETSET";
	const std::string g_substr_cmd = "SUBSTR";
	const std::string g_mget_cmd = "MGET";
	const std::string g_mset_cmd = "MSET";
	const std::string g_msetnx_cmd = "MSETNX";
	const std::string g_append_cmd = "APPEND";
	/******************list操作*************************************/
	const std::string g_rpush_cmd  = "RPUSH";
	const std::string g_lpush_cmd  = "LPUSH";
	const std::string g_llen_cmd   = "LLEN";
	const std::string g_lrange_cmd = "LRANGE";
	const std::string g_rpop_cmd   = "RPOP";
	const std::string g_lpop_cmd   = "LPOP";
	const std::string g_brpop_cmd  = "BRPOP";
	const std::string g_blpop_cmd  = "BLPOP";
	const std::string g_trim_cmd   = "LTRIM";
	const std::string g_lindex_cmd = "LINDEX";
	const std::string g_lset_cmd   = "LSET";
	const std::string g_lrem_cmd   = "LREM";
	const std::string g_rpoplpush_cmd  = "RPOPLPUSH";
	const std::string g_lpushx_cmd = "LPUSHX";
	const std::string g_rpushx_cmd = "RPUSHX";
	const std::string g_list_insert_cmd = "LINSERT";
	const std::string g_brpoplpush_cmd = "BRPOPLPUSH";
	/*******************set操作*************************************/
	const std::string g_sadd_cmd = "SADD";
	const std::string g_srem_cmd = "SREM";
	const std::string g_sismember_cmd = "SISMEMBER";
	const std::string g_spop_elem_cmd = "SPOP";
	const std::string g_srandmember_cmd = "SRANDMEMBER";
	const std::string g_smove_cmd = "SMOVE";
	const std::string g_ssize_cmd = "SCARD"; //奇葩名字
	const std::string g_smembers_cmd = "SMEMBERS";
	const std::string g_sinter_cmd = "SINTER";
	const std::string  g_sinter_store_cmd = "SINTERSTORE";
	const std::string g_sunion_cmd = "SUNION";//
	const std::string  g_sunion_store_cmd = "SUNIONSTORE";
	const std::string g_sdiff_cmd = "SDIFF";
	const std::string g_sdiff_store_cmd = "SDIFFSTORE";
	/*********************zset操作*********************************/
	const std::string g_zset_add_cmd     = "ZADD";
	const std::string g_zset_score_cmd   = "ZSCORE";
	const std::string g_zset_incrby_cmd =  "ZINCRBY";

	typedef std::string BYTES;
	typedef std::string KEY;
	typedef std::vector<KEY> KEYS;
	typedef std::vector<std::pair<KEY, BYTES> > PAIRS;//多个key一起发
	typedef PAIRS::value_type PAIR;
	typedef std::pair<int, BYTES> INT_VALUE;
	typedef std::vector<BYTES>VALUES;

	enum status
	{
		unconnected_ = -1,
		results_ = 0,
		int_result_ = 2,
		errors_ = 4,
		status_ = 8
	};

	enum redis_cmd
	{
		auth = 0,
		select = 1,
		set = 2,
		setex = 3,
		incrby = 4,
		del = 5,
		exists = 6,
		get = 7,
		expire = 8,
		expire_at = 9,
		ttl = 10,
		get_set = 11,
		substr = 12,
		mget = 13,
		mset = 14,
		msetnx = 15,
		append = 16,
		rpush = 17,
		lpush = 18,
		llen = 19,
		lrange = 20,
		rpop = 21,
		lpop = 22,
		brpop = 23,
		blpop = 24,
		ltrim = 25,
		lindex = 26,
		lset = 27,
		lrem = 28,
		rpoplpush = 29,
		lpushx = 30,
		rpushx = 31,
		list_insert=32,
		brpoplpush =33,
		sadd = 34,
		srem =35,
		sismember=36,
		spop_elem =37,
		srandmember =38,
		smove =39,
		ssize =40,
		smembers =41,
		sinter =42,
		ssinter_store =43,
		sunion =44,
		ssunion_store=45,
		sdiff =46,
		sdiff_store=47,
		zset_add = 48,
		zset_score =49,
		zset_incrby =50,
	};

	enum request_type
	{
		none = -1,
		string_request = 0,
		list_request = 1,
		set_request =2,
		zset_request =3,
		hash_request=4,
	};

	struct String {};
	struct List {};
	struct Set{};
	struct ZSet{};
	struct Hash{};
	struct A{};
}

#endif // cpp_define_h__
