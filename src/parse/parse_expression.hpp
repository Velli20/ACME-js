#pragma once

namespace acme {

namespace {

template <auto k_level>
[[nodiscard]] constexpr auto predecense_table()
{
    if constexpr ( k_level == 0 )
    {
        return std::array
        {
            token_type::tok_multiply,
            token_type::tok_divide,
            token_type::tok_modulo
        };
    }

    else if constexpr ( k_level == 1 )
    {
        return std::array
        {
            token_type::tok_plus,
            token_type::tok_minus
        };
    }

    else if constexpr ( k_level == 2 )
    {
        return std::array
        {
            token_type::tok_left_shift,
            token_type::tok_right_shift,
            token_type::tok_zero_fill_right_shift
        };
    }

    else if constexpr ( k_level == 3 )
    {
        return std::array
        {
            token_type::tok_equal,
            token_type::tok_instanceof,
            token_type::tok_not_equal,
            token_type::tok_strict_equal,
            token_type::tok_strict_not_equal,
            token_type::tok_greater_than,
            token_type::tok_greater_than_or_equal,
            token_type::tok_less_than,
            token_type::tok_less_than_or_equal
        };
    }

    else if constexpr ( k_level == 4 )
    {
        return std::array
        {
            token_type::tok_logical_and,
            token_type::tok_logical_or,
            token_type::tok_bitwise_and,
            token_type::tok_bitwise_or,
            token_type::tok_bitwise_xor
        };
    }

    else if constexpr ( k_level == 5 )
    {
        return std::array
        {
            token_type::tok_assignment,
            token_type::tok_assignment_plus,
            token_type::tok_assignment_minus,
            token_type::tok_assignment_multiply,
            token_type::tok_assignment_exponential,
            token_type::tok_assignment_divide,
            token_type::tok_assignment_modulo,
            token_type::tok_assignment_left_shift,
            token_type::tok_assignment_right_shift,
        };
    }
}

} // namespace

// UnaryExpression ::
//  ('!' | '~'| '-'| '+'| '--'| '++' | 'typeof' | 'void' | 'delete' )

constexpr auto parser::parse(state::unary_expression) -> ast::UniqueAstNode
{
    // Accepted token types for an unary expression.

    /* static */ constexpr auto k_expected = std::array
    {
        token_type::tok_delete,
        token_type::tok_void,
        token_type::tok_typeof,
        token_type::tok_increment,
        token_type::tok_decrement,

        token_type::tok_plus,
        token_type::tok_minus,

        token_type::tok_bitwise_not,
        token_type::tok_logical_not
    };

    const auto op_type = current_token();

    if ( expect_one_of(k_expected, false, true) == true )
    {
        if ( auto right = transition(state::unary_expression{}); right.get() != nullptr )
        {
            return ast::UnaryExpression::make(context(), std::move(right), op_type.type(), position());
        }
    }

    return transition(state::postfix_expression{});
}

// MultiplicativeExpression ::
//  <UnaryExpression>
//
// <MultiplicativeExpression>
//  <MultiplicativeOperator> ('*' | '/' | '%')

constexpr auto parser::parse(state::multiplicative_expression) -> ast::UniqueAstNode
{
    // Accepted token types for a multiplicative expression.

    /* static */ constexpr auto k_expected = std::array
    {
        token_type::tok_multiply,
        token_type::tok_divide,
        token_type::tok_modulo
    };

    // Parse left hand side unary expression.

    auto left = transition(state::unary_expression{});

    // Get current token.

    auto op_type = current_token();

    while ( expect_one_of(k_expected, false, true) == true )
    {
        if ( auto right = transition(state::unary_expression{}); right.get() != nullptr )
        {
            left = ast::BinaryExpression::make(context(), std::move(left), std::move(right), op_type.type(), position());
        }

        else
        {
            break;
        }

        op_type = current_token();
    }

    return left;
}

constexpr auto parser::parse(state::additive_expression) -> ast::UniqueAstNode
{
    // Accepted token types for an additive expression.

    /* static */ constexpr auto k_expected = std::array
    {
        token_type::tok_plus,
        token_type::tok_minus
    };

    // Parse left hand side expression.

    auto left = transition(state::multiplicative_expression{});

    // Get current token.

    auto op_type = current_token();

    while ( expect_one_of(k_expected, false, true) == true )
    {
        if ( auto right = transition(state::multiplicative_expression{}); right.get() != nullptr )
        {
            left = ast::BinaryExpression::make(context(), std::move(left), std::move(right), op_type.type(), position());
        }

        else
        {
            break;
        }

        op_type = current_token();
    }

    return left;
}

constexpr auto parser::parse(state::shift_expression) -> ast::UniqueAstNode
{
    // Accepted token types for shift expression.

    /* static */ constexpr auto k_expected = std::array
    {
        token_type::tok_left_shift,
        token_type::tok_right_shift,
        token_type::tok_zero_fill_right_shift
    };

    // Parse left hand side expression.

    auto left = transition(state::additive_expression{});

    // Get current token.

    auto op_type = current_token();

    while ( expect_one_of(k_expected, false, true) == true )
    {
        if ( auto right = transition(state::additive_expression{}); right.get() != nullptr )
        {
            left = ast::BinaryExpression::make(context(), std::move(left), std::move(right), op_type.type(), position());
        }

        else
        {
            break;
        }

        op_type = current_token();
    }

    return left;
}

constexpr auto parser::parse(state::equality_expression) -> ast::UniqueAstNode
{
    // Accepted token types for an equality expression.

    /* static */ constexpr auto k_expected = std::array
    {
        token_type::tok_equal,
        token_type::tok_not_equal,
        token_type::tok_strict_equal,
        token_type::tok_strict_not_equal,
        token_type::tok_greater_than,
        token_type::tok_greater_than_or_equal,
        token_type::tok_less_than,
        token_type::tok_less_than_or_equal,
        token_type::tok_instanceof,
    };

    // Parse left hand side expression.

    auto left = transition(state::shift_expression{});

    // Get current token.

    auto op_type = current_token();

    while ( expect_one_of(k_expected, false, true) == true )
    {
        if ( auto right = transition(state::shift_expression{}); right.get() != nullptr )
        {
            left = ast::BinaryExpression::make(context(), std::move(left), std::move(right), op_type.type(), position());
        }

        else
        {
            break;
        }

        op_type = current_token();
    }

    return left;
}

constexpr auto parser::parse(state::logical_expression) -> ast::UniqueAstNode
{
    // Accepted token types for a logical expression.

    /* static */ constexpr auto k_expected = std::array
    {
        token_type::tok_logical_and,
        token_type::tok_logical_or,
        token_type::tok_bitwise_and,
        token_type::tok_bitwise_or,
        token_type::tok_bitwise_xor
    };

    // Parse left hand side expression.

    auto left = transition(state::equality_expression{});

    // Get current token.

    auto op_type = current_token();

    while ( expect_one_of(k_expected, false, true) == true )
    {
        if ( auto right = transition(state::equality_expression{}); right.get() != nullptr )
        {
            left = ast::BinaryExpression::make(context(), std::move(left), std::move(right), op_type.type(), position());
        }

        else
        {
            break;
        }

        op_type = current_token();
    }

    return left;
}

// <TernaryExpression> ::
//  <Expression> '?' <Expression> ':' <Expression>

constexpr auto parser::parse(state::ternary_expression, ast::UniqueAstNode condition) -> ast::UniqueAstNode
{
    if ( expect(token_type::tok_ternary, false, true) == false )
    {
        return condition;
    }

    // Parse left hand side expression.

    auto consequent = transition(state::expression{});

    // Expect a colon ':'.

    if ( expect(token_type::tok_colon, true, true) == false )
    {
        return {};
    }

    // Parse right hand side expression.

    auto alternate = transition(state::expression{});

    return ast::TernaryExpression::make(context(), position(), std::move(condition), std::move(consequent), std::move(alternate));
}

constexpr auto parser::parse(state::expression) -> ast::UniqueAstNode
{
    // Parse left hand side expression.

    auto left = transition(state::logical_expression{});

    // Check for a ternary expression '?'.

    if ( expect(token_type::tok_ternary, false, false) == true )
    {
        return transition(state::ternary_expression{}, std::move(left));
    }

    // Accepted token types for an assignment expression.

    /* static */ constexpr auto k_expected = std::array
    {
        token_type::tok_assignment,
        token_type::tok_assignment_plus,
        token_type::tok_assignment_minus,
        token_type::tok_assignment_multiply,
        token_type::tok_assignment_exponential,
        token_type::tok_assignment_divide,
        token_type::tok_assignment_modulo,
        token_type::tok_assignment_left_shift,
        token_type::tok_assignment_right_shift,
    };

    // Get current token.

    const auto op_type = current_token();

    // Try parse an assignment expression.

    if ( expect_one_of(k_expected, false, true) == true )
    {
        auto right = transition(state::expression{});

        return ast::BinaryExpression::make(context(), std::move(left), std::move(right), op_type.type(), position());
    }

    return left;
}

} // namespace acme
