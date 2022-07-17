#pragma once

namespace acme {

enum class transition_state
{
    none,
    arrow_function_expression,
    arrow_function,
    block_statement,
    variable_statement,
    if_statement,
    for_statement,
    for_in_statement,
    do_statement,
    while_statement,
    function_statement,
    function_declaration,
    function_expression,
    continue_statement,
    break_statement,
    switch_statement,
    try_statement,
    debugger_statement,
    return_statement,
    literal_expression,
    single_quoted_literal,
    double_quoted_literal,
    array_literal,
    variable_declaration,
    expression,
    unary_expression,
    comment,
    multiline_comment,
    statement,
    labelled_statement,
    empty_statement,
    throw_statement,
    with_statement,
    this_expression,
    ternary_expression,
    member_expression,
    postfix_expression,
    object_declaration,

    logical_expression,
    equality_expression,
    shift_expression,
    additive_expression,
    multiplicative_expression,
    primary_expression,
    binary_expression,
    object_property,
    parameter_list,
    new_expression

};

namespace state {

using arrow_function            = typename std::integral_constant<acme::transition_state, acme::transition_state::arrow_function>;
using arrow_function_expression = typename std::integral_constant<acme::transition_state, acme::transition_state::arrow_function_expression>;
using block_statement           = typename std::integral_constant<acme::transition_state, acme::transition_state::block_statement>;
using variable_statement        = typename std::integral_constant<acme::transition_state, acme::transition_state::variable_statement>;
using if_statement              = typename std::integral_constant<acme::transition_state, acme::transition_state::if_statement>;
using for_statement             = typename std::integral_constant<acme::transition_state, acme::transition_state::for_statement>;
using for_in_statement          = typename std::integral_constant<acme::transition_state, acme::transition_state::for_in_statement>;
using do_statement              = typename std::integral_constant<acme::transition_state, acme::transition_state::do_statement>;
using while_statement           = typename std::integral_constant<acme::transition_state, acme::transition_state::while_statement>;
using function_statement        = typename std::integral_constant<acme::transition_state, acme::transition_state::function_statement>;
using function_declaration      = typename std::integral_constant<acme::transition_state, acme::transition_state::function_declaration>;
using function_expression       = typename std::integral_constant<acme::transition_state, acme::transition_state::function_expression>;
using continue_statement        = typename std::integral_constant<acme::transition_state, acme::transition_state::continue_statement>;
using break_statement           = typename std::integral_constant<acme::transition_state, acme::transition_state::break_statement>;
using switch_statement          = typename std::integral_constant<acme::transition_state, acme::transition_state::switch_statement>;
using try_statement             = typename std::integral_constant<acme::transition_state, acme::transition_state::try_statement>;
using debugger_statement        = typename std::integral_constant<acme::transition_state, acme::transition_state::debugger_statement>;
using return_statement          = typename std::integral_constant<acme::transition_state, acme::transition_state::return_statement>;
using literal_expression        = typename std::integral_constant<acme::transition_state, acme::transition_state::literal_expression>;
using single_quoted_literal     = typename std::integral_constant<acme::transition_state, acme::transition_state::single_quoted_literal>;
using double_quoted_literal     = typename std::integral_constant<acme::transition_state, acme::transition_state::double_quoted_literal>;
using array_literal             = typename std::integral_constant<acme::transition_state, acme::transition_state::array_literal>;
using variable_declaration      = typename std::integral_constant<acme::transition_state, acme::transition_state::variable_declaration>;
using expression                = typename std::integral_constant<acme::transition_state, acme::transition_state::expression>;
using unary_expression          = typename std::integral_constant<acme::transition_state, acme::transition_state::unary_expression>;
using statement                 = typename std::integral_constant<acme::transition_state, acme::transition_state::statement>;
using labelled_statement        = typename std::integral_constant<acme::transition_state, acme::transition_state::labelled_statement>;
using empty_statement           = typename std::integral_constant<acme::transition_state, acme::transition_state::empty_statement>;
using throw_statement           = typename std::integral_constant<acme::transition_state, acme::transition_state::throw_statement>;
using with_statement            = typename std::integral_constant<acme::transition_state, acme::transition_state::with_statement>;
using member_expression         = typename std::integral_constant<acme::transition_state, acme::transition_state::member_expression>;
using this_expression           = typename std::integral_constant<acme::transition_state, acme::transition_state::this_expression>;
using ternary_expression        = typename std::integral_constant<acme::transition_state, acme::transition_state::ternary_expression>;
using postfix_expression        = typename std::integral_constant<acme::transition_state, acme::transition_state::postfix_expression>;
using object_declaration        = typename std::integral_constant<acme::transition_state, acme::transition_state::object_declaration>;


using logical_expression        = typename std::integral_constant<acme::transition_state, acme::transition_state::logical_expression>;
using equality_expression       = typename std::integral_constant<acme::transition_state, acme::transition_state::equality_expression>;
using shift_expression          = typename std::integral_constant<acme::transition_state, acme::transition_state::shift_expression>;
using additive_expression       = typename std::integral_constant<acme::transition_state, acme::transition_state::additive_expression>;
using multiplicative_expression = typename std::integral_constant<acme::transition_state, acme::transition_state::multiplicative_expression>;
using primary_expression        = typename std::integral_constant<acme::transition_state, acme::transition_state::primary_expression>;
using binary_expression         = typename std::integral_constant<acme::transition_state, acme::transition_state::binary_expression>;
using object_property           = typename std::integral_constant<acme::transition_state, acme::transition_state::object_property>;
using parameter_list            = typename std::integral_constant<acme::transition_state, acme::transition_state::parameter_list>;
using new_expression            = typename std::integral_constant<acme::transition_state, acme::transition_state::new_expression>;

} // namespace state

using fsm_type = std::variant
<
  state::arrow_function,
  state::arrow_function_expression,
  state::statement,
  state::block_statement,
  state::variable_statement,
  state::if_statement,
  state::for_statement,
  state::for_in_statement,
  state::do_statement,
  state::while_statement,
  state::function_statement,
  state::function_declaration,
  state::continue_statement,
  state::break_statement,
  state::switch_statement,
  state::try_statement,
  state::debugger_statement,
  state::return_statement,
  state::literal_expression,
  state::single_quoted_literal,
  state::double_quoted_literal,
  state::array_literal,
  state::variable_declaration,
  state::expression,
  state::unary_expression,
  state::with_statement,
  state::empty_statement,
  state::member_expression,
  state::postfix_expression,
  state::this_expression,
  state::ternary_expression,
  state::primary_expression,
  state::binary_expression,

  state::logical_expression,
  state::equality_expression,
  state::shift_expression,
  state::additive_expression,
  state::multiplicative_expression,
  state::object_property,
  state::function_expression,
  state::parameter_list,
  state::new_expression
>;

} // namespace acme
