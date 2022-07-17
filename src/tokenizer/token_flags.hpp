#pragma once

namespace acme {

enum class token_flags : std::uint32_t
{
    none                 = 0b0000'00000'00000'00000,
    punctuator           = 0b0000'00000'00000'00010,
    literal              = 0b0000'00101'00000'00000,
    keyword              = 0b0000'00001'00000'00000,
};

} // namespace acme

template <>
struct acme::is_bitmask_enum <acme::token_flags> : std::true_type {};
