#pragma once

namespace acme {

template<opcode k_op>
void stack_op(virtual_machine& vm)
{
    if constexpr ( k_op == opcode::jump_if_false || k_op == opcode::jump_if_true )
    {
        constexpr auto k_jump_cond = (k_op == opcode::jump_if_false ? false : true);

        const auto offset = vm.current_immediate();
        auto cond         = vm.stack().pop_back();

        if ( to_boolean(cond) == k_jump_cond )
        {
            vm.jump_to(offset);
        }
    }

    if constexpr ( k_op == opcode::jump_to )
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
        vm.pop_scope();
    }

    else if constexpr ( k_op == opcode::duplicate_top )
    {
        auto top = vm.stack().top();
        vm.stack().push_back(top);
    }
}

} // namespace acme
