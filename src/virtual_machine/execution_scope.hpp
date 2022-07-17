#pragma once

namespace acme {

struct execution_scope
{
    using var_stack_type = acme::containers::var_stack<24>;

    [[nodiscard]] auto locals() -> var_stack_type&
    {
        return m_locals;
    }

    var_stack_type m_locals{};
};

} // namespace acme
