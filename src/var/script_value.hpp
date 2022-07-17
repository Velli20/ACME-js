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
    constexpr explicit script_value(auto v)
        : m_value{v}
        {}

    constexpr script_value() = default;

    [[nodiscard]] constexpr decltype(auto) value() const
    {
        return m_value;
    }

    [[nodiscard]] constexpr auto type() const -> acme::js_type
    {
        return std::visit(acme::overloaded
        {
            [](const acme::boolean&)     { return acme::boolean_type();    },
            [](const acme::number&)      { return acme::number_type();     },
            [](const acme::string&)      { return acme::string_type();     },
            [](const std::nullptr_t&)    { return acme::null_type();       },
            [](const acme::function&)    { return acme::function_type();   },
            [](const acme::identifier&)  { return acme::identifier_type(); },
            [](const acme::undefined&)   { return acme::undefined_type();  },
            [](const acme::object&)      { return acme::object_type();     }
        }, m_value);
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
            return deref<acme::identifier>(std::addressof(m_value));
        }

        else if constexpr ( std::is_same_v<T, acme::boolean> )
        {
            return deref<acme::boolean>(std::addressof(m_value));
        }

        else if constexpr ( std::is_same_v<T, acme::number> )
        {
            return deref<acme::number>(std::addressof(m_value));
        }

        else if constexpr ( std::is_same_v<T, acme::string> )
        {
            return deref<acme::string>(std::addressof(m_value));
        }

        else if constexpr ( std::is_same_v<T, acme::undefined> )
        {
            return deref<acme::undefined>(std::addressof(m_value));
        }

        else if constexpr ( std::is_same_v<T, acme::object> )
        {
            return deref<acme::object>(std::addressof(m_value));
        }
    }

    constexpr auto assign(auto&& new_value)
    {
        m_value = decltype(m_value){std::forward<decltype(new_value)>(new_value)};
    }

    constexpr auto assign(acme::script_value other)
    {
        m_value = other.value();
    }

    //void* operator new(size_t size);

    [[nodiscard]] constexpr auto operator==(const acme::script_value& rhs) const noexcept
    {
        if ( type() != rhs.type() )
        {
            return false;
        }

        return m_value == rhs.m_value;
    }

    [[nodiscard]] constexpr auto operator!=(const acme::script_value& rhs) const noexcept
    {
        return not operator==(rhs);
    }

    using value_type = std::variant<acme::undefined,
                                    std::nullptr_t,
                                    acme::boolean,
                                    acme::number,
                                    acme::string,
                                    acme::function,
                                    acme::identifier,
                                    acme::object>;

    static_assert(std::variant_size_v<value_type> == 8, "" );

    value_type m_value{};
};

template <typename T> requires(std::is_same_v<T, acme::number>)
constexpr auto is_true(const T v)
{
    return std::not_equal_to<double>()(0.0, v.m_value);
}

[[nodiscard]] constexpr auto is_undefined(const acme::script_value& v)
{
    return std::holds_alternative<acme::undefined>(v.value());
}

[[nodiscard]] constexpr auto is_null(const acme::script_value& v)
{
    return std::holds_alternative<std::nullptr_t>(v.value());
}

[[nodiscard]] constexpr auto is_boolean(const acme::script_value& v)
{
    return std::holds_alternative<acme::boolean>(v.value());
}

[[nodiscard]] constexpr auto is_number(const acme::script_value& v)
{
    return std::holds_alternative<acme::number>(v.value());
}

[[nodiscard]] constexpr auto is_string(const acme::script_value& v)
{
    return std::holds_alternative<acme::string>(v.value());
}

[[nodiscard]] constexpr auto is_function(const acme::script_value& v)
{
    return std::holds_alternative<acme::function>(v.value());
}

[[nodiscard]] constexpr auto is_identifier(const acme::script_value& v)
{
    return std::holds_alternative<acme::identifier>(v.value());
}

[[nodiscard]] constexpr auto is_object(const acme::script_value& v)
{
    return std::holds_alternative<acme::object>(v.value());
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
