

namespace acme::eval {

auto visit(
    concepts::emit_visitor auto callback,
    const ast::UniqueAstNode&   p,
    emit_context&               context
)
{
    using return_type = acme::script_value;

    if ( ast::instanceof<ast::Literal>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::Literal>(), context);
    }

    else if ( ast::instanceof<ast::ArrayLiteral>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::ArrayLiteral>(), context);
    }

    else if ( ast::instanceof<ast::Identifier>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::Identifier>(), context);
    }

    else if ( ast::instanceof<ast::BinaryExpression>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::BinaryExpression>(), context);
    }

    else if ( ast::instanceof<ast::VariableDeclaration>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::VariableDeclaration>(), context);
    }

    else if ( ast::instanceof<ast::IfStatement>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::IfStatement>(), context);
    }

    else if ( ast::instanceof<ast::BlockStatement>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::BlockStatement>(), context);
    }

    else if ( ast::instanceof<ast::UnaryExpression>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::UnaryExpression>(), context);
    }

    else if ( ast::instanceof<ast::LoopStatement>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::LoopStatement>(), context);
    }

    else if ( ast::instanceof<ast::ThisExpression>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::ThisExpression>(), context);
    }

    else if ( ast::instanceof<ast::SimpleStatement>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::SimpleStatement>(), context);
    }

    else if ( ast::instanceof<ast::MemberExpression>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::MemberExpression>(), context);
    }

    else if ( ast::instanceof<ast::TernaryExpression>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::TernaryExpression>(), context);
    }

    else if ( ast::instanceof<ast::ObjectLiteral>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::ObjectLiteral>(), context);
    }

    else if ( ast::instanceof<ast::ObjectProperty>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::ObjectProperty>(), context);
    }

    else if ( ast::instanceof<ast::ObjectPropertyGetter>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::ObjectPropertyGetter>(), context);
    }

    else if ( ast::instanceof<ast::ObjectPropertySetter>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::ObjectPropertySetter>(), context);
    }

    else if ( ast::instanceof<ast::AstNodeList>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::AstNodeList>(), context);
    }

    else if ( ast::instanceof<ast::FunctionDeclaration>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::FunctionDeclaration>(), context);
    }

    else if ( ast::instanceof<ast::FunctionExpression>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::FunctionExpression>(), context);
    }

    else if ( ast::instanceof<ast::CallExpression>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::CallExpression>(), context);
    }

    else if ( ast::instanceof<ast::NewExpression>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::NewExpression>(), context);
    }

    else if ( ast::instanceof<ast::MetaProperty>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::MetaProperty>(), context);
    }

    return return_type{};
}

} // namespace acme::ast
