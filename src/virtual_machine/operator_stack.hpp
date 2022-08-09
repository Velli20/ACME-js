#pragma once

namespace acme {

template<opcode k_op>
void stack_op(virtual_machine& vm)
{
    // Jump if the value on top of the stack is true.

    if constexpr ( k_op == opcode::jump_if_true )
    {
        const auto offset = vm.current_immediate();
        auto cond         = vm.stack().pop_back();

        if ( to_boolean(cond) == true )
        {
            vm.jump_to(offset);
        }
    }

    // Jump if the value on top of the stack is false.

    else if constexpr ( k_op == opcode::jump_if_false  )
    {
        const auto offset = vm.current_immediate();
        auto cond         = vm.stack().pop_back();

        if ( to_boolean(cond) == false )
        {
            vm.jump_to(offset);
        }
    }

    // Jump from the current position to another position in the code, determined by a offset.

    else if constexpr ( k_op == opcode::jump_to )
    {
        const auto offset = vm.current_immediate();
        vm.jump_to(offset);
    }

    else if constexpr ( k_op == opcode::push_stack_frame )
    {
        vm.push_scope();
    }

    else if constexpr ( k_op == opcode::pop_stack_frame )
    {
        for ( auto count = vm.current_immediate(); count != 0; --count )
        {
            vm.pop_scope();
        }
    }

    else if constexpr ( k_op == opcode::duplicate_top )
    {
        auto top = vm.stack().top();
        vm.stack().push_back(top);
    }
}

} // namespace acme
