#pragma once

namespace acme {

struct parser_context
{
    using memory_resource_type = platform::pmr::memory_resource;

    /* constexpr */ parser_context() noexcept
        : m_resource{nullptr}
        , m_pool{nullptr}
    {}

    /* constexpr */ parser_context(memory_resource_type* resource) noexcept
        : m_resource{resource}
        , m_pool{resource}
    {}

    [[nodiscard]] constexpr auto get_string_pool() noexcept -> string_pool&
    {
        return m_pool;
    }

    [[nodiscard]] constexpr auto resource() const noexcept -> memory_resource_type*
    {
        assert(m_resource != nullptr);
        return m_resource;
    }

    private:

    memory_resource_type* m_resource{};
    string_pool           m_pool;
};

} // namespace acme
