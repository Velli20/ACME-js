
#include "operator_binary.hpp"
#include "operator_constant.hpp"
#include "operator_push.hpp"
#include "operator_stack.hpp"
#include "operator_unary.hpp"
#include "operator_var.hpp"

namespace acme {

namespace {

constexpr auto run_op(
    virtual_machine&  vm,
    acme::instruction ins
)
{
    auto op = operand(ins);
    switch ( op )
    {
        case opcode::binary_add:
            binary_op<opcode::binary_add>(vm);
            break;

        case opcode::binary_sub:
            binary_op<opcode::binary_sub>(vm);
            break;

        case opcode::binary_mul:
            binary_op<opcode::binary_mul>(vm);
            break;

        case opcode::binary_div:
            binary_op<opcode::binary_div>(vm);
            break;

        case opcode::binary_mod:
            binary_op<opcode::binary_mod>(vm);
            break;

        case opcode::binary_pow:
            binary_op<opcode::binary_pow>(vm);
            break;

        case opcode::compare_equal:
            binary_op<opcode::compare_equal>(vm);
            break;

        case opcode::compare_less_than:
            binary_op<opcode::compare_less_than>(vm);
            break;

        case opcode::compare_greater_than:
            binary_op<opcode::compare_greater_than>(vm);
            break;

        case opcode::compare_instanceof:
            binary_op<opcode::compare_instanceof>(vm);
            break;

        case opcode::compare_strict_equal:
            binary_op<opcode::compare_strict_equal>(vm);
            break;

        case opcode::compare_not:
            unary_op<opcode::compare_not>(vm);
            break;

        case opcode::typeof_value:
            unary_op<opcode::typeof_value>(vm);
            break;

        case opcode::unary_delete:
            unary_op<opcode::unary_delete>(vm);
            break;

        case opcode::unary_negate:
            unary_op<opcode::unary_negate>(vm);
            break;

        case opcode::load_var:
            var_op<opcode::load_var>(vm);
            break;

        case opcode::store_var:
            var_op<opcode::store_var>(vm);
            break;

        case opcode::initialize:
            var_op<opcode::initialize>(vm);
            break;

        case opcode::constant_double:
            constant_op<opcode::constant_double>(vm);
            break;

        case opcode::constant_i32:
            constant_op<opcode::constant_i32>(vm);
            break;

        case opcode::constant_u32:
            constant_op<opcode::constant_u32>(vm);
            break;

        case opcode::constant_identifier:
            constant_op<opcode::constant_identifier>(vm);
            break;

        case opcode::constant_string:
            constant_op<opcode::constant_string>(vm);
            break;

        case opcode::push_bool_true:
            push_op<opcode::push_bool_true>(vm);
            break;

        case opcode::push_bool_false:
            push_op<opcode::push_bool_false>(vm);
            break;

        case opcode::push_null:
            push_op<opcode::push_null>(vm);
            break;

        case opcode::push_undefined:
            push_op<opcode::push_undefined>(vm);
            break;

        case opcode::duplicate_top:
            stack_op<opcode::duplicate_top>(vm);
            break;

        case opcode::push_stack_frame:
            stack_op<opcode::push_stack_frame>(vm);
            break;

        case opcode::pop_stack_frame:
            stack_op<opcode::pop_stack_frame>(vm);
            break;

        case opcode::jump_if_false:
            stack_op<opcode::jump_if_false>(vm);
            break;

        case opcode::jump_if_true:
            stack_op<opcode::jump_if_true>(vm);
            break;

        case opcode::jump_to:
            stack_op<opcode::jump_to>(vm);
            break;

        case opcode::no_opearation:
            break;
    }
}

} // namespace

auto virtual_machine::execute(const bytecode& code)
{
    m_bytecode = code;

    push_scope();

    while ( true )
    {
        if ( const auto ins = load_instruction(); ins.has_value() )
        {
            run_op(*this, ins.value());
        }

        else
        {
            break;
        }
    }

    //pop_scope();
}

} // namespace acme
