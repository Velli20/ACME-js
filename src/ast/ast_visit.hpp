

namespace acme::ast {

auto visit(concepts::ast_visitor auto callback, const UniqueAstNode& p)
{
    using return_type = typename std::invoke_result<decltype(callback), ast::Literal>::type;

    if ( ast::instanceof<ast::Literal>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::Literal>());
    }

    else if ( ast::instanceof<ast::ArrayLiteral>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::ArrayLiteral>());
    }

    else if ( ast::instanceof<ast::Identifier>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::Identifier>());
    }

    else if ( ast::instanceof<ast::BinaryExpression>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::BinaryExpression>());
    }

    else if ( ast::instanceof<ast::VariableDeclaration>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::VariableDeclaration>());
    }

    else if ( ast::instanceof<ast::IfStatement>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::IfStatement>());
    }

    else if ( ast::instanceof<ast::BlockStatement>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::BlockStatement>());
    }

    else if ( ast::instanceof<ast::UnaryExpression>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::UnaryExpression>());
    }

    else if ( ast::instanceof<ast::LoopStatement>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::LoopStatement>());
    }

    else if ( ast::instanceof<ast::ThisExpression>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::ThisExpression>());
    }

    else if ( ast::instanceof<ast::ReturnStatement>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::ReturnStatement>());
    }

    else if ( ast::instanceof<ast::BreakStatement>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::BreakStatement>());
    }

    else if ( ast::instanceof<ast::LabelledStatement>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::LabelledStatement>());
    }

    else if ( ast::instanceof<ast::ContinueStatement>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::ContinueStatement>());
    }

    else if ( ast::instanceof<ast::MemberExpression>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::MemberExpression>());
    }

    else if ( ast::instanceof<ast::TernaryExpression>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::TernaryExpression>());
    }

    else if ( ast::instanceof<ast::ObjectLiteral>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::ObjectLiteral>());
    }

    else if ( ast::instanceof<ast::ObjectProperty>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::ObjectProperty>());
    }

    else if ( ast::instanceof<ast::ObjectPropertyGetter>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::ObjectPropertyGetter>());
    }

    else if ( ast::instanceof<ast::ObjectPropertySetter>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::ObjectPropertySetter>());
    }

    else if ( ast::instanceof<ast::AstNodeList>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::AstNodeList>());
    }

    else if ( ast::instanceof<ast::FunctionDeclaration>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::FunctionDeclaration>());
    }

    else if ( ast::instanceof<ast::FunctionExpression>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::FunctionExpression>());
    }

    else if ( ast::instanceof<ast::CallExpression>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::CallExpression>());
    }

    else if ( ast::instanceof<ast::NewExpression>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::NewExpression>());
    }

    else if ( ast::instanceof<ast::MetaProperty>(p) )
    {
        return std::invoke(callback, p.get()->deref<ast::MetaProperty>());
    }

    return return_type{};
}

} // namespace acme::ast
