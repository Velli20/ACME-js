
#include "memory/memory.hpp"
#include "base/base.hpp"
#include "string_pool/string_pool.hpp"
#include "parse/parser_context.hpp"


#include "tokenizer/tokenizer.hpp"
#include "ast/ast.hpp"
#include "parse/parse.hpp" // FIXME
#include "render.hpp"
#include "to_json.hpp"

#if __has_include(<nlohmann/json.hpp>)

namespace acme::render {

auto to_json(const ast::UniqueAstNode& p) -> nlohmann::json
{
    return ast::visit(render::json_printer, p);
}

} // namespace acme::render

namespace acme {

auto to_json(const acme::parser::ast_node_list_type& ast_nodes) -> std::string
{
    nlohmann::json result{};

    for ( const auto& p : ast_nodes )
    {
        result += render::to_json(p);
    }

    return result.dump(2);
}

} // namespace acme

#else

auto to_json(const acme::parser::ast_node_list_type& ast_nodes) -> std::string
{
    return {};
}

#endif /* __has_include(<nlohmann/json.hpp>) */
