#pragma once

#include <iostream>

namespace acme::eval {

auto emit(const ast::UniqueAstNode& p, emit_context& context) -> acme::script_value;

static constexpr struct
{
    auto operator()(const ast::Identifier& v, emit_context& context) -> acme::script_value
    {
        context.emit(v);
        if ( context.state() != emit_context::emit_state::k_variable_declaration )
        {
            context.emit_instruction(opcode::load_var);
        }

        return {};
    }

    auto operator()(const ast::DeclarationKind& v, emit_context& context) -> acme::script_value
    {
        return {};
    }

    auto operator()(const ast::ArrayLiteral& lit, emit_context& context) -> acme::script_value
    {
        return {};
    }

    auto operator()(const ast::Literal& lit, emit_context& context) -> acme::script_value
    {
        using namespace acme::ast;

        const auto& value = lit.value();

        if ( std::holds_alternative<String>(value) )
        {
            context.emit(std::get<String>(value));
        }

        if ( std::holds_alternative<Float>(value) )
        {
            context.emit(std::get<Float>(value));
        }

        if ( std::holds_alternative<UnsignedInteger>(value) )
        {
            context.emit(std::get<UnsignedInteger>(value));
        }

        if ( std::holds_alternative<Integer>(value) )
        {
            context.emit(std::get<Integer>(value));
        }

        if ( std::holds_alternative<Boolean>(value) )
        {
            context.emit(std::get<Boolean>(value));
        }

        if ( std::holds_alternative<Null>(value) )
        {
            context.emit(std::get<Null>(value));
        }

        return {};
    }

    template <typename T> requires(std::is_same<T, ast::FunctionDeclaration>::value or std::is_same<T, ast::FunctionExpression>::value)
    auto operator()(const T& v, emit_context& context) -> acme::script_value
    {
        return {};
    }

    auto operator()(const ast::VariableDeclaration& v, emit_context& context) -> acme::script_value
    {
        if ( auto& id = v.identifier(); id.get() != nullptr  )
        {
            context.state(emit_context::emit_state::k_variable_declaration);
            eval::emit(id, context);
            context.state(emit_context::emit_state::k_none);
        }

        if ( auto& init = v.initializer(); init.get() != nullptr )
        {
            eval::emit(init, context);
        }

        else
        {
            context.emit_instruction(opcode::push_undefined);
        }

        context.emit_instruction(opcode::initialize);

        return {};
    }

    auto operator()(const ast::BinaryExpression& v, emit_context& context) -> acme::script_value
    {
        const auto is_assignment_op = [&]()
        {
            static constexpr auto k_lut = std::to_array
            ({
                token_type::tok_assignment,
                token_type::tok_assignment_plus,
                token_type::tok_assignment_minus,
                token_type::tok_assignment_multiply,
                token_type::tok_assignment_divide,
                token_type::tok_assignment_modulo,
                token_type::tok_assignment_exponential
            });

            return std::find(k_lut.cbegin(), k_lut.end(), v.operand()) != k_lut.cend();
        }();

        if ( is_assignment_op == false )
        {
            eval::emit(v.right(), context);
            eval::emit(v.left(), context);
        }

        else
        {
            eval::emit(v.left(), context);
            eval::emit(v.right(), context);
        }

        switch ( v.operand() )
        {
            case token_type::tok_plus:
            case token_type::tok_assignment_plus:
                context.emit_instruction(opcode::binary_add);
                break;

            case token_type::tok_minus:
            case token_type::tok_assignment_minus:
                context.emit_instruction(opcode::binary_sub);
                break;

            case token_type::tok_multiply:
            case token_type::tok_assignment_multiply:
                context.emit_instruction(opcode::binary_mul);
                break;

            case token_type::tok_divide:
            case token_type::tok_assignment_divide:
                context.emit_instruction(opcode::binary_div);
                break;

            case token_type::tok_exponential:
            case token_type::tok_assignment_exponential:
                context.emit_instruction(opcode::binary_pow);
                break;

            case token_type::tok_modulo:
            case token_type::tok_assignment_modulo:
                context.emit_instruction(opcode::binary_mod);
                break;

            case token_type::tok_assignment:
                break;

            case token_type::tok_left_shift:
            case token_type::tok_assignment_left_shift:
                break;

            case token_type::tok_right_shift:
            case token_type::tok_assignment_right_shift:
                break;

            case token_type::tok_zero_fill_right_shift:
            case token_type::tok_assignment_zero_fill_right_shift:
                break;

            case token_type::tok_bitwise_and:
                break;

            case token_type::tok_bitwise_or:
                break;

            case token_type::tok_bitwise_xor:
                break;

            case token_type::tok_bitwise_not:
                break;

            case token_type::tok_instanceof:
                context.emit_instruction(opcode::compare_instanceof);
                break;

            case token_type::tok_equal:
                context.emit_instruction(opcode::compare_equal);
                break;

            case token_type::tok_strict_equal:
                context.emit_instruction(opcode::compare_strict_equal);
                break;

            case token_type::tok_not_equal:
                context.emit_instruction(opcode::compare_equal);
                context.emit_instruction(opcode::compare_not);
                break;

            case token_type::tok_strict_not_equal:
                context.emit_instruction(opcode::compare_strict_equal);
                context.emit_instruction(opcode::compare_not);
                break;

            case token_type::tok_greater_than:
                context.emit_instruction(opcode::compare_greater_than);
                break;

            case token_type::tok_less_than:
                context.emit_instruction(opcode::compare_less_than);
                break;

            case token_type::tok_greater_than_or_equal:
                break;

            case token_type::tok_less_than_or_equal:
                break;

            default:
                break;
        }

        if ( is_assignment_op )
        {
            context.state(emit_context::emit_state::k_variable_declaration);
            eval::emit(v.left(), context);
            context.state(emit_context::emit_state::k_none);

            context.emit_instruction(opcode::store_var);
        }

        return {};
    }

    auto operator()(const ast::UnaryExpression& v, emit_context& context) -> acme::script_value
    {
        eval::emit(v.expression(), context);

        switch ( v.operand() )
        {
            case token_type::tok_typeof:
                context.emit_instruction(opcode::typeof_value);
                break;

            case token_type::tok_delete:
                context.emit_instruction(opcode::unary_delete);
                break;

            case token_type::tok_minus:
                context.emit_instruction(opcode::unary_negate);
                break;

            default:
                break;
        }

        return {};
    }

    auto operator()(const ast::BlockStatement& v, emit_context& context) -> acme::script_value
    {
        if ( const auto& body = v.body(); body.get() != nullptr )
        {
            context.emit_instruction(opcode::push_stack_frame);

            eval::emit(body, context);

            context.emit_instruction(opcode::pop_stack_frame);
        }

        return {};
    }

    auto operator()(const ast::MemberExpression& v, emit_context& context) -> acme::script_value
    {
        return {};
    }

    auto operator()(const ast::AstNodeList& v, emit_context& context) -> acme::script_value
    {
        if ( const auto& list = v.nodes(); list.empty() == false )
        {
            for ( const auto& p : list )
            {
                eval::emit(p, context);
            }
        }

        return {};
    }

    auto operator()(const ast::ObjectLiteral& v, emit_context& context) -> acme::script_value
    {
        return {};
    }

    auto operator()(const ast::ObjectPropertySetter& v, emit_context& context) -> acme::script_value
    {
        return {};
    }

    auto operator()(const ast::ObjectPropertyGetter& v, emit_context& context) -> acme::script_value
    {
        return {};
    }

    auto operator()(const ast::ObjectProperty& v, emit_context& context) -> acme::script_value
    {
        return {};
    }

    auto operator()(const ast::ThisExpression& v, emit_context& context) -> acme::script_value
    {
        return {};
    }

    auto operator()(const ast::IfStatement& v, emit_context& context) -> acme::script_value
    {
        // Get condition expression.

        const auto& condition = v.condition();
        assert(condition.get() != nullptr);

        // Get 'true' branch.

        const auto& consequent = v.consequent();
        assert(consequent.get() != nullptr);

        // Get 'false' branch.

        const auto& alternate = v.alternate();

        // Emit condition code.

        eval::emit(condition, context);

        const auto index_if_false = context.emit_instruction(opcode::jump_if_false);

        // Emit 'true' condition branch.

        eval::emit(consequent, context);

        // Jump instruction to exit conditional code section if 'true' branch was executed.

        const auto index_if_true = context.emit_instruction(opcode::jump_to);

        // Patch jump target offset to skip 'true' branch code.

        immediate(context.at(index_if_false), context.count() + 1);

        // Emit 'false' branch code.

        if ( alternate.get() != nullptr )
        {
            eval::emit(alternate, context);
        }

        immediate(context.at(index_if_true), context.count() + 1);

        return {};
    }

    auto operator()(const ast::TernaryExpression& v, emit_context& context) -> acme::script_value
    {
        // Get condition expression.

        const auto& condition = v.condition();
        assert(condition.get() != nullptr);

        // Get 'true' condition expression.

        const auto& consequent = v.consequent();
        assert(consequent.get() != nullptr);

        // Get 'false' condition expression.

        const auto& alternate = v.alternate();
        assert(alternate.get() != nullptr);

        // Emit condition expression.

        eval::emit(condition, context);

        // Jump target to skip 'true' branch code.

        const auto index_if_false = context.emit_instruction(opcode::jump_if_false);

        // Emit 'true' condition expression.

        eval::emit(consequent, context);

        // Jump target to skip 'false' branch code.

        const auto index_if_true = context.emit_instruction(opcode::jump_to);

        // Patch jump target if condition failed.

        immediate(context.at(index_if_false), context.count() + 1);

        // Emit 'false' condition branch.

        eval::emit(alternate, context);

        // Patch 'true' condition end jump target.

        immediate(context.at(index_if_true), context.count() + 1);

        return {};
    }

    auto operator()(const ast::LoopStatement& v, emit_context& context) -> acme::script_value
    {
        // 'for' loop.

        if ( v.kind() == ast::loop_kind::k_for_loop )
        {
            // Emit initializer expression.

            if ( const auto& initializer = v.initializer(); initializer.get() != nullptr )
            {
                eval::emit(initializer, context);
            }

            // Emit loop test condition.

            auto test_condition = [&]()
            {
                if ( const auto& condition = v.condition(); condition.get() != nullptr )
                {
                    auto position = context.count() + 1;
                    eval::emit(condition, context);

                    return position;
                }

                return context.emit_instruction(opcode::push_bool_true);
            }();

            // Jump over the update expression and loop body if test condition is false.

            const auto index_if_false = context.emit_instruction(opcode::jump_if_false);

            // Emit update expression.

            if ( const auto& update = v.update(); update.get() != nullptr )
            {
                eval::emit(update, context);
            }

            // Emit loop body code.

            if ( const auto& body = v.body(); body.get() != nullptr )
            {
                eval::emit(body, context);
            }

            // Jump to test condition.

            context.emit_instruction(opcode::jump_to, test_condition);

            // Patch jump target if loop condition is false.

            immediate(context.at(index_if_false), context.count() + 1);
        }

        // 'while' loop.

        else if ( v.kind() == ast::loop_kind::k_while_loop )
        {
            // Emit loop test condition.

            auto test_condition = [&]()
            {
                if ( const auto& condition = v.condition(); condition.get() != nullptr )
                {
                    auto position = context.count() + 1;
                    eval::emit(condition, context);

                    return position;
                }

                return context.emit_instruction(opcode::push_bool_true);
            }();

            // Jump over the loop body if test condition is false.

            const auto index_if_false = context.emit_instruction(opcode::jump_if_false);

            // Emit loop body code.

            if ( const auto& body = v.body(); body.get() != nullptr )
            {
                eval::emit(body, context);
            }

            // Jump to test condition.

            context.emit_instruction(opcode::jump_to, test_condition);

            // Patch jump target if loop condition is false.

            immediate(context.at(index_if_false), context.count() + 1);
        }

        return {};
    }

    auto operator()(const ast::ReturnStatement& v, emit_context& context) -> acme::script_value
    {
        return {};
    }

    auto operator()(const ast::LabelledStatement& v, emit_context& context) -> acme::script_value
    {
        return {};
    }

    auto operator()(const ast::BreakStatement& v, emit_context& context) -> acme::script_value
    {
        return {};
    }

    auto operator()(const ast::ContinueStatement& v, emit_context& context) -> acme::script_value
    {
        return {};
    }

    auto operator()(const ast::CallExpression& v, emit_context& context) -> acme::script_value
    {
        return {};
    }

    auto operator()(const ast::NewExpression& v, emit_context& context) -> acme::script_value
    {
        return {};
    }

    auto operator()(const ast::MetaProperty& v, emit_context& context) -> acme::script_value
    {
        return {};
    }

} emit_visitor{};

} // namespace acme::eval
