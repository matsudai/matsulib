#pragma once

#include <type_traits>

namespace matsulib
{
	namespace _detail
	{
		struct IteratorChecker
		{
		protected:
			template <class _T> static constexpr auto check(typename _T::iterator*) -> std::true_type;
			template <class _T> static constexpr auto check(...) -> std::false_type;
		};
	}
	
	template <class _T>
	struct has_iterator : public _detail::IteratorChecker
	{
	public:
		using type = decltype(check <_T>(nullptr));
		static constexpr auto value = type::value;
	};
}