#pragma once

namespace acme {

auto to_json(const acme::parser::ast_node_list_type&) -> std::string;

} // namespace acme
