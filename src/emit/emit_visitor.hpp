#pragma once

#include <iostream>

namespace acme::eval {

auto emit(const ast::UniqueAstNode& p, emit_context& context) -> acme::script_value;

static constexpr struct
{
    auto operator()(const ast::Identifier& v, emit_context& context) -> acme::script_value
    {
        context.emit(v);
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
            eval::emit(id, context);
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
            if ( ast::instanceof<ast::Identifier>(v.right()) )
            {
                context.emit_instruction(opcode::load_var);
            }

            eval::emit(v.left(), context);
            if ( ast::instanceof<ast::Identifier>(v.left()) )
            {
                context.emit_instruction(opcode::load_var);
            }
        }

        else
        {
            eval::emit(v.left(), context);
            if ( ast::instanceof<ast::Identifier>(v.left()) )
            {
                context.emit_instruction(opcode::load_var);
            }

            eval::emit(v.right(), context);
            if ( ast::instanceof<ast::Identifier>(v.right()) )
            {
                context.emit_instruction(opcode::load_var);
            }
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
            case token_type::tok_less_than:
                context.emit_instruction(opcode::compare_less_than);
                break;

            case token_type::tok_greater_than_or_equal:
                break;

                break;

            case token_type::tok_less_than_or_equal:
                break;

            default:
                break;
        }

        if ( is_assignment_op )
        {
            eval::emit(v.left(), context);
            context.emit_instruction(opcode::store_var);
        }

        return {};
    }

    auto operator()(const ast::UnaryExpression& v, emit_context& context) -> acme::script_value
    {
        eval::emit(v.expression(), context);
        if ( ast::instanceof<ast::Identifier>(v.expression()) )
        {
            context.emit_instruction(opcode::load_var);
        }

        switch ( v.operand() )
        {
            case token_type::tok_typeof:
                context.emit_instruction(opcode::typeof_value);
                break;

            case token_type::tok_delete:
                context.emit_instruction(opcode::unary_delete);
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

    auto operator()(const ast::ObjectExpression& v, emit_context& context) -> acme::script_value
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
        if ( const auto& condition = v.condition(); condition.get() != nullptr )
        {
            eval::emit(condition, context);
        }

        // Duplicate the result of the test condition for the alternate condition test.

        context.emit_instruction(opcode::duplicate_top);

        const auto index_if_false = context.emit_instruction(opcode::jump_if_false);

        if ( const auto& consequent = v.consequent(); consequent.get() != nullptr )
        {
            eval::emit(consequent, context);
        }

        immediate(context.at(index_if_false), context.count() + 1);
        const auto index_if_true  = context.emit_instruction(opcode::jump_if_true);

        if ( const auto& alternate = v.alternate(); alternate.get() != nullptr )
        {
            eval::emit(alternate, context);
        }

        context.emit_instruction(opcode::no_opearation);
        immediate(context.at(index_if_true), context.count());

        return {};
    }

    auto operator()(const ast::TernaryExpression& v, emit_context& context) -> acme::script_value
    {
        return {};
    }

    auto operator()(const ast::ForLoopStatement& v, emit_context& context) -> acme::script_value
    {
        if ( const auto& initializer = v.initializer(); initializer.get() != nullptr )
        {
            eval::emit(initializer, context);
        }

        auto test_condition = context.count() + 1;
        if ( const auto& condition = v.condition(); condition.get() != nullptr )
        {
            eval::emit(condition, context);
        }

        const auto index_if_false = context.emit_instruction(opcode::jump_if_false);

        if ( const auto& update = v.update(); update.get() != nullptr )
        {
            eval::emit(update, context);
        }

        if ( const auto& body = v.body(); body.get() != nullptr )
        {
            eval::emit(body, context);
        }

        auto repeat = context.emit_instruction(opcode::jump_to);
        immediate(context.at(repeat), test_condition);

        context.emit_instruction(opcode::no_opearation);
        immediate(context.at(index_if_false), context.count());

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
