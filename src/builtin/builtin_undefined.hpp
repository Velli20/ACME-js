#pragma once

namespace acme {

struct undefined
{
    [[nodiscard]] constexpr bool operator==(const undefined& rhs) const noexcept = default;
    [[nodiscard]] constexpr bool operator!=(const undefined& rhs) const noexcept = default;
};

} // namespace acme
