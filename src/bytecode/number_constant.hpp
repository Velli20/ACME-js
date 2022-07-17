#pragma once

namespace acme {

union number_constant
{
    bool             m_bool;
    float            m_float;
    std::int32_t     m_i32;
    std::uint32_t    m_u32;
    acme::identifier m_hash;
};

static_assert(std::is_standard_layout<number_constant>::value, "");
static_assert(sizeof(number_constant) == 4, "");

} // namespace acme
