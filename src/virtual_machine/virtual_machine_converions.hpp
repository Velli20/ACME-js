#pragma once

namespace acme {

[[nodiscard]] constexpr auto to_double(acme::script_value v) -> double
{
    using namespace std::string_view_literals;

    switch ( v.type() )
    {
        case acme::boolean_type:
            return v.as<acme::boolean>().value();

        case acme::null_type:
            return 0.0;

        case acme::undefined_type:
            return NAN;

        case acme::number_type:
            return v.as<acme::number>().value();

        case acme::object_type:
            return 0.0; // TODO
            // return v.as<acme::object>();

        case acme::string_type:
        {
            auto string_v = v.as<acme::string>().value();
            if ( string_v.empty() )
            {
                return 0.0;
            }

            if ( string_v == "Infinity"sv || string_v == "+Infinity"sv )
            {
               return INFINITY;
            }

            if ( string_v == "-Infinity" )
            {
               return -INFINITY;
            }

            acme::tokenizer tok{string_v};

            if ( auto token = parse_number(tok); token.has_value() )
            {
                auto token_v = token.value();
                if ( token_v.is_double() )
                {
                    return token_v.to_double();
                }

                if ( token_v.is_signed_integer() )
                {
                    return static_cast<double>(token_v.to_signed_integer());
                }

                if ( token_v.is_unsigned_integer() )
                {
                    return static_cast<double>(token_v.to_unsigned_integer());
                }
            }

            break;
        }

        case acme::function_type:
            break;

        case acme::identifier_type:
            break;
    }

    return {};
}

[[nodiscard]] constexpr auto to_boolean(acme::script_value v) -> bool
{
    switch ( v.type() )
    {
        case acme::boolean_type:
            return v.as<acme::boolean>().value();

        case acme::null_type:
            return false;

        case acme::undefined_type:
            return false;

        case acme::number_type:
            return is_nan(v) ? false : to_double(v) != 0.0;

        case acme::string_type:
            return v.as<acme::string>().value().empty() == true ? false : true;

        case acme::object_type:
            break;

        case acme::function_type:
            break;

        case acme::identifier_type:
            break;
    }

    return {};
}

namespace {

[[nodiscard]] auto double_to_string(
    acme::virtual_machine& vm,
    acme::script_value     v
) -> acme::string
{
    using namespace std::string_view_literals;

    if ( is_nan(v) )
    {
        return acme::string{"NaN"sv};
    }

    const auto number_value = to_double(v);

    if ( number_value == +0.0 || number_value == -0.0 )
    {
        return acme::string{"0"sv};
    }

    if ( number_value == static_cast<double>(NAN) )
    {
        return acme::string{"Infinity"sv};
    }

    if ( number_value == static_cast<double>(INFINITY) )
    {
        return acme::string{"Infinity"sv};
    }

    if ( -number_value ==  static_cast<double>(INFINITY) )
    {
        return acme::string{"-Infinity"sv};
    }

    constexpr auto k_max = std::numeric_limits<double>::digits + 2;

    auto buffer = std::array<char, k_max + 1>{};

    if ( auto len = snprintf(buffer.data(), buffer.size(), "%.1f", number_value); len > 0 && len < buffer.size() )
    {
        auto str_view = std::string_view{buffer.data(), static_cast<std::size_t>(len)};
        auto new_str  = vm.string_pool().intern(str_view);

        return acme::string{ new_str };
    }

    assert(false && "String buffer overflow");
    return acme::string{""};
}

} // namespace

[[nodiscard]] constexpr auto to_string(
    acme::virtual_machine& vm,
    acme::script_value     v
) -> std::string_view
{
    using namespace std::string_view_literals;

    switch ( v.type() )
    {
        case acme::boolean_type:
            return v.as<acme::boolean>().value() ? "true"sv : "false"sv;

        case acme::null_type:
            return "null";

        case acme::undefined_type:
            return "undefined";

        case acme::number_type:
            return double_to_string(vm, v).value();

        case acme::string_type:
            return v.as<acme::string>().value();

        case acme::object_type:
            break;

        case acme::function_type:
            break;

        case acme::identifier_type:
            break;
    }

    return {};
}

[[nodiscard]] constexpr auto to_object(acme::script_value v) -> acme::object
{
    using namespace std::string_view_literals;

    switch ( v.type() )
    {
        case acme::boolean_type:
            break;

        case acme::null_type:
            break;

        case acme::undefined_type:
            break;

        case acme::number_type:
            break;

        case acme::string_type:
            break;

        case acme::object_type:
            break;

        case acme::function_type:
            break;

        case acme::identifier_type:
            break;
    }

    return {};
}

} // namespace acme
