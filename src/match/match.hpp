#pragma once

#include "concepts.hpp"
#include "codepoint.hpp"

namespace acme {

constexpr auto match_any = nullptr;

template <concepts::character_like auto... to_not_match>
constexpr auto match_not = [](concepts::character_like auto x) constexpr
{
    return ((x != to_not_match) && ... && true);
};

// TODO: MSVC compiler error.
// template <concepts::character_like auto... to_match>
template <char... to_match>
constexpr auto match_one_of = [](char x) constexpr
{
    return ((x == to_match) || ...);
};

template <concepts::character_like auto from, concepts::character_like auto to, bool ignore_case = true>
constexpr auto match_range = [](concepts::character_like auto x) constexpr
{
    if constexpr ( ignore_case )
    {
        return match_range<codepoint::to_lower(from), codepoint::to_lower(to), false>(codepoint::to_lower(x));
    }

    else
    {
        return x >= from && x <= to;
    }
};

static_assert(match_range<'a', 'F'>('a') == true,  "");
static_assert(match_range<'a', 'F'>('A') == true,  "");
static_assert(match_range<'a', 'F'>('f') == true,  "");
static_assert(match_range<'a', 'F'>('F') == true,  "");
static_assert(match_range<'a', 'F'>('G') == false, "");
static_assert(match_range<'a', 'F'>('0') == false, "");

constexpr auto match_identifier = [](concepts::character_like auto x) constexpr
{
    return codepoint::is_letter(x) || x == '_' || x == '$';
};

constexpr auto match_private_identifier = [](concepts::string_like auto s) constexpr
{
    if ( s.size() < 2 )
    {
        return false;
    }

    return s[0] == '#' && match_identifier(s[1]);
};

constexpr auto match_not_punctuator = [](concepts::character_like auto x)
{
    return codepoint::is_whitespace(x) || codepoint::is_alphanumeric(x);
};

constexpr auto match_not_alphanum = [](concepts::character_like auto x)
{
    return codepoint::is_whitespace(x) || codepoint::is_alphanumeric(x) == false;
};

template <concepts::rule_table k_grammar>
constexpr auto match(std::string_view input) noexcept -> std::optional<typename k_grammar::match_type>
{
    for ( const auto& item : k_grammar::keywords )
    {
        if ( item.match(input) )
        {
            return item;
        }
    }

    for ( const auto& item : k_grammar::table )
    {
        if ( item.match(input) )
        {
            return item;
        }
    }

    return {};
}

} // namespace acme::match
