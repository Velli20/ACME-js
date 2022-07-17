
#include "concepts.hpp"

namespace acme {

namespace detail {

template <typename T>
struct default_delete
{
    constexpr default_delete()noexcept = default;

    template<class U>
    default_delete(const default_delete<U>&) noexcept;

    inline void operator()(T* pointer) const
    {
        assert(pointer != nullptr);
        delete pointer;
    }
};

template <typename T, typename Deleter = detail::default_delete<T>>
struct unique_ptr
{
    using value_type = T;

    private:

    template <typename, typename>
    friend struct unique_ptr;

    public:

    constexpr unique_ptr() = default;

    explicit constexpr unique_ptr(
        T*        ptr,
        Deleter&& deleter
    ) noexcept
        : m_pointer{ptr}
        , m_deleter{std::move(deleter)}
    {}

    unique_ptr(const unique_ptr& other) = delete;

    constexpr unique_ptr(unique_ptr&& other) noexcept
        : m_pointer{other.release()}
        , m_deleter{std::move(other.m_deleter)}
    {}

    template <typename OtherT>
    constexpr unique_ptr(unique_ptr<OtherT, Deleter>&& other) noexcept
        : m_pointer{other.release()}
        , m_deleter{std::move(other.m_deleter)}
    {}

    constexpr ~unique_ptr()
    {
        do_destroy();
    }

    [[nodiscard]] constexpr auto operator==(const unique_ptr& other) const noexcept
    {
        return m_pointer == other.m_pointer;
    }

    [[nodiscard]] constexpr auto operator!=(const unique_ptr& other) const noexcept
    {
        return !operator==(other);
    }

    [[nodiscard]] constexpr auto operator==(const T* p) const noexcept
    {
        return m_pointer == p;
    }

    [[nodiscard]] constexpr auto operator!=(const T* p) const noexcept
    {
        return !operator==(p);
    }

    // Disable copy from lvalue

    unique_ptr& operator=(const unique_ptr& other) = delete;

    /* [[nodiscard]]  */constexpr auto operator=(unique_ptr&& other) noexcept -> unique_ptr&
    {
        reset(other.release());

        m_deleter = std::move(other.m_deleter);

        return *this;
    }

    template <typename OtherT>
    /* [[nodiscard]]  */constexpr auto operator=(unique_ptr<OtherT, Deleter>&& other) noexcept -> unique_ptr&
    {
        reset(other.release());

        m_deleter = std::move(other.m_deleter);

        return *this;
    }

    [[nodiscard]] constexpr auto operator*() const noexcept -> T&
    {
        return *m_pointer;
    }

    [[nodiscard]] constexpr auto operator->() const noexcept -> T*
    {
        return m_pointer;
    }

    [[nodiscard]] constexpr explicit operator bool() const noexcept
    {
        return m_pointer != nullptr;
    }

    [[nodiscard]] constexpr auto get() const noexcept -> T*
    {
        return m_pointer;
    }

    constexpr void reset(T* p) noexcept
    {
        do_destroy();

        m_pointer = p;
    }

    constexpr auto release() noexcept -> T*
    {
        T* saved_p = m_pointer;
        m_pointer  = nullptr;
        return saved_p;
    }

    private:

    constexpr void do_destroy() noexcept
    {
        if ( auto* pointer = release();  pointer != nullptr )
        {
            std::invoke(m_deleter, pointer);
        }
    }

    T*      m_pointer{};
    Deleter m_deleter{};
};

struct unique_ptr_pmr_deleter
{
    inline void operator()(void* ptr)
    {
        allocator->deallocate(ptr, size, align);
    }

    platform::pmr::memory_resource* allocator{};
    std::size_t                     size{};
    std::size_t                     align{};
};

} // namespace detail

template <typename T>
using unique_ptr = detail::unique_ptr<T, detail::unique_ptr_pmr_deleter>;

template <typename T>
[[nodiscard]] constexpr auto make_unique(
    platform::pmr::memory_resource* resource,
    auto&&...                       arguments
)
{
    assert(resource != nullptr);

    return unique_ptr<T>
    {
        new (resource->allocate(sizeof(T), alignof(T)))
            T(std::forward<decltype(arguments)>(arguments)...),
                detail::unique_ptr_pmr_deleter{resource, sizeof(T), alignof(T)}
    };
}

template <typename Type>
struct optional_ptr : acme::unique_ptr<Type>
{
    using base = acme::unique_ptr<Type>;

    using base::base;
    using base::operator=;

    constexpr optional_ptr(base&& other) noexcept :
        base(std::move(other))
    {}
};

} // namespace acme
