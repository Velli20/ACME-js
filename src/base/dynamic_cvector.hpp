#pragma once

namespace acme {

template <typename T>
struct dynamic_cvector
{
    using value_type      = T;
    using size_type       = std::size_t;
    using const_reference = const value_type&;
    using reference       = value_type&;
    using difference_type = std::ptrdiff_t;
    using allocator_type  = std::allocator<value_type>;
    using pointer         = typename std::allocator_traits<allocator_type>::pointer;
    using const_pointer   = typename std::allocator_traits<allocator_type>::const_pointer;
    using iterator        = value_type*;
    using const_iterator  = const value_type*;

    constexpr ~dynamic_cvector()
    {
        clear();
        deallocate(m_data, m_capacity);
        m_capacity = 0;
    }

    constexpr dynamic_cvector() = default;

    constexpr dynamic_cvector(platform::pmr::memory_resource*)
    {}

    constexpr dynamic_cvector(size_type n) requires(std::is_default_constructible_v<T>)
        : m_capacity{n}
        , m_size{n}
        , m_data{allocate(m_capacity)}
    {
        for ( size_type i{}; i < m_size; ++i )
        {
            construct(i);
        }
    }

    template <std::convertible_to<value_type>... Ts>
    constexpr dynamic_cvector(
        std::in_place_t,
        Ts&&... ts
    )
    {
        reserve(sizeof...(ts));
        (emplace_back(std::forward<Ts>(ts)), ...);
    }

    template <std::convertible_to<value_type>... Ts>
    constexpr dynamic_cvector(
        std::in_place_type_t<value_type>,
        Ts&&... ts
    )
    : dynamic_cvector{std::in_place, std::forward<Ts>(ts)...}
    {}

    constexpr dynamic_cvector(const dynamic_cvector& b)
        : m_capacity{b.m_capacity}
        , m_size{b.m_size}
        , m_data{allocate(m_capacity)}
    {
        // Copy construct the storage array elements.

        for ( size_type i{}; i < m_size; ++i )
        {
            construct(i, b[i]);
        }
    }

    constexpr dynamic_cvector(dynamic_cvector&& b)
        : m_capacity{std::exchange(b.m_capacity, 0)}
        , m_size{std::exchange(b.m_size, 0)}
        , m_data{std::exchange(b.m_data, nullptr)}
    {}

    constexpr auto operator=(const dynamic_cvector& b) -> dynamic_cvector&
    {
        // if `b` has a larger capacity, then clear and reallocate our storage array
        if ( m_capacity < b.m_capacity )
        {
            clear();
            deallocate(m_data, m_capacity);
            m_data = allocate(b.m_capacity);
            m_capacity = b.m_capacity;
        }

        // copy assign or construct as necessary
        size_type i{};
        for ( ; i < std::min(m_size, b.m_size); ++i )
        {
            m_data[i] = b[i];
        }

        for ( ; i < b.m_size; ++i )
        {
            construct(i, b[i]);
        }

        for ( ; i < m_size; ++i )
        {
            destroy(i);
        }

        m_size = b.m_size;
        return *this;
    }

    constexpr auto operator=(dynamic_cvector&& b) -> dynamic_cvector&
    {
        // destroy our active storage array

        clear();
        deallocate(m_data, m_capacity);

        // and take all of b's data

        m_capacity = std::exchange(b.m_capacity, 0);
        m_size     = std::exchange(b.m_size, 0);
        m_data     = std::exchange(b.m_data, nullptr);

        return *this;
    }

    // Comparisons.
    [[nodiscard]] constexpr friend auto operator==(dynamic_cvector const& a, dynamic_cvector const& b)
    {
        return a.size() == b.size() and std::equal(a.begin(), a.end(), b.begin());
    }

    [[nodiscard]] constexpr friend auto operator<=>(dynamic_cvector const& a, dynamic_cvector const& b)
    {
        return std::lexicographical_compare_three_way(a.begin(), a.end(), b.begin(), b.end());
    }

    // Element access.

    [[nodiscard]] constexpr auto operator[](size_type i) const -> const_reference
    {
        assert(0 <= i && i < m_size);
        return m_data[i];
    }

    [[nodiscard]] constexpr auto operator[](size_type i) -> reference
    {
        assert(0 <= i && i < m_size);
        return m_data[i];
    }

    [[nodiscard]] constexpr auto at(size_type pos) -> reference
    {
        return operator[](pos);
    }

    [[nodiscard]] constexpr auto at(size_type pos) const -> const_reference
    {
        return operator[](pos);
    }

    [[nodiscard]] constexpr auto front() const -> const_reference
    {
        assert(m_size > 0);
        return m_data[0];
    }

    [[nodiscard]] constexpr auto front() -> reference
    {
        assert(m_size > 0);
        return m_data[0];
    }

    [[nodiscard]] constexpr auto back() const -> const_reference
    {
        assert(m_size > 0);
        return m_data[m_size - 1];
    }

    [[nodiscard]] constexpr auto back() -> reference
    {
        assert(m_size > 0);
        return m_data[m_size - 1];
    }

    [[nodiscard]] auto data() const -> const pointer
    {
        return m_data;
    }

    [[nodiscard]] auto data() -> pointer
    {
        return m_data;
    }

    [[nodiscard]] constexpr auto cbegin() const -> const_iterator
    {
        return m_data;
    }

    [[nodiscard]] constexpr auto cend() const -> const_iterator
    {
        return m_data + m_size;
    }

    [[nodiscard]] constexpr auto begin() const -> iterator
    {
        return m_data;
    }

    [[nodiscard]] constexpr auto begin() -> iterator
    {
        return m_data;
    }

    [[nodiscard]] constexpr auto end() const -> iterator
    {
        return m_data + m_size;
    }

    [[nodiscard]] constexpr auto end() -> iterator
    {
        return m_data + m_size;
    }

    constexpr auto rbegin() const { return std::reverse_iterator(end()); }
    constexpr auto rbegin()       { return std::reverse_iterator(end()); }
    constexpr auto   rend() const { return std::reverse_iterator(begin()); }
    constexpr auto   rend()       { return std::reverse_iterator(begin()); }

    // Capacity
    [[nodiscard]] constexpr auto empty() const
    {
        return m_size == 0;
    }

    [[nodiscard]] constexpr auto size() const
    {
        return m_size;
    }

    [[nodiscard]] static constexpr auto max_size()
    {
        return std::numeric_limits<decltype(m_capacity)>::max();
    }

    constexpr void reserve(size_type n)
    {
        if ( m_capacity < n )
        {
            reallocate(n);
        }
    }

    [[nodiscard]] constexpr auto capacity() const
    {
        return m_capacity;
    }

    constexpr void shrink_to_fit()
    {
        if ( m_size < m_capacity )
        {
            reallocate(m_size);
        }
    }

    [[nodiscard]] friend constexpr auto size(const dynamic_cvector& v)
    {
        return v.m_size;
    }

    // Modifiers
    template <typename... Ts>
    constexpr auto emplace_back(Ts&&... ts) -> T&
    {
        static_assert(std::is_constructible_v<T, Ts...>);

        if ( m_size == m_capacity )
        {
            reserve(std::max(2 * m_capacity, decltype(m_capacity){1}));
        }

        return construct(m_size++, std::forward<Ts>(ts)...);
    }

    constexpr auto push_back(const_reference t) -> T&
    {
        if ( m_size == m_capacity )
        {
            reserve(std::max(2 * m_capacity, decltype(m_capacity){1}));
        }

        return construct(m_size++, t);
    }

    constexpr auto push_back(T&& t) -> T&
    {
        if ( m_size == m_capacity )
        {
            reserve(std::max(2 * m_capacity, decltype(m_capacity){1}));
        }

        return construct(m_size++, std::move(t));
    }

    constexpr auto pop_back() -> T
    {
        assert(m_size > 0);

        m_size -= 1;
        return std::move(m_data[m_size]);
    }

    constexpr void remove_at(size_type i)
    {
        assert(0 <= i and i < m_size);

        for ( size_type e = --m_size; i < e; ++i )
        {
            m_data[i] = std::move(m_data[i + 1]);
        }
    }

    constexpr void remove(pointer i)
    {
        remove_at(std::distance(begin(), i));
    }

    constexpr auto erase(const_iterator first,
                         const_iterator last
    )
    {
        auto const dst = begin() + (first - begin());
        auto const src = begin() + (last - begin());

        auto const old_end = end();
        auto const new_end = std::move(src, old_end, dst);

        m_size = new_end - begin();

        if constexpr ( not std::is_trivially_destructible_v<value_type> )
        {
            std::destroy(new_end, old_end);
        }

        return new_end;
    }

    constexpr iterator erase(const_iterator position)
    {
        return erase(position, position + 1);
    }

    constexpr void resize(size_type n) requires(std::is_default_constructible_v<T>)
    {
        // reserve enough space for n
        if ( m_capacity < n )
        {
            reserve(n);
        }

        // if n was smaller than the size, destroy the excess elements
        for ( auto i = n; i < m_size; ++i )
        {
            destroy(i);
        }

        // if n was larger than the size, then default construct the excess
        for ( auto i = m_size; i < n; ++i )
        {
            construct(i);
        }

        m_size = n;
    }

    constexpr void clear()
    {
        // destroy the active elements

        for ( size_type i{}; i < m_size; ++i)
        {
            destroy(i);
        }

        m_size = 0;
    }

    private:

    [[nodiscard]] constexpr auto allocate(size_type n) -> pointer
    {
        return (n != 0) ? m_allocator.allocate(n) : nullptr;
    }

    constexpr void deallocate(
      pointer   ptr,
      size_type n
    )
    {
        if ( ptr )
        {
            m_allocator.deallocate(ptr, n);
        }
    }

    template <class... Ts>
    constexpr auto construct(
        size_type i,
        Ts&&...   ts
    ) -> reference
    {
        return *std::construct_at(m_data + i, std::forward<Ts>(ts)...);
    }

    constexpr void destroy(size_type i)
    {
        std::destroy_at(m_data + i);
    }

    constexpr void reallocate(size_type n)
    {
        auto* old = std::exchange(m_data, allocate(n));

        for ( size_type i{}; i < m_size; ++i )
        {
            construct(i, std::move(old[i]));
        }

        if ( old != nullptr )
        {
            m_allocator.deallocate(old, m_capacity);
        }

        m_capacity = n;
    }

    allocator_type m_allocator{};
    size_type      m_capacity{};
    size_type      m_size{};
    pointer        m_data{};
};

template <typename T, std::convertible_to<T>... Ts>
dynamic_cvector(std::in_place_t, T, Ts...) -> dynamic_cvector<T>;

template <typename T, std::convertible_to<T>... Ts>
dynamic_cvector(std::in_place_type_t<T>, Ts...) -> dynamic_cvector<T>;

template <typename T, typename F>
constexpr auto erase_if(acme::dynamic_cvector<T>& c, F pred)
{
    const auto begin = c.begin();
    const auto end   = c.end();

    auto it = std::remove_if(begin, end, pred);
    c.erase(it, end);

    return end - it;
}

} // namespace acme
