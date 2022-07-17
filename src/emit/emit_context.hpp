#pragma once

namespace acme {

struct emit_context
{
    using bytecode_list_type         = acme::dynamic_cvector<acme::instruction>;
    using number_constants_list_type = acme::dynamic_cvector<acme::number_constant>;
    using string_constants_list_type = acme::dynamic_cvector<acme::string_constant>;

    constexpr auto count()
    {
        return m_bytecode.size() - 1;
    }

    constexpr auto emit_instruction(
        acme::opcode                      operand,
        acme::instruction::immediate_type imm = {}
    )
    {
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

        if constexpr ( std::is_same_v<T, ast::Integer> )
        {
            m_numbers.emplace_back(acme::number_constant { .m_i32 = constant.value() } );
            emit_instruction(opcode::constant_i32, numbers_index);
        }

        else if constexpr ( std::is_same_v<T, ast::UnsignedInteger> )
        {
            m_numbers.emplace_back(acme::number_constant { .m_u32 = constant.value() } );
            emit_instruction(opcode::constant_u32, numbers_index);
        }

        else if constexpr ( std::is_same_v<T, ast::Float> )
        {
            m_numbers.emplace_back(acme::number_constant { .m_float = static_cast<float>(constant.value()) } );
            emit_instruction(opcode::constant_double, numbers_index);
        }

        else if constexpr ( std::is_same_v<T, ast::Boolean> )
        {
            emit_instruction(constant.value() ? opcode::push_bool_true : opcode::push_bool_false);
        }

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

            auto buffer_offset = m_string_buffer.length();

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

        else if constexpr ( std::is_same_v<T, ast::Null> )
        {
            emit_instruction(opcode::push_null);
        }

        else if constexpr ( std::is_same_v<T, ast::Identifier> )
        {
            acme::identifier id{ constant.value().view() };

            m_numbers.emplace_back(acme::number_constant { .m_hash = id } );
            emit_instruction(opcode::constant_identifier, numbers_index);
        }

        return m_numbers.size() - 1;
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
};

} // namespace acme::eval
