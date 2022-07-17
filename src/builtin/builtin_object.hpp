#pragma once

namespace acme {

struct object
{
    constexpr object() = default;

    [[nodiscard]] constexpr auto instanceof(const acme::object& v) const noexcept
    {
        return false;
    }

    [[nodiscard]] constexpr bool operator==(const object& rhs) const noexcept = default;
    [[nodiscard]] constexpr bool operator!=(const object& rhs) const noexcept = default;
};

} // namespace acme
