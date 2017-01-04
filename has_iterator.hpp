#pragma once
#include <type_traits>
namespace matsulib
{
	namespace _detail
	{
		struct IteratorChecker
		{
		protected:
			struct True { static constexpr bool value = true; };
			struct False { static constexpr bool value = false; };

		protected:
			template <class _T> static constexpr auto check(typename _T::iterator*)->True;
			template <class _T> static constexpr auto check(...)->False;
		};
	}
	
	template <class _T>
	struct has_iterator : public _detail::IteratorChecker
	{
	public:
		static constexpr auto value = decltype(check <_T>(nullptr))::value;
	};
}