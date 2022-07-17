#pragma once

#include "forward_types.hpp"

#include "concepts.hpp"
#include "match/match.hpp"

#include "token_position.hpp"
#include "token_type.hpp"
#include "token_flags.hpp"
#include "token_item.hpp"

#include "token_table.hpp"
#include "tokenize_number.hpp"

namespace acme {

template <typename char_type>
struct tokenizer
{
    using char_traits     = std::char_traits<char_type>;
    using string_type     = std::basic_string_view<char_type, char_traits>;
    using size_type       = typename std::basic_string_view<char_type, char_traits>::size_type;
    using checkpoint_type = position::checkpoint_type;
    using token_item_type = token_table::match_type;

    explicit constexpr tokenizer(string_type input) noexcept
        : m_input{input}
        , m_input_original{input}
    {}

    constexpr tokenizer(const tokenizer&) noexcept = default;
    constexpr tokenizer(tokenizer&&) noexcept      = default;

    constexpr tokenizer& operator=(const tokenizer&) noexcept = default;
    constexpr tokenizer& operator=(tokenizer&&)      noexcept = default;

    virtual ~tokenizer() = default;

    [[nodiscard]] constexpr auto available() const noexcept
    {
        return m_input.size();
    }

    [[nodiscard]] constexpr auto empty() const noexcept
    {
        return available() < 1;
    }

    [[nodiscard]] constexpr auto source_location() const noexcept -> checkpoint_type
    {
        return m_position.source_location();
    }

    [[nodiscard]] constexpr auto source() const noexcept -> acme::position::value_type
    {
        return m_position();
    }

    [[nodiscard]] constexpr auto position() const noexcept
    {
        return m_position;
    }

    [[nodiscard]] constexpr auto from(size_type first, size_type last) const -> string_type
    {
        const auto count = last - first;

        if ( std::is_constant_evaluated() == false )
        {
            assert(last >= first);
            assert((last - first) < m_input_original.size());
        }

        return m_input_original.substr(first, count);
    }

    [[nodiscard]] constexpr auto from(size_type first) const
    {
        return from(first, source_location().index());
    }

    [[nodiscard]] constexpr auto from(const checkpoint_type& c) const
    {
        return from(c.index(), source_location().index());
    }

    [[nodiscard]] constexpr auto from(
        const checkpoint_type& first,
        const checkpoint_type& last
    ) const
    {
        return from(first.index(), last.index());
    }

    constexpr auto eat(size_type n) noexcept
    {
        if ( empty() )
        {
            return;
        }

        if ( n > m_input.size() )
        {
            n = m_input.size() - 1;
        }

        for ( std::size_t i{}; i != n; ++i )
        {
            m_position(m_input[i]);
        }

        m_input = m_input.substr(n);
    }

    constexpr auto eat(const token_item_type& tok)
    {
        eat(tok.length());
    }

    [[nodiscard]] constexpr auto peek() const noexcept -> char_type
    {
        if ( empty() )
        {
            return char_traits::eof();
        }

        return m_input.operator[](0);
    }

    template <typename T, T... Is>
    [[nodiscard]] constexpr auto peek(std::integer_sequence<T, Is...>) const noexcept
    {
        return std::array<char, sizeof...(Is)> { (Is < available() ? m_input[Is] : static_cast<char_type>(char_traits::eof()))... };
    }

    template <size_type N>
    [[nodiscard]] constexpr auto peek() const noexcept -> std::array<char_type, N>
    {
        return peek(std::make_integer_sequence<size_type, N>{});
    }

    [[nodiscard]] constexpr auto eol(bool do_consume = false) noexcept -> bool
    {
        if ( auto c = peek(); c == char_traits::eof() )
        {
            if ( do_consume ) { eat(1); }

            return true;
        }

        if ( auto res = match<token_table>(m_input); res.has_value() && res.value().match(acme::token_type::tok_line_terminator) )
        {
            if ( do_consume )
            {
                eat(res.value().length());
            }

            return true;
        }

        return false;
    }

    [[nodiscard]] constexpr auto consume() noexcept -> std::optional<char_type>
    {
        if ( auto res = peek(); char_traits::not_eof(res) )
        {
            eat(1u);
            return res;
        }

        return {};
    }

    template <size_type N>
    [[nodiscard]] constexpr auto consume() noexcept -> std::optional<decltype(peek<N>())>
    {
        if ( auto res = peek<N>(); not res.empty() )
        {
            eat(N);
            return res;
        }

        return {};
    }

    [[nodiscard]] constexpr auto next() noexcept -> token_item_type
    {
        if ( empty() )
        {
            return {};
        }

        while ( true )
        {
            if ( auto c = peek(); acme::match_one_of<' ', '\t'>(c) )
            {
                eat(1u);
            }

            else
            {
                break;
            }
        }

        const auto is_next_number = [&]()
        {
            auto [c1, c2] = peek<2>();

            if ( codepoint::is_decimal_digit(c1) )
            {
                return true;
            }

            if ( acme::match_one_of<'-', '+'>(c1) && codepoint::is_decimal_digit(c2) )
            {
                return true;
            }

            return false;
        }();

        if ( is_next_number == false )
        {
            if ( auto res = match<token_table>(m_input); res.has_value() )
            {
                eat(res.value().length());
                return res.value();
            }
        }

        const auto checkpoint = source_location();

        // Numeric literal.

        if ( is_next_number )
        {
            if ( auto number = parse_number(*this); number.has_value() )
            {
                return number.value();
            }
        }

        // String literal.

        while ( true )
        {
            if ( empty() )
            {
                return {};
            }

            // Returns a string view of an unrecognized tokens.

            if ( auto c = match<token_table>(m_input); c.has_value() && c.value().is_keyword() == false )
            {
                return token_item::make(from(checkpoint));
            }

            if ( auto c = consume(); c.has_value() == false )
            {
                return token_item::make(from(checkpoint));
            }
        }

        return {};
    }

    constexpr auto until_token(token_type token)
    {
        while ( true )
        {
            const auto res = next().type();
            if ( res == token )
            {
                return true;
            }

            if ( res == token_type::tok_none || res == token_type::tok_eof )
            {
                return false;
            }
        }

        return false;
    }

    private:

    string_type    m_input{};
    string_type    m_input_original{};
    acme::position m_position{};
};

} // namespace acme
