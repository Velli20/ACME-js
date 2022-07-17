#pragma once

namespace acme {

constexpr auto parser::parse(state::function_declaration) -> ast::UniqueAstNode
{
    // Expect a 'function' keyword.

    if ( expect(token_type::tok_function, false, true) == false )
    {
        return {};
    }

    // Identifier.

    auto id = transition(state::literal_expression{});
    if ( ast::instanceof<ast::Identifier>(id) == false )
    {
        std::cerr << "Function statements must have a name.\n";
        return {};
    }

    // Parse formal parameters.

    auto params = parse_sequence(token_type::tok_opening_parenthesis, token_type::tok_closing_parenthesis, true, state::parameter_list{});

    // Parse body.

    auto body = transition(state::block_statement{});

    return ast::FunctionDeclaration::make(context(), std::move(id), std::move(params), std::move(body), position());
}

constexpr auto parser::parse(state::function_expression) -> ast::UniqueAstNode
{
    // Expect a 'function' keyword.

    if ( expect(token_type::tok_function, false, true) == false )
    {
        return {};
    }

    // Parse formal parameters.

    auto params = parse_sequence(token_type::tok_opening_parenthesis, token_type::tok_closing_parenthesis, true, state::parameter_list{});

    // TODO: Check parameters.

#if 0
    if ( ast::instanceof<ast::AstNodeList>(params) == false )
    {
        return {};
    }
#endif

    // Parse body.

    auto body = transition(state::block_statement{});

    return ast::FunctionExpression::make(context(), std::move(params), std::move(body), position());
}

constexpr auto parser::parse(state::arrow_function) -> ast::UniqueAstNode
{
    if ( expect(token_type::tok_arrow_function, false, true) == false )
    {
        return {};
    }

    return {};
}

} // namespace acme
