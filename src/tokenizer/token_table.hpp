#pragma once

namespace acme {

struct token_table
{
    using char_type          = char;
    using match_positon_type = std::size_t;
    using match_type         = token_item;
    using char_traits        = std::char_traits<char_type>;

    static constexpr token_item keywords[] =
    {
        { std::string_view{"new"},               match_not_alphanum,        token_type::tok_new,                 token_flags::keyword },
        { std::string_view{"delete"},            match_not_alphanum,        token_type::tok_delete,              token_flags::none    },
        { std::string_view{"typeof"},            match_not_alphanum,        token_type::tok_typeof,              token_flags::none    },
        { std::string_view{"void"},              match_not_alphanum,        token_type::tok_void,                token_flags::keyword },
        { std::string_view{"await"},             match_not_alphanum,        token_type::tok_await,               token_flags::none    },
        { std::string_view{"in"},                match_not_alphanum,        token_type::tok_in,                  token_flags::keyword },
        { std::string_view{"of"},                match_not_alphanum,        token_type::tok_of,                  token_flags::keyword },
        { std::string_view{"instanceof"},        match_not_alphanum,        token_type::tok_instanceof,          token_flags::keyword },
        { std::string_view{"true"},              match_not_alphanum,        token_type::tok_true,                token_flags::keyword | token_flags::literal },
        { std::string_view{"false"},             match_not_alphanum,        token_type::tok_false,               token_flags::keyword | token_flags::literal },
        { std::string_view{"undefined"},         match_not_alphanum,        token_type::tok_undefined,           token_flags::keyword | token_flags::literal },
        { std::string_view{"null"},              match_not_alphanum,        token_type::tok_null,                token_flags::keyword },
        { std::string_view{"this"},              match_not_alphanum,        token_type::tok_this,                token_flags::keyword },
        { std::string_view{"if"},                match_not_alphanum,        token_type::tok_if,                  token_flags::keyword },
        { std::string_view{"else"},              match_not_alphanum,        token_type::tok_else,                token_flags::keyword },
        { std::string_view{"case"},              match_not_alphanum,        token_type::tok_case,                token_flags::keyword },
        { std::string_view{"do"},                match_not_alphanum,        token_type::tok_do,                  token_flags::keyword },
        { std::string_view{"for"},               match_not_alphanum,        token_type::tok_for,                 token_flags::keyword },
        { std::string_view{"continue"},          match_not_alphanum,        token_type::tok_continue,            token_flags::keyword },
        { std::string_view{"break"},             match_not_alphanum,        token_type::tok_break,               token_flags::keyword },
        { std::string_view{"return"},            match_not_alphanum,        token_type::tok_return,              token_flags::keyword },
        { std::string_view{"with"},              match_not_alphanum,        token_type::tok_with,                token_flags::keyword },
        { std::string_view{"enum"},              match_not_alphanum,        token_type::tok_enum,                token_flags::keyword },
        { std::string_view{"switch"},            match_not_alphanum,        token_type::tok_enum,                token_flags::keyword },
        { std::string_view{"throw"},             match_not_alphanum,        token_type::tok_throw,               token_flags::keyword },
        { std::string_view{"try"},               match_not_alphanum,        token_type::tok_try,                 token_flags::keyword },
        { std::string_view{"catch"},             match_not_alphanum,        token_type::tok_catch,               token_flags::keyword },
        { std::string_view{"debugger"},          match_not_alphanum,        token_type::tok_debugger,            token_flags::keyword },
        { std::string_view{"while"},             match_not_alphanum,        token_type::tok_while,               token_flags::keyword },
        { std::string_view{"export"},            match_not_alphanum,        token_type::tok_export,              token_flags::keyword },
        { std::string_view{"import"},            match_not_alphanum,        token_type::tok_import,              token_flags::keyword },
        { std::string_view{"var"},               match_not_alphanum,        token_type::tok_var,                 token_flags::keyword },
        { std::string_view{"let"},               match_not_alphanum,        token_type::tok_let,                 token_flags::keyword },
        { std::string_view{"const"},             match_not_alphanum,        token_type::tok_const,               token_flags::keyword },
        { std::string_view{"yield"},             match_not_alphanum,        token_type::tok_yield,               token_flags::keyword },
        { std::string_view{"function"},          match_not_alphanum,        token_type::tok_function,            token_flags::keyword },
        { std::string_view{"function*"},         match_not_alphanum,        token_type::tok_function_generator,  token_flags::keyword },
        { std::string_view{"class"},             match_not_alphanum,        token_type::tok_class,               token_flags::keyword },
        { std::string_view{"extends"},           match_not_alphanum,        token_type::tok_extends,             token_flags::keyword },
        { std::string_view{"super"},             match_not_alphanum,        token_type::tok_super,               token_flags::keyword },
        { std::string_view{"from"},              match_not_alphanum,        token_type::tok_from,                token_flags::keyword },
        { std::string_view{"get"},               match_not_alphanum,        token_type::tok_get,                 token_flags::keyword },
        { std::string_view{"set"},               match_not_alphanum,        token_type::tok_set,                 token_flags::keyword },
        { std::string_view{"static"},            match_not_alphanum,        token_type::tok_static,              token_flags::keyword },
        { std::string_view{"constructor"},       match_not_alphanum,        token_type::tok_constructor,         token_flags::keyword },
        { std::string_view{"default"},           match_not_alphanum,        token_type::tok_default,             token_flags::keyword },
        { std::string_view{"finally"},           match_not_alphanum,        token_type::tok_finally,             token_flags::keyword },
    };

    static constexpr token_item table[] =
    {
        { std::string_view{"="},                 match_not<'='>,            token_type::tok_assignment,                       token_flags::none    },
        { std::string_view{"=>"},                match_not<'=', '>'>,       token_type::tok_arrow_function,                   token_flags::none    },
        { std::string_view{"=="},                match_not<'='>,            token_type::tok_equal,                            token_flags::none    },
        { std::string_view{"==="},               match_any,                 token_type::tok_strict_equal,                     token_flags::none    },

        { std::string_view{"!="},                match_not<'='>,            token_type::tok_not_equal,                        token_flags::none    },
        { std::string_view{"!=="},               match_any,                 token_type::tok_strict_not_equal,                 token_flags::none    },
        { std::string_view{">"},                 match_not<'=', '>'>,       token_type::tok_greater_than,                     token_flags::none    },
        { std::string_view{">="},                match_not<'='>,            token_type::tok_greater_than_or_equal,            token_flags::none    },
        { std::string_view{"<"},                 match_not<'=', '<'>,       token_type::tok_less_than,                        token_flags::none    },
        { std::string_view{"<="},                match_not<'='>,            token_type::tok_less_than_or_equal,               token_flags::none    },
        { std::string_view{"++"},                match_any,                 token_type::tok_increment,                        token_flags::none    },
        { std::string_view{"+"},                 match_not<'+', '='>,       token_type::tok_plus,                             token_flags::none    },
        { std::string_view{"--"},                match_any,                 token_type::tok_decrement,                        token_flags::none    },
        { std::string_view{"-"},                 match_not<'-', '='>,       token_type::tok_minus,                            token_flags::none    },
        { std::string_view{"**"},                match_not<'='>,            token_type::tok_exponential,                      token_flags::none    },
        { std::string_view{"*"},                 match_not<'*', '/', '='>,  token_type::tok_multiply,                         token_flags::none    },
        { std::string_view{"/"},                 match_not<'*', '/', '='>,  token_type::tok_divide,                           token_flags::none    },
        { std::string_view{"%"},                 match_not<'='>,            token_type::tok_modulo,                           token_flags::none    },
        { std::string_view{"&"},                 match_not<'&'>,            token_type::tok_bitwise_and,                      token_flags::none    },
        { std::string_view{"|"},                 match_not<'|'>,            token_type::tok_bitwise_or,                       token_flags::none    },
        { std::string_view{"^"},                 match_any,                 token_type::tok_bitwise_xor,                      token_flags::none    },
        { std::string_view{"~"},                 match_any,                 token_type::tok_bitwise_not,                      token_flags::none    },
        { std::string_view{"<<"},                match_not<'='>,            token_type::tok_left_shift,                       token_flags::none    },
        { std::string_view{">>"},                match_not<'>', '='>,       token_type::tok_right_shift,                      token_flags::none    },
        { std::string_view{">>>"},               match_not<'='>,            token_type::tok_zero_fill_right_shift,            token_flags::none    },
        { std::string_view{"&&"},                match_any,                 token_type::tok_logical_and,                      token_flags::none    },
        { std::string_view{"||"},                match_any,                 token_type::tok_logical_or,                       token_flags::none    },
        { std::string_view{"!"},                 match_any,                 token_type::tok_logical_not,                      token_flags::none    },
        { std::string_view{"??"},                match_any,                 token_type::tok_nullish_coalescing,               token_flags::none    },
        { std::string_view{";"},                 match_any,                 token_type::tok_semicolon,                        token_flags::none    },
        { std::string_view{","},                 match_any,                 token_type::tok_comma,                            token_flags::none    },
        { std::string_view{"+="},                match_any,                 token_type::tok_assignment_plus,                  token_flags::none    },
        { std::string_view{"-="},                match_any,                 token_type::tok_assignment_minus,                 token_flags::none    },
        { std::string_view{"*="},                match_any,                 token_type::tok_assignment_multiply,              token_flags::none    },
        { std::string_view{"**="},               match_any,                 token_type::tok_assignment_exponential,           token_flags::none    },
        { std::string_view{"/="},                match_any,                 token_type::tok_assignment_divide,                token_flags::none    },
        { std::string_view{"%="},                match_any,                 token_type::tok_assignment_modulo,                token_flags::none    },
        { std::string_view{"<<="},               match_any,                 token_type::tok_assignment_left_shift,            token_flags::none    },
        { std::string_view{">>="},               match_any,                 token_type::tok_assignment_right_shift,           token_flags::none    },
        { std::string_view{">>>="},              match_any,                 token_type::tok_assignment_zero_fill_right_shift, token_flags::none    },
        { std::string_view{"?"},                 match_not<'?','.'>,        token_type::tok_ternary,                          token_flags::none    },
        { std::string_view{"("},                 match_any,                 token_type::tok_opening_parenthesis,              token_flags::none    },
        { std::string_view{")"},                 match_any,                 token_type::tok_closing_parenthesis,              token_flags::none    },
        { std::string_view{"{"},                 match_any,                 token_type::tok_opening_bracket,                  token_flags::none    },
        { std::string_view{"}"},                 match_any,                 token_type::tok_closing_bracket,                  token_flags::none    },
        { std::string_view{"/*"},                match_any,                 token_type::tok_multi_line_comment_start,         token_flags::none    },
        { std::string_view{"*/"},                match_any,                 token_type::tok_multi_line_comment_end,           token_flags::none    },
        { std::string_view{"<!--"},              match_any,                 token_type::tok_html_comment_start,               token_flags::none    },
        { std::string_view{"-->"},               match_any,                 token_type::tok_html_comment_end,                 token_flags::none    },
        { std::string_view{"//"},                match_any,                 token_type::tok_single_line_comment_start,        token_flags::none    },
        { std::string_view{"#!"},                match_any,                 token_type::tok_hashbang_comment_start,           token_flags::none    },
        { std::string_view{"\r"},                match_not<'\n'>,           token_type::tok_line_terminator,                  token_flags::none    },
        { std::string_view{"\n"},                match_any,                 token_type::tok_line_terminator,                  token_flags::none    },
        { std::string_view{"\r\n"},              match_any,                 token_type::tok_line_terminator,                  token_flags::none    },
        { std::string_view{" "},                 match_any,                 token_type::tok_whitespace,                       token_flags::none    },
        { std::string_view{"\t"},                match_any,                 token_type::tok_whitespace,                       token_flags::none    },
        { std::string_view{"\\"},                match_any,                 token_type::tok_backslash,                        token_flags::none    },
        { std::string_view{":"},                 match_any,                 token_type::tok_colon,                            token_flags::none    },
        { std::string_view{"'"},                 match_any,                 token_type::tok_single_quote,                     token_flags::literal },
        { std::string_view{"\""},                match_any,                 token_type::tok_double_quote,                     token_flags::literal },
        { std::string_view{"."},                 match_not<'.'>,            token_type::tok_dot,                              token_flags::none    },
        { std::string_view{"?."},                match_not<'='>,            token_type::tok_optional_chaining,                token_flags::none    },
        { std::string_view{"["},                 match_any,                 token_type::tok_opening_square_bracket,           token_flags::none    },
        { std::string_view{"]"},                 match_any,                 token_type::tok_closing_square_bracket,           token_flags::none    },
        { std::string_view{"..."},               match_any,                 token_type::tok_rest_parameters,                  token_flags::none    },
    };

    static constexpr auto to_string(token_type t) -> std::string_view
    {
        if ( t == acme::token_type::tok_identifier )
        {
            return std::string_view{"identifier"};
        }

        if ( t == acme::token_type::tok_signed_number )
        {
            return std::string_view{"signed number"};
        }

        if ( t == acme::token_type::tok_unsigned_number )
        {
            return std::string_view{"unsigned number"};
        }

        for ( const auto& item : keywords )
        {
            if ( item == t )
            {
                return item.to_string();
            }
        }

        for ( const auto& item : table )
        {
            if ( item == t )
            {
                return item.to_string();
            }
        }

        return std::string_view{"Empty"};
    }
};

} // namespace acme
