#pragma once

namespace acme {

struct emit_context;

struct loop_context
{
    using offset_type = acme::instruction::immediate_type;

    private:

    constexpr auto increment_stack_frame_depth()
    {
        m_stack_frame_depth++;
    }

    constexpr auto decrement_stack_frame_depth()
    {
        m_stack_frame_depth--;
    }

    public:

    [[nodiscard]] constexpr auto stack_frame_depth() const
    {
        return m_stack_frame_depth;
    }

    [[nodiscard]] constexpr auto condition_check_offset() const
    {
        return m_loop_condition_check_offset;
    }

    constexpr auto condition_check_offset(offset_type offset)
    {
        m_loop_condition_check_offset = offset;
    }

    friend struct emit_context;

    offset_type m_loop_condition_check_offset{};
    std::size_t m_stack_frame_depth{};
};

struct emit_context
{
    private:

    public:

    using bytecode_list_type         = acme::dynamic_cvector<acme::instruction>;
    using number_constants_list_type = acme::dynamic_cvector<acme::number_constant>;
    using string_constants_list_type = acme::dynamic_cvector<acme::string_constant>;

    enum class emit_state : std::uint32_t
    {
        k_none = 0u,
        k_variable_declaration,
    };

    [[nodiscard]] constexpr auto count() const
    {
        return m_bytecode.size() - 1;
    }

    [[nodiscard]] constexpr auto state() const
    {
        return m_state;
    }

    constexpr auto state(emit_state new_state)
    {
        m_state = new_state;
    }

    constexpr auto emit_instruction(
        acme::opcode                      operand,
        acme::instruction::immediate_type imm = {}
    )
    {
        // Keep track of how stack frames pushed during the current loop context.
        // This is needed in case of statements that result jump condition to an arbitrary
        // bytecode position. In that case we need to pop all of the stack frames pushed during the loop.

        if ( operand == opcode::push_stack_frame && m_loop_context != nullptr )
        {
            m_loop_context->increment_stack_frame_depth();
        }

        else if ( operand == opcode::pop_stack_frame && m_loop_context != nullptr )
        {
            m_loop_context->decrement_stack_frame_depth();
        }

        m_bytecode.push_back(instruction::make(operand, imm));
        return count();
    }

    constexpr auto at(bytecode_list_type::size_type index) -> bytecode_list_type::reference
    {
        return m_bytecode.at(index);
    }

    template <typename T>
    constexpr auto emit(const T& constant) requires(concepts::ast_literal<T> || std::is_same_v<T, ast::Identifier>)
    {
        const auto numbers_index = m_numbers.size();
        const auto strings_index = m_strings.size();

        // Signed integer constant.

        if constexpr ( std::is_same_v<T, ast::Integer> )
        {
            m_numbers.emplace_back(acme::number_constant { .m_i32 = constant.value() } );
            emit_instruction(opcode::constant_i32, numbers_index);
        }

        // Unsigned integer constant.

        else if constexpr ( std::is_same_v<T, ast::UnsignedInteger> )
        {
            m_numbers.emplace_back(acme::number_constant { .m_u32 = constant.value() } );
            emit_instruction(opcode::constant_u32, numbers_index);
        }

        // Float constant.

        else if constexpr ( std::is_same_v<T, ast::Float> )
        {
            m_numbers.emplace_back(acme::number_constant { .m_float = static_cast<float>(constant.value()) } );
            emit_instruction(opcode::constant_double, numbers_index);
        }

        // Boolean constant.

        else if constexpr ( std::is_same_v<T, ast::Boolean> )
        {
            emit_instruction(constant.value() ? opcode::push_bool_true : opcode::push_bool_false);
        }

        // String constant.

        else if constexpr ( std::is_same_v<T, ast::String> )
        {
            acme::identifier hash_value{ constant.value().view() };

            decltype(strings_index) index{};

            for ( const auto& s : m_strings )
            {
                if ( s.m_hash == hash_value )
                {
                    emit_instruction(opcode::constant_string, index);
                    return index;
                }
            }

            const auto buffer_offset = m_string_buffer.length();

            m_string_buffer.append(constant.value().view());

            m_strings.emplace_back(acme::string_constant
            {
                .m_hash          = hash_value,
                .m_buffer_offset = static_cast<std::uint32_t>(buffer_offset),
                .m_length        = static_cast<std::uint32_t>(constant.value().view().length())
            });

            emit_instruction(opcode::constant_string, strings_index);

            return m_strings.size() - 1;
        }

        // Null constant.

        else if constexpr ( std::is_same_v<T, ast::Null> )
        {
            emit_instruction(opcode::push_null);
        }

        // Variable ID constant.

        else if constexpr ( std::is_same_v<T, ast::Identifier> )
        {
            acme::identifier id{ constant.value().view() };

            m_numbers.emplace_back(acme::number_constant { .m_hash = id } );
            emit_instruction(opcode::constant_identifier, numbers_index);
        }

        return m_numbers.size() - 1;
    }

    [[nodiscard]] constexpr auto current_loop_context() const -> const loop_context*
    {
        return m_loop_context;
    }

    [[nodiscard]] constexpr auto current_loop_context() -> loop_context*
    {
        return m_loop_context;
    }

    [[nodiscard]] constexpr auto set_loop_context(loop_context* context)
    {
        m_loop_context = context;
    }

    [[nodiscard]] constexpr auto bytecode() const -> acme::bytecode
    {
        return acme::bytecode
        {
            std::span{m_bytecode},
            std::span{m_numbers},
            std::span{m_strings},
            std::span{m_string_buffer.data(), m_string_buffer.length()},
        };
    }

    private:

    bytecode_list_type         m_bytecode{};
    number_constants_list_type m_numbers{};
    string_constants_list_type m_strings{};
    std::string                m_string_buffer{};
    emit_state                 m_state{};
    loop_context*              m_loop_context{};
};

} // namespace acme::eval
