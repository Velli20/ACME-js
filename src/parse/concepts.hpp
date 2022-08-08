#pragma once

namespace acme::concepts {

template <typename T>
concept parser = requires(const T parser, acme::token_type token)
{
    { parser.source_location() } -> std::same_as<position::checkpoint_type>;
};


} // namespace acme::concepts
