
#pragma once

namespace acme::containers {

template <std::size_t N, typename T>
struct stack : public std::array<T, N>
{
    using base       = std::array<T, N>;
    using item_type  = typename base::value_type;

    constexpr stack()                       = default;
    constexpr stack(const stack&)           = default;
    constexpr stack&operator=(const stack&) = default;
    constexpr stack&operator=(stack&&)      = default;
    constexpr stack(stack&&)                = default;

    constexpr ~stack()
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

    constexpr auto push_back(const T& value)
    {
        if ( std::is_constant_evaluated() == false )
        {
            assert(num_entries < N);
        }

        base::operator[](num_entries) = value;
        ++num_entries;
    }

    template <typename V>
    constexpr auto push_back(V&& value)
    {
        if ( std::is_constant_evaluated() == false )
        {
            assert(num_entries < N);
        }

        if constexpr ( std::is_constructible_v<T, V> )
        {
            base::operator[](num_entries) = T{std::forward<decltype(value)>(value)};
            ++num_entries;
        }

        else
        {
            base::operator[](num_entries) = std::move(value);
            ++num_entries;
        }
    }

    constexpr auto pop_back()
    {
        if ( std::is_constant_evaluated() == false )
        {
            assert(num_entries > decltype(num_entries){});
        }

        --num_entries;
        return std::move(base::operator[](num_entries));
    }

    [[nodiscard]] constexpr auto get(const std::size_t entry) const -> const item_type&
    {
        return base::operator[](entry);
    }

    [[nodiscard]] constexpr auto top() const
    {
        return base::operator[](num_entries - 1);
    }

    [[nodiscard]] constexpr auto get(const std::size_t entry) -> item_type&
    {
        return base::operator[](entry);
    }

    [[nodiscard]] constexpr auto size() const noexcept
    {
        return num_entries;
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

    private:

    std::size_t num_entries{};
};

} // namespace acme
