
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wextra-semi"
#endif /* __clang__ */

#define TTS_MAIN
#include <tts/tts.hpp>

#if defined(__clang__)
#pragma clang diagnostic pop
#endif /* __clang__ */

#include <iostream>

#include "base/base.hpp"
#include "string_pool/string_pool.hpp"

#include "tokenizer/tokenizer.hpp"
#include "var/script_value.hpp"


namespace acme::cx_test {

const auto is_fail = [](auto& tok, auto expected)
{
    auto res = tok.next();

    const bool bool_is_fail = res.match(expected) != true;

    if ( std::is_constant_evaluated() == false && bool_is_fail )
    {
        std::cout << "Expected(" << static_cast<std::uint32_t>(expected) << ") got(" << static_cast<std::uint32_t>(res.type()) << ")\n";
    }

    return bool_is_fail;
};

const auto is_fail_lit = [](auto& tok, auto lit, acme::token_type type = acme::token_type::tok_none)
{
    auto res = tok.next();

    if ( type != acme::token_type::tok_none )
    {
        if ( res.match(type) == false )
        {
            if ( std::is_constant_evaluated() == false )
            {
                std::cout << "Expected(" << static_cast<std::uint32_t>(type) << ") got(" << static_cast<std::uint32_t>(res.type()) << ")\n";
                std::cout << "token: \"" << res.to_string() << "\"\n";
                assert(res.type() == type);
            }

            return true;
        }
    }

    if constexpr ( std::is_same<decltype(lit), token_item::unsigned_number_type>::value )
    {
        if ( res.match(acme::token_type::tok_unsigned_number) == false )
        {
            if ( std::is_constant_evaluated() == false )
            {
                std::cout << "Expected(token_item::unsigned_number_type) got(" << static_cast<std::uint32_t>(res.type()) << ")\n";
            }

            return true;
        }

        const bool is_fail = res.to_unsigned_integer() != lit;

        if ( std::is_constant_evaluated() == false && is_fail )
        {
            std::cout << "Expected:" << lit << " Got: \"" << res.to_unsigned_integer() << "\"\n";
        }

        return is_fail;
    }

    else if constexpr ( std::is_same<decltype(lit), token_item::signed_number_type>::value )
    {
        if ( res.match(acme::token_type::tok_signed_number) == false )
        {
            if ( std::is_constant_evaluated() == false )
            {
                std::cout << "Expected(token_type::tok_signed_number) got(" << static_cast<std::uint32_t>(res.type()) << ")\n";
            }

            return true;
        }

        const bool is_fail = res.to_signed_integer() != lit;
        if ( std::is_constant_evaluated() == false && is_fail )
        {
            std::cout << "Expected:" << lit << " Got: \"" << res.to_signed_integer() << "\"\n";
        }
        return is_fail;
    }

    else if constexpr ( std::is_same<decltype(lit), token_item::float_number_type>::value )
    {
        if ( res.match(acme::token_type::tok_float_number) == false )
        {
            if ( std::is_constant_evaluated() == false )
            {
                std::cout << "Expected(acme::token_type::tok_float_number)\n";
            }

            return true;
        }

        const bool is_fail = res.to_double() != lit;
        if ( std::is_constant_evaluated() == false && is_fail )
        {
            std::cout << "Expected:" << lit << " Got: \"" << res.to_double() << "\"\n";
        }

        return is_fail;
    }

    else if constexpr ( std::is_same<decltype(lit), token_item::string_type>::value )
    {
        const bool is_fail = res.to_string() != lit;
        if ( std::is_constant_evaluated() == false && is_fail )
        {
            std::cout << "Expected:" << lit << " Got: \"" << res.to_string() << "\"\n";
        }

        return is_fail;
    }

    else
    {
        if ( std::is_constant_evaluated() == false )
        {
            std::cout << "Expected:" << lit << " Got: \"" << res.to_string() << "\"\n";
        }

        return true;
    }
};

} // namespace acme::cx_test

constexpr auto cx_test_0005() -> bool
{
    using namespace acme;

    auto s = std::string_view{"Hello!\r\nWorld"};

    tokenizer tok{s};

    if ( tok.eol()                                                           ) { return false; }
    if ( auto [column, line, index] = tok.source(); column != 0 or line != 0 ) { return false; }
    if ( auto c = tok.consume();  c != 'H'  ) { return false; }
    if ( auto [column, line, index] = tok.source(); column != 1 or line != 0 ) { return false; }
    if ( auto c = tok.consume();  c != 'e'  ) { return false; }
    if ( auto [column, line, index] = tok.source(); column != 2 or line != 0 ) { return false; }
    if ( auto c = tok.consume();  c != 'l'  ) { return false; }

    {
        auto expected = std::array{'l', 'o', '!'};
        auto got      = tok.peek<3>();

        if ( not std::equal(std::crbegin(expected), std::crend(expected), std::crbegin(got)) )
        {
            return false;
        }
    }

    {
        auto expected = std::array{'l', 'o', '!'};
        auto got      = tok.consume<3>();

        if ( got.has_value() == false or not std::equal(std::crbegin(expected), std::crend(expected), std::crbegin(got.value())) )
        {
            return false;
        }
    }

    if ( auto [column, line, index] = tok.source(); column != 6 or line != 0 ) { return false; }

    // Check for an eol.

    if ( tok.eol() == false )
    {
        return false;
    }

    // Eat eol.

    if ( tok.eol(true) == false )
    {
        return false;
    }

    if ( tok.eol()     or tok.peek()    != 'W' ) { return false; }
    if ( tok.eol(true) or tok.consume() != 'W' ) { return false; }

    {
        auto expected = std::array{'o', 'r', 'l', 'd'};
        auto got      = tok.consume<4>();

        if ( got.has_value() == false or not std::equal(std::crbegin(expected), std::crend(expected), std::crbegin(got.value())) )
        {
            return false;
        }
    }

    if ( tok.eol()     == false ) { return false; }
    if ( tok.eol(true) == false ) { return false; }
    if ( tok.eol(true) == false ) { return false; }

    return true;
}

constexpr auto cx_test_0003() -> bool
{
    using namespace acme;

    auto expect = std::array
    {
        token_type::tok_opening_bracket,
        token_type::tok_opening_parenthesis,
        token_type::tok_closing_parenthesis,
        token_type::tok_opening_square_bracket,
        token_type::tok_closing_square_bracket,
        token_type::tok_rest_parameters,
        token_type::tok_dot,
        token_type::tok_semicolon,
        token_type::tok_comma,
        token_type::tok_less_than,
        token_type::tok_greater_than,
        token_type::tok_less_than_or_equal,
        token_type::tok_greater_than_or_equal,
        token_type::tok_equal,
        token_type::tok_not_equal,
        token_type::tok_strict_equal,
        token_type::tok_strict_not_equal,
        token_type::tok_plus,
        token_type::tok_minus,
        token_type::tok_multiply,
        token_type::tok_modulo,
        token_type::tok_exponential,
        token_type::tok_increment,
        token_type::tok_decrement,
        token_type::tok_left_shift,
        token_type::tok_right_shift,
        token_type::tok_zero_fill_right_shift,
        token_type::tok_bitwise_and,
        token_type::tok_bitwise_or,
        token_type::tok_bitwise_xor,
        token_type::tok_logical_not,
        token_type::tok_bitwise_not,
        token_type::tok_logical_and,
        token_type::tok_logical_or,
        token_type::tok_nullish_coalescing,
        token_type::tok_ternary,
        token_type::tok_colon,
        token_type::tok_assignment,

        token_type::tok_assignment_plus,

        token_type::tok_assignment_minus,

        token_type::tok_assignment_multiply,

        token_type::tok_assignment_modulo,

        token_type::tok_assignment_exponential,

        token_type::tok_assignment_left_shift,

        token_type::tok_assignment_right_shift,

        token_type::tok_assignment_zero_fill_right_shift,

        token_type::tok_bitwise_and,
        token_type::tok_assignment,

        token_type::tok_bitwise_or,
        token_type::tok_assignment,

        token_type::tok_bitwise_xor,
        token_type::tok_assignment,

        token_type::tok_logical_and,
        token_type::tok_assignment,

        token_type::tok_logical_or,
        token_type::tok_assignment,

        token_type::tok_nullish_coalescing,
        token_type::tok_assignment,
        token_type::tok_new,
    };

    using namespace std::string_view_literals;

    auto input = R"({()[]....;, < >  <=>= == != === !== +-* % ** ++ -- << >> >>> & | ^ ! ~ && || ?? ? : = += -= *= %= **= <<= >>= >>>= &= |= ^= &&= ||= ??= new)"sv;

    tokenizer tok{input};

    for ( auto it = std::cbegin(expect); it != std::cend(expect); )
    {
        auto value = tok.next();
        if ( value.match(acme::token_type::tok_whitespace) )
        {
            continue;
        }

        if ( value.match(*it) == false )
        {
            if ( std::is_constant_evaluated() == false )
            {
                std::cout << "Expected \"" << token_table::to_string(*it) << " \" got \"" << token_table::to_string(value.type()) << "\"\n";
                assert(value.type() == *it);
            }

            return false;
        }

        std::advance(it, 1);
    }

    return true;
}

constexpr auto cx_test_0001() -> bool
{
    using namespace acme;
    using namespace std::string_view_literals;

    auto expect = std::array
    {
        token_type::tok_const,
        token_type::tok_let,
        token_type::tok_var,
        token_type::tok_instanceof,
        token_type::tok_of,
    };

    auto input = R"(const let var instanceof of)"sv;

    tokenizer tok{input};

    for ( auto it = std::cbegin(expect); it != std::cend(expect); )
    {
        auto value = tok.next();
        if ( value.match(acme::token_type::tok_whitespace) )
        {
            continue;
        }

        if ( value.match(*it) == false )
        {
            if ( std::is_constant_evaluated() == false )
            {
                std::cout << "Expected \"" << token_table::to_string(*it) << " \" got \"" << token_table::to_string(value.type()) << "\"\n";
                assert(value.type() == *it);
            }

            return false;
        }

        std::advance(it, 1);
    }

    return true;
}

constexpr auto cx_test_0002() -> bool
{
    using namespace acme;
    using namespace std::string_view_literals;

    tokenizer tok{" var #_2someIdentifier = +0123456789; 2someString2 0xdeAD120F; 0b101100 0o255 -178 0.341"sv};

    if ( 0b1011'00 != token_item::unsigned_number_type{44} )
    {
        return false;
    }

    if ( cx_test::is_fail(tok, acme::token_type::tok_var)        ) { return false; }

    if ( cx_test::is_fail_lit(tok, "#_2someIdentifier"sv, acme::token_type::tok_private_identifier) )
    {
        return false;
    }

    if ( cx_test::is_fail(tok, acme::token_type::tok_assignment) ) { return false; }

    if ( cx_test::is_fail_lit(tok, token_item::unsigned_number_type{123456789}) )
    {
        return false;
    }

    if ( cx_test::is_fail(tok,     acme::token_type::tok_semicolon)                ) { return false; }
    if ( cx_test::is_fail_lit(tok, "2someString2"sv, acme::token_type::tok_string) ) { return false; }

    if ( cx_test::is_fail_lit(tok, token_item::unsigned_number_type{0xdeAD120F})   ) { return false; }

    if ( cx_test::is_fail(tok,     acme::token_type::tok_semicolon)                ) { return false; }

    if ( cx_test::is_fail_lit(tok, token_item::unsigned_number_type{0b1011'00})    ) { return false; }

    if ( cx_test::is_fail_lit(tok, token_item::unsigned_number_type{173})          ) { return false; }

    if ( cx_test::is_fail_lit(tok, token_item::signed_number_type{-178})           ) { return false; }

    if ( cx_test::is_fail_lit(tok, token_item::float_number_type{0.341})           ) { return false; }

    return true;
}

TTS_CASE( "Check that constexpr expectation can be met" )
{
// #if !defined(_MSC_VER)
//     TTS_CONSTEXPR_EXPECT(cx_test_0001() == true);
//     TTS_CONSTEXPR_EXPECT(cx_test_0002() == true);
//     TTS_CONSTEXPR_EXPECT(cx_test_0003() == true);
//     TTS_CONSTEXPR_EXPECT(cx_test_0005() == true);
// #endif /* _MSC_VER */

    TTS_EXPECT(cx_test_0001() == true);
    TTS_EXPECT(cx_test_0002() == true);
    TTS_EXPECT(cx_test_0003() == true);
    TTS_EXPECT(cx_test_0005() == true);
};

TTS_CASE("Concepts")
{
    TTS_CONSTEXPR_EXPECT(acme::concepts::tokenizer<acme::tokenizer<char>>);
};

TTS_CASE("Tokenize integer")
{
    using namespace acme;
    using namespace std::string_view_literals;

    acme::tokenizer tokenizer{"152"sv};

    auto [token, digit_count] = parse_integer(tokenizer, 10, false, true);
    TTS_EXPECT(digit_count == 3);
    TTS_EXPECT(token.to_unsigned_integer() == 152);
};

TTS_CASE("Tokenize number")
{
    using namespace acme;
    using namespace std::string_view_literals;

    {
        acme::tokenizer tokenizer{"152"sv};

        auto token = parse_number(tokenizer);
        TTS_EXPECT(token.value().to_unsigned_integer() == 152);
    }

    {
        acme::tokenizer tokenizer{"-152"sv};

        auto token = parse_number(tokenizer);
        TTS_EXPECT(token.value().to_signed_integer() == -152);
    }

    {
        acme::tokenizer tokenizer{"0.341"sv};

        auto token = parse_number(tokenizer);
        TTS_IEEE_EQUAL(token.value().to_double(), 0.341);
    }

    {
        acme::tokenizer tokenizer{"0.54e2"sv};

        auto token = parse_number(tokenizer);
        TTS_IEEE_EQUAL(token.value().to_double(), 0.54);
    }

    {
        acme::tokenizer tokenizer{"-0.54e2"sv};

        auto token = parse_number(tokenizer);
        TTS_IEEE_EQUAL(token.value().to_double(), -0.54);
    }
};
