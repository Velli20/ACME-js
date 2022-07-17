#pragma once

namespace acme::ast {

enum class node_type
{
    literal_expression,
    identifier_expression,
    object_property,
    object_property_setter,
    object_property_getter,
    object_expression,
    call_expression,
    new_expression,
    this_expression,
    ternary_expression,
    bloc_kStatement,
    member_expression,
    binary_expression,
    unary_expression,
    function_expression,
    sequence_expression,
    if_statement,
    labelled_statement,
    return_statement,
    break_statement,
    continue_statement,
    forLoop_statement,
    function_Declaration,
    variable_Declaration,
    ast_node_list,
    meta_property,
    array_literal,
};

struct node
{
    node_type m_type;
};

// Forward declarations.

struct AstNode;
struct Statement;
struct Expression;

struct Literal;
struct Identifier;
struct ObjectProperty;
struct ObjectPropertySetter;
struct ObjectPropertyGetter;
struct ObjectExpression;
struct CallExpression;
struct NewExpression;
struct ThisExpression;
struct TernaryExpression;
struct BlockStatement;
struct MemberExpression;
struct BinaryExpression;
struct UnaryExpression;
struct FunctionExpression;
struct FunctionDeclaration;
struct IfStatement;
struct LabelledStatement;
struct ReturnStatement;
struct BreakStatement;
struct ContinueStatement;
struct ForLoopStatement;
struct FunctionDeclaration;
struct VariableDeclaration;
struct AstNodeList;
struct MetaProperty;
struct ArrayLiteral;
struct SequenceExpression;

using UniqueAstNode                 = acme::unique_ptr<AstNode>;
using UniqueAstNodeList             = acme::unique_ptr<AstNodeList>;

using UniqueLiteral                 = acme::unique_ptr<Literal>;
using UniqueIdentifier              = acme::unique_ptr<Identifier>;
using UniqueObjectProperty          = acme::unique_ptr<ObjectProperty>;
using UniqueObjectPropertySetter    = acme::unique_ptr<ObjectPropertySetter>;
using UniqueObjectPropertyGetter    = acme::unique_ptr<ObjectPropertyGetter>;
using UniqueObjectExpression        = acme::unique_ptr<ObjectExpression>;
using UniqueStatement               = acme::unique_ptr<Statement>;
using UniqueExpression              = acme::unique_ptr<Expression>;
using UniqueCallExpression          = acme::unique_ptr<CallExpression>;
using UniqueNewExpression           = acme::unique_ptr<NewExpression>;
using UniqueThisExpression          = acme::unique_ptr<ThisExpression>;
using UniqueTernaryExpression       = acme::unique_ptr<TernaryExpression>;
using UniqueBlockStatement          = acme::unique_ptr<BlockStatement>;
using UniqueMemberExpression        = acme::unique_ptr<MemberExpression>;
using UniqueBinaryExpression        = acme::unique_ptr<BinaryExpression>;
using UniqueFunctionExpression      = acme::unique_ptr<FunctionExpression>;
using UniqueFunctionDeclaration     = acme::unique_ptr<FunctionDeclaration>;
using UniqueUnaryExpression         = acme::unique_ptr<UnaryExpression>;
using UniqueIfStatement             = acme::unique_ptr<IfStatement>;
using UniqueLabelledStatement       = acme::unique_ptr<LabelledStatement>;
using UniqueReturnStatement         = acme::unique_ptr<ReturnStatement>;
using UniqueBreakStatement          = acme::unique_ptr<BreakStatement>;
using UniqueContinueStatement       = acme::unique_ptr<ContinueStatement>;
using UniqueForLoopStatement        = acme::unique_ptr<ForLoopStatement>;
using UniqueVariableDeclaration     = acme::unique_ptr<VariableDeclaration>;
using UniqueMetaProperty            = acme::unique_ptr<MetaProperty>;
using UniqueArrayExpression         = acme::unique_ptr<ArrayLiteral>;
using UniqueSequenceExpression      = acme::unique_ptr<SequenceExpression>;

/* TODO: PropertySetParameterList */

struct AstNode
{
    using rtti_value_type = decltype(rtti::type_index<AstNode>());

    // No virtual destructor required (not destructed polymorphically).

    virtual ~AstNode() = default;

    template <typename T>
    [[nodiscard]] constexpr auto is() const
    {
        return m_rtti_type == rtti::type_index<T>() /*::value*/;
    }

    template <typename T>
    [[nodiscard]] constexpr auto as() -> T*
    {
        return m_rtti_type== rtti::type_index<T>() ? static_cast<T*>(this) : nullptr;
    }

    template <typename T>
    [[nodiscard]] constexpr auto deref() -> T&
    {
        return *(as<T>());
    }

    [[nodiscard]] constexpr auto type()
    {
        return m_rtti_type;
    }

    protected:

    explicit constexpr AstNode(
        acme::position  position,
        rtti_value_type rtti_type
    )
        : m_parse_location{std::move(position)},
          m_rtti_type{rtti_type}
        {}

    AstNode(AstNode&&)                 = default;
    AstNode& operator=(const AstNode&) = default;
    AstNode& operator=(AstNode&&)      = default;

    acme::position  m_parse_location{};
    rtti_value_type m_rtti_type{};
};

template <typename T>
constexpr auto instanceof(const UniqueAstNode& node)
{
    return node.get() != nullptr && node.get()->is<T>();
}

struct DeclarationKind
{
    using value_type = token_type;

    [[nodiscard]] constexpr auto value() const -> value_type
    {
        return m_token;
    }

    token_type m_token{};
};

struct Statement : public AstNode
{
    explicit constexpr Statement(
        acme::position  position,
        rtti_value_type rtti_type
    )
        : AstNode{std::move(position), rtti_type}
        {}
};

struct Expression : public Statement
{
    explicit constexpr Expression(
        acme::position  position,
        rtti_value_type rtti_type
    )
        : Statement{std::move(position), rtti_type}
        {}
};

struct Integer
{
    using value_type = std::int32_t;

    [[nodiscard]] constexpr auto value() const -> value_type
    {
        return m_value;
    }

    value_type m_value{};
};

struct UnsignedInteger
{
    using value_type = std::uint32_t;

    [[nodiscard]] constexpr auto value() const -> value_type
    {
        return m_value;
    }

    value_type m_value{};
};

struct Float
{
    using value_type = double;

    [[nodiscard]] constexpr auto value() const -> value_type
    {
        return m_value;
    }

    value_type m_value{};
};

struct String
{
    using value_type = acme::pool_string;

    [[nodiscard]] constexpr auto value() const noexcept -> const value_type&
    {
        return m_value;
    }

    value_type m_value{};
};

struct Boolean
{
    using value_type = bool;

    [[nodiscard]] constexpr auto value() const -> value_type
    {
        return m_value;
    }

    value_type m_value{};
};

struct Null
{
    using value_type = bool;

    [[nodiscard]] constexpr auto value() const -> value_type
    {
        return m_value;
    }

    value_type m_value{};
};

struct Undefined
{
    using value_type = char;

    [[nodiscard]] constexpr auto value() const -> value_type
    {
        return m_value;
    }

    value_type m_value{};
};

struct Identifier : public Expression
{
    using value_type = acme::pool_string;

    static constexpr auto rtti_type = rtti::type_index<Identifier>();

    constexpr Identifier(
        value_type     id,
        acme::position position
    )
        : Expression{std::move(position), rtti_type}
        , m_id(std::move(id))
        {}

    [[nodiscard]] constexpr auto value() const noexcept -> const value_type&
    {
        return m_id;
    }

    [[nodiscard]] static constexpr auto make(
        acme::parser_context& context,
        std::string_view      id,
        acme::position        position) -> UniqueIdentifier
    {
        auto interned_string = context.get_string_pool().intern(id);

        return acme::make_unique<Identifier>(context.resource(), std::move(interned_string), std::move(position));
    }

    value_type m_id{};
};

struct Literal : public Expression
{
    using value_type =
        std::variant<
            String,
            Integer,
            UnsignedInteger,
            Float,
            Boolean,
            Null,
            Undefined
        >;

    static constexpr auto rtti_type = rtti::type_index<Literal>();

    template <typename T>
    constexpr Literal(
        T              literal,
        acme::position position
    )
        : Expression{std::move(position), rtti_type}
        , m_value(std::move(literal))
        {}

    [[nodiscard]] constexpr auto value() const noexcept -> const value_type&
    {
        return m_value;
    }

    [[nodiscard]] static constexpr auto make(
        acme::parser_context& context,
        auto&&                value,
        acme::position        position) -> UniqueLiteral
    {
        return acme::make_unique<Literal>(context.resource(), std::forward<decltype(value)>(value), std::move(position));
    }

    value_type m_value{};
};

struct ObjectPropertySetter : public Expression
{
    static constexpr auto rtti_type = rtti::type_index<ObjectPropertySetter>();

    constexpr ObjectPropertySetter(
        UniqueAstNode  function_body,
        UniqueAstNode  formals,
        acme::position position
    )
        : Expression{std::move(position), rtti_type}
        , m_function_body{std::move(function_body)}
        , m_formals{std::move(formals)}
        {}

    [[nodiscard]] static constexpr auto make(
        acme::parser_context& context,
        UniqueAstNode         function_body,
        UniqueAstNode         formals,
        acme::position        position
    ) -> UniqueObjectPropertySetter
    {
        return acme::make_unique<ObjectPropertySetter>(context.resource(), std::move(function_body), std::move(formals), std::move(position));
    }

    [[nodiscard]] constexpr auto function_body() const noexcept -> const UniqueAstNode&
    {
        return m_function_body;
    }

    [[nodiscard]] constexpr auto formals() const noexcept -> const UniqueAstNode&
    {
        return m_formals;
    }

    UniqueAstNode m_function_body{};
    UniqueAstNode m_formals{};
};

struct ObjectPropertyGetter : public Expression
{
    static constexpr auto rtti_type = rtti::type_index<ObjectPropertyGetter>();

    constexpr ObjectPropertyGetter(
        UniqueAstNode  function_body,
        acme::position position
    )
        : Expression{std::move(position), rtti_type}
        , m_function_body{std::move(function_body)}
        {}

    [[nodiscard]] static constexpr auto make(
        acme::parser_context& context,
        UniqueAstNode         function_body,
        acme::position        position
    ) -> UniqueObjectPropertyGetter
    {
        return acme::make_unique<ObjectPropertyGetter>(context.resource(), std::move(function_body), std::move(position));
    }

    [[nodiscard]] constexpr auto function_body() const noexcept -> const UniqueAstNode&
    {
        return m_function_body;
    }

    UniqueAstNode m_function_body{};
};

struct ObjectProperty : public Expression
{
    static constexpr auto rtti_type = rtti::type_index<ObjectProperty>();

    constexpr ObjectProperty(
        UniqueAstNode key,
        UniqueAstNode value,
        acme::position position
    )
        : Expression{std::move(position), rtti_type}
        , m_key{std::move(key)}
        , m_value{std::move(value)}
        {}

    [[nodiscard]] static constexpr auto make(
        acme::parser_context& context,
        UniqueAstNode         key,
        UniqueAstNode         value,
        acme::position        position
    ) -> UniqueObjectProperty
    {
        return acme::make_unique<ObjectProperty>(context.resource(), std::move(key), std::move(value), std::move(position));
    }

    [[nodiscard]] constexpr auto key() const noexcept -> const UniqueAstNode&
    {
        return m_key;
    }

    [[nodiscard]] constexpr auto value() const noexcept -> const UniqueAstNode&
    {
        return m_value;
    }

    UniqueAstNode m_key{};
    UniqueAstNode m_value{};
};

struct ObjectExpression : public Expression
{
    static constexpr auto rtti_type = rtti::type_index<ObjectExpression>();

    constexpr ObjectExpression(
        UniqueAstNode  properties,
        acme::position position
    )
        : Expression{std::move(position), rtti_type}
        , m_properties{std::move(properties)}
        {}

    [[nodiscard]] static constexpr auto make(
        acme::parser_context& context,
        UniqueAstNode         properties,
        acme::position        position
    ) -> UniqueObjectExpression
    {
        return acme::make_unique<ObjectExpression>(context.resource(), std::move(properties), std::move(position));
    }

    [[nodiscard]] constexpr auto properties() const noexcept -> const UniqueAstNode&
    {
        return m_properties;
    }

    UniqueAstNode m_properties{};
};

struct BlockStatement : public Statement
{
    static constexpr auto rtti_type = rtti::type_index<BlockStatement>();

    constexpr BlockStatement(
        UniqueAstNode  statements,
        acme::position position
    )
        : Statement{std::move(position), rtti_type}
        , m_statements{std::move(statements)}
        {}

    [[nodiscard]] static constexpr auto make(
        acme::parser_context& context,
        UniqueAstNode         statements,
        acme::position        position
    ) -> UniqueBlockStatement
    {
        return acme::make_unique<BlockStatement>(context.resource(), std::move(statements), std::move(position));
    }

    [[nodiscard]] constexpr auto body() const noexcept -> const UniqueAstNode&
    {
        return m_statements;
    }

    UniqueAstNode m_statements{};
};

struct ForLoopStatement : public Statement
{
    static constexpr auto rtti_type = rtti::type_index<ForLoopStatement>();

    constexpr ForLoopStatement(acme::position position)
        : Statement{std::move(position), rtti_type}
        {}

    [[nodiscard]] static constexpr auto make(
        acme::parser_context& context,
        acme::position        position) -> UniqueForLoopStatement
    {
        return acme::make_unique<ForLoopStatement>(context.resource(), std::move(position));
    }

    [[nodiscard]] constexpr auto initializer() const noexcept -> const UniqueAstNode&
    {
        return m_initilizer;
    }

    [[nodiscard]] constexpr auto initializer() noexcept -> UniqueAstNode&
    {
        return m_initilizer;
    }

    constexpr auto initializer(UniqueAstNode init)
    {
        m_initilizer = std::move(init);
    }

    [[nodiscard]] constexpr auto condition() const noexcept -> const UniqueAstNode&
    {
        return m_condition;
    }

    constexpr auto condition(UniqueAstNode cond)
    {
        m_condition = std::move(cond);
    }

    [[nodiscard]] constexpr auto update() const noexcept -> const UniqueAstNode&
    {
        return m_update;
    }

    constexpr auto update(UniqueAstNode u)
    {
        m_update = std::move(u);
    }

    [[nodiscard]] constexpr auto body() const noexcept -> const UniqueAstNode&
    {
        return m_body;
    }

    constexpr auto body(UniqueAstNode u)
    {
        m_body = std::move(u);
    }

    UniqueAstNode m_initilizer{};
    UniqueAstNode m_condition{};
    UniqueAstNode m_update{};
    UniqueAstNode m_body{};
};

struct ReturnStatement : public Statement
{
    static constexpr auto rtti_type = rtti::type_index<ReturnStatement>();

    constexpr ReturnStatement(acme::position position)
        : Statement{std::move(position), rtti_type}
        {}

    [[nodiscard]] static constexpr auto make(
        acme::parser_context& context,
        acme::position        position
    ) -> UniqueReturnStatement
    {
        return acme::make_unique<ReturnStatement>(context.resource(), std::move(position));
    }

    constexpr auto argument(UniqueAstNode node)
    {
        m_argument = std::move(node);
    }

    [[nodiscard]] constexpr auto argument() const noexcept -> const UniqueAstNode&
    {
        return m_argument;
    }

    UniqueAstNode m_argument{};
};

struct LabelledStatement : public Statement
{
    static constexpr auto rtti_type = rtti::type_index<LabelledStatement>();

    constexpr LabelledStatement(
        UniqueAstNode  identifier,
        acme::position position
    )
        : Statement{std::move(position), rtti_type}
        , m_label_identifier{std::move(identifier)}
        {}

    [[nodiscard]] static constexpr auto make(
        acme::parser_context& context,
        UniqueAstNode         node,
        acme::position        position
    ) -> UniqueLabelledStatement
    {
        return acme::make_unique<LabelledStatement>(context.resource(), std::move(node), std::move(position));
    }

    [[nodiscard]] constexpr auto label_identifier() const noexcept -> const UniqueAstNode&
    {
        return m_label_identifier;
    }

    UniqueAstNode m_label_identifier{};
};

struct BreakStatement : public Statement
{
    static constexpr auto rtti_type = rtti::type_index<BreakStatement>();

    constexpr BreakStatement(acme::position position)
        : Statement{std::move(position), rtti_type}
        {}

    [[nodiscard]] static constexpr auto make(
        acme::parser_context& context,
        acme::position        position
    ) -> UniqueBreakStatement
    {
        return acme::make_unique<BreakStatement>(context.resource(), std::move(position));
    }

    constexpr auto label(UniqueAstNode node)
    {
        m_label = std::move(node);
    }

    [[nodiscard]] constexpr auto label() const noexcept -> const UniqueAstNode&
    {
        return m_label;
    }

    UniqueAstNode m_label{};
};

struct ContinueStatement : public Statement
{
    static constexpr auto rtti_type = rtti::type_index<ContinueStatement>();

    constexpr ContinueStatement(acme::position position)
        : Statement{std::move(position), rtti_type}
        {}

    [[nodiscard]] static constexpr auto make(
        acme::parser_context& context,
        acme::position        position
    ) -> UniqueContinueStatement
    {
        return acme::make_unique<ContinueStatement>(context.resource(), std::move(position));
    }

    constexpr auto label(UniqueAstNode node)
    {
        m_label = std::move(node);
    }

    [[nodiscard]] constexpr auto label() const noexcept -> const UniqueAstNode&
    {
        return m_label;
    }

    UniqueAstNode m_label{};
};

struct IfStatement : public Statement
{
    static constexpr auto rtti_type = rtti::type_index<IfStatement>();

    constexpr IfStatement(acme::position position)
        : Statement{std::move(position), rtti_type}
        {}

    [[nodiscard]] static constexpr auto make(
        acme::parser_context& context,
        acme::position        position
    ) -> UniqueIfStatement
    {
        return acme::make_unique<IfStatement>(context.resource(), std::move(position));
    }

    constexpr auto condition(UniqueAstNode node) noexcept
    {
        m_condition = std::move(node);
    }

    [[nodiscard]] constexpr auto condition() const noexcept -> const UniqueAstNode&
    {
        return m_condition;
    }

    constexpr auto consequent(UniqueAstNode node) noexcept
    {
        m_consequent = std::move(node);
    }

    [[nodiscard]] constexpr auto consequent() const noexcept -> const UniqueAstNode&
    {
        return m_consequent;
    }

    constexpr auto alternate(UniqueAstNode node) noexcept
    {
        m_alternate = std::move(node);
    }

    [[nodiscard]] constexpr auto alternate() const noexcept -> const UniqueAstNode&
    {
        return m_alternate;
    }

    ast::UniqueAstNode m_condition{};
    ast::UniqueAstNode m_consequent{};
    ast::UniqueAstNode m_alternate{};
};

struct VariableDeclaration : public Statement
{
    static constexpr auto rtti_type = rtti::type_index<VariableDeclaration>();

    constexpr VariableDeclaration(
        UniqueAstNode   id,
        DeclarationKind kind,
        acme::position  position
    )
        : Statement{std::move(position), rtti_type}
        , m_id(std::move(id))
        , m_dec{kind}
        {}

    [[nodiscard]] static constexpr auto make(
        acme::parser_context& context,
        UniqueAstNode         id,
        DeclarationKind       kind,
        acme::position        position
    ) -> UniqueVariableDeclaration
    {
        return acme::make_unique<VariableDeclaration>(context.resource(), std::move(id), kind, std::move(position));
    }

    constexpr auto assignment(UniqueAstNode init) noexcept
    {
        m_initilizer = std::move(init);
    }

    [[nodiscard]] constexpr auto kind() const noexcept -> const DeclarationKind&
    {
        return m_dec;
    }

    [[nodiscard]] constexpr auto identifier() const noexcept -> const UniqueAstNode&
    {
        return m_id;
    }

    [[nodiscard]] constexpr auto identifier() noexcept -> UniqueAstNode&
    {
        return m_id;
    }

    [[nodiscard]] constexpr auto initializer() const noexcept -> const UniqueAstNode&
    {
        return m_initilizer;
    }

    [[nodiscard]] constexpr auto initializer() noexcept -> UniqueAstNode&
    {
        return m_initilizer;
    }

    UniqueAstNode   m_id{};
    DeclarationKind m_dec{};
    UniqueAstNode   m_initilizer{};
};

struct AstNodeList : public Statement
{
    using list_type = acme::dynamic_cvector<ast::UniqueAstNode>;

    static constexpr auto rtti_type = rtti::type_index<AstNodeList>();

    AstNodeList(
        acme::position                  position,
        platform::pmr::memory_resource* resource
    )
        : Statement{std::move(position), rtti_type}
        , m_parameters{resource}
        {}

    [[nodiscard]] static constexpr auto make(
        acme::parser_context& context,
        acme::position        position
    ) -> UniqueAstNodeList
    {
        return acme::make_unique<AstNodeList>(context.resource(), std::move(position), context.resource());
    }

    constexpr auto insert(UniqueAstNode s)
    {
        m_parameters.push_back(std::move(s));
    }

    [[nodiscard]] constexpr auto nodes() const noexcept -> const list_type&
    {
        return m_parameters;
    }

    [[nodiscard]] constexpr auto count() const noexcept
    {
        return m_parameters.size();
    }

    [[nodiscard]] constexpr auto at(list_type::size_type i) -> UniqueAstNode&
    {
        return m_parameters.at(i);
    }

    list_type m_parameters{};
};

struct ArrayLiteral : public Expression
{
    static constexpr auto rtti_type = rtti::type_index<ArrayLiteral>();

    constexpr ArrayLiteral(
        UniqueAstNode  elements,
        acme::position position
    )
        : Expression{std::move(position), rtti_type}
        , m_elements{std::move(elements)}
        {}

    [[nodiscard]] static constexpr auto make(
        acme::parser_context& context,
        UniqueAstNode         elements,
        acme::position        position
    ) -> UniqueArrayExpression
    {
        return acme::make_unique<ArrayLiteral>(context.resource(), std::move(elements), std::move(position));
    }

    [[nodiscard]] constexpr auto elements() const noexcept -> const UniqueAstNode&
    {
        return m_elements;
    }

    UniqueAstNode m_elements{};
};

struct SequenceExpression : public Expression
{
    static constexpr auto rtti_type = rtti::type_index<SequenceExpression>();

    constexpr SequenceExpression(
        acme::position position,
        UniqueAstNode  expressions
    )
        : Expression{std::move(position), rtti_type}
        , m_expressions{std::move(expressions)}
        {}

    [[nodiscard]] static constexpr auto make(
        acme::parser_context& context,
        acme::position        position,
        UniqueAstNode         expressions
    ) -> UniqueSequenceExpression
    {
        return acme::make_unique<SequenceExpression>(context.resource(), std::move(position), std::move(expressions));
    }

    UniqueAstNode m_expressions{};
};

struct MetaProperty : public Expression
{
    static constexpr auto rtti_type = rtti::type_index<MetaProperty>();

    enum class property_type
    {
        new_target
    };

    constexpr MetaProperty(
        acme::position position,
        property_type  type
    )
        : Expression{std::move(position), rtti_type}
        , m_type{type}
        {}

    [[nodiscard]] static constexpr auto make(
        acme::parser_context& context,
        acme::position        position,
        property_type         type
    ) -> UniqueMetaProperty
    {
        return acme::make_unique<MetaProperty>(context.resource(), std::move(position), type);
    }

    [[nodiscard]] constexpr auto type() const noexcept
    {
        return m_type;
    }

    property_type m_type{};
};

struct FunctionDeclaration : public Statement
{
    static constexpr auto rtti_type = rtti::type_index<FunctionDeclaration>();

    constexpr FunctionDeclaration(
        UniqueAstNode  identifier,
        UniqueAstNode  parameters,
        UniqueAstNode  body,
        acme::position position
    )
        : Statement{std::move(position), rtti_type}
        , m_id{std::move(identifier)}
        , m_parameters{std::move(parameters)}
        , m_body{std::move(body)}
        {}

    [[nodiscard]] constexpr auto identifier() const noexcept -> const UniqueAstNode&
    {
        return m_id;
    }

    [[nodiscard]] constexpr auto parameters() const noexcept -> const UniqueAstNode&
    {
        return m_parameters;
    }

    [[nodiscard]] constexpr auto body() const noexcept -> const UniqueAstNode&
    {
        return m_body;
    }

    [[nodiscard]] static constexpr auto make(
        acme::parser_context& context,
        UniqueAstNode         identifier,
        UniqueAstNode         parameters,
        UniqueAstNode         body,
        acme::position        position
    ) -> UniqueAstNode
    {
        return acme::make_unique<FunctionDeclaration>(
            context.resource(),
            std::move(identifier),
            std::move(parameters),
            std::move(body),
            std::move(position)
        );
    }

    UniqueAstNode m_id{};
    UniqueAstNode m_parameters{};
    UniqueAstNode m_body{};
};

struct FunctionExpression : public Expression
{
    static constexpr auto rtti_type = rtti::type_index<FunctionExpression>();

    constexpr FunctionExpression(
        UniqueAstNode  parameters,
        UniqueAstNode  body,
        acme::position position
    )
        : Expression{std::move(position), rtti_type}
        , m_parameters{std::move(parameters)}
        , m_body{std::move(body)}
        {}

    [[nodiscard]] constexpr auto parameters() const noexcept -> const UniqueAstNode&
    {
        return m_parameters;
    }

    [[nodiscard]] constexpr auto body() const noexcept -> const UniqueAstNode&
    {
        return m_body;
    }

    [[nodiscard]] static constexpr auto make(
        acme::parser_context& context,
        UniqueAstNode         parameters,
        UniqueAstNode         body,
        acme::position        position
    ) -> UniqueAstNode
    {
        return acme::make_unique<FunctionExpression>(
            context.resource(),
            std::move(parameters),
            std::move(body),
            std::move(position)
        );
    }

    UniqueAstNode m_parameters{};
    UniqueAstNode m_body{};
};

struct UnaryExpression : public Expression
{
    static constexpr auto rtti_type = rtti::type_index<UnaryExpression>();

    constexpr UnaryExpression(
        UniqueAstNode    expression,
        acme::token_type op,
        acme::position   position
    )
        : Expression{std::move(position), rtti_type}
        , m_expression(std::move(expression))
        , m_op{op}
        {}

    [[nodiscard]] static constexpr auto make(
        acme::parser_context& context,
        UniqueAstNode         expression,
        token_type            operand,
        acme::position        position
    ) -> UniqueUnaryExpression
    {
        return acme::make_unique<UnaryExpression>(context.resource(), std::move(expression), operand, std::move(position));
    }

    [[nodiscard]] constexpr auto expression() const noexcept -> const UniqueAstNode&
    {
        return m_expression;
    }

    [[nodiscard]] constexpr auto operand() const noexcept
    {
        return m_op;
    }

    UniqueAstNode m_expression{};
    token_type    m_op{};
};


struct BinaryExpression : public Expression
{
    static constexpr auto rtti_type = rtti::type_index<BinaryExpression>();

    constexpr BinaryExpression(
        UniqueAstNode   left,
        acme::position position
    )
        : Expression{std::move(position), rtti_type}
        , m_left(std::move(left))
        {}

    constexpr BinaryExpression(
        UniqueAstNode    left,
        UniqueAstNode    right,
        acme::token_type op,
        acme::position   position
    )
        : Expression{std::move(position), rtti_type}
        , m_left(std::move(left))
        , m_right(std::move(right))
        , m_op{op}
        {}

    [[nodiscard]] static constexpr auto make(
        acme::parser_context& context,
        UniqueAstNode         left,
        UniqueAstNode         right,
        acme::token_type      op,
        acme::position        position) -> UniqueBinaryExpression
    {
        return acme::make_unique<BinaryExpression>(context.resource(), std::move(left), std::move(right), op, std::move(position));
    }

    [[nodiscard]] static constexpr auto make(
        acme::parser_context& context,
        UniqueAstNode         left,
        acme::position        position
    ) -> UniqueBinaryExpression
    {
        return acme::make_unique<BinaryExpression>(context.resource(), std::move(left), std::move(position));
    }

    [[nodiscard]] constexpr auto right() const noexcept -> const UniqueAstNode&
    {
        return m_right;
    }

    [[nodiscard]] constexpr auto right() noexcept -> UniqueAstNode&
    {
        return m_right;
    }

    [[nodiscard]] constexpr auto left() const noexcept -> const UniqueAstNode&
    {
        return m_left;
    }

    [[nodiscard]] constexpr auto left() noexcept -> UniqueAstNode&
    {
        return m_left;
    }

    [[nodiscard]] constexpr auto operand() const noexcept
    {
        return m_op;
    }

    UniqueAstNode m_left{};
    UniqueAstNode m_right{};
    token_type    m_op{};
};

struct ThisExpression : public Expression
{
    static constexpr auto rtti_type = rtti::type_index<ThisExpression>();

    constexpr ThisExpression(acme::position position)
        : Expression{std::move(position), rtti_type}
        {}

    [[nodiscard]] static constexpr auto make(
        acme::parser_context& context,
        acme::position        position
    ) -> UniqueAstNode
    {
        return acme::make_unique<ThisExpression>(context.resource(), std::move(position));
    }
};

struct TernaryExpression : public Expression
{
    static constexpr auto rtti_type = rtti::type_index<TernaryExpression>();

    constexpr TernaryExpression(
        acme::position position,
        UniqueAstNode  condition,
        UniqueAstNode  consequent,
        UniqueAstNode  alternate
    )
        : Expression{std::move(position), rtti_type}
        , m_condition{std::move(condition)}
        , m_consequent{std::move(consequent)}
        , m_alternate{std::move(alternate)}
        {}

    [[nodiscard]] static constexpr auto make(
        acme::parser_context& context,
        acme::position        position,
        UniqueAstNode         condition,
        UniqueAstNode         consequent,
        UniqueAstNode         alternate
    ) -> UniqueAstNode
    {
        return acme::make_unique<TernaryExpression>(
            context.resource(),
            std::move(position),
            std::move(condition),
            std::move(consequent),
            std::move(alternate)
        );
    }

    [[nodiscard]] constexpr auto condition() const noexcept -> const UniqueAstNode&
    {
        return m_condition;
    }

    [[nodiscard]] constexpr auto consequent() const noexcept -> const UniqueAstNode&
    {
        return m_consequent;
    }

    [[nodiscard]] constexpr auto alternate() const noexcept -> const UniqueAstNode&
    {
        return m_alternate;
    }

    ast::UniqueAstNode m_condition{};
    ast::UniqueAstNode m_consequent{};
    ast::UniqueAstNode m_alternate{};
};

struct CallExpression : public Expression
{
    static constexpr auto rtti_type = rtti::type_index<CallExpression>();

    constexpr CallExpression(
        acme::position position,
        UniqueAstNode  callee,
        UniqueAstNode  arguments
    )
        : Expression{std::move(position), rtti_type}
        , m_callee{std::move(callee)}
        , m_arguments{std::move(arguments)}
        {}

    [[nodiscard]] static constexpr auto make(
        acme::parser_context& context,
        acme::position        position,
        UniqueAstNode         callee,
        UniqueAstNode         arguments) -> UniqueAstNode
    {
        return acme::make_unique<CallExpression>(
            context.resource(),
            std::move(position),
            std::move(callee),
            std::move(arguments)
        );
    }

    [[nodiscard]] constexpr auto arguments() const noexcept -> const UniqueAstNode&
    {
        return m_arguments;
    }

    [[nodiscard]] constexpr auto callee() const noexcept -> const UniqueAstNode&
    {
        return m_callee;
    }

    UniqueAstNode m_callee;
    UniqueAstNode m_arguments;
};

struct NewExpression : public Expression
{
    static constexpr auto rtti_type = rtti::type_index<NewExpression>();

    constexpr NewExpression(
        acme::position position,
        UniqueAstNode  callee,
        UniqueAstNode  arguments
    )
        : Expression{std::move(position), rtti_type}
        , m_callee{std::move(callee)}
        , m_arguments{std::move(arguments)}
        {}

    [[nodiscard]] static constexpr auto make(
        acme::parser_context& context,
        acme::position        position,
        UniqueAstNode         callee,
        UniqueAstNode         arguments) -> UniqueAstNode
    {
        return acme::make_unique<NewExpression>(
            context.resource(),
            std::move(position),
            std::move(callee),
            std::move(arguments)
        );
    }

    [[nodiscard]] constexpr auto arguments() const noexcept -> const UniqueAstNode&
    {
        return m_arguments;
    }

    [[nodiscard]] constexpr auto callee() const noexcept -> const UniqueAstNode&
    {
        return m_callee;
    }

    UniqueAstNode m_callee;
    UniqueAstNode m_arguments;
};

struct MemberExpression : public Expression
{
    static constexpr auto rtti_type = rtti::type_index<MemberExpression>();

    constexpr MemberExpression(acme::position position)
        : Expression{std::move(position), rtti_type}
        {}

    constexpr MemberExpression(
        acme::position position,
        UniqueAstNode  property,
        UniqueAstNode  object
    )
        : Expression{std::move(position), rtti_type}
        , m_property{std::move(property)}
        , m_object{std::move(object)}
        {}

    [[nodiscard]] static constexpr auto make(
        acme::parser_context& context,
        acme::position        position,
        UniqueAstNode         obj,
        UniqueAstNode         prop
    ) -> UniqueAstNode
    {
        return acme::make_unique<MemberExpression>(context.resource(), std::move(position), std::move(obj), std::move(prop));
    }

    constexpr auto property(UniqueAstNode node)
    {
        m_property = std::move(node);
    }

    [[nodiscard]] constexpr auto property() const noexcept -> const UniqueAstNode&
    {
        return m_property;
    }

    constexpr auto object(UniqueAstNode node)
    {
        m_object = std::move(node);
    }

    [[nodiscard]] constexpr auto object() const noexcept -> const UniqueAstNode&
    {
        return m_object;
    }

    UniqueAstNode m_property{};
    UniqueAstNode m_object{};
};

//static_assert(std::is_trivially_destructible_v<BinaryExpression>);

static_assert(BinaryExpression::rtti_type != VariableDeclaration::rtti_type, "");

} // namespace acme::ast

