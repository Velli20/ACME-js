#pragma once

namespace acme {

struct string_constant
{
    acme::identifier m_hash;
    std::uint32_t    m_buffer_offset;
    std::uint32_t    m_length;
};

static_assert(std::is_standard_layout<string_constant>::value, "");
static_assert(sizeof(string_constant) == 12, "");

} // namespace acme
