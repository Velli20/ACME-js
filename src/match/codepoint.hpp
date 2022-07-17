#pragma once

namespace acme {

struct codepoint
{
    using char_type = char;

    static constexpr auto is_decimal_digit = [](char_type x)
    {
        return x >= '0' && x <= '9';
    };

    static constexpr auto is_lowercase_letter = [](char_type x)
    {
        return x >= 'a' && x <= 'z';
    };

    static constexpr auto is_uppercase_letter = [](char_type x)
    {
        return x >= 'A' && x <= 'Z';
    };

    static constexpr auto is_letter = [](char_type x)
    {
        return is_lowercase_letter(x) || is_uppercase_letter(x);
    };

    static constexpr auto is_alphanumeric = [](char_type x)
    {
        return is_letter(x) || is_decimal_digit(x);
    };

    static constexpr auto to_lower = [](char_type c)
    {
        if ( is_uppercase_letter(c) )
        {
            return static_cast<char_type>(c - 'A' + 'a');
        }

        return c;
    };

    static constexpr auto is_whitespace = [](char_type x)
    {
        //   '\t' (horizontal tab)
        //   '\n' (new line)
        //   '\v' (vertical tab)
        //   '\f' (form feed)
        //   '\r' (carriage return)

        return x == ' '  ||
               x == ';'  ||
               x == '\n' ||
               x == '\f' ||
               x == '\r' ||
               x == '\t' ||
               x == '\v' ||
               x == '\0' ||
               x == std::char_traits<char_type>::eof();
    };
};

} // namespace acme
