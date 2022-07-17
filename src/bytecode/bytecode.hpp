#pragma once

#include "opcode.hpp"
#include "number_constant.hpp"
#include "string_constant.hpp"
#include "instruction.hpp"

namespace acme {

struct bytecode
{
    using instructions_view     = std::span<const acme::instruction>;
    using number_constants_view = std::span<const acme::number_constant>;
    using string_constants_view = std::span<const acme::string_constant>;
    using string_buffer_view    = std::span<const char>;

    constexpr bytecode() = default;

    constexpr bytecode(
        instructions_view     instructions,
        number_constants_view number_constants = {},
        string_constants_view string_constant  = {},
        string_buffer_view    string_buffer  = {}
    )
        : m_instructions{instructions}
        , m_number_constants{number_constants}
        , m_string_constants{string_constant}
        , m_string_buffer{string_buffer}
    {}

    [[nodiscard]] constexpr auto instruction(std::size_t offset) const -> std::optional<acme::instruction>
    {
        if ( offset < m_instructions.size() )
        {
            return m_instructions[offset];
        }

        return {};
    }

    [[nodiscard]] constexpr auto instructions() const
    {
        return m_instructions;
    }

    [[nodiscard]] constexpr auto var_id(std::size_t offset) const -> acme::identifier
    {
        if ( offset >= m_number_constants.size() )
        {
            return {};
        }

        return m_number_constants[offset].m_hash;
    }

    template <typename T>
    [[nodiscard]] constexpr auto constant(std::size_t offset) const -> acme::script_value
    {
        if ( offset >= m_number_constants.size() )
        {
            return {};
        }

        if constexpr ( std::is_same_v<T, double> )
        {
            return acme::script_value{acme::number{static_cast<double>(m_number_constants[offset].m_float)}};
        }

        else if constexpr ( std::is_same_v<T, std::int32_t> )
        {
            return acme::script_value{acme::number{m_number_constants[offset].m_i32}};
        }

        else if constexpr ( std::is_same_v<T, std::uint32_t> )
        {
            return acme::script_value{acme::number{m_number_constants[offset].m_u32}};
        }

        else if constexpr ( std::is_same_v<T, acme::identifier> )
        {
            return acme::script_value{m_number_constants[offset].m_hash};
        }

        else if constexpr ( std::is_same_v<T, acme::string> )
        {
            const auto [hash, buffer_offset, length] = m_string_constants[offset];

            auto str = std::string_view{&(m_string_buffer[buffer_offset]), length};

            return acme::script_value{acme::string{str}};
        }
    }

    instructions_view     m_instructions{};
    number_constants_view m_number_constants{};
    string_constants_view m_string_constants{};
    string_buffer_view    m_string_buffer{};
};

} // namespace acme
