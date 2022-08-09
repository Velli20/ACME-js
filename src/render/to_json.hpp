#pragma once

#if __has_include(<nlohmann/json.hpp>)

#include <nlohmann/json.hpp>

namespace acme::render {

auto to_json(const ast::UniqueAstNode& p) -> nlohmann::json;

static constexpr struct
{
    auto operator()(const ast::Identifier& v)
    {
        using namespace std::string_view_literals;

        nlohmann::json result{};

        if ( auto res = v.value(); res.empty() == false )
        {
            result["name"sv] = std::move(res);
        }

        return result;
    }

    auto operator()(const ast::DeclarationKind& v) -> std::string_view
    {
        using namespace std::string_view_literals;

        switch ( v.value() )
        {
            case token_type::tok_var:
                return "var"sv;

            case token_type::tok_let:
                return "let"sv;

            case token_type::tok_const:
                return "const"sv;

            default:
                break;
        }

        return {};
    }

    auto operator()(const ast::Literal& lit) -> nlohmann::json
    {
        using namespace std::string_view_literals;
        using namespace acme::ast;

        const auto& value = lit.value();

        nlohmann::json result{};

        if ( std::holds_alternative<String>(value) )
        {
            result["type"sv]  = "string"sv;
            result["value"sv] = std::get<String>(value).value().view();
        }

        if ( std::holds_alternative<Float>(value) )
        {
            result["type"sv]  = "float"sv;
            result["value"sv] = std::to_string(std::get<Float>(value).value());
        }

        if ( std::holds_alternative<UnsignedInteger>(value) )
        {
            result["type"sv]  = "uint"sv;
            result["value"sv] = std::to_string(std::get<UnsignedInteger>(value).value());
        }

        if ( std::holds_alternative<Integer>(value) )
        {
            result["type"sv]  = "int"sv;
            result["value"sv] = std::to_string(std::get<Integer>(value).value());
        }

        if ( std::holds_alternative<Boolean>(value) )
        {
            result["type"sv]  = "boolean"sv;
            result["value"sv] = std::get<Boolean>(value).value() ? "true"sv : "false"sv;
        }

        if ( std::holds_alternative<Null>(value) )
        {
            result["type"sv]  = "null"sv;
            result["value"sv] = "null"sv;
        }

        return result;
    }

    auto operator()(const ast::ArrayLiteral& lit) -> nlohmann::json
    {
        using namespace std::string_view_literals;

        nlohmann::json result{};

        if ( const auto& elements = lit.elements(); elements.get() != nullptr )
        {
            result["elements"sv] = render::to_json(elements);
        }

        return result;
    }

    auto operator()(const ast::FunctionDeclaration& v) -> nlohmann::json
    {
        using namespace std::string_view_literals;

        nlohmann::json result{};

        if ( auto res = render::to_json(v.identifier()); res.empty() == false )
        {
            result["identifier"sv] = std::move(res);
        }

        if ( auto res = render::to_json(v.parameters()); res.empty() == false )
        {
            result["parameters"sv] = std::move(res);
        }

        if ( auto res = render::to_json(v.body()); res.empty() == false )
        {
            result["body"sv] = std::move(res);
        }

        return result;
    }

    auto operator()(const ast::FunctionExpression& v) -> nlohmann::json
    {
        using namespace std::string_view_literals;

        nlohmann::json result{};

        if ( auto res = render::to_json(v.parameters()); res.empty() == false )
        {
            result["parameters"sv] = std::move(res);
        }

        if ( auto res = render::to_json(v.body()); res.empty() == false )
        {
            result["body"sv] = std::move(res);
        }

        return result;
    }

    auto operator()(const ast::VariableDeclaration& v) -> nlohmann::json
    {
        using namespace std::string_view_literals;

        nlohmann::json result{};

        if ( auto res = operator()(v.kind()); res.empty() == false )
        {
            result["kind"sv] = std::move(res);
        }

        if ( auto res = render::to_json(v.identifier()); res.empty() == false )
        {
            result["identifier"sv] = std::move(res);
        }

        if ( auto res = render::to_json(v.initializer()); res.empty() == false )
        {
            result["init"sv] = std::move(res);
        }

        return result;
    }

    auto operator()(const ast::BinaryExpression& v) -> nlohmann::json
    {
        using namespace std::string_view_literals;

        nlohmann::json result{};

        if ( auto op = v.operand(); op != token_type::tok_none )
        {
            result["operand"sv] = token_table::to_string(op);
        }

        if ( const auto& left = v.left(); left.get() != nullptr )
        {
            result["left"sv] = render::to_json(left);
        }

        if ( const auto& right = v.right(); right.get() != nullptr )
        {
            result["right"sv] = render::to_json(right);
        }

        return result;
    }

    auto operator()(const ast::UnaryExpression& v) -> nlohmann::json
    {
        using namespace std::string_view_literals;

        nlohmann::json result{};

        if ( auto op = v.operand(); op != token_type::tok_none )
        {
            result["operand"sv] = token_table::to_string(op);
        }

        if ( const auto& expression = v.expression(); expression.get() != nullptr )
        {
            result["argument"sv] = render::to_json(expression);
        }

        return result;
    }

    auto operator()(const ast::BlockStatement& v) -> nlohmann::json
    {
        using namespace std::string_view_literals;

        nlohmann::json result{};

        if ( const auto& body = v.body(); body.get() != nullptr )
        {
            result["body"sv] = render::to_json(body);
        }

        return result;
    }

    auto operator()(const ast::MemberExpression& v) -> nlohmann::json
    {
        using namespace std::string_view_literals;

        nlohmann::json result{};

        if ( const auto& o = v.object(); o.get() != nullptr )
        {
            result["object"sv] = render::to_json(o);
        }

        if ( const auto& o = v.property(); o.get() != nullptr )
        {
            result["property"sv] = render::to_json(o);
        }

        return result;
    }

    auto operator()(const ast::AstNodeList& v) -> nlohmann::json
    {
        using namespace std::string_view_literals;

        nlohmann::json result{};

        if ( const auto& list = v.nodes(); list.empty() == false )
        {
            for ( const auto& p : list )
            {
                result["nodes"sv] += render::to_json(p);
            }
        }

        return result;
    }

    auto operator()(const ast::ObjectLiteral& v) -> nlohmann::json
    {
        using namespace std::string_view_literals;

        nlohmann::json result{};

        if ( const auto& properties = v.properties(); properties.get() != nullptr )
        {
            result["properties"sv] = render::to_json(properties);
        }

        return result;
    }

    auto operator()(const ast::ObjectPropertySetter& v) -> nlohmann::json
    {
        using namespace std::string_view_literals;

        nlohmann::json result{};

        if ( const auto& formals = v.formals(); formals.get() != nullptr )
        {
            result["formals"sv] = render::to_json(formals);
        }

        if ( const auto& function_body = v.function_body(); function_body.get() != nullptr )
        {
            result["body"sv] = render::to_json(function_body);
        }

        return result;
    }

    auto operator()(const ast::ObjectPropertyGetter& v) -> nlohmann::json
    {
        using namespace std::string_view_literals;

        nlohmann::json result{};

        if ( const auto& function_body = v.function_body(); function_body.get() != nullptr )
        {
            result["body"sv] = render::to_json(function_body);
        }

        return result;
    }

    auto operator()(const ast::ObjectProperty& v) -> nlohmann::json
    {
        using namespace std::string_view_literals;

        nlohmann::json result{};

        auto property_type_string = [](const ast::UniqueAstNode& n)
        {
            if ( ast::instanceof<ast::ObjectPropertySetter>(n) ) { return "setter"sv; }
            if ( ast::instanceof<ast::ObjectPropertyGetter>(n) ) { return "getter"sv; }

            return "key/value"sv;
        }(v.value());

        if ( property_type_string.empty() == false )
        {
            result["property type string"sv] = std::move(property_type_string);
        }

        if ( const auto& key = v.key(); key.get() != nullptr )
        {
            result["key"sv] = render::to_json(key);
        }

        if ( const auto& value = v.value(); value.get() != nullptr )
        {
            result["value"sv] = render::to_json(value);
        }

        return result;
    }

    auto operator()(const ast::ThisExpression& v) -> nlohmann::json
    {
        using namespace std::string_view_literals;

        return {};
    }

    auto operator()(const ast::IfStatement& v) -> nlohmann::json
    {
        using namespace std::string_view_literals;

        nlohmann::json result{};

        if ( const auto& condition = v.condition(); condition.get() != nullptr )
        {
            result["condition"sv] = render::to_json(condition);
        }

        if ( const auto& consequent = v.consequent(); consequent.get() != nullptr )
        {
            result["consequent"sv] = render::to_json(consequent);
        }

        if ( const auto& alternate = v.alternate(); alternate.get() != nullptr )
        {
            result["alternate"sv] = render::to_json(alternate);
        }

        return result;
    }

    auto operator()(const ast::TernaryExpression& v) -> nlohmann::json
    {
        using namespace std::string_view_literals;

        nlohmann::json result{};

        if ( const auto& condition = v.condition(); condition.get() != nullptr )
        {
            result["condition"sv] = render::to_json(condition);
        }

        if ( const auto& consequent = v.consequent(); consequent.get() != nullptr )
        {
            result["consequent"sv] = render::to_json(consequent);
        }

        if ( const auto& alternate = v.alternate(); alternate.get() != nullptr )
        {
            result["alternate"sv] = render::to_json(alternate);
        }

        return result;
    }

    auto operator()(const ast::LoopStatement& v) -> nlohmann::json
    {
        using namespace std::string_view_literals;

        nlohmann::json result{};

        if ( const auto& initializer = v.initializer(); initializer.get() != nullptr )
        {
            result["initializers"sv] = render::to_json(initializer);
        }

        if ( const auto& condition = v.condition(); condition.get() != nullptr )
        {
            result["condition"sv] = render::to_json(condition);
        }

        if ( const auto& update = v.update(); update.get() != nullptr )
        {
            result["update"sv] = render::to_json(update);
        }

        if ( const auto& body = v.body(); body.get() != nullptr )
        {
            result["body"sv] = render::to_json(body);
        }

        return result;
    }

    auto operator()(const ast::SimpleStatement& v) -> nlohmann::json
    {
        using namespace std::string_view_literals;

        nlohmann::json result{};

        if ( const auto& argument = v.argument(); argument.get() != nullptr )
        {
            result["argument"sv] = render::to_json(argument);
        }

        return result;
    }

    auto operator()(const ast::CallExpression& v) -> nlohmann::json
    {
        using namespace std::string_view_literals;

        nlohmann::json result{};

        if ( const auto& callee = v.callee(); callee.get() != nullptr )
        {
            result["callee"sv] = render::to_json(callee);
        }

        if ( const auto& arguments = v.arguments(); arguments.get() != nullptr )
        {
            result["arguments"sv] = render::to_json(arguments);
        }

        return result;
    }

    auto operator()(const ast::NewExpression& v) -> nlohmann::json
    {
        using namespace std::string_view_literals;

        nlohmann::json result{};

        if ( const auto& callee = v.callee(); callee.get() != nullptr )
        {
            result["callee"sv] = render::to_json(callee);
        }

        if ( const auto& arguments = v.arguments(); arguments.get() != nullptr )
        {
            result["arguments"sv] = render::to_json(arguments);
        }

        return result;
    }

    auto operator()(const ast::MetaProperty& v) -> nlohmann::json
    {
        using namespace std::string_view_literals;

        nlohmann::json result{};

        if ( const auto type = v.type(); type == ast::MetaProperty::property_type::new_target )
        {
            result["arguments"sv] = "new.target"sv;
        }

        return result;
    }

} json_printer{};

} // namespace acme::render

#endif /* __has_include(<nlohmann/json.hpp> */
