#pragma once

namespace acme::concepts {

template <typename Func>
concept emit_visitor = requires(Func target_functor, emit_context& context)
{
    requires requires(const ast::Identifier& identifier)
    {
        std::invoke(target_functor, identifier, context);
    };

    requires requires(const ast::DeclarationKind& decl_kind)
    {
        std::invoke(target_functor, decl_kind, context);
    };

    requires requires(const ast::Literal& lit)
    {
        std::invoke(target_functor, lit, context);
    };

    requires requires(const ast::ArrayLiteral& lit)
    {
        std::invoke(target_functor, lit, context);
    };

    requires requires(const ast::FunctionDeclaration& function_declaration)
    {
        std::invoke(target_functor, function_declaration, context);
    };

    requires requires(const ast::FunctionExpression& function_expression)
    {
        std::invoke(target_functor, function_expression, context);
    };

    requires requires(const ast::VariableDeclaration& variable_declaration)
    {
        std::invoke(target_functor, variable_declaration, context);
    };

    requires requires(const ast::BinaryExpression& binary_expression)
    {
        std::invoke(target_functor, binary_expression, context);
    };

    requires requires(const ast::UnaryExpression& unary_expression)
    {
        std::invoke(target_functor, unary_expression, context);
    };

    requires requires(const ast::BlockStatement& block_statement)
    {
        std::invoke(target_functor, block_statement, context);
    };

    requires requires(const ast::MemberExpression& member_expression)
    {
        std::invoke(target_functor, member_expression, context);
    };

    requires requires(const ast::AstNodeList& list)
    {
        std::invoke(target_functor, list, context);
    };

    requires requires(const ast::ObjectExpression& object_expression)
    {
        std::invoke(target_functor, object_expression, context);
    };

    requires requires(const ast::ObjectPropertySetter& object_property_setter)
    {
        std::invoke(target_functor, object_property_setter, context);
    };

    requires requires(const ast::ObjectPropertyGetter& object_property_getter)
    {
        std::invoke(target_functor, object_property_getter, context);
    };

    requires requires(const ast::ObjectProperty& object_property)
    {
        std::invoke(target_functor, object_property, context);
    };

    requires requires(const ast::ThisExpression& this_expression)
    {
        std::invoke(target_functor, this_expression, context);
    };

    requires requires(const ast::IfStatement& if_statement)
    {
        std::invoke(target_functor, if_statement, context);
    };

    requires requires(const ast::TernaryExpression& ternary_expression)
    {
        std::invoke(target_functor, ternary_expression, context);
    };

    requires requires(const ast::ForLoopStatement& for_loop_statement)
    {
        std::invoke(target_functor, for_loop_statement, context);
    };

    requires requires(const ast::ReturnStatement& return_statement)
    {
        std::invoke(target_functor, return_statement, context);
    };

    requires requires(const ast::LabelledStatement& labelled_statement)
    {
        std::invoke(target_functor, labelled_statement, context);
    };

    requires requires(const ast::BreakStatement& break_statement)
    {
        std::invoke(target_functor, break_statement, context);
    };

    requires requires(const ast::ContinueStatement& continue_statement)
    {
        std::invoke(target_functor, continue_statement, context);
    };

    requires requires(const ast::CallExpression& call_expression)
    {
        std::invoke(target_functor, call_expression, context);
    };

    requires requires(const ast::NewExpression& new_expression)
    {
        std::invoke(target_functor, new_expression, context);
    };

    requires requires(const ast::MetaProperty& meta_property)
    {
        std::invoke(target_functor, meta_property, context);
    };
};

} // namespace acme::concepts
