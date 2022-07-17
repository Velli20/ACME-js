#pragma once

namespace acme {

struct fixed_buffer_resource : public platform::pmr::memory_resource
{
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap            = std::true_type;
    using is_always_equal                        = std::false_type;
    using value_type                             = std::byte;

    constexpr fixed_buffer_resource(
        void* const       buffer,
        const std::size_t size
    ) noexcept
        : m_buffer{static_cast<value_type*>(buffer)}
        , m_size{size}
        {}

    [[nodiscard]] void* do_allocate(
        const std::size_t bytes,
        const std::size_t alignment = alignof(std::max_align_t)
    ) override
    {
        const auto mask        = alignment - 1;
        const auto offset      = (alignment - std::uintptr_t(m_buffer)) & mask;
        const auto total_bytes = offset + bytes;

        if ( total_bytes > m_size )
        {
            assert(false && "Buffer overflow");
            std::abort();
        }

        m_size   -= total_bytes;
        m_buffer += total_bytes;

        return m_buffer - bytes;
    }

    auto do_deallocate(
        void*       /* pointer */,
        std::size_t /* bytes */,
        std::size_t /* alignment */
    ) -> void override
    {}

    bool do_is_equal(const platform::pmr::memory_resource& other) const noexcept override
    {
        return operator==(*this, other);
    }

    private:

    [[nodiscard]] friend constexpr bool operator==(
        const fixed_buffer_resource& lhs,
        const fixed_buffer_resource& rhs
    ) noexcept
    {
        return lhs.m_buffer == rhs.m_buffer && lhs.m_size == rhs.m_size;
    }

    [[nodiscard]] friend constexpr bool operator!=(
        const fixed_buffer_resource& lhs,
        const fixed_buffer_resource& rhs
    ) noexcept
    {
        return not operator==(lhs, rhs);
    }

    value_type* m_buffer;
    std::size_t m_size;
};

} // namespace acme
