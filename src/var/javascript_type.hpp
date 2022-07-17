#pragma once

namespace acme {

enum class js_type : std::uint32_t
{
    undefined_type,
    null_type,
    boolean_type,
    number_type,
    string_type,
    object_type,
    function_type,
    identifier_type
};

[[maybe_unused]] static constexpr auto boolean_type    = std::integral_constant<acme::js_type, acme::js_type::boolean_type>{};
[[maybe_unused]] static constexpr auto null_type       = std::integral_constant<acme::js_type, acme::js_type::null_type>{};
[[maybe_unused]] static constexpr auto undefined_type  = std::integral_constant<acme::js_type, acme::js_type::undefined_type>{};
[[maybe_unused]] static constexpr auto number_type     = std::integral_constant<acme::js_type, acme::js_type::number_type>{};
[[maybe_unused]] static constexpr auto string_type     = std::integral_constant<acme::js_type, acme::js_type::string_type>{};
[[maybe_unused]] static constexpr auto object_type     = std::integral_constant<acme::js_type, acme::js_type::object_type>{};
[[maybe_unused]] static constexpr auto function_type   = std::integral_constant<acme::js_type, acme::js_type::function_type>{};
[[maybe_unused]] static constexpr auto identifier_type = std::integral_constant<acme::js_type, acme::js_type::identifier_type>{};

} // namespace acme
