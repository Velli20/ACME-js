#pragma once

namespace acme {

template <typename T>
struct is_bitmask_enum : std::false_type {};

template <typename T> requires(is_bitmask_enum<T>::value)
[[nodiscard]] constexpr auto operator^=(T& lhs, T rhs) noexcept
{
    using Type = typename std::underlying_type<T>::type;
    lhs = static_cast<T>(static_cast<Type>(lhs) ^ static_cast<Type>(rhs));
    return lhs;
}

template <typename T> requires(is_bitmask_enum<T>::value)
[[nodiscard]] constexpr auto operator|(T lhs, T rhs) noexcept
{
    using Type = typename std::underlying_type<T>::type;
    return static_cast<T>(static_cast<Type>(lhs) | static_cast<Type>(rhs));
}

template <typename T> requires(is_bitmask_enum<T>::value)
[[nodiscard]] constexpr auto operator|=(T& lhs, T rhs) noexcept
{
    using Type = typename std::underlying_type<T>::type;
    lhs = static_cast<T>(static_cast<Type>(lhs) | static_cast<Type>(rhs));
    return lhs;
}

template <typename T> requires(is_bitmask_enum<T>::value)
[[nodiscard]] constexpr auto operator&=(T& lhs, T rhs) noexcept
{
    using Type = typename std::underlying_type<T>::type;
    lhs = static_cast<T>(static_cast<Type>(lhs) & static_cast<Type>(rhs));
    return lhs;
}

template <typename T> requires(is_bitmask_enum<T>::value)
[[nodiscard]] constexpr auto operator&(T lhs, T rhs) noexcept
{
    using Type = typename std::underlying_type<T>::type;
    return static_cast<T>(static_cast<Type>(lhs) & static_cast<Type>(rhs));
}

template <typename T> requires(is_bitmask_enum<T>::value)
[[nodiscard]] constexpr auto operator^(T lhs, T rhs) noexcept
{
    using Type = typename std::underlying_type<T>::type;
    return static_cast<T>(static_cast<Type>(lhs) ^ static_cast<Type>(rhs));
}

template <typename T> requires(is_bitmask_enum<T>::value)
[[nodiscard]] constexpr auto operator~(T rhs) noexcept
{
    using Type = typename std::underlying_type<T>::type;
    return static_cast<T>(~static_cast<Type>(rhs));
}

template <typename T> requires(is_bitmask_enum<T>::value)
[[nodiscard]] constexpr auto has_flag(T value, T mask) noexcept
{
    using Type = typename std::underlying_type<T>::type;
    return static_cast<Type>(value & mask) == static_cast<Type>(mask) ? true : false;
}

template <typename T> requires(is_bitmask_enum<T>::value)
[[nodiscard]] constexpr auto has_any_flag(T value, T mask) noexcept
{
    using Type = typename std::underlying_type<T>::type;
    return (static_cast<Type>(value & mask) != 0 ? true : false);
}

} // namespace acme
