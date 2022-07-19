

namespace acme {

[[nodiscard]] constexpr auto op_negate(acme::script_value v) -> acme::script_value
{
    if ( v.type() == acme::number_type )
    {
        return acme::script_value { acme::number { to_double(v) * -1 } };
    }

    return acme::script_value { acme::boolean { !to_boolean(v) } };
}

[[nodiscard]] constexpr auto op_strict_equal(
    acme::virtual_machine& vm,
    acme::script_value     lhs,
    acme::script_value     rhs
) noexcept -> acme::script_value
{
    if ( lhs.type() != rhs.type() )
    {
        return acme::script_value { acme::boolean { false } };
    }

    if ( is_undefined(lhs) || is_null(lhs) )
    {
        return acme::script_value{ acme::boolean { true } };
    }

    if ( lhs.type() == acme::string_type )
    {
        auto s1 = lhs.as<acme::string>().value();
        auto s2 = rhs.as<acme::string>().value();

        return acme::script_value { acme::boolean { s1 == s2 } };
    }

    if ( lhs.type() == acme::number_type )
    {
        auto n1 = lhs.as<acme::number>().value();
        auto n2 = rhs.as<acme::number>().value();

        return acme::script_value { acme::boolean { n1 == n2 } };
    }

    if ( lhs.type() == acme::boolean_type )
    {
        auto b1 = lhs.as<acme::boolean>().value();
        auto b2 = rhs.as<acme::boolean>().value();

        return acme::script_value { acme::boolean { b1 == b2 } };
    }

    return acme::script_value { acme::boolean { false } };
}

[[nodiscard]] constexpr auto op_equal(
    acme::virtual_machine& vm,
    acme::script_value     lhs,
    acme::script_value     rhs
) noexcept -> acme::script_value
{
    const bool lnc = is_undefined(lhs) || is_null(lhs);
    const bool rnc = is_undefined(rhs) || is_null(rhs);

    // Undefined and null are equally comparable and cannot be compared to anything else.

    if ( lnc == true || rnc == true )
    {
        return acme::script_value{acme::boolean{lnc == true && rnc == true}};
    }

    // Return the result of performing Strict Equality Comparison x if types are equal.

    if ( lhs.type() == rhs.type() )
    {
        return op_strict_equal(vm, lhs, rhs);
    }

    if ( lhs.type() == acme::number_type || lhs.type() == acme::boolean_type )
    {
        auto n1 = to_double(lhs);
        auto n2 = to_double(rhs);

        return acme::script_value { acme::boolean { n1 == n2 } };
    }

    if ( lhs.type() == acme::boolean_type )
    {
        auto n1 = to_boolean(lhs);
        auto n2 = to_boolean(rhs);

        return acme::script_value { acme::boolean { n1 == n2 } };
    }

    if ( lhs.type() == acme::string_type )
    {
        auto b1 = to_string(vm, lhs);
        auto b2 = to_string(vm, rhs);

        return acme::script_value { acme::boolean { b1 == b2 } };
    }

    return acme::script_value { acme::boolean { false } };
}

[[nodiscard]] constexpr auto op_less_than(
    acme::script_value lhs,
    acme::script_value rhs
) noexcept -> acme::script_value
{
    auto n1 = to_double(lhs);
    auto n2 = to_double(rhs);

    return acme::script_value { acme::boolean { n1 < n2 } };
}

[[nodiscard]] constexpr auto op_add(
    acme::virtual_machine&   vm,
    const acme::script_value lhs,
    const acme::script_value rhs
) noexcept -> acme::script_value
{
    if ( lhs.type() == acme::string_type && rhs.type() == acme::string_type )
    {
        auto s1 = to_string(vm, lhs);
        auto s2 = to_string(vm, rhs);

        return acme::script_value{ acme::string{  vm.string_pool().concatanate(s1, s2) } };
    }

    return acme::script_value{ acme::number{ to_double(lhs) + to_double(rhs) } };
}

[[nodiscard]] constexpr auto op_sub(
    const acme::script_value lhs,
    const acme::script_value rhs
) noexcept -> acme::script_value
{
    return acme::script_value{ acme::number{ to_double(lhs) - to_double(rhs) } };
}

[[nodiscard]] constexpr auto op_mul(
    const acme::script_value lhs,
    const acme::script_value rhs
) noexcept -> acme::script_value
{
    return acme::script_value{ acme::number{ to_double(lhs) * to_double(rhs) } };
}

[[nodiscard]] constexpr auto op_pow(
    const acme::script_value lhs,
    const acme::script_value rhs
) noexcept -> acme::script_value
{
    return acme::script_value{ acme::number{ std::pow(to_double(lhs), to_double(rhs)) } };
}

[[nodiscard]] constexpr auto op_div(
    const acme::script_value lhs,
    const acme::script_value rhs
) noexcept -> acme::script_value
{
    if ( auto rhs_d = to_double(rhs); rhs_d != 0.0 )
    {
        return acme::script_value{ acme::number{ to_double(lhs) / rhs_d } };
    }

    return {};
}

[[nodiscard]] constexpr auto op_mod(
    const acme::script_value lhs,
    const acme::script_value rhs
) noexcept -> acme::script_value
{
    if ( auto rhs_d = to_double(rhs); rhs_d != 0.0 )
    {
        return acme::script_value{ acme::number{ (signed) to_double(lhs) % (signed) rhs_d } };
    }

    return {};
}

[[nodiscard]] constexpr auto op_instanceof(
    const acme::script_value lhs,
    const acme::script_value rhs
) noexcept -> acme::script_value
{
    if ( rhs.type() != acme::object_type )
    {
        return acme::script_value{acme::boolean{false}};
    }

    return acme::script_value{acme::boolean{to_object(lhs).instanceof(to_object(rhs)) }};
}

} // namespace acme
