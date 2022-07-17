#pragma once

#include "hash.hpp"

namespace acme::rtti::detail {

template <class T>
constexpr auto class_name() -> std::string_view
{
#if defined(_MSC_VER)
    return __FUNCSIG__;
#else
    return __PRETTY_FUNCTION__;
#endif
}

template <class T>
constexpr auto rtti_name()
{
    constexpr auto probeclass_name = class_name<double>();
    constexpr auto raw_length      = std::string_view("double").size();
    constexpr auto prefix_length   = probeclass_name.find("double");

    static_assert(prefix_length != std::string_view::npos, "Cannot extract typename from function signature");

    constexpr auto suffix_length = probeclass_name.size() - prefix_length - raw_length;
    constexpr auto name          = class_name<T>();

    return std::string_view(name.data() + prefix_length, name.size() - prefix_length - suffix_length);
}

} // acme::rtti::detail

namespace acme::rtti {

template <class T>
constexpr auto type_index()
{
    constexpr auto name = detail::rtti_name<T>();

    return acme::detail::hash_fnv1a(name);
}

}  // namespace acme::rtti
