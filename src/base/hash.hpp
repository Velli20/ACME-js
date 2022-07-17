#pragma once

namespace acme::detail {

template <typename T>
concept container_like = requires(T container)
{
    std::begin(container);
    std::end(container);
};

// initial source from: https://gist.github.com/Lee-R/3839813

[[nodiscard]] constexpr auto hash_fnv1a(
    const container_like auto& container,
    std::uint32_t              hash_init) noexcept
{
    // FNV-1a hash. See: http://www.isthe.com/chongo/tech/comp/fnv/

    constexpr auto hash_prime = std::uint32_t{1'6777'619u};

    auto hash = hash_init;

    for ( const auto i : container )
    {
        hash ^= i;
        hash *= hash_prime;
    }

    return hash;
}

[[nodiscard]] constexpr auto hash_fnv1a(const container_like auto& container) noexcept
{
    constexpr auto hash_init  = std::uint32_t{2'1661'362'61u};

    return hash_fnv1a(container, hash_init);
}

} // namespace acme::detail

namespace acme {

struct identifier
{
    using value_type = decltype(acme::detail::hash_fnv1a(std::string_view{""}));

    static constexpr auto none = std::integral_constant<value_type, std::numeric_limits<value_type>::max()>{};

    constexpr identifier() = default;

    constexpr explicit identifier(std::string_view s)
        : m_hash{acme::detail::hash_fnv1a(s)}
        {}

    constexpr identifier(const identifier&)                = default;
    constexpr identifier(identifier&&) noexcept            = default;

    constexpr identifier& operator=(const identifier&)     = default;
    constexpr identifier& operator=(identifier&&) noexcept = default;

    [[nodiscard]] constexpr auto operator==(const auto& rhs) const noexcept
    {
        return m_hash == rhs.m_hash;
    }

    [[nodiscard]] constexpr auto operator!=(const auto& rhs) const noexcept
    {
        return not operator==(rhs);
    }

    [[nodiscard]] constexpr auto value() const noexcept
    {
        return m_hash;
    }

    // private:

    value_type m_hash{identifier::none};
};

static_assert(std::is_standard_layout<identifier>::value, "");
static_assert(sizeof(identifier) == 4, "");

} // namespace acme

namespace acme::literals {

consteval auto operator"" _id(const char* src, std::size_t len)
{
    return acme::identifier{std::string_view{src, len}};
}

} // namespace acme::literals

static_assert(

    []() constexpr
    {
        using namespace acme::literals;

        constexpr auto k_var_1 = "var1"_id;
        constexpr auto k_var_2 = "var2"_id;
        constexpr auto k_var_3 = "var3"_id;

        return k_var_1 != k_var_2 && k_var_1 != k_var_3;

    }() == true, "[ACME identifier]"
);

static_assert(

    []() constexpr
    {
        using namespace std::string_view_literals;
        using namespace acme::literals;

        constexpr auto k_var_1 = "var1"_id;
        constexpr auto k_var_2 = "var2"_id;
        constexpr auto k_var_3 = "var3"_id;

        if ( k_var_1.value() != acme::detail::hash_fnv1a("var1"sv ) ) { return false; }
        if ( k_var_2.value() != acme::detail::hash_fnv1a("var2"sv ) ) { return false; }
        if ( k_var_3.value() != acme::detail::hash_fnv1a("var3"sv ) ) { return false; }

        return true;

    }() == true, "[ACME identifier]"
);
