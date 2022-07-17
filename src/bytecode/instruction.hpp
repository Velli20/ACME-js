#pragma once

namespace acme {

struct instruction
{
    using immediate_type   = std::uint32_t;
    using stack_index_type = std::uint32_t;

    union
    {
        struct
        {
            opcode         m_code;
            immediate_type m_immediate : 24;
        };

        stack_index_type m_stack_index;
    };

    [[nodiscard]] static constexpr auto make(opcode c, immediate_type im) -> instruction
    {
        return { { .m_code = c,  .m_immediate = im } };
    }
};

static_assert(std::is_standard_layout<instruction>::value, "");
//static_assert(sizeof(instruction) == 4, "");

[[nodiscard]] constexpr auto operand(const instruction i)
{
    return i.m_code;
}

[[nodiscard]] constexpr auto immediate(const instruction i)
{
    return i.m_immediate;
}

[[nodiscard]] constexpr auto immediate(
    instruction&                i,
    instruction::immediate_type imm
)
{
    i.m_immediate = imm;
}

[[nodiscard]] static constexpr auto to_string(instruction ins) noexcept -> std::string_view
{
    return to_string(operand(ins));
}

} // namespace acme
