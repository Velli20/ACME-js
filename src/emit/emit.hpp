#pragma once

#include "emit_context.hpp"

namespace acme {

void emit(const acme::parser::ast_node_list_type&, emit_context&);

} // namespace acme
