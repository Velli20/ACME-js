
#include "memory/memory.hpp"
#include "base/base.hpp"
#include "string_pool/string_pool.hpp"
#include "parse/parser_context.hpp"

#include "tokenizer/tokenizer.hpp"
#include "var/script_value.hpp"
#include "ast/ast.hpp"
#include "parse/parse.hpp"
#include "bytecode/bytecode.hpp"
#include "virtual_machine/virtual_machine.hpp"
#include "emit.hpp"
#include "concepts.hpp"
#include "emit_visit.hpp"
#include "emit_visitor.hpp"

namespace acme::eval {

auto emit(
    const ast::UniqueAstNode& p,
    emit_context&            context
) -> acme::script_value
{
    return eval::visit(eval::emit_visitor, p, context);
}

} // namespace acme::eval

namespace acme {

void emit(
    const acme::parser::ast_node_list_type& ast_nodes,
    acme::emit_context&                     context
)
{
    for ( auto& p : ast_nodes )
    {
        eval::emit(p, context);
    }

    context.emit_instruction(opcode::no_opearation);
}

} // namespace acme

