#pragma once

namespace acme {

constexpr auto parser::parse(state::this_expression) -> ast::UniqueAstNode
{
    if ( expect(token_type::tok_this, false, true) == false )
    {
        return {};
    }

    return ast::ThisExpression::make(context(), position());
}

constexpr auto parser::parse(state::object_property) -> ast::UniqueAstNode
{
    using namespace std::string_view_literals;

    // TODO: Page 519

    // Check for get/set method.

    const auto [is_get, is_set] = [&]() -> std::pair<bool, bool>
    {
        if ( expect(token_type::tok_get, false, true) == true )
        {
            return { true, false };
        }

        if ( expect(token_type::tok_set, false, true) == true )
        {
            return { false, true };
        }

        return {};
    }();

    // Parse property key.

    auto key = transition(state::literal_expression{});
    if ( ast::instanceof<ast::Identifier>(key) == false )
    {
        assert(false);
        parser_syntax_error("Expected an identifier, instead of got '%s'.", ast::to_string(key));
        return {};
    }

    // Parse value...

    ast::UniqueAstNode value{};

    // Parse getter.

    if ( is_get == true )
    {
        // Parse parenthesis '()'.

        auto params = parse_sequence(token_type::tok_opening_parenthesis, token_type::tok_closing_parenthesis, true, state::parameter_list{});
        if ( ast::instanceof<ast::AstNodeList>(params) )
        {
            parser_syntax_error("Getter functions must have no formal parameters."sv);
            return {};
        }

        // Parse body.

        if ( auto body = transition(state::block_statement{}); ast::instanceof<ast::BlockStatement>(body) )
        {
            // Make object property getter.

            value = ast::ObjectPropertyGetter::make(context(), std::move(body), position());
        }
    }

    // Parse setter.

    else if ( is_set == true )
    {
        // Parse formal parameters.

        auto params = parse_sequence(token_type::tok_opening_parenthesis, token_type::tok_closing_parenthesis, true, state::parameter_list{});
        if ( ast::instanceof<ast::AstNodeList>(params) == false || params.get()->deref<ast::AstNodeList>().count() != 1 )
        {
            parser_syntax_error("Setter must have exactly one formal parameter."sv);
            return {};
        }

        // Parse body.

        if ( auto body = transition(state::block_statement{}); ast::instanceof<ast::BlockStatement>(body) )
        {
            // Make object property getter.

            value = ast::ObjectPropertySetter::make(context(), std::move(body), std::move(params), position());
        }
    }

    // Parse key/value property. Expect a colon ':' after the property key identifier.

    else if ( expect(token_type::tok_colon, false, true) == true )
    {
        // Parse value.

        value = transition(state::expression{});
    }

    if ( value.get() == nullptr )
    {
        parser_syntax_error("Expected a value"sv);
        return {};
    }

    return ast::ObjectProperty::make(context(), std::move(key), std::move(value), position());
}

constexpr auto parser::parse(state::object_literal) -> ast::UniqueAstNode
{
    using namespace std::string_view_literals;

    auto list = ast::AstNodeList::make(context(), position());
    assert(list.get() != nullptr);

    // TODO: Computed property names.
    // TODO: Shorthand method names.
    // TODO: Shorthand property names.

    while ( true )
    {
        // Parse object property.

        if ( auto property = transition(state::object_property{}); property.get() != nullptr )
        {
            list->insert(std::move(property));
        }

        else
        {
            break;
        }

        // Continue on a comma operator.

        if ( expect(token_type::tok_comma, false, true) == false )
        {
            break;
        }
    }

    return ast::ObjectLiteral::make(context(), std::move(list), position());
}

// <PostfixExpression> ::
//     <LeftHandSideExpression>
//     <LeftHandSideExpression> ('++' | '--')

constexpr auto parser::parse(state::postfix_expression) -> ast::UniqueAstNode
{
    auto left = transition(state::member_expression{});

    // Accepted token types for a postfix expression.

    /* static */ constexpr auto k_expected = std::array
    {
        token_type::tok_increment,
        token_type::tok_decrement,
    };

    const auto op_type = current_token().type();

    // Rewrite postfix addition expression 'i++' as 'i += 1'
    // and counterwise subtraction 'i--' as 'i -= 1'.

    if ( expect_one_of(k_expected, false, true) == true )
    {
        auto inc_by = op_type == token_type::tok_increment ? 1 : -1;
        auto right  = ast::Literal::make(context(), ast::Integer{inc_by}, position());

        return ast::BinaryExpression::make(
            context(),
            std::move(left),
            std::move(right),
            token_type::tok_assignment_plus,
            position()
        );
    }

    return left;
}

constexpr auto parser::parse(state::member_expression) -> ast::UniqueAstNode
{
    using namespace std::string_view_literals;

    auto left = transition(state::new_expression{});

    do
    {
        // Member expression.

        if ( expect(token_type::tok_dot, false, true) == true )
        {
            if ( auto exp = transition(state::literal_expression{}); exp.get() != nullptr )
            {
                if ( ast::instanceof<ast::Identifier>(exp) == false )
                {
                    parser_syntax_error("Expected an identifier before '.'"sv);
                    return {};
                }

                left = ast::MemberExpression::make(context(), position(), std::move(left), std::move(exp));
            }

            else
            {
                parser_syntax_error("Expected a property name after '.'"sv);
                return {};
            }
        }

        // Bracket expression.

        else if ( expect(token_type::tok_opening_square_bracket, false, false) == true )
        {
    #if 0
            if ( auto arguments = parse_sequence(token_type::tok_opening_square_bracket, token_type::tok_closing_square_bracket, true, state::argument_list{}); arguments.get() != nullptr )
            {
                left = ast::SequenceExpression::make(context(), position(), std::move(left), std::move(arguments));
            }


    #endif
        }

        // Call expression.

        else if ( expect(token_type::tok_opening_parenthesis, false, true) == true )
        {
            // Parse arguments.

            auto arguments = parse_comma_sequence(state::expression{});

            // Expect a closing parenthesis.

            if ( expect(token_type::tok_closing_parenthesis, true, true) == false )
            {
                assert(false);
                return {};
            }

            left = ast::CallExpression::make(context(), position(), std::move(left), std::move(arguments));
        }

        else
        {
            break;
        }
    }
    while ( true );

    return left;
}

constexpr auto parser::parse(state::arrow_function_expression) -> ast::UniqueAstNode
{
    if ( expect(token_type::tok_arrow_function, false, true) == false )
    {
        return {};
    }

    // if ( previous_transition() == state::parenthesis_expression{} )
    // {
    //     /*
    //         formal parmaters -> return
    //     */
    // }

    // else
    // {
    //     // TODO: Otherwise expect an identifier.

    // }

    return {};
}

constexpr auto parser::parse(state::new_expression) -> ast::UniqueAstNode
{
    using namespace std::string_view_literals;

    // Expect a 'new' keyword.

    if ( expect(token_type::tok_new, false, true) == true )
    {
        // Check for "new.target" pseudo-property.

        const bool is_new_target = [&]()
        {
            // Expect dot.

            if ( expect(token_type::tok_dot, false, true) == false )
            {
                return false;
            }

            // Expect "target" property key.

            if ( auto key = transition(state::literal_expression{}); ast::instanceof<ast::Identifier>(key) == true )
            {
                if ( key.get()->as<ast::Identifier>()->value() == "target"sv )
                {
                    return true;
                }
            }

            return false;
        }();

        if ( is_new_target )
        {
            return ast::MetaProperty::make(context(), position(), ast::MetaProperty::property_type::new_target);
        }

        // Check for "new import".

        else if ( expect(token_type::tok_import, false, true) == true )
        {
            /* TODO */
        }

        // Expect an identifier.

        auto identifier = transition(state::literal_expression{});

        if ( ast::instanceof<ast::Identifier>(identifier) == false )
        {
            parser_syntax_error("Expected an identifier, instead of got '%s'."sv, ast::to_string(identifier));
            return {};
        }

        // Expect an opening parenthesis '('.

        if ( expect(token_type::tok_opening_parenthesis, false, true) == true )
        {
            // Parse arguments.

            auto arguments = parse_comma_sequence(state::expression{});

            // Expect a closing parenthesis ')'.

            if ( expect(token_type::tok_closing_parenthesis, true, true) == false )
            {
                return {};
            }

            return ast::NewExpression::make(context(), position(), std::move(identifier), std::move(arguments));
        }
    }

    return transition(state::primary_expression{});
}

// <PrimaryExpression> ::
//     'this'
//     <IdentifierReference>
//     <Literal>
//     <ArrayLiteral>
//     <ObjectLiteral>
//     <FunctionExpression>
//     <RegularExpressionLiteral>
//     <TemplateLiteral>
//     <NewExpression>

constexpr auto parser::parse(state::primary_expression) -> ast::UniqueAstNode
{
    ast::UniqueAstNode result{};

    if ( expect(token_type::tok_opening_parenthesis, false, false) == true )
    {
        result = parse_sequence(token_type::tok_opening_parenthesis, token_type::tok_closing_parenthesis, true, state::expression{});
    }

    else if ( expect(token_type::tok_this, false, false) == true )
    {
        result = transition(state::this_expression{});
    }

    else if ( expect(token_type::tok_opening_bracket, false, false) == true )
    {
        result = parse_sequence(token_type::tok_opening_bracket, token_type::tok_closing_bracket, true, state::object_literal{});
    }

    else if ( expect(token_type::tok_function, false, false) == true )
    {
        result = transition(state::function_expression{});
    }

    else if ( expect(token_type::tok_arrow_function, false, false) == true )
    {
        result = transition(state::arrow_function_expression{});
    }

    else if ( expect(token_type::tok_opening_square_bracket, false, false) == true )
    {
        result = parse_sequence(token_type::tok_opening_square_bracket, token_type::tok_closing_square_bracket, true, state::array_literal{});
    }

    else if ( current_token().is_literal() )
    {
        result = transition(state::literal_expression{});
    }

    return result;
}

} // namespace acme
