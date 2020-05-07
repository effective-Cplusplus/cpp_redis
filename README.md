<h2 align="center">cpp_redis</h2>

## 介绍
1. 基于boost.asio
1. 支持string、list、set、hash、zset五种数据结构
1. 支持事务提交
1. 新增实现redis锁

## 说明
1. 使用c++11以及以上编译(若c++11编译不过，请用c++14)
## 列子
```cpp
#include <iostream>
#include <cppredis/client.hpp>
//string请求
cpp_redis::redis_client<cpp_redis::String> client;
bool falg = client.connect_to(IP地址,端口,redis密码);
int a = 0;
falg = client.set_db_num(a);
falg = client.setex("name", "yinpinghua", 30);
falg = client.set("name", "yinpinghua");
auto result = client.incr("name1");
auto result1 = client.decr("name1");
falg = client.delete_key("name1");
falg = client.is_key_exist("name1");
auto value =client.get_reflect_value("name");

falg = client.expire("name1",30);
falg = client.expire_at("name", 1587468798);
client.setex("name4", "112345", 60);

auto expire = client.remainder_ttl("name4");

std::string str1 = client.get_set_key("name","5678941");

client.substr_reflect_value("name", 0, 5);
auto vecs1 =client.multi_get_keys("name");
auto vecs2= client.multi_get_keys("name", "name1");
auto vecs3 = client.multi_get_keys("name", "name1", "name2");

client.multi_set_if_not_set("name", "1234", "name1", "456", "name3", "789","name4","562450");

client.multi_set_keys("name", "1234", "name1", "456", "name3", "789", "name4", "562450");
client.append_value("name4", "123456789");
client.set("name1234", "456789");
client.expire("name1234", 60);
//list请求
cpp_redis::redis_client<cpp_redis::List> client;
bool falg = client.connect_to(IP地址,端口,redis密码);
client.list_rpush("name2", "5678945");
client.list_lpush("name222", "5678945");

client.list_size("name2221234");
client.list_lpop("name222");
client.list_rpush("name22", "12");
client.list_rpush("name22", "4562145");
client.list_rpush("name22", "56");
client.list_rpush("name22", "78");

client.list_lpush("name22", "78");
client.list_lpush("name22", "56");
client.list_lpush("name22", "34");
client.list_lpush("name22", "12");
auto  reslut =client.list_rpoplpush("name22", "name");
client.list_rpush("name22", "78");
client.list_lpop("name22");

client.list_lpush_if("name22", "12");
client.list_del_elem("name22","56",-1);

auto res = client.list_insert_after("name", "98", "123456790");

auto res = client.list_brpoplpush("name","name2345",20);
auto err = client.get_current_error();

//set请求
cpp_redis::redis_client<cpp_redis::Set> client;
bool falg = client.connect_to(IP地址,端口,redis密码);
auto res = client.set_add("name", "1234", "5678","910245","78945");
auto res1 = client.set_delete_elem("name", "1234", "5678");
auto res = client.set_is_member("name", "78945");
auto res1 = client.set_rand_elem("name111",2);

auto res1 = client.set_move_elem("name", "name1", "7894511111");

auto res1 = client.set_get_size("name111");

auto res1 = client.set_sinter("name","name1");

auto res1 = client.set_union_store("name3","name","name2");

auto res12 = client.set_diff( "name", "name2");

auto res1 = client.set_diff_store("name4","name", "name1");

//Zset请求
cpp_redis::redis_client<cpp_redis::ZSet> client;
bool falg = client.connect_to(IP地址,端口,redis密码);

auto res = client.zset_add("name", 1, "12345", 2, 56);
std::string store = "yinpinghua";
auto res2 = client.zset_incrby("name",2,"yinpinghua");
std::string error = client.get_current_error();
int i = client.zset_count("name");
auto count = client.zset_range_counts("name", 1, 4);
auto res1 = client.zset_range("name", 1, 2,false);
auto res1 = client.zset_revrrank("name", 0, 1);

auto res1 = client.zset_range_score("name",0,1,false,true);

auto res1 = client.zset_revrange_score("name", 7, 0,true);

auto res1 = client.zset_rank("name","sp1");
auto res = client.zset_rank("name", "yinpinghua");
auto res1 = client.zset_revrank("name", "yinpinghua");

auto res1 = client.zset_rem("name", "yinpinghua","sp",1);

std::string str = "yinpinghua";
auto res1 = client.zset_add("name",1, &str[0], 2, "sp", 1.25, "shagua");

auto res = client.zset_remrangeby_rank("name", 1, 2);

auto res = client.zset_remrangebyscore("name", 1, 2);
std::string str = "yinpinghua";
auto res1 = client.zset_add("name", 1, &str[0], 2, "sp", 1.25, "shagua");

auto res = client.zset_rangebylex("name1","[1", "[3");
auto res = client.zset_lexcount("name1", "[1", "[3");

auto res = client.zset_remrangebylex("name1", "[1", "[3");
auto res = client.zset_union_store("name4", 2, cpp_redis::aggregate_mothod::agg_sum,0,1,"name", "name2");
auto res = client.zset_inter_store("name6", 2, cpp_redis::aggregate_mothod::agg_sum, 0, 1, "name", "name2";

//hash请求
cpp_redis::redis_client<cpp_redis::Hash> client;
bool falg = client.connect_to(IP地址,端口,redis密码);

auto res = client.hash_set("name", false, "123456");
auto  res = client.hash_setx("name", 12, "123456");

auto  res = client.hash_setx("name", 12, "123456");

auto res = client.hash_exists("name", false);

std::string field;
auto res = client.hash_get("name", 1);

auto res1 = client.hash_set("db", 123, "redis.com");
auto res = client.hash_del("name", false);

auto res = client.hash_len("db");

std::string key = "db";
std::string files = "123";

auto res = client.hash_strlen(std::move(key), files);

auto res = client.hash_incrby(std::move(key), files, 40);
auto res1 = client.hash_incrby(std::move(key), false, 40);

auto res112 = client.hash_incrby_float(std::move(key), 456, 1.22f);
float f = 1.23f;
auto res = client.hash_mset(std::move(key), "123456", 23, 4, 56, false, f);

auto res = client.hash_mget(std::move(key), 1, 4, false, 123456);
auto res = client.hash_keys(std::move(key));
auto res = client.hash_vals(std::move(key));

auto res = client.hash_get_all(std::move(key));
