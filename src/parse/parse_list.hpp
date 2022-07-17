#pragma once

namespace acme {

constexpr auto parser::parse(state::parameter_list) -> ast::UniqueAstNode
{
    auto list = parse_comma_sequence(state::literal_expression{});
    if ( list.get() != nullptr && list->count() == 0u )
    {
        return {};
    }

    // Special case for the 'AstNodeList': Accept only identifier names.

    if ( const auto& parameters = list.get()->nodes(); parameters.empty() == false )
    {
        for ( const auto& p : parameters )
        {
            if ( p.get() != nullptr && ast::instanceof<ast::Identifier>(p) == false )
            {
                // TODO: std::cerr << "Expected a formal parameter\n"sv;
                return {};
            }
        }
    }

    return list;
}

} // namespace acme
