#pragma once

namespace acme {

namespace {

[[nodiscard]] constexpr auto parse_quoted(
    auto&      parser,
    token_type token
) -> ast::UniqueAstNode
{
    const auto parse_position = parser.position();
    const auto checkpoint     = parser.source_location();
    const auto stop_char      = token == token_type::tok_single_quote ? '\'' : '\"';

    if ( parser.expect(token, false, false) == false )
    {
        return {};
    }

    while ( true )
    {
        parser.eat(1);

        if ( parser.empty() )
        {
            break;
        }

        if ( auto c = parser.peek(); c == stop_char )
        {
            break;
        }
    }

    auto raw_string      = parser.from(checkpoint);
    auto interned_string = parser.context().get_string_pool().intern(raw_string);

    parser.eat(1);

    return ast::Literal::make(parser.context(), ast::String{interned_string}, parse_position);
}

} // namespace

constexpr auto parser::parse(state::literal_expression) -> ast::UniqueAstNode
{
    auto token          = current_token();
    auto parse_position = position();

    if ( token.is_literal() == false )
    {
        return {};
    }

    ast::UniqueAstNode result{};

    switch ( token.type() )
    {
        case token_type::tok_single_quote:
            result = parse_quoted(*this, token_type::tok_single_quote);
            break;

        case token_type::tok_double_quote:
            result = parse_quoted(*this, token_type::tok_double_quote);
            break;

        case acme::token_type::tok_identifier:
            result = ast::Identifier::make(context(), token.to_string(), parse_position);
            break;

        case acme::token_type::tok_string:
            result = ast::Literal::make(context(), ast::String{context().get_string_pool().intern(token.to_string())}, parse_position);
            break;

        case acme::token_type::tok_signed_number:
            result = ast::Literal::make(context(), ast::Integer{token.to_signed_integer()}, parse_position);
            break;

        case acme::token_type::tok_unsigned_number:
            result = ast::Literal::make(context(), ast::UnsignedInteger{token.to_unsigned_integer()}, parse_position);
            break;

        case acme::token_type::tok_float_number:
            result = ast::Literal::make(context(), ast::Float{token.to_double()}, parse_position);
            break;

        case acme::token_type::tok_true:
            result = ast::Literal::make(context(), ast::Boolean{true}, parse_position);
            break;

        case acme::token_type::tok_false:
            result = ast::Literal::make(context(), ast::Boolean{false}, parse_position);
            break;

        case acme::token_type::tok_null:
            result = ast::Literal::make(context(), ast::Null{}, parse_position);
            break;

        case acme::token_type::tok_undefined:
            result = ast::Literal::make(context(), ast::Undefined{}, parse_position);
            break;

        default:
            return {};
    }

    // On success eat the extracted literal token.

    next_token();

    return result;
}

// <ArrayLiteral> ::
//    ('[' <Element> ']')
//    ('[' <ElementList> ']')
//    ('[' <ElementList> ',' <Element> ']')

constexpr auto parser::parse(state::array_literal) -> ast::UniqueAstNode
{
    auto next_literal_or_null = [&]() -> ast::UniqueAstNode
    {
        if ( current_token().is_literal() )
        {
            return transition(state::literal_expression{});
        }

        return ast::Literal::make(context(), ast::Null{}, position());
    };

    auto list = ast::AstNodeList::make(context(), position());
    assert(list.get() != nullptr);

    while ( true )
    {
        // Get next array element.

        if ( auto token = current_token(); token.is_literal() )
        {
            // Parse next literal or append just a Null element in case of a comma opeator.

            if ( auto element = next_literal_or_null(); element.get() != nullptr )
            {
                list->insert(std::move(element));
            }

            // Break if no literal element was parsed.

            else
            {
                //std::cerr << "Expected an array element\n";
                break;
            }
        }

        // Stop parsing if no comma operator ','.

        if ( expect(token_type::tok_comma, false, true) == false )
        {
            break;
        }
    }

    return ast::ArrayLiteral::make(context(), std::move(list), position());
}

} // namespace acme
