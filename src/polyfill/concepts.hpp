#pragma once

// https://en.cppreference.com/w/cpp/feature_test

#if not defined __cpp_lib_concepts

// https://en.cppreference.com/w/cpp/header/concepts

namespace xstd {

template < class T >
concept integral = std::is_integral_v<T>;

template < typename T >
concept signed_integral = xstd::integral<T> && std::is_signed_v<T>;

template < typename T >
concept unsigned_integral = xstd::integral<T> && !xstd::signed_integral<T>;

template < typename R, typename F, typename... Args >
concept invocable_with_return = std::is_invocable_r_v<R, F, Args...>;

template < class T >
concept floating_point = std::is_floating_point_v<T>;

template < typename T, typename U >
concept same_as =
    std::is_same_v<T, U> &&
    std::is_same_v<U, T>;

template < typename Derived, typename Base >
concept derived_from =
    std::is_base_of_v<Base, Derived> &&
    std::is_convertible_v<const volatile Derived*, const volatile Base*>;

template < typename From, typename To >
concept convertible_to =
    std::is_convertible_v<From, To> &&
    requires { static_cast<To>(std::declval<From>()); };

template < typename T >
concept destructible =
    std::is_nothrow_destructible_v<T>;

template < typename T, typename... Args >
concept constructible_from =
    destructible<T> &&
    std::is_constructible_v<T, Args...>;

template < typename T >
concept move_constructible =
    constructible_from<T, T> &&
    convertible_to<T, T>;

template < typename T >
concept copy_constructible =
    move_constructible<T> &&
    constructible_from<T, T&> && convertible_to<T&, T> &&
    constructible_from<T, const T&> && convertible_to<const T&, T> &&
    constructible_from<T, const T> && convertible_to<const T, T>;

template < class F, class... Args >
concept invocable =
requires(F&& f, Args&&... args) {
    std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
    /* not required to be equality preserving */
};

template < class F, class... Args >
concept regular_invocable = xstd::invocable<F, Args...>;

namespace {

template <class B>
concept __boolean_testable_impl =                // exposition only
    xstd::convertible_to<B, bool>;

} // namespace

template <class B>
concept boolean_testable =                       // exposition only
    __boolean_testable_impl<B> &&
    requires (B&& b) {
        { !std::forward<B>(b) } -> __boolean_testable_impl;
    };

template < class F, class... Args >
concept predicate =
    xstd::regular_invocable<F, Args...> &&
    xstd::boolean_testable<std::invoke_result_t<F, Args...>>;

} // namespace xstd

namespace std {

using xstd::integral;
using xstd::signed_integral;
using xstd::unsigned_integral;
using xstd::invocable_with_return;
using xstd::floating_point;
using xstd::derived_from;
using xstd::convertible_to;
using xstd::destructible;
using xstd::constructible_from;
using xstd::move_constructible;
using xstd::copy_constructible;
using xstd::regular_invocable;
using xstd::invocable;
using xstd::predicate;

} // namespace std

#endif /* not __cpp_lib_concepts */

