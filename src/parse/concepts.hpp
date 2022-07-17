#pragma once

namespace acme::concepts {

template <typename T>
concept parser = requires(T parser, acme::token_type token)
{
    { parser.excpect(token, true, true) } -> std::same_as<bool>;
};


} // namespace acme::concepts
