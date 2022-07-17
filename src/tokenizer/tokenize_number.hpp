#pragma once

namespace acme {

[[nodiscard]] constexpr auto parse_integer(
    concepts::tokenizer auto& tok,
    auto                      radix,
    bool                      as_signed,
    bool                      do_consume
) noexcept
{
    auto number = token_item::unsigned_number_type{};
    auto digits = token_item::unsigned_number_type{};

    // Do accumulate numeric literal.

    while ( true )
    {
        const auto d = tok.peek();

        // Handle binary-literal.

        if ( acme::match_range<'0', '1'>(d) == true && radix == token_item::unsigned_number_type{2}  )
        {
            tok.eat(1);
            number *= radix;
            number += static_cast<token_item::unsigned_number_type>(d - '0');
            digits += 1;
        }

        // Handle octal-literal.

        else if ( acme::match_range<'0', '7'>(d) == true && radix == token_item::unsigned_number_type{8}  )
        {
            tok.eat(1);
            number *= radix;
            number += static_cast<token_item::unsigned_number_type>(d - '0');
            digits += 1;
        }

        // Handle decimal-literal.

        else if ( codepoint::is_decimal_digit(d) == true )
        {
            tok.eat(1);
            number *= radix;
            number += static_cast<token_item::unsigned_number_type>(d - '0');
            digits += 1;
        }

        // Handle hex-literal.

        else if ( acme::match_range<'a', 'f', true>(d) == true && radix == token_item::unsigned_number_type{16} )
        {
            tok.eat(1);
            number *= radix;
            number += static_cast<token_item::unsigned_number_type>(codepoint::to_lower(d) - 'a' + 10);
            digits += 1;
        }

        // Finish on whitespace.

        else if ( codepoint::is_whitespace(d) || acme::match_one_of<'e', '.', ',', ';', ')'>(d) )
        {
            constexpr auto to_signed = [](token_item::signed_number_type num) -> token_item::signed_number_type
            {
                return token_item::signed_number_type{-1} * static_cast<token_item::signed_number_type>(num);
            };

            if ( as_signed )
            {
                return std::pair{ token_item::make(to_signed(number)), digits };
            }

            else
            {
                return std::pair{ token_item::make(number), digits };
            }
        }

        else
        {
            break;
        }

        // Overflow check.

        if ( digits > std::numeric_limits<token_item::signed_number_type>::digits )
        {
            break;
        }
    }

    return std::pair{ token_table::match_type{}, token_item::unsigned_number_type{} };
}

namespace {

[[nodiscard]] constexpr auto parse_float(
    concepts::tokenizer auto& tok,
    std::integral auto        significand,
    const bool                as_signed
) -> double
{
    auto exp       = token_item::unsigned_number_type{};
    auto sign      = as_signed ? -1.0 : 1.0;
    auto exp_minus = false;

    // Parse fraction.

    auto [fraction, fraction_digit_count] = parse_integer(tok, 10, false, true);

    // Parse exponent part.

    if ( const auto d = tok.peek(); acme::match_one_of<'e', 'E'>(d) )
    {
        tok.eat(1);

        if ( auto minus = tok.peek(); minus == '-' )
        {
            exp_minus = true;
            auto [expp, exp_digit_count] = parse_integer(tok, 10, false, true);

            exp = expp.to_unsigned_integer();
        }
    }

    auto fraction_digit = static_cast<double>(fraction.to_unsigned_integer());
    for ( std::size_t i{}; i < fraction_digit_count; ++i )
    {
        fraction_digit /= 10.0;
    }

    auto result = sign * (significand + fraction_digit);
    for ( std::size_t i{}; i < exp; ++i )
    {
        result = exp_minus ? result / 10 : result * 10;
    }

    return result;
}

#if 0

struct Limit {
    int64_t minVal;
    uint64_t maxVal;
    const char* minStr;
    const char* maxStr;
};

// clang-format off
static const Limit limits [] =
{
    // bool
    {         0,          1,           "0",         "1" },
    // int8
    {       -128,        127,        "-128",       "127" },
    // uint8
    {          0,        255,           "0",        "255" },
    // int16
    {      -32768,      32767,      "-32768",      "32767" },
    // uint16
    {          0,      65535,           "0",      "65535" },
    // int32
    { -2147483648, 2147483647, "-2147483648", "2147483647" },
    // uint32
    {          0, 4294967295,           "0", "4294967295" },
    // int64
    // NOTE: minimum should be -..808, but clang complains about it..
    {-9223372036854775807ll,  9223372036854775807llu, "-9223372036854775808", "9223372036854775807" },
    // uint64
    {                   0,  18446744073709551615llu, "0", "18446744073709551615" },
};
// clang-format on

static const Limit* getLimit(int width) {
    switch (width) {
    case 1: return &limits[0];
    case 7: return &limits[1];
    case 8: return &limits[2];
    case 15: return &limits[3];
    case 16: return &limits[4];
    case 31: return &limits[5];
    case 32: return &limits[6];
    case 63: return &limits[7];
    case 64: return &limits[8];
    default:
        fprintf(stderr, "UNHANDLED width %d\n", width);
        TODO;
        return 0;
    }
}
#endif


} // namespace

[[nodiscard]] constexpr auto parse_number(concepts::tokenizer auto& tok) -> std::optional<token_table::match_type>
{
    const auto as_signed = [&]()
    {
        if ( auto c = tok.peek(); acme::match_one_of<'-', '+'>(c) )
        {
            tok.eat(1);
            return c == '-';
        }

        return false;
    }();

    const auto radix = [&]()
    {
        auto [first, second] = tok.template peek<2>();

        // Check if hex-literal.

        if ( first == '0' && codepoint::to_lower(second) == 'x' )
        {
            tok.eat(2);
            return token_item::unsigned_number_type{16};
        }

        // Or binary.

        if ( first == '0' && codepoint::to_lower(second) == 'b' )
        {
            tok.eat(2);
            return token_item::unsigned_number_type{2};
        }

        // Or octal.

        if ( first == '0' && codepoint::to_lower(second) == 'o' )
        {
            tok.eat(2);
            return token_item::unsigned_number_type{8};
        }

        return token_item::unsigned_number_type{10};
    }();

    if ( auto [number, number_digits] = parse_integer(tok, radix, as_signed, true); number_digits > decltype(number_digits){} )
    {
        // Continue as float.

        if ( const auto d = tok.peek(); acme::match_one_of<'e', '.'>(d) && radix == token_item::unsigned_number_type{10} )
        {
            tok.eat(1);

            if ( number.type() == acme::token_type::tok_signed_number )
            {
                auto floaty = parse_float(tok, number.to_signed_integer(), as_signed);

                return token_item::make(floaty);
            }

            else if ( number.type() == acme::token_type::tok_unsigned_number )
            {
                auto floaty = parse_float(tok, number.to_unsigned_integer(), as_signed);

                return token_item::make(floaty);
            }
        }

        // Return an integer.

        return number;
    }

    return {};
}

} // namespace acme

