#pragma once

namespace acme {

struct string
{
    using value_type     = std::string_view;
    using reference_type = std::reference_wrapper<acme::pool_string>;
    using variant_type   = std::variant
                            <
                                std::monostate,
                                value_type,
                                reference_type
                            >;


    constexpr string() = default;

    explicit constexpr string(value_type v)
        : m_value{v}
        {}

    constexpr string& operator=(value_type v) noexcept
    {
        m_value = v;

        return *this;
    }

    [[nodiscard]] constexpr auto operator==(const string& rhs) const noexcept
    {
        return rhs.value() == value();
    }

    [[nodiscard]] constexpr auto operator!=(const string& rhs) const noexcept
    {
        return not operator==(rhs);
    }

    static constexpr decltype(auto) deref(auto* ptr)
    {
        return *(std::get_if<reference_type>(ptr));
    }

    [[nodiscard]] constexpr auto value() const noexcept -> value_type
    {
        auto* ptr = std::addressof(m_value);

        if ( std::holds_alternative<reference_type>(m_value) )
        {
            if ( auto ref = deref(ptr); ref.get() != nullptr )
            {
                return ref.get().view();
            }
        }

        else if ( std::holds_alternative<value_type>(m_value) )
        {
            return *(std::get_if<value_type>(ptr));
        }

        return {};
    }

    variant_type m_value{};
};

} // namespace acme
