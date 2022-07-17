#pragma once

namespace acme {

struct function
{
    using value_type = void(*)();

    constexpr function() = default;

    template <typename F> requires(std::is_invocable_r_v<void, F>)
    constexpr function(F&& f)
        : m_value{std::forward<F>(f)}
        {}

    constexpr auto operator()() const
    {
        return std::invoke(m_value);
    }

    [[nodiscard]] constexpr bool operator==(const acme::function& rhs) const noexcept
    {
        return m_value == rhs.m_value;
    }

    [[nodiscard]] constexpr bool operator!=(const acme::function& rhs) const noexcept
    {
        return not operator==(rhs);
    }

    value_type m_value{};
};

} // namespace acme
