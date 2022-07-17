#pragma once

namespace acme {

struct boolean
{
    using value_type = bool;

    constexpr boolean() = default;

    explicit constexpr boolean(value_type v)
        : m_value{v}
        {}

    constexpr boolean& operator=(value_type v) noexcept
    {
        m_value = v;
        return *this;
    }

    [[nodiscard]] constexpr bool operator==(const boolean& rhs) const noexcept
    {
        return m_value == rhs.m_value;
    }

    [[nodiscard]] constexpr bool operator!=(const boolean& rhs) const noexcept
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
