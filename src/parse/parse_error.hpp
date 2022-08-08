#pragma once

namespace acme {

template <typename... Args>
constexpr auto syntax_error(
    acme::concepts::parser auto&     parser,
    acme::concepts::string_like auto format_message,
    Args&&...                        args
) noexcept
{
    if ( std::is_constant_evaluated() == false )
    {
        char error_buffer[256];

        std::cerr << "At line: " << parser.source_location().line() << " column: " << parser.source_location().column() << '\n';
        snprintf(error_buffer, sizeof(error_buffer), format_message.data(), args...);
        std::cerr << error_buffer << '\n';
    }
}

} // namespace acme
