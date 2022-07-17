#pragma once

namespace acme {

struct parser : public acme::tokenizer<char>
{
    static constexpr auto k_max_parse_depth       = 64u;
    static constexpr auto k_max_token_stack_depth = 2u;

    using base               = acme::tokenizer<char>;
    using string_type        = base::string_type;
    using token_item_type    = base::token_item_type;
    using ast_node_list_type = acme::dynamic_cvector<ast::UniqueAstNode>;
    using stack_type         = acme::containers::stack<k_max_parse_depth, acme::transition_state>;
    using token_stack_type   = acme::containers::stack<k_max_token_stack_depth, token_item_type>;

    explicit parser(
        string_type                     input,
        platform::pmr::memory_resource* resource
    ) noexcept
        : base{input}
        , m_context{resource}
        , m_nodes{resource}
        {}

    constexpr auto parse(state::function_declaration)                             -> ast::UniqueAstNode;
    constexpr auto parse(state::function_expression)                              -> ast::UniqueAstNode;
    constexpr auto parse(state::arrow_function_expression)                        -> ast::UniqueAstNode;

    constexpr auto parse(state::literal_expression)                               -> ast::UniqueAstNode;
    constexpr auto parse(state::single_quoted_literal)                            -> ast::UniqueAstNode;
    constexpr auto parse(state::double_quoted_literal)                            -> ast::UniqueAstNode;
    constexpr auto parse(state::parameter_list)                                   -> ast::UniqueAstNode;

    constexpr auto parse(state::array_literal)                                    -> ast::UniqueAstNode;
    constexpr auto parse(state::arrow_function)                                   -> ast::UniqueAstNode;
    constexpr auto parse(state::object_property)                                  -> ast::UniqueAstNode;
    constexpr auto parse(state::logical_expression)                               -> ast::UniqueAstNode;
    constexpr auto parse(state::equality_expression)                              -> ast::UniqueAstNode;
    constexpr auto parse(state::shift_expression)                                 -> ast::UniqueAstNode;
    constexpr auto parse(state::additive_expression)                              -> ast::UniqueAstNode;
    constexpr auto parse(state::multiplicative_expression)                        -> ast::UniqueAstNode;
    constexpr auto parse(state::unary_expression)                                 -> ast::UniqueAstNode;
    constexpr auto parse(state::ternary_expression, ast::UniqueAstNode)           -> ast::UniqueAstNode;
    constexpr auto parse(state::primary_expression)                               -> ast::UniqueAstNode;
    constexpr auto parse(state::binary_expression)                                -> ast::UniqueAstNode;
    constexpr auto parse(state::postfix_expression, ast::UniqueAstNode)           -> ast::UniqueAstNode;
    constexpr auto parse(state::object_declaration)                               -> ast::UniqueAstNode;
    constexpr auto parse(state::block_statement)                                  -> ast::UniqueAstNode;
    constexpr auto parse(state::variable_statement)                               -> ast::UniqueAstNode;
    constexpr auto parse(state::if_statement)                                     -> ast::UniqueAstNode;
    constexpr auto parse(state::for_statement)                                    -> ast::UniqueAstNode;
    constexpr auto parse(state::do_statement)                                     -> ast::UniqueAstNode;
    constexpr auto parse(state::while_statement)                                  -> ast::UniqueAstNode;
    constexpr auto parse(state::function_statement)                               -> ast::UniqueAstNode;
    constexpr auto parse(state::continue_statement)                               -> ast::UniqueAstNode;
    constexpr auto parse(state::break_statement)                                  -> ast::UniqueAstNode;
    constexpr auto parse(state::switch_statement)                                 -> ast::UniqueAstNode;
    constexpr auto parse(state::try_statement)                                    -> ast::UniqueAstNode;
    constexpr auto parse(state::debugger_statement)                               -> ast::UniqueAstNode;
    constexpr auto parse(state::return_statement)                                 -> ast::UniqueAstNode;
    constexpr auto parse(state::variable_declaration)                             -> ast::UniqueAstNode;
    constexpr auto parse(state::expression)                                       -> ast::UniqueAstNode;
    constexpr auto parse(state::labelled_statement)                               -> ast::UniqueAstNode;
    constexpr auto parse(state::with_statement)                                   -> ast::UniqueAstNode;
    constexpr auto parse(state::empty_statement)                                  -> ast::UniqueAstNode;
    constexpr auto parse(state::statement)                                        -> ast::UniqueAstNode;
    constexpr auto parse(state::this_expression)                                  -> ast::UniqueAstNode;
    constexpr auto parse(state::member_expression, ast::UniqueAstNode)            -> ast::UniqueAstNode;
    constexpr auto parse(state::new_expression)                                   -> ast::UniqueAstNode;

    constexpr auto parse(...) -> ast::UniqueAstNode
    {
        if ( std::is_constant_evaluated() == false )
        {
            std::cout
            << "<unhandled token='" << current_token().to_string()
            << "' id='" << static_cast<std::uint32_t>(current_token().type()) <<  "'/>";
        }

        next_token();
        return {};
    }

    template <typename... Args>
    constexpr auto transition(auto&& state, Args&&... args) -> ast::UniqueAstNode
    {
        m_stack.push_back(decltype(state){});

        auto result = parse(decltype(state){}, std::forward<decltype(args)>(args)...);

        m_stack.pop_back();
        return result;
    }

    // Parse code section that is surrounded by some specific token (parenthesis, bracket, quote...).

    template <typename... Args>
    [[nodiscard]] constexpr auto parse_sequence(
        token_type entry_token,
        token_type exit_token,
        bool       require,
        Args&&...  args
    ) -> ast::UniqueAstNode
    {
        // Expect and consume provided token.

        if ( expect(entry_token, require, true) == false )
        {
            return {};
        }

        else
        {
            require = true;
        }

        auto result = transition(std::forward<decltype(args)>(args)...);

        // Expect and consume the same token type as on entry.

        if ( expect(exit_token, require, true) == false )
        {
            return {};
        }

        return result;
    }

    template <typename... Args>
    [[nodiscard]] constexpr auto parse_comma_sequence(Args&&...  args) -> ast::UniqueAstNodeList
    {
        // Create an ast node for the argument/parameter list.

        auto list = ast::AstNodeList::make(context(), position());

        if ( std::is_constant_evaluated() == false )
        {
            assert(list.get() != nullptr);
        }

        while ( true )
        {
            // Parse next item.

            if ( auto param = transition(std::forward<decltype(args)>(args)...); param.get() != nullptr )
            {
                list->insert(std::move(param));
            }

            // Continue parsing on a comma token ','.

            if ( expect(token_type::tok_comma, false, true) == false )
            {
                break;
            }
        }

        return list;
    }

    [[nodiscard]] constexpr auto previous_transition() const noexcept -> acme::transition_state
    {
        if ( m_stack.empty() == false )
        {
            return m_stack[m_stack.size() - 1];
        }

        return {};
    }

    [[nodiscard]] constexpr auto context() -> parser_context&
    {
        return m_context;
    }

    [[nodiscard]] constexpr auto current_token() const -> const token_item_type&
    {
        return m_current_token;
    }

    [[nodiscard]] constexpr auto expect(
        acme::token_type t,
        bool             error_if_not,
        bool             advance = false
    ) -> bool
    {
        if ( current_token() != t )
        {
            if ( std::is_constant_evaluated() == false && error_if_not == true )
            {
                std::cerr << "SyntaxError: Expected '" << token_table::to_string(t) << "' instead got token '" << current_token().to_string() << "'.\n";
                std::cerr << "At line: " << source_location().line() << " column: " << source_location().column() << '\n';
            }

            return false;
        }

        if ( advance )
        {
            next_token();
        }

        return true;
    }

    [[nodiscard]] constexpr auto expect_one_of(
        std::span<const acme::token_type> l,
        bool                              error_if_not,
        bool                              advance = false
    ) -> bool
    {
        for ( const auto& c : l )
        {
            if ( expect(c, error_if_not, advance) )
            {
                return true;
            }
        }

        return false;
    }


    [[nodiscard]] constexpr auto expect_statement_end()
    {
        constexpr auto k_tokens = std::array
        {
            token_type::tok_eof,
            token_type::tok_else,
            token_type::tok_semicolon
        };

        return expect_one_of(k_tokens, false, true);
    }

    private:

    [[nodiscard]] constexpr auto get_next_token() -> token_item_type
    {
        while ( true )
        {
            auto token = base::next();

            if ( token.type() == token_type::tok_multi_line_comment_start )
            {
                if ( base::until_token(token_type::tok_multi_line_comment_end) )
                {
                    continue;
                }
            }

            if ( token.type() == token_type::tok_single_line_comment_start || token.type() == token_type::tok_hashbang_comment_start )
            {
                if ( base::until_token(token_type::tok_line_terminator) )
                {
                    continue;
                }
            }

            if ( token.type() == token_type::tok_whitespace || token.type() == token_type::tok_line_terminator )
            {
                continue;
            }

            return token;
        }

        return {};
    }

    public:

    constexpr auto next_token() -> bool
    {
        if ( m_token_stack.empty() == false )
        {
            m_current_token = m_token_stack.pop_back();
            return true;
        }

        m_current_token = get_next_token();

        if ( current_token().type() != token_type::tok_none &&  current_token().type() != token_type::tok_eof )
        {
            return true;
        }

        return {};
    }

    constexpr auto peek_token() -> token_item_type
    {
        if ( m_token_stack.empty() == false )
        {
            return m_token_stack.get(0);
        }

        if ( auto token = get_next_token(); token.type() !=  token_type::tok_none )
        {
            m_token_stack.push_back(token);
            return token;
        }

        return {};
    }

    constexpr auto parse_all()
    {
        next_token();

        while ( true )
        {
            if ( available() < 1 )
            {
                break;
            }

            if ( current_token().type() == token_type::tok_eof || current_token().type() == token_type::tok_none )
            {
                break;
            }

            else if ( current_token().type() == token_type::tok_line_terminator )
            {
                continue;
            }

            if ( auto p = parse(state::statement{});  p.get() != nullptr )
            {
                m_nodes.push_back(std::move(p));
                continue;
            }

            else if ( next_token() == false )
            {
                break;
            }
        }
    }

    [[nodiscard]] constexpr auto ast_nodes() const noexcept -> const ast_node_list_type&
    {
        return m_nodes;
    }

    [[nodiscard]] constexpr auto ast_nodes() noexcept -> ast_node_list_type&
    {
        return m_nodes;
    }

    /* private: */
    parser_context     m_context{};
    ast_node_list_type m_nodes{};
    token_item_type    m_current_token{};
    token_stack_type   m_token_stack{};
    stack_type         m_stack{};
};

} // namespace acme
