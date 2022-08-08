#pragma once

namespace acme {

template<opcode k_op>
constexpr void binary_op(virtual_machine& vm)
{
    /* BinaryExpression:

        Stack on entry:
            [left]
            [right]

        Stack on exit
            [result]
    */

    auto left  = vm.stack().pop_back();
    auto right = vm.stack().pop_back();

    auto result = [&]() -> acme::script_value
    {
        // Binary '+' operator.

        if constexpr ( k_op == opcode::binary_add )
        {
            return op_add(vm, left, right);
        }

        // Binary '-' operator.

        else if constexpr ( k_op == opcode::binary_sub )
        {
            return op_sub(left, right);
        }

        // Binary '*' operator.

        else if constexpr ( k_op == opcode::binary_mul )
        {
            return op_mul(left, right);
        }

        // Binary '/' operator.

        else if constexpr ( k_op == opcode::binary_div )
        {
            return op_div(left, right);
        }

        // Binary '%' operator.

        else if constexpr ( k_op == opcode::binary_mod )
        {
            return op_mod(left, right);
        }

        // Binary '**' operator

        else if constexpr ( k_op == opcode::binary_pow )
        {
            return op_pow(left, right);
        }

        // Binary '===' operator.

        else if constexpr ( k_op == opcode::compare_strict_equal )
        {
            return op_strict_equal(vm, left, right);
        }

        // Binary '==' operator.

        else if constexpr ( k_op == opcode::compare_equal )
        {
            return op_equal(vm, left, right);
        }

        // Binary '<' operator.

        else if constexpr ( k_op == opcode::compare_less_than )
        {
            return op_less_than(left, right);
        }

        // Binary '>' operator.

        else if constexpr ( k_op == opcode::compare_greater_than )
        {
            return op_greater_than(left, right);
        }

        // Binary 'instanceof' operator.

        else if constexpr ( k_op == opcode::compare_instanceof )
        {
            return op_instanceof(left, right);
        }

    }();

    vm.stack().push_back(result);
}

} // namespace acme
