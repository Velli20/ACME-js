#pragma once

namespace acme {

template<opcode k_op>
void var_op(virtual_machine& vm)
{
    if constexpr ( k_op == opcode::store_var )
    {
        auto id    = vm.stack().pop_back();
        auto value = vm.stack().pop_back();

        if ( auto var = vm.get_var(id.as<acme::identifier>()); var.has_value() )
        {
            auto& ref = var.value().get();
            ref.assign(value);
        }
    }

    else if constexpr ( k_op == opcode::load_var )
    {
        auto id = vm.stack().pop_back();

        if ( auto var = vm.get_var(id.as<acme::identifier>()); var.has_value() )
        {
            auto ref = var.value().get();
            vm.stack().push_back(ref);
        }
    }

    else if constexpr ( k_op == opcode::initialize )
    {
        auto value = vm.stack().pop_back();
        auto id    = vm.stack().pop_back();

        vm.locals().push(id.as<acme::identifier>(), value);
    }
}

} // namespace acme
