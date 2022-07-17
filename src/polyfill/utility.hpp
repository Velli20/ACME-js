#pragma once

// https://en.cppreference.com/w/cpp/header/utility

#if not defined __cpp_lib_integer_comparison_functions

namespace xstd {

template< class T, class U >
constexpr bool cmp_equal( T t, U u ) noexcept
{
    using UT = std::make_unsigned_t<T>;
    using UU = std::make_unsigned_t<U>;
    if constexpr (std::is_signed_v<T> == std::is_signed_v<U>)
    {
        return t == u;
    }
    else if constexpr (std::is_signed_v<T>)
    {
        return t < 0 ? false : UT(t) == u;
    }
    else
    {
        return u < 0 ? false : t == UU(u);
    }
}

template< class T, class U >
constexpr bool cmp_not_equal( T t, U u ) noexcept
{
    return not xstd::cmp_equal(t, u);
}

template< class T, class U >
constexpr bool cmp_less( T t, U u ) noexcept
{
    using UT = std::make_unsigned_t<T>;
    using UU = std::make_unsigned_t<U>;
    if constexpr (std::is_signed_v<T> == std::is_signed_v<U>)
    {
        return t < u;
    }
    else
     if constexpr (std::is_signed_v<T>)
    {
        return t < 0 ? true : UT(t) < u;
    }

    else
    {
        return u < 0 ? false : t < UU(u);
    }
}

template< class T, class U >
constexpr bool cmp_greater( T t, U u ) noexcept
{
    return xstd::cmp_less(u, t);
}

template< class T, class U >
constexpr bool cmp_less_equal( T t, U u ) noexcept
{
    return not xstd::cmp_greater(t, u);
}

template< class T, class U >
constexpr bool cmp_greater_equal( T t, U u ) noexcept
{
    return not xstd::cmp_less(t, u);
}

} // namespace xstd

namespace std {

using xstd::cmp_equal;
using xstd::cmp_not_equal;
using xstd::cmp_less;
using xstd::cmp_greater;
using xstd::cmp_less_equal;
using xstd::cmp_greater_equal;

} // namespace std

#endif /* not __cpp_lib_integer_comparison_functions */

#if not defined __cpp_lib_to_underlying

namespace xstd {

template < typename Enum >
[[nodiscard]] constexpr std::underlying_type_t<Enum> to_underlying(Enum e) noexcept
{
    return static_cast<std::underlying_type_t<Enum>>(e);
}

} // namespace xstd

namespace std {

using xstd::to_underlying;

} // namespace std

#endif /* not __cpp_lib_to_underlying */
