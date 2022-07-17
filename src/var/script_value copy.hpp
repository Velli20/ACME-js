#pragma once

#include "forward_types.hpp"

#include "builtin/builtin_object.hpp"
#include "builtin/builtin_number.hpp"
#include "builtin/builtin_boolean.hpp"
#include "builtin/builtin_string.hpp"
#include "builtin/builtin_undefined.hpp"
#include "builtin/builtin_function.hpp"
#include "javascript_type.hpp"

namespace acme {

struct script_value
{
    private:

    struct value_type_2
    {
        js_type type{};

        union
        {
            acme::undefined  undefined{};
            std::nullptr_t   nul;
            acme::boolean    boolean;
            acme::number     number;
            acme::string     string;
            acme::function   function;
            acme::identifier identifier;
            acme::object     object;
        } u;
    };

    public:

    constexpr explicit script_value(auto v)
    {
        assign(std::move(v));
    }

    constexpr script_value() = default;

    [[nodiscard]] constexpr decltype(auto) value() const
    {
        return m_value2;
    }

    [[nodiscard]] constexpr auto type() const -> acme::js_type
    {
        return m_value2.type;
    }

    template <typename T>
    static constexpr decltype(auto) deref(auto* ptr)
    {
        return *(std::get_if<T>(ptr));
    }

    template <typename T>
    [[nodiscard]] constexpr auto as()
    {
        if constexpr ( std::is_same_v<T, acme::identifier> )
        {
            return m_value2.u.identifier;
        }

        else if constexpr ( std::is_same_v<T, acme::boolean> )
        {
            return m_value2.u.boolean;
        }

        else if constexpr ( std::is_same_v<T, acme::number> )
        {
            return m_value2.u.number;
        }

        else if constexpr ( std::is_same_v<T, acme::string> )
        {
            return m_value2.u.string;
        }

        else if constexpr ( std::is_same_v<T, acme::undefined> )
        {
            return m_value2.u.undefined;
        }

        else if constexpr ( std::is_same_v<T, acme::object> )
        {
            return m_value2.u.object;
        }
    }

    constexpr auto assign(auto&& new_value)
    {
        if constexpr ( std::is_same_v<decltype(new_value), acme::undefined> )
        {
            m_value2.type = js_type::undefined_type;
        }

        else if constexpr ( std::is_same_v<decltype(new_value), std::nullptr_t> )
        {
            m_value2.type = js_type::null_type;
        }

        else if constexpr ( std::is_same_v<decltype(new_value), acme::boolean> )
        {
            m_value2.type      = js_type::boolean_type;
            m_value2.u.boolean = new_value;
        }

        else if constexpr ( std::is_same_v<decltype(new_value), acme::number> )
        {
            m_value2.type     = js_type::number_type;
            m_value2.u.number = new_value;
        }

        else if constexpr ( std::is_same_v<decltype(new_value), acme::string> )
        {
            m_value2.type     = js_type::string_type;
            m_value2.u.string = new_value;
        }

        else if constexpr ( std::is_same_v<decltype(new_value), acme::object> )
        {
            m_value2.type     = js_type::object_type;
            m_value2.u.object = new_value;
        }

        else if constexpr ( std::is_same_v<decltype(new_value), acme::function> )
        {
            m_value2.type       = js_type::function_type;
            m_value2.u.function = new_value;
        }

        else if constexpr ( std::is_same_v<decltype(new_value), acme::identifier> )
        {
            m_value2.type         = js_type::identifier_type;
            m_value2.u.identifier = new_value;
        }
    }

    constexpr auto assign(acme::script_value other)
    {
        m_value2 = other.value();
    }

    [[nodiscard]] constexpr auto operator==(const acme::script_value& rhs) const noexcept
    {
        if ( type() != rhs.type() )
        {
            return false;
        }

        if ( type() == js_type::boolean_type )
        {
            return m_value2.u.boolean == rhs.m_value2.u.boolean;
        }

        else if ( type() == js_type::number_type )
        {
            return m_value2.u.number == rhs.m_value2.u.number;
        }

        else if ( type() == js_type::string_type )
        {
            return m_value2.u.string == rhs.m_value2.u.string;
        }

        else if ( type() == js_type::object_type )
        {
            return m_value2.u.object == rhs.m_value2.u.object;
        }

        else if ( type() == js_type::function_type )
        {
            return m_value2.u.function == rhs.m_value2.u.function;
        }

        else if ( type() == js_type::identifier_type )
        {
            return m_value2.u.identifier == rhs.m_value2.u.identifier;
        }

        if ( std::is_constant_evaluated() == false )
        {
            assert(false);
        }

        return false;
    }

    [[nodiscard]] constexpr auto operator!=(const acme::script_value& rhs) const noexcept
    {
        return not operator==(rhs);
    }

    value_type_2 m_value2{};
};

template <typename T> requires(std::is_same_v<T, acme::number>)
constexpr auto is_true(const T v)
{
    return std::not_equal_to<double>()(0.0, v.m_value);
}

[[nodiscard]] constexpr auto is_undefined(const acme::script_value& v)
{
    return v.type() == js_type::undefined_type;
}

[[nodiscard]] constexpr auto is_null(const acme::script_value& v)
{
    return v.type() == js_type::null_type;
}

[[nodiscard]] constexpr auto is_boolean(const acme::script_value& v)
{
    return v.type() == js_type::boolean_type;
}

[[nodiscard]] constexpr auto is_number(const acme::script_value& v)
{
    return v.type() == js_type::number_type;
}

[[nodiscard]] constexpr auto is_string(const acme::script_value& v)
{
    return v.type() == js_type::string_type;
}

[[nodiscard]] constexpr auto is_function(const acme::script_value& v)
{
    return v.type() == js_type::function_type;
}

[[nodiscard]] constexpr auto is_identifier(const acme::script_value& v)
{
    return v.type() == js_type::identifier_type;
}

[[nodiscard]] constexpr auto is_object(const acme::script_value& v)
{
    return v.type() == js_type::object_type;
}

[[nodiscard]] constexpr auto is_nan(acme::script_value v)
{
    if ( v.type() == acme::number_type )
    {
        return v.as<acme::number>().value() == NAN;
    }

    return false;
}

[[nodiscard]] constexpr auto value_typeof(acme::script_value v) -> std::string_view
{
    using namespace std::string_view_literals;

    switch ( v.type() )
    {
        case acme::boolean_type:
            return "boolean"sv;

        case acme::null_type:
            return "object"sv;

        case acme::undefined_type:
            return "undefined"sv;

        case acme::number_type:
            return "number"sv;

        case acme::string_type:
            return "string"sv;

        case acme::object_type:
            return "object"sv;

        case acme::function_type:
            return "function"sv;

        case acme::identifier_type:
            return "object"sv;
    }

    return {};
}

[[nodiscard]] constexpr auto value_typeof_s(acme::script_value v) -> acme::script_value
{
    return acme::script_value{ acme::string{ value_typeof(v) } };
}

} // namespace acme
