#ifndef cpp_hash_client_h__
#define cpp_hash_client_h__

#include "client_interface.hpp"

namespace cpp_redis
{
	class hash_client :public client {
	public:
		hash_client() = default;
		virtual ~hash_client() {

		}

		virtual int hash_set(std::string&& field, std::string&& value)
		{
			check_args();
			return 0;
		}
	};
}

#endif // cpp_hash_client_h__
