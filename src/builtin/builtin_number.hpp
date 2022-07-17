#pragma once

namespace acme {

struct number
{
    using value_type = double;

    static_assert(std::numeric_limits<value_type>::is_iec559);

    constexpr number() = default;

    explicit constexpr number(value_type v)
        : m_value{v}
        {}

    template <std::integral I>
    constexpr number(I v)
        : m_value{static_cast<value_type>(v)}
        {}

    constexpr number(const number&)                = default;
    constexpr number(number&&) noexcept            = default;

    constexpr number& operator=(const number&)     = default;
    constexpr number& operator=(number&&) noexcept = default;

    constexpr number& operator=(value_type v) noexcept
    {
        m_value = v;
        return *this;
    }

    [[nodiscard]] constexpr auto operator+(const acme::number& rhs) const noexcept
    {
        return m_value + rhs.m_value;
    }

    constexpr auto operator+=(const acme::number& rhs) noexcept -> acme::number&
    {
        m_value += rhs.m_value;
        return *this;
    }

    [[nodiscard]] constexpr bool operator==(const acme::number& rhs) const noexcept
    {
        return m_value == rhs.m_value;
    }

    [[nodiscard]] constexpr bool operator!=(const acme::number& rhs) const noexcept
    {
        return not operator==(rhs);
    }

    [[nodiscard]] constexpr bool operator==(std::integral auto rhs) const noexcept
    {
        return m_value == static_cast<value_type>(rhs);
    }

    [[nodiscard]] constexpr bool operator!=(std::integral auto rhs) const noexcept
    {
        return not operator==(rhs);
    }

    [[nodiscard]] constexpr auto value() const noexcept
    {
        return m_value;
    }

    value_type m_value{};
};

} // namespace acme
