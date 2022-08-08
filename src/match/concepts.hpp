#pragma once

namespace acme::concepts {

template <typename T>
concept rule_table = requires()
{
    { T::table[0] } -> std::same_as<const token_item&>;
};

} // namespace acme::concepts
