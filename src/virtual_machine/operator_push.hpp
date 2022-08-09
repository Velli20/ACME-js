#pragma once

namespace acme {

template <opcode k_op>
void push_op(virtual_machine& vm)
{
    auto result = [&]() constexpr -> acme::script_value
    {
        // Push the boolean value true on the stack.

        if constexpr ( k_op == opcode::push_bool_true )
        {
            return acme::script_value{acme::boolean{true}};
        }

        // Push the boolean value false on the stack.

        else if constexpr ( k_op == opcode::push_bool_false )
        {
            return acme::script_value{acme::boolean{false}};
        }

        // Push null value on the stack.

        else if constexpr ( k_op == opcode::push_null )
        {
            return acme::script_value{std::nullptr_t{}};
        }

        // Push undefined value on the stack.

        else if constexpr ( k_op == opcode::push_undefined )
        {
            return acme::script_value{acme::undefined{}};
        }
    }();

    vm.stack().push_back(result);
}

} // namespace acme
