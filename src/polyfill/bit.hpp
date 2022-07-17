#pragma once

#if not defined __cpp_lib_bit_cast

// https://en.cppreference.com/w/cpp/header/bit

namespace xstd {

template <class To, class From>
std::enable_if_t< sizeof(To) == sizeof(From) && std::is_trivially_copyable_v<From> && std::is_trivially_copyable_v<To>, To>
bit_cast(const From& src) noexcept
{
    static_assert(std::is_trivially_constructible_v<To>, "This implementation additionally requires destination type to be trivially constructible");

    To dst;
    std::memcpy(&dst, &src, sizeof(To));
    return dst;
}

} // namespace xstd

// https://en.cppreference.com/w/cpp/feature_test

namespace std {

using xstd::bit_cast;

} // namespace std

#endif /* not __cpp_lib_bit_cast */
