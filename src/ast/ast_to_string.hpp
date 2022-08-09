#pragma once

namespace acme::ast {

/*constexpr*/ inline auto to_string(const UniqueAstNode& value) -> std::string_view
{
    using namespace std::string_view_literals;

    if ( value.get() == nullptr )
    {
        return "nullptr"sv;
    }

    switch ( value.get()->type() )
    {
        case rtti::type_index<Statement>():
            return "Statement"sv;

        case rtti::type_index<Expression>():
            return "Expression"sv;

        case rtti::type_index<Literal>():
            return "Literal"sv;

        case rtti::type_index<Identifier>():
            return "Identifier"sv;

        case rtti::type_index<ObjectProperty>():
            return "ObjectProperty"sv;

        case rtti::type_index<ObjectLiteral>():
            return "ObjectLiteral"sv;

        case rtti::type_index<ThisExpression>():
            return "ThisExpression"sv;

        case rtti::type_index<TernaryExpression>():
            return "TernaryExpression"sv;

        case rtti::type_index<BlockStatement>():
            return "BlockStatement"sv;

        case rtti::type_index<MemberExpression>():
            return "MemberExpression"sv;

        case rtti::type_index<BinaryExpression>():
            return "BinaryExpression"sv;

        case rtti::type_index<UnaryExpression>():
            return "UnaryExpression"sv;

        case rtti::type_index<FunctionExpression>():
            return "FunctionExpression"sv;

        case rtti::type_index<IfStatement>():
            return "IfStatement"sv;

        case rtti::type_index<LoopStatement>():
        {
            switch ( value.get()->deref<ast::LoopStatement>().kind() )
            {
                case loop_kind::k_for_loop:
                    return "ForLoopStatement"sv;

                case loop_kind::k_while_loop:
                    return "WhileLoopStatement"sv;

                case loop_kind::k_do_while_loop:
                    return "WhileLoopStatement"sv;
            }
        }

        case rtti::type_index<SimpleStatement>():
        {
            switch ( value.get()->deref<ast::SimpleStatement>().kind() )
            {
                case simple_statement_kind::k_return_statement:
                    return "ReturnStatement"sv;

                case simple_statement_kind::k_break_statement:
                    return "BreakStatement"sv;

                case simple_statement_kind::k_continue_statement:
                    return "ContinueStatement"sv;

                case simple_statement_kind::k_debugger_statement:
                    return "DebuggerStatement"sv;

                case simple_statement_kind::k_label_statement:
                    return "LabelledStatement"sv;
            }
        }

        case rtti::type_index<VariableDeclaration>():
            return "VariableDeclaration"sv;

        case rtti::type_index<AstNodeList>():
            return "AstNodeList"sv;
    }

    return "Unknown"sv;
}

} // namespace acme::ast
