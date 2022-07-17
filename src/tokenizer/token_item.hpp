#pragma once

namespace acme {

struct token_item
{
    using char_type            = char;
    using unsigned_number_type = std::uint32_t;
    using signed_number_type   = std::int32_t;
    using float_number_type    = double;
    using char_traits          = std::char_traits<char_type>;
    using string_type          = std::basic_string_view<char_type, char_traits>;
    using predicate_type       = bool (*)(char_type);
    using value_type           = std::variant<string_type, unsigned_number_type, signed_number_type, float_number_type>;

    [[nodiscard]] constexpr /* explicit */ operator token_type() const noexcept
    {
        return m_type;
    }

    [[nodiscard]] constexpr bool operator==(token_type rhs) const noexcept
    {
        return m_type == rhs;
    }

    [[nodiscard]] constexpr bool operator!=(token_type rhs) const noexcept
    {
        return not operator==(rhs);
    }

    [[nodiscard]] constexpr auto type() const noexcept
    {
        return m_type;
    }

    [[nodiscard]] constexpr auto to_string() const noexcept -> string_type
    {
        if ( std::holds_alternative<string_type>(m_value) )
        {
            return std::get<string_type>(m_value);
        }

        return {};
    }

    [[nodiscard]] constexpr auto is_unsigned_integer() const noexcept -> unsigned_number_type
    {
        return std::holds_alternative<unsigned_number_type>(m_value);
    }

    [[nodiscard]] constexpr auto to_unsigned_integer() const noexcept -> unsigned_number_type
    {
        if ( is_unsigned_integer() )
        {
            return std::get<unsigned_number_type>(m_value);
        }

        return {};
    }

    [[nodiscard]] constexpr auto is_signed_integer() const noexcept -> signed_number_type
    {
        return std::holds_alternative<signed_number_type>(m_value);
    }

    [[nodiscard]] constexpr auto to_signed_integer() const noexcept -> signed_number_type
    {
        if ( is_signed_integer() )
        {
            return std::get<signed_number_type>(m_value);
        }

        return {};
    }

    [[nodiscard]] constexpr auto is_double() const noexcept -> bool
    {
        return std::holds_alternative<float_number_type>(m_value);
    }

    [[nodiscard]] constexpr auto to_double() const noexcept -> float_number_type
    {
        if ( is_double() )
        {
            return std::get<float_number_type>(m_value);
        }

        return {};
    }

    [[nodiscard]] constexpr auto length() const noexcept -> string_type::size_type
    {
        return to_string().size();
    }

    [[nodiscard]] constexpr auto match(token_type token) const noexcept -> bool
    {
        return m_type == token;
    }

    [[nodiscard]] constexpr auto is_literal() const noexcept -> bool
    {
        return has_flag(m_flags, token_flags::literal);
    }

    [[nodiscard]] constexpr auto is_keyword() const noexcept -> bool
    {
        return has_flag(m_flags, token_flags::keyword);
    }

    [[nodiscard]] constexpr auto is_identifier() const noexcept -> bool
    {
        return match(acme::token_type::tok_identifier) || match(acme::token_type::tok_private_identifier);
    }

    [[nodiscard]] constexpr auto match(std::string_view input) const noexcept -> bool
    {
        auto is_match = false;

        if ( std::holds_alternative<string_type>(m_value) )
        {
            is_match = input.starts_with(to_string());
        }

        if ( is_match && m_right_side_predicate != nullptr )
        {
            return (input.size() >= length()) && std::invoke(m_right_side_predicate, input[length()]);
        }

        return is_match;
    }

    [[nodiscard]] static constexpr auto make(token_item::string_type s) -> token_item
    {
        auto type = token_type::tok_string;

        if ( s.empty() )
        {
            type = token_type::tok_none;
        }

        if ( match_private_identifier(s) )
        {
            type = token_type::tok_private_identifier;
        }

        else if ( match_identifier(s[0]) )
        {
            type = token_type::tok_identifier;
        }

        return { s, match_any, type, token_flags::literal };
    }

    [[nodiscard]] static constexpr auto make(token_item::unsigned_number_type n) -> token_item
    {
        return { n, match_any, token_type::tok_unsigned_number, token_flags::literal };
    }

    [[nodiscard]] static constexpr auto make(token_item::signed_number_type n) -> token_item
    {
        return { n, match_any, token_type::tok_signed_number, token_flags::literal };
    }

    [[nodiscard]] static constexpr auto make(token_item::float_number_type n) -> token_item
    {
        return { n, match_any, token_type::tok_float_number, token_flags::literal };
    }

    value_type     m_value{};
    predicate_type m_right_side_predicate{};
    token_type     m_type{token_type::tok_none};
    token_flags    m_flags{};
};

} // namespace acme

