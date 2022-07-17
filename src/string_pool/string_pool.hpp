#pragma once

namespace acme {

struct string_pool
{
    // using view_type  = std::basic_string_view<CharType, CharTrait>;
    // using value_type = typename view_type::value_type;
    // using size_type  = typename view_type::size_type;

    struct string_header
    {
        using parent_type   = acme::string_pool;
        using view_type     = std::string_view;
        using size_type     = view_type::size_type;
        using value_type    = view_type::value_type;
        using pointer       = view_type::pointer;
        using const_pointer = view_type::const_pointer;
        using hash_type     = decltype(acme::detail::hash_fnv1a(view_type{""}));

        private:

        public:

        constexpr string_header(
            parent_type*      parent,
            hash_type         hash,
            view_type         text
        ) noexcept
            : m_parent{parent}
            , m_length{text.length()}
            , m_hash{hash}
        {
            std::copy_n(text.data(), text.length(), data());
        }

        template <typename... Strings>
        constexpr string_header(
            parent_type*      parent,
            hash_type         hash,
            Strings...        strings
        ) noexcept
            : m_parent{parent}
            , m_length{}
            , m_hash{hash}
        {
            ([&](auto s)
            {
                std::copy_n(s.data(), s.length(), data() + m_length);
                m_length += s.length();
            }(strings), ...);
        }

        constexpr string_header(parent_type* parent) noexcept
            : m_parent{parent}
        {}

        constexpr void acquire() noexcept
        {
            // m_ref_count.fetch_add(1);
            m_ref_count += 1;
        }

        constexpr void release() noexcept
        {
            // if ( m_ref_count.fetch_sub(1) == 1 )
            if ( m_ref_count = m_ref_count - 1; m_ref_count == 0 )
            {
                m_parent->unintern(this);
            }
        }

        [[nodiscard]] constexpr auto view() const noexcept
        {
            return view_type{data(), m_length};
        }

        [[nodiscard]] constexpr auto hash() const noexcept
        {
            return m_hash;
        }

        [[nodiscard]] constexpr const auto reference_count() const
        {
            // return m_ref_count.load();
            return m_ref_count;
        }

        [[nodiscard]] constexpr auto operator==(hash_type hash) const noexcept
        {
            return m_hash == hash;
        }

        [[nodiscard]] constexpr auto operator==(const string_header& rhs) const noexcept
        {
            return operator==(rhs.hash());
        }

        [[nodiscard]] constexpr auto data() -> char*
        {
            return m_data;
        }

        [[nodiscard]] constexpr auto data() const -> const char*
        {
            return m_data;
        }

        [[nodiscard]] constexpr auto parent() -> parent_type*
        {
            return m_parent;
        }

        [[nodiscard]] constexpr auto parent() const -> const parent_type*
        {
            return m_parent;
        }

        private:

        parent_type*             m_parent{};
        size_type                m_length{};
        hash_type                m_hash{};
        // std::atomic<std::size_t> m_ref_count{};
        std::size_t              m_ref_count{};
        value_type               m_data[1]{};
    };

    struct string_ref : public std::string_view
    {
        using base            = std::string_view;
        using size_type       = base::size_type;
        using difference_type = base::difference_type;
        using value_type      = base::value_type;
        using reference       = base::reference;
        using const_reference = base::const_reference;
        using pointer         = base::pointer;
        using const_pointer   = base::const_pointer;
        using iterator        = base::iterator;
        using const_iterator  = base::const_iterator;

        using view_type       = base;
        using const_view_type = const base;

        private:

        constexpr void release(string_header* header)
        {
            if ( header )
            {
                header->release();
            }
        }

        public:

        constexpr void acquire()
        {
            m_header->acquire();
        }

        constexpr string_ref() noexcept = default;

        constexpr string_ref(string_header* header) noexcept
            : base{header->view()}
            , m_header{header}
        {
            acquire();
        }

        constexpr string_ref(const string_ref& other) noexcept
            : string_ref{other.m_header}
        {}

        constexpr string_ref& operator=(const string_ref& other)
        {
            release(m_header);

            m_header = other.m_header;
            acquire();
            return *this;
        }

        constexpr string_ref(string_ref&& other) noexcept
            : m_header{std::exchange(other.m_header, nullptr)}
        {}

        constexpr string_ref& operator=(string_ref&& other) noexcept
        {
            std::swap(m_header, other.m_header);
            return *this;
        }

        [[nodiscard]] constexpr auto operator==(const string_ref& rhs) const noexcept
        {
            return m_header == rhs.m_header;
        }

        constexpr ~string_ref()
        {
            if ( m_header != nullptr )
            {
                release(m_header);
            }
        }

        constexpr void swap(string_ref& other) noexcept
        {
            std::exchange(other.m_header, m_header);
        }

        [[nodiscard]] constexpr const auto view() const
        {
            assert(m_header != nullptr);
            return m_header->view();
        }

        [[nodiscard]] constexpr const auto reference_count() const
        {
            assert(m_header != nullptr);
            return m_header->reference_count();
        }

        [[nodiscard]] constexpr decltype(auto) parent() const
        {
            assert(m_header != nullptr);
            return m_header->parent();
        }

        [[nodiscard]] constexpr decltype(auto) parent()
        {
            assert(m_header != nullptr);
            return m_header->parent();
        }

        private:

        string_header* m_header{};
    };

    using map_type = typename acme::dynamic_cvector<string_header*>;

    private:

    [[nodiscard]] auto make_string(
        string_header::hash_type hash,
        std::string_view         text
    ) -> string_header*
    {
        assert(m_resource != nullptr);

        auto* ptr = m_resource->allocate(text.length() + sizeof(string_header) + 1, alignof(string_header));
        assert(ptr != nullptr);

        union
        {
            string_header* m_header;
            void*          m_pointer;
        }
        aliased { .m_pointer = ptr };

        return std::construct_at(aliased.m_header, this, hash, text);
    }

    template <typename... Strings>
    [[nodiscard]] auto make_string(
        string_header::hash_type hash,
        Strings...               strings
    ) -> string_header*
    {
        assert(m_resource != nullptr);

        auto* ptr = m_resource->allocate(sizeof(string_header) + 1 + (strings.length(), ...), alignof(string_header));
        assert(ptr != nullptr);

        union
        {
            string_header* m_header;
            void*          m_pointer;
        }
        aliased { .m_pointer = ptr };

        return std::construct_at(aliased.m_header, this, hash, std::forward<decltype(strings)>(strings)...);
    }

    void unintern(string_header* h)
    {
        assert(m_resource != nullptr);

        if ( h == nullptr )
        {
            return;
        }

        const auto erased = acme::erase_if(m_map, [hash = h->hash()](auto* s)
        {
            return s->hash() == hash;
        });

        assert(erased == 1);
        m_resource->deallocate(h, alignof(string_header));
    }

    template <typename I>
    [[nodiscard]] constexpr auto deref(I&& it) -> std::iter_reference_t<std::remove_cvref_t<I>>
    {
        return *it;
    }

    [[nodiscard]] constexpr auto find(string_header::hash_type hash) -> acme::string_pool::string_header*
    {
#if 1
        for ( auto* it : m_map )
        {
            if ( it->hash() == hash )
            {
                return it;
            }
        }

#else

        auto it = std::lower_bound(m_map.begin(), m_map.end(), hash, [](auto* lhs, auto val) { return lhs->hash() > val; });
        if ( it != m_map.end() )
        {
            return it;
        }

#endif

        return {};
    }

    public:

    string_pool(platform::pmr::memory_resource* resource)
        : m_map{resource},
          m_resource{resource}
        {}

    [[nodiscard]] constexpr auto concatanate(
        std::string_view left,
        std::string_view right
    ) -> string_ref
    {
        if ( left.empty() && right.empty() )
        {
            return {};
        }

        const auto hash1 = acme::detail::hash_fnv1a(left);
        const auto hash2 = acme::detail::hash_fnv1a(right, hash1);

        // Search within interned strings with a given hash value.

        if ( auto* it = find(hash2); it != nullptr )
        {
            return string_ref{it};
        }

        if ( auto* interned = make_string(hash2, left, right); interned != nullptr )
        {
            m_map.push_back(interned);
            return string_ref{interned};
        }

        assert(false && "Out of memory");
        return {};
    }

    [[nodiscard]] constexpr auto intern(std::string_view text) -> string_ref
    {
        if ( text.empty() )
        {
            return {};
        }

        const auto hash = acme::detail::hash_fnv1a(text);

        // Search within interned strings with a given hash value.

        if ( auto* it = find(hash); it != nullptr )
        {
            return string_ref{it};
        }

        // If not found then insert the given string to the string pool.

        if ( auto* interned = make_string(hash, text); interned != nullptr )
        {
            // FIXME:
            /*
            const auto predicate = [](auto* lhs, auto* rhs)
            {
                return lhs->hash() > rhs->hash();
            };

            m_map.insert(std::lower_bound(m_map.begin(), m_map.end(), interned, predicate), interned);
            */

            m_map.push_back(interned);
            return string_ref{interned};
        }

        assert(false && "Out of memory");
        return {};
    }

    [[nodiscard]] constexpr auto count() const
    {
        return m_map.size();
    }

    private:

    map_type                        m_map;
    platform::pmr::memory_resource* m_resource;
};

using pool_string = typename string_pool::string_ref;

} // namespace acme
