#pragma once

namespace acme::containers {

template <std::size_t N = 24, typename T = std::pair<acme::identifier, acme::script_value>>
struct var_stack : public std::array<T, N>
{
    using base               = std::array<T, N>;
    using item_type          = typename base::value_type;
    using index_type         = std::size_t;
    using key_type           = std::remove_const_t<typename item_type::first_type>;
    using value_type         = typename item_type::second_type;
    using optional_reference = std::optional<std::reference_wrapper<acme::script_value>>;

    static constexpr auto invalid_index = std::numeric_limits<index_type>::max();

    constexpr var_stack()                           = default;
    constexpr var_stack(const var_stack&)           = default;
    constexpr var_stack&operator=(const var_stack&) = default;
    constexpr var_stack&operator=(var_stack&&)      = default;
    constexpr var_stack(var_stack&&)                = default;

    constexpr ~var_stack()
    {
        clear();
    }

    using base::cbegin;
    using base::cend;
    using base::rbegin;
    using base::rend;
    using base::size;
    using base::data;
    using base::operator[];
    using base::at;

    [[nodiscard]] constexpr auto check(key_type id)
    {
        return not get(id).has_value();
    }

    template <typename V>
    constexpr auto push(key_type k, const V& value) -> index_type
    {
        static_assert(std::is_same_v<V, value_type> or std::is_constructible_v<value_type, V>, "");

        if ( not std::is_constant_evaluated() )
        {
            assert(num_entries < capacity());
        }

        if constexpr ( std::is_same_v<V, value_type> )
        {
            base::operator[](num_entries) = std::pair{k, value};
            ++num_entries;

            return num_entries - 1;
        }

        else if constexpr ( std::is_constructible_v<value_type, V> )
        {
            base::operator[](num_entries) = std::pair{k, value_type{value}};
            ++num_entries;

            return num_entries - 1;
        }

        return invalid_index;
    }

    template <typename V>
    constexpr auto push(key_type k, V&& value)
    {
        static_assert(std::is_same_v<V, value_type> or std::is_constructible_v<value_type, V>, "");

        if ( not std::is_constant_evaluated() )
        {
            assert(num_entries < capacity());
        }

        if constexpr ( std::is_same_v<V, value_type> )
        {
            base::operator[](num_entries) = std::pair{k, value};
            ++num_entries;

            return num_entries - 1;
        }

        else if constexpr ( std::is_constructible_v<value_type, V> )
        {
            base::operator[](num_entries) = std::pair{k, value_type{std::forward<decltype(value)>(value)}};
            ++num_entries;

            return num_entries - 1;
        }

        return invalid_index;
    }

    template <typename K, typename V>
    constexpr auto push(K k, V&& value)
    {
        if constexpr ( std::is_same_v<K, key_type> )
        {
            return push(k, std::forward<decltype(value)>(value));
        }

        else if constexpr ( std::is_constructible_v<key_type, K> )
        {
            return push(key_type{k}, std::forward<decltype(value)>(value));
        }
    }

    [[nodiscard]] constexpr auto size() const noexcept
    {
        return num_entries;
    }

    [[nodiscard]] constexpr auto capacity() const noexcept
    {
        return N;
    }

    [[nodiscard]] constexpr bool empty() const noexcept
    {
        return num_entries == 0;
    }

    constexpr void clear()
    {
        while ( not empty() )
        {
            pop_back();
        }
    }

    constexpr auto pop_back()
    {
        --num_entries;
        return std::get<1>(std::move(base::operator[](num_entries)));
    }

    [[nodiscard]] constexpr auto get(index_type entry) const -> const value_type&
    {
        return std::get<1>(base::operator[](entry));
    }

    [[nodiscard]] constexpr auto get(index_type entry) -> value_type&
    {
        return std::get<1>(base::operator[](entry));
    }

    [[nodiscard]] constexpr auto top() -> optional_reference
    {
        if ( num_entries < 1 )
        {
            return {};
        }

        return std::reference_wrapper{get(num_entries - 1)};
    }

    [[nodiscard]] constexpr auto get(acme::identifier id) -> optional_reference
    {
        for ( auto& [idc, v] : *this )
        {
            if ( idc == id )
            {
                return std::reference_wrapper{v};
            }
        }

        return {};
    }

    index_type num_entries{};
};

} // namespace acme::containers
