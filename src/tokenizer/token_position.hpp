#pragma once

namespace acme {

struct position
{
    using char_type       = char;
    using index_type      = std::size_t;
    using value_type      = std::array<index_type, 3>;
    using checkpoint_type = position;

    friend struct tokenizer<char_type>;

    constexpr position() = default;

    constexpr position(index_type c, index_type l, index_type i)
        : m_column{c},
          m_line{l},
          m_index{i}
    {}

    constexpr auto operator()(char_type c)
    {
        m_index++;

        if ( c == '\n' )
        {
            m_line  += 1u;
            m_column = 0u;
        }

        else if ( c == '\t' )
        {
            m_column += 4u;
        }

        // else if ( c == '\r' )
        // {
        //     m_column = 0u;
        // }

        else
        {
            m_column += 1u;
        }
    }

    [[nodiscard]] constexpr auto operator()() const noexcept -> value_type
    {
        return value_type{m_column, m_line, m_index};
    }

    [[nodiscard]] constexpr operator value_type() const noexcept
    {
        return operator()();
    }

    [[nodiscard]] constexpr auto index() const noexcept
    {
        return m_index;
    }

    [[nodiscard]] constexpr auto column() const noexcept
    {
        return m_column;
    }

    [[nodiscard]] constexpr auto line() const noexcept
    {
        return m_line;
    }

    protected:

    [[nodiscard]] constexpr auto source_location() const noexcept -> checkpoint_type
    {
        return checkpoint_type{m_column, m_line, m_index};
    }

    private:

    index_type m_column{};
    index_type m_line{};
    index_type m_index{};
};

} // namespace acme
