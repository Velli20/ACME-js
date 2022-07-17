#pragma once

namespace acme {

struct virtual_machine
{
    using exec_scope_stack     = acme::dynamic_cvector<acme::execution_scope>;
    using program_counter_type = std::size_t;
    using stack_type           = acme::containers::stack<24, acme::script_value>;
    using var_stack_type       = acme::containers::var_stack<24>;
    using immediate_type       = acme::instruction::immediate_type;

    constexpr virtual_machine() = default;

    virtual_machine(platform::pmr::memory_resource* resource)
        : m_string_pool{resource}
        {}

    inline auto execute(const bytecode& code);

    [[nodiscard]] auto program_counter() -> program_counter_type&
    {
        return m_pc;
    }

    constexpr auto push_scope()
    {
        m_scope_stack.push_back({});
    }

    constexpr auto pop_scope()
    {
        if ( std::is_constant_evaluated() == false )
        {
            assert(m_scope_stack.empty() == false);
        }

        auto s = m_scope_stack.pop_back();
        s.locals().clear();
    }

    [[nodiscard]] auto get_var(acme::identifier id) -> var_stack_type::optional_reference
    {
        for ( auto it = m_scope_stack.rbegin(); it != m_scope_stack.rend(); ++it )
        {
            if ( auto var = (*it).locals().get(id); var.has_value() == true )
            {
                return var;
            }
        }

        return {};
    }

    [[nodiscard]] auto stack() -> stack_type&
    {
        return m_stack;
    }

    [[nodiscard]] auto locals() -> var_stack_type&
    {
        if ( std::is_constant_evaluated() == false )
        {
            assert(m_scope_stack.empty() == false);
        }

        return m_scope_stack.back().locals();
    }

    template <typename T>
    [[nodiscard]] auto constant(std::integral auto offset) const
    {
        return m_bytecode.constant<T>(offset);
    }

    constexpr auto jump_to(program_counter_type offset)
    {
        if ( std::is_constant_evaluated() == false )
        {
            assert(offset < m_bytecode.instructions().size());
        }

        m_pc = offset;
    }

    [[nodiscard]] constexpr auto current_op() const
    {
        return m_current_op;
    }

    [[nodiscard]] constexpr auto current_immediate() const
    {
        return m_current_imm;
    }

    [[nodiscard]] constexpr auto string_pool() -> acme::string_pool&
    {
        return m_string_pool;
    }

    private:

    [[nodiscard]] auto load_instruction() -> std::optional<acme::instruction>
    {
        if ( auto instruction = m_bytecode.instruction(m_pc); instruction.has_value() )
        {
            m_pc         += 1;
            m_current_op  = operand(instruction.value());
            m_current_imm = immediate(instruction.value());

            return instruction;
        }

        return {};
    }

    program_counter_type m_pc{};
    stack_type           m_stack{};
    bytecode             m_bytecode{};
    exec_scope_stack     m_scope_stack{};
    opcode               m_current_op{};
    immediate_type       m_current_imm{};
    acme::string_pool    m_string_pool{nullptr};
};

} // namespace acme
