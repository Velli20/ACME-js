#pragma once

namespace acme {

template<opcode k_op>
void unary_op(virtual_machine& vm)
{
    auto right  = vm.stack().pop_back();
    auto result = [&]() -> acme::script_value
    {
        if constexpr ( k_op == opcode::compare_not )
        {
            return op_negate(right);
        }

        else if constexpr ( k_op == opcode::typeof_value )
        {
            return value_typeof_s(right);
        }

        else if constexpr ( k_op == opcode::unary_delete )
        {
            return acme::script_value{acme::boolean{true}};
        }

    }();

    vm.stack().push_back(result);
}

} // namespace acme
