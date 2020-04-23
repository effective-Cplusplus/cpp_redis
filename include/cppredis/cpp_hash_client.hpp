#ifndef cpp_hash_client_h__
#define cpp_hash_client_h__

namespace cpp_redis
{
	class hash_client :public client {
	public:
		hash_client() = default;
		virtual ~hash_client() {

		}
	};
}

#endif // cpp_hash_client_h__
