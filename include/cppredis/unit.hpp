#ifndef unit_h__
#define unit_h__
#include <sstream>
#include <regex>
#include <tuple>
#include "traits.hpp"

namespace cpp_redis {
	namespace unit {
#define PATTERN_IPV4   "^(\\d|[1-9]\\d|1\\d{2}|2[0-4]\\d|25[0-5])(\\.(\\d|[1-9]\\d|1\\d{2}|2[0-4]\\d|25[0-5])){3}$"
		/* IPv6 pattern */
#define PATTERN_IPV6   "^((([0-9A-Fa-f]{1,4}:){7}[0-9A-Fa-f]{1,4})|(([0-9A-Fa-f]{1,4}:){1,7}:)|(([0-9A-Fa-f]{1,4}:)"  \
                                       "{6}:[0-9A-Fa-f]{1,4})|(([0-9A-Fa-f]{1,4}:){5}(:[0-9A-Fa-f]{1,4}){1,2})|(([0-9A-Fa-f]{1,4}:)"  \
                                       "{4}(:[0-9A-Fa-f]{1,4}){1,3})|(([0-9A-Fa-f]{1,4}:){3}(:[0-9A-Fa-f]{1,4}){1,4})|(([0-9A-Fa-f]"  \
                                       "{1,4}:){2}(:[0-9A-Fa-f]{1,4}){1,5})|([0-9A-Fa-f]{1,4}:(:[0-9A-Fa-f]{1,4}){1,6})|(:(:[0-9A-Fa-f]"  \
                                       "{1,4}){1,7})|(([0-9A-Fa-f]{1,4}:){6}(\\d|[1-9]\\d|1\\d{2}|2[0-4]\\d|25[0-5])(\\.(\\d|[1-9]\\d|1\\d{2}" \
                                       "|2[0-4]\\d|25[0-5])){3})|(([0-9A-Fa-f]{1,4}:){5}:(\\d|[1-9]\\d|1\\d{2}|2[0-4]\\d|25[0-5])" \
                                       "(\\.(\\d|[1-9]\\d|1\\d{2}|2[0-4]\\d|25[0-5])){3})|(([0-9A-Fa-f]{1,4}:){4}(:[0-9A-Fa-f]{1,4})" \
                                       "{0,1}:(\\d|[1-9]\\d|1\\d{2}|2[0-4]\\d|25[0-5])(\\.(\\d|[1-9]\\d|1\\d{2}|2[0-4]\\d|25[0-5])){3})|" \
                                       "(([0-9A-Fa-f]{1,4}:){3}(:[0-9A-Fa-f]{1,4}){0,2}:(\\d|[1-9]\\d|1\\d{2}|2[0-4]\\d|25[0-5])" \
                                       "(\\.(\\d|[1-9]\\d|1\\d{2}|2[0-4]\\d|25[0-5])){3})|(([0-9A-Fa-f]{1,4}:){2}(:[0-9A-Fa-f]{1,4})" \
                                       "{0,3}:(\\d|[1-9]\\d|1\\d{2}|2[0-4]\\d|25[0-5])(\\.(\\d|[1-9]\\d|1\\d{2}|2[0-4]\\d|25[0-5])){3})|" \
                                       "([0-9A-Fa-f]{1,4}:(:[0-9A-Fa-f]{1,4}){0,4}:(\\d|[1-9]\\d|1\\d{2}|2[0-4]\\d|25[0-5])" \
                                       "(\\.(\\d|[1-9]\\d|1\\d{2}|2[0-4]\\d|25[0-5])){3})|(:(:[0-9A-Fa-f]{1,4})" \
                                       "{0,5}:(\\d|[1-9]\\d|1\\d{2}|2[0-4]\\d|25[0-5])(\\.(\\d|[1-9]\\d|1\\d{2}|2[0-4]\\d|25[0-5])){3}))$"



		static 	bool ip_addr_check(const std::string& ip_addr_dot_format)
		{

			std::regex expression(PATTERN_IPV4);
			return (std::regex_match(ip_addr_dot_format, expression));
		}

		static 	bool ip_v6_check(const std::string& ip_addr_dot_format)
		{
			std::regex expression(PATTERN_IPV6);
			return (std::regex_match(ip_addr_dot_format, expression));
		}

		static std::string int_to_string(std::uint32_t value)
		{
			std::ostringstream ostr;
			ostr << value;
			return std::move(ostr.str());
		}

		static std::string int_to_string(std::int32_t value)
		{
			std::ostringstream ostr;
			ostr << value;
			return std::move(ostr.str());
		}

		static std::string int_to_string(std::int16_t value)
		{
			std::ostringstream ostr;
			ostr << value;
			return std::move(ostr.str());
		}

		static std::string int_to_string(std::uint16_t value)
		{
			std::ostringstream ostr;
			ostr << value;
			return std::move(ostr.str());
		}

		static std::string int_to_string(std::int64_t value)
		{
			std::ostringstream ostr;
			ostr << value;
			return std::move(ostr.str());
		}

		static std::string int_to_string(std::uint64_t value)
		{
			std::ostringstream ostr;
			ostr << value;
			return std::move(ostr.str());
		}

		static std::string float_to_string(float value)
		{
			std::ostringstream ostr;
			ostr << value;
			return std::move(ostr.str());
		}

		static std::string double_to_string(double value)
		{
			std::ostringstream ostr;
			ostr << value;
			return std::move(ostr.str());
		}

		static std::string char_to_str(char value)
		{
			std::ostringstream ostr;
			ostr << value;
			return std::move(ostr.str());
		}

		static std::string char_to_str(unsigned char value)
		{
			std::ostringstream ostr;
			ostr << value;
			return std::move(ostr.str());
		}

		static int string_to_int(std::string && str)
		{
			if (str.empty()){
				return INT_MAX;
			}

			int value;
			std::istringstream istr(str);
			istr >>value;
			return value;
		}

		//正数编负数
		constexpr int turn(int a)
		{
			return (~a + 1);
		}

		template<typename F, typename...Ts, std::size_t...Is>
		void for_each_tuple_front(const std::tuple<Ts...>& tuple, F func, cpp_redis::traits::index_sequence<Is...>) {
			constexpr auto SIZE = std::tuple_size<cpp_redis::traits::remove_reference_t<decltype(tuple)>>::value;
#if (_MSC_VER >= 1700 && _MSC_VER <= 1900) //vs2012-vs2015
			if (constexpr(SIZE > 0)) {
				using expander = int[];
				(void)expander {
					((void)std::forward<F>(func)(std::get<Is>(tuple), std::integral_constant<size_t, Is>{}), false)...
				};
			}
#else
			if constexpr (SIZE > 0) {
				using expander = int[];
				(void)expander {
					((void)std::forward<F>(func)(std::get<Is>(tuple), std::integral_constant<size_t, Is>{}), false)...
				};
			}
#endif // _MSC_VER <=1923


		}

		template<typename F, typename...Ts>
		void for_each_tuple_front(const std::tuple<Ts...>& tuple, F func) {
			for_each_tuple_front(tuple, func, cpp_redis::traits::make_index_sequence<sizeof...(Ts)>());
		}

		template<typename F, typename...Ts, std::size_t...Is>
		void for_each_tuple_back(const std::tuple<Ts...>& tuple, F func, cpp_redis::traits::index_sequence<Is...>) {
			//匿名构造函数调用
			constexpr auto SIZE = std::tuple_size<cpp_redis::traits::remove_reference_t<decltype(tuple)>>::value;
#if (_MSC_VER >= 1700 && _MSC_VER <= 1900) //vs2012-vs2015
			if (constexpr (SIZE > 0)) {
				[](...) {}(0,
					((void)std::forward<F>(func)(std::get<Is>(tuple), std::integral_constant<size_t, Is>{}), false)...
					);
			}
#else
			if constexpr (SIZE > 0) {
				[](...) {}(0,
					((void)std::forward<F>(func)(std::get<Is>(tuple), std::integral_constant<size_t, Is>{}), false)...
					);
			}
#endif // #ifdef _MSC_VER <=1900


		}

		template<typename F, typename...Ts>
		void for_each_tuple_back(std::tuple<Ts...>& tuple, F func) {
			for_each_tuple_back(tuple, func, cpp_redis::traits::make_index_sequence<sizeof...(Ts)>());
		}


	}
}
#endif // unit_h__
