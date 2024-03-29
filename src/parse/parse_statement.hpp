#pragma once

namespace acme {

constexpr auto parser::parse(state::block_statement) -> ast::UniqueAstNode
{
    // Expect and consume an opening bracket '{'.

    if ( expect(token_type::tok_opening_bracket, false, true) == false )
    {
        return {};
    }

    auto list = ast::AstNodeList::make(context(), position());
    assert(list.get() != nullptr);

    while ( true )
    {
        // Break on a closing bracket '}'.

        if ( expect(token_type::tok_closing_bracket, false, false) == true )
        {
            break;
        }

        // Try parse next statement.

        if ( auto statement = transition(state::statement{}); statement.get() != nullptr )
        {
            list->insert(std::move(statement));
        }
    }

    // Expect and consume a closing bracket '}'.

    if ( expect(token_type::tok_closing_bracket, true, true) == false )
    {
        return {};
    }

    if ( list->count() == 0 )
    {
        return {};
    }

    return ast::BlockStatement::make(context(), std::move(list), position());
}

// <VariableStatement> ::
//  ('var' | 'const' | 'let') <Identifier> '=' <Expression> (';')

constexpr auto parser::parse(state::variable_statement) -> ast::UniqueAstNode
{
    using namespace std::string_view_literals;

    // Get declaration type.

    /* static */ constexpr auto k_expected = std::array
    {
        token_type::tok_var,
        token_type::tok_const,
        token_type::tok_let
    };

    const auto decl_type = current_token();

    if ( expect_one_of(k_expected, false, true) == false )
    {
        return {};
    }

    // Lambda to parse next variable declaration.

    auto next_variable_declaration = [&]() -> ast::UniqueVariableDeclaration
    {
        if ( auto id = transition(state::literal_expression{}); ast::instanceof<ast::Identifier>(id) )
        {
            return ast::VariableDeclaration::make(context(), std::move(id), ast::DeclarationKind{decl_type.type()}, position());
        }

        parser_syntax_error("Expected an identifier."sv);
        return {};
    };

    // Create a variable statement list in case of a comma separated initializer list.

    auto statements = ast::AstNodeList::make(context(), position());
    assert(statements.get() != nullptr);

    while ( true )
    {
        // Get next statement.

        auto var_statement = next_variable_declaration();
        if ( var_statement.get() == nullptr )
        {
            break;
        }

        // Handle assignment/initialization on an assignment operator '='.

        if ( expect(token_type::tok_assignment, false, true) == true )
        {
            if ( auto p = transition(state::expression{}); p.get() != nullptr )
            {
                var_statement.get()->assignment(std::move(p));
            }

            else
            {
                break;
            }
        }

        // Append new variable declaration/statements to the list.

        statements->insert(std::move(var_statement));

        // Stop if current token is semicolon ';' or continue on comma ','.

        if ( expect(token_type::tok_comma, false, true) == true )
        {
            continue;
        }

        if ( expect(token_type::tok_semicolon, true, false) == true )
        {
            break;
        }
    }

    // Expect a semicolon ';'.

    if ( expect(token_type::tok_semicolon, true, true) == false )
    {
        return {};
    }

    // Return first variable from the list if the list holds only one variable.

    if ( statements->count() == 1u )
    {
        return std::move(statements->at(0u));
    }

    return statements;
}

// <ForStatement> ::
//  'for' '(' <Expression> ';' <Expression> ';' <Expression> ';' ')'

constexpr auto parser::parse(state::for_statement) -> ast::UniqueAstNode
{
    using namespace std::string_view_literals;

    // Expect a 'for' keyword.

    if ( expect(token_type::tok_for, false, true) == false )
    {
        return {};
    }

    // Create loop statement node.

    auto loop_statement = ast::LoopStatement::make(context(), position(), ast::loop_kind::k_for_loop);
    if ( loop_statement.get() == nullptr )
    {
        assert(false && "Out of memory");
        return {};
    }

    // Expect an opening parenthesis '('.

    if ( expect(token_type::tok_opening_parenthesis, true, true) == false )
    {
        return {};
    }

    // Initializers (optional).

    if ( auto initializer = transition(state::variable_statement{}); initializer.get() != nullptr )
    {
        loop_statement.get()->initializer(std::move(initializer));
    }

    // Test condition expression (optional).

    if ( auto condition = transition(state::expression{}); condition.get() != nullptr )
    {
        loop_statement.get()->condition(std::move(condition));
    }

    // Expect a semicolon ';' after a condition.

    if ( expect(token_type::tok_semicolon, true, true) == false )
    {
        return {};
    }

    // Update expression (optional).

    if ( auto update = transition(state::expression{}); update.get() != nullptr )
    {
        loop_statement.get()->update(std::move(update));
    }

    // Expect a closing parenthesis ')'.

    if ( expect(token_type::tok_closing_parenthesis, true, true) == false )
    {
        return {};
    }

    // Loop body '{' ... '}'.

    if ( auto body = transition(state::block_statement{}); body.get() != nullptr )
    {
        loop_statement.get()->body(std::move(body));
    }

    return loop_statement;
}

// <WhileStatement> ::
//  'while' '(' <Expression> ')'

constexpr auto parser::parse(state::while_statement) -> ast::UniqueAstNode
{
    using namespace std::string_view_literals;

    // Expect a 'while' keyword.

    if ( expect(token_type::tok_while, false, true) == false )
    {
        return {};
    }

    // Parse while loop condition expression.

    auto condition = parse_sequence(
        /* entry_token */ token_type::tok_opening_parenthesis,
        /* exit_token  */ token_type::tok_closing_parenthesis,
        /* require     */ true,
        /* args        */ state::expression{});

    if ( condition.get() == nullptr )
    {
        parser_syntax_error("Expected '(' to start a while loop condition."sv);
        return {};
    }

    // Create loop statement node.

    auto loop_statement = ast::LoopStatement::make(context(), position(), ast::loop_kind::k_while_loop);
    if ( loop_statement.get() == nullptr )
    {
        assert(false && "Out of memory");
        return {};
    }

    loop_statement.get()->condition(std::move(condition));

    // Loop body '{' ... '}'.

    if ( auto body = transition(state::block_statement{}); body.get() != nullptr )
    {
        loop_statement.get()->body(std::move(body));
    }

    return loop_statement;
}

// <IfStatement> ::
//     'if' <Expression> <Statement>
//     'if' <Expression> <Statement> 'else' <Statement>

constexpr auto parser::parse(state::if_statement) -> ast::UniqueAstNode
{
    // Expect 'if' keyword.

    if ( expect(token_type::tok_if, false, true) == false )
    {
        return {};
    }

    auto result = ast::IfStatement::make(context(), position());

    // Parse conditional expression surrounded by parenthesis.

    if ( auto condition = parse_sequence(token_type::tok_opening_parenthesis, token_type::tok_closing_parenthesis, true, state::expression{}); condition.get() != nullptr )
    {
        result.get()->condition(std::move(condition));
    }

    // Parse consequent block statement:

    if ( auto consequent = transition(state::block_statement{}); consequent.get() != nullptr )
    {
        result.get()->consequent(std::move(consequent));
    }

    // Handle 'else' keyword.

    if ( expect(token_type::tok_else, false, true) == true )
    {
        // Parse alternative block statement:
        //  'else' '{' <Statement> '}'.
        //  'else' 'if' <Expression> '{' <Statement> '}'.

        if ( auto alternate = transition(state::statement{}); alternate.get() != nullptr )
        {
            result.get()->alternate(std::move(alternate));
        }
    }

    return result;
}

// <ReturnStatement> ::
//    'return' ';'
//    'return' <Expression> ';'

constexpr auto parser::parse(state::return_statement) -> ast::UniqueAstNode
{
    // Expect 'return' keyword.

    if ( expect(token_type::tok_return, false, true) == false )
    {
        return {};
    }

    auto result = ast::SimpleStatement::make(context(), position(), ast::simple_statement_kind::k_return_statement);

    // Parse return value.

    if ( auto argument = transition(state::expression{}); argument.get() != nullptr )
    {
        result.get()->argument(std::move(argument));
    }

    // Expect a semicolon ';'.

    if ( expect(token_type::tok_semicolon, true, true) == false )
    {
        return {};
    }

    return result;
}

// <BreakStatement> ::
//    'break' ';'
//    'break' <LabelIdentifier> ';'

constexpr auto parser::parse(state::break_statement) -> ast::UniqueAstNode
{
    using namespace std::string_view_literals;

    // Expect 'break' keyword.

    if ( expect(token_type::tok_break, false, true) == false )
    {
        return {};
    }

    auto result = ast::SimpleStatement::make(context(), position(), ast::simple_statement_kind::k_break_statement);

    // Optional jump label identifier.

    if ( expect(token_type::tok_identifier, false, false) == true )
    {
        auto id = transition(state::literal_expression{});

        if ( ast::instanceof<ast::Identifier>(id) == false )
        {
            parser_syntax_error("Expected an identifier or ';'"sv);
            return {};
        }

        result.get()->argument(std::move(id));
    }

    // Expect a semicolon ';'.

    if ( expect(token_type::tok_semicolon, true, true) == false )
    {
        return {};
    }

    return result;
}

// <ContinueStatement> :
//    'continue' ';'
//    'continue' <LabelIdentifier> ';'

constexpr auto parser::parse(state::continue_statement) -> ast::UniqueAstNode
{
    using namespace std::string_view_literals;

    // Expect 'return' keyword.

    if ( expect(token_type::tok_continue, false, true) == false )
    {
        return {};
    }

    auto result = ast::SimpleStatement::make(context(), position(), ast::simple_statement_kind::k_continue_statement);

    // Optional jump label identifier.

    if ( expect(token_type::tok_identifier, false, false) == true )
    {
        auto id = transition(state::literal_expression{});

        if ( ast::instanceof<ast::Identifier>(id) == false )
        {
            parser_syntax_error("Expected an identifier or ';'"sv);
            return {};
        }

        result.get()->argument(std::move(id));
    }

    // Expect a semicolon ';'.

    if ( expect(token_type::tok_semicolon, true, true) == false )
    {
        return {};
    }

    return result;
}

// <LabelledStatement> ::
//    <LabelIdentifier> ':'

constexpr auto parser::parse(state::labelled_statement) -> ast::UniqueAstNode
{
    // Expect an identifier...

    if ( expect(token_type::tok_identifier, false, false) == false )
    {
        return {};
    }

    if ( peek_token() != token_type::tok_colon )
    {
        return {};
    }

    if ( auto id = transition(state::literal_expression{}); ast::instanceof<ast::Identifier>(id) )
    {
        if ( expect(token_type::tok_colon, false, true) == true )
        {
            auto result = ast::SimpleStatement::make(context(), position(), ast::simple_statement_kind::k_label_statement);
            assert(result.get() != nullptr && "Out of memory");

            result.get()->argument(std::move(id));

            return result;
        }
    }

    return {};
}

constexpr auto parser::parse(state::do_statement)           -> ast::UniqueAstNode { return {}; }
constexpr auto parser::parse(state::function_statement)     -> ast::UniqueAstNode { return {}; }
constexpr auto parser::parse(state::switch_statement)       -> ast::UniqueAstNode { return {}; }
constexpr auto parser::parse(state::try_statement)          -> ast::UniqueAstNode { return {}; }
constexpr auto parser::parse(state::debugger_statement)     -> ast::UniqueAstNode { return {}; }
constexpr auto parser::parse(state::with_statement)         -> ast::UniqueAstNode { return {}; }

// <EmptyStatement> ::
//  ';'

constexpr auto parser::parse(state::empty_statement) -> ast::UniqueAstNode
{
    if ( expect(token_type::tok_semicolon, false, true) == false )
    {
        return {};
    }

    return {};
}

constexpr auto parser::parse(state::statement) -> ast::UniqueAstNode
{
    // Special case to handle label statements.

    if ( expect(token_type::tok_identifier, false, false) == true && peek_token() == token_type::tok_colon )
    {
        return transition(state::labelled_statement{});
    }

    switch ( current_token().type() )
    {
        case token_type::tok_opening_bracket:
            return transition(state::block_statement{});


        case token_type::tok_var:
        case token_type::tok_let:
        case token_type::tok_const:
            return transition(state::variable_statement{});

        case token_type::tok_break:
            return transition(state::break_statement{});

        case token_type::tok_continue:
            return transition(state::continue_statement{});

        case token_type::tok_debugger:
            return transition(state::debugger_statement{});

        case token_type::tok_do:
            return transition(state::do_statement{});

        case token_type::tok_for:
            return transition(state::for_statement{});

        case token_type::tok_function:
            return transition(state::function_declaration{});

        case token_type::tok_if:
            return transition(state::if_statement{});

        case token_type::tok_return:
            return transition(state::return_statement{});

        case token_type::tok_switch:
            return transition(state::switch_statement{});

        case token_type::tok_throw:
            return transition(state::throw_statement{});

        case token_type::tok_try:
            return transition(state::try_statement{});

        case token_type::tok_while:
            return transition(state::while_statement{});

        case token_type::tok_with:
            return transition(state::with_statement{});

        default:
            break;
    }

    // Expression statement.

    if ( auto exp = transition(state::expression{}); exp.get() != nullptr )
    {
        // Expect semicolon ';'.

        if ( expect(token_type::tok_semicolon, true, true) == false )
        {
            assert(false);
            return {};
        }

        return exp;
    }

    return {};
}

} // namespace acme
