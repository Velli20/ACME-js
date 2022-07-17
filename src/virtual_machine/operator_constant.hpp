#pragma once

namespace acme {

template<opcode k_op>
void constant_op(virtual_machine& vm)
{
    const auto offset = vm.current_immediate();

    if constexpr ( k_op == opcode::constant_double )
    {
        auto v = vm.constant<double>(offset);
        vm.stack().push_back(v);
    }

    else if constexpr ( k_op == opcode::constant_i32 )
    {
        auto v = vm.constant<std::int32_t>(offset);
        vm.stack().push_back(v);
    }

    else if constexpr ( k_op == opcode::constant_u32 )
    {
        auto v = vm.constant<std::uint32_t>(offset);
        vm.stack().push_back(v);
    }

    else if constexpr ( k_op == opcode::constant_identifier )
    {
        auto v = vm.constant<acme::identifier>(offset);
        vm.stack().push_back(v);
    }

    else if constexpr ( k_op == opcode::constant_string )
    {
        auto v = vm.constant<acme::string>(offset);
        vm.stack().push_back(v);
    }
}

} // namespace acme
