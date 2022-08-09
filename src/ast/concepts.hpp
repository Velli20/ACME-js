#pragma once

namespace acme::concepts {


template <typename T>
concept ast_literal = std::is_same_v<T, ast::Float>           ||
                      std::is_same_v<T, ast::String>          ||
                      std::is_same_v<T, ast::UnsignedInteger> ||
                      std::is_same_v<T, ast::Integer>         ||
                      std::is_same_v<T, ast::Boolean>         ||
                      std::is_same_v<T, ast::Null>;

template <typename Func, typename... Args>
concept ast_visitor = requires(Func target_functor, Args&&... args)
{
    requires requires(const ast::Identifier& identifier)
    {
        std::invoke(target_functor, identifier, std::forward<decltype(args)>(args)...);
    };

    requires requires(const ast::DeclarationKind& decl_kind)
    {
        std::invoke(target_functor, decl_kind, std::forward<decltype(args)>(args)...);
    };

    requires requires(const ast::Literal& lit)
    {
        std::invoke(target_functor, lit, std::forward<decltype(args)>(args)...);
    };

    requires requires(const ast::ArrayLiteral& lit)
    {
        std::invoke(target_functor, lit, std::forward<decltype(args)>(args)...);
    };

    requires requires(const ast::FunctionDeclaration& function_declaration)
    {
        std::invoke(target_functor, function_declaration, std::forward<decltype(args)>(args)...);
    };

    requires requires(const ast::FunctionExpression& function_expression)
    {
        std::invoke(target_functor, function_expression, std::forward<decltype(args)>(args)...);
    };

    requires requires(const ast::VariableDeclaration& variable_declaration)
    {
        std::invoke(target_functor, variable_declaration, std::forward<decltype(args)>(args)...);
    };

    requires requires(const ast::BinaryExpression& binary_expression)
    {
        std::invoke(target_functor, binary_expression, std::forward<decltype(args)>(args)...);
    };

    requires requires(const ast::UnaryExpression& unary_expression)
    {
        std::invoke(target_functor, unary_expression, std::forward<decltype(args)>(args)...);
    };

    requires requires(const ast::BlockStatement& block_statement)
    {
        std::invoke(target_functor, block_statement, std::forward<decltype(args)>(args)...);
    };

    requires requires(const ast::MemberExpression& member_expression)
    {
        std::invoke(target_functor, member_expression, std::forward<decltype(args)>(args)...);
    };

    requires requires(const ast::AstNodeList& node_list)
    {
        std::invoke(target_functor, node_list, std::forward<decltype(args)>(args)...);
    };

    requires requires(const ast::ObjectLiteral& object_expression)
    {
        std::invoke(target_functor, object_expression, std::forward<decltype(args)>(args)...);
    };

    requires requires(const ast::ObjectPropertySetter& object_property_setter)
    {
        std::invoke(target_functor, object_property_setter, std::forward<decltype(args)>(args)...);
    };

    requires requires(const ast::ObjectPropertyGetter& object_property_getter)
    {
        std::invoke(target_functor, object_property_getter, std::forward<decltype(args)>(args)...);
    };

    requires requires(const ast::ObjectProperty& object_property)
    {
        std::invoke(target_functor, object_property, std::forward<decltype(args)>(args)...);
    };

    requires requires(const ast::ThisExpression& this_expression)
    {
        std::invoke(target_functor, this_expression, std::forward<decltype(args)>(args)...);
    };

    requires requires(const ast::IfStatement& if_statement)
    {
        std::invoke(target_functor, if_statement, std::forward<decltype(args)>(args)...);
    };

    requires requires(const ast::TernaryExpression& ternary_expression)
    {
        std::invoke(target_functor, ternary_expression, std::forward<decltype(args)>(args)...);
    };

    requires requires(const ast::LoopStatement& for_loop_statement)
    {
        std::invoke(target_functor, for_loop_statement, std::forward<decltype(args)>(args)...);
    };

    requires requires(const ast::SimpleStatement& simple_statement)
    {
        std::invoke(target_functor, simple_statement, std::forward<decltype(args)>(args)...);
    };

    requires requires(const ast::CallExpression& call_expression)
    {
        std::invoke(target_functor, call_expression, std::forward<decltype(args)>(args)...);
    };

    requires requires(const ast::NewExpression& new_expression)
    {
        std::invoke(target_functor, new_expression, std::forward<decltype(args)>(args)...);
    };

    requires requires(const ast::MetaProperty& meta_property)
    {
        std::invoke(target_functor, meta_property, std::forward<decltype(args)>(args)...);
    };
};

} // namespace acme::concepts
