#pragma once

namespace acme {

template <concepts::nullable_type T>
struct not_null
{
    using value_type = T;

    static constexpr void assert_not_null(const T& value) noexcept
    {
        if constexpr ( concepts::smart_pointer<T> )
        {
            assert(value.get() != nullptr);
        }

        else
        {
            assert(value != nullptr);
        }
    }

    [[nodiscard]] constexpr not_null()
        : m_value{}
    {
        assert_not_null(m_value);
    }

    [[nodiscard]] constexpr not_null(T value) noexcept
        : m_value{std::move(value)}
    {
        assert_not_null(m_value);
    }

    [[nodiscard]] constexpr not_null(auto&& ... args) noexcept
        : m_value{std::forward<decltype(args)>(args)...}
    {
        assert_not_null(m_value);
    }

    [[nodiscard]] constexpr not_null(const not_null& other) noexcept
        : m_value{other.m_value}
        {}

    [[nodiscard]] constexpr auto copy() const noexcept -> T
    {
        assert_not_null(m_value);
        return T{ m_value };
    }

    [[nodiscard]] constexpr auto reference() const noexcept -> const T&
    {
        assert_not_null(m_value);
        return m_value;
    }

    [[nodiscard]] constexpr auto unsafe_reference() noexcept -> T&
    {
        return m_value;
    }

    constexpr auto operator=(const not_null& other) noexcept -> not_null&
    {
        m_value = other.m_value;
        return *this;
    }

    [[nodiscard]] constexpr auto operator*() noexcept -> auto&
    {
        assert_not_null(m_value);
        return *m_value;
    }
    [[nodiscard]] constexpr auto operator*() const noexcept -> const auto&
    {
        assert_not_null(m_value);
        return *m_value;
    }

    [[nodiscard]] constexpr auto operator->() noexcept -> auto&
    {
        assert_not_null(m_value);
        return m_value;
    }

    [[nodiscard]] constexpr auto operator->() const noexcept -> const auto&
    {
        assert_not_null(m_value);
        return m_value;
    }

    [[nodiscard]] friend constexpr auto operator==(const not_null& lhs, const std::nullptr_t rhs) noexcept
    {
        return lhs.m_value == rhs;
    }

    [[nodiscard]] friend constexpr auto operator==(const not_null& lhs, const not_null& rhs) noexcept
    {
        return lhs.m_value == rhs.m_value;
    }

    [[nodiscard]] friend constexpr auto operator<=>(const not_null& lhs, const not_null& rhs) noexcept
    {
        return lhs.m_value <=> rhs.m_value;
    }

    private:

    T m_value{};
};

} // namespace acme
