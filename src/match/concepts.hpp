#pragma once

namespace acme::concepts {

template <typename T>
concept rule_table = requires()
{
    { T::table[0] } -> std::same_as<const token_item&>;
};

template <typename T>
concept string_like = std::is_convertible_v<T, std::string_view> || std::is_convertible_v<T, platform::pmr::string>;

} // namespace acme::concepts
