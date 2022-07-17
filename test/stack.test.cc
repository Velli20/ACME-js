#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wextra-semi"
#endif /* __clang__ */

#define TTS_MAIN
#include <tts/tts.hpp>

#if defined(__clang__)
#pragma clang diagnostic pop
#endif /* __clang__ */

#include <iostream>

#include "memory/memory.hpp"
#include "base/base.hpp"
#include "string_pool/string_pool.hpp"

#include "tokenizer/tokenizer.hpp"
#include "var/script_value.hpp"
#include "parse/parser_context.hpp"
#include "ast/ast.hpp"
#include "parse/parse.hpp"
#include "bytecode/bytecode.hpp"
#include "virtual_machine/virtual_machine.hpp"

static constexpr auto test_data()
{
    using namespace std::string_view_literals;
    using namespace acme::literals;
    using namespace acme;

    acme::containers::var_stack<24, std::pair<acme::identifier, acme::script_value>> stack{};

    stack.push("test_var1"sv,  acme::string{"Hello"sv});
    stack.push("test_var2"sv,  acme::boolean{false});
    stack.push("test_var3"_id, acme::number{27});
    stack.push("test_var4"sv,  acme::number{0.1});
    stack.push("test_var5"sv,  acme::undefined{});
    stack.push("test_var6"_id, nullptr);
    stack.push("test_var7"_id, +[]() {});

    return stack;
}


static_assert(

    []() constexpr
    {
        auto stack = test_data();

        using namespace std::string_view_literals;
        using namespace acme::literals;
        using namespace acme;

        if ( auto v = stack.get("test_var1"_id); not v.has_value() ) { return false; }
        if ( auto v = stack.get("test_var2"_id); not v.has_value() ) { return false; }
        if ( auto v = stack.get("test_var3"_id); not v.has_value() ) { return false; }
        if ( auto v = stack.get("test_var4"_id); not v.has_value() ) { return false; }
        if ( auto v = stack.get("test_var5"_id); not v.has_value() ) { return false; }
        if ( auto v = stack.get("test_var6"_id); not v.has_value() ) { return false; }

        if ( stack.get(0).type() != acme::string_type              ) { return false; }
        if ( stack.get(1).type() != acme::boolean_type             ) { return false; }
        if ( stack.get(2).type() != acme::number_type              ) { return false; }
        if ( stack.get(3).type() != acme::number_type              ) { return false; }
        if ( stack.get(4).type() != acme::undefined_type           ) { return false; }
        if ( stack.get(5).type() != acme::null_type                ) { return false; }
        if ( stack.get(6).type() != acme::function_type            ) { return false; }

        if ( auto v = stack.pop_back(); not is_function(v)         ) { return false; }
        if ( auto v = stack.pop_back(); not is_null(v)             ) { return false; }
        if ( auto v = stack.pop_back(); not is_undefined(v)        ) { return false; }
        if ( auto v = stack.pop_back(); not is_number(v)           ) { return false; }
        if ( auto v = stack.pop_back(); not is_number(v)           ) { return false; }
        if ( auto v = stack.pop_back(); not is_boolean(v)          ) { return false; }
        if ( auto v = stack.pop_back(); not is_string(v)           ) { return false; }

        return true;

    }() == true, "[ACME stack] Test primitive types"
);

# if 0
static_assert(

    []() constexpr
    {
        auto stack = test_data();

        using namespace std::string_view_literals;
        using namespace acme::literals;
        using namespace acme;

        if ( stack.get(0).as<acme::string>()    != acme::string{"Hello"sv} ) { return false; }
        if ( stack.get(1).as<acme::boolean>()   != acme::boolean{false}    ) { return false; }
        if ( stack.get(2).as<acme::number>()    != acme::number{27 }       ) { return false; }
        if ( stack.get(3).as<acme::number>()    != acme::number{0.1}       ) { return false; }
        if ( stack.get(4).as<acme::undefined>() != acme::undefined{}       ) { return false; }

        {
            {
                auto v = stack.get(1).as<acme::boolean>();
                if ( v != acme::boolean{false} ) { return false; }
                stack.get(1).assign(acme::boolean{true});
            }

            {
                auto v = stack.get(1).as<acme::boolean>();
                if ( v != acme::boolean{true}  ) { return false; }
                stack.get(1).assign(acme::boolean{false});
            }
        }

        {
            {
                auto v = stack.get(4).as<acme::undefined>();
                if ( v != acme::undefined{} ) { return false; }
                stack.get(4).assign(acme::boolean{true});
            }

            {
                auto v = stack.get(4).as<acme::boolean>();
                if ( v != acme::boolean{true} ) { return false; }
                stack.get(4).assign(acme::string{"World"sv});
            }

            {
                auto v = stack.get(4).as<acme::string>();
                if ( v != acme::string{"World"sv} ) { return false; }
                stack.get(4).assign(acme::undefined{});
            }
        }

        return true;

    }() == true, "[ACME stack] Test value reference assignment"
);
#endif

static_assert(

    []() constexpr
    {
        return is_undefined(acme::script_value{});

    }() == true, "[ACME acme::script_value] should default construct as undefined"
);

static_assert(

    []() constexpr
    {
        using namespace std::string_view_literals;
        using namespace acme::literals;
        using namespace acme;

        {
            auto v1 = acme::script_value{acme::number{10}};
            auto v2 = acme::script_value{acme::number{10}};

            if ( is_true(acme::number{10}) == false ) { return false; }

            if ( v1 != v2 )       { return false; }
            if ( not (v1 == v2) ) { return false; }
            if ( not is_number(v1) ) { return false; }

            if ( acme::to_double(v1)  != 10.0 ) { return false; }
            if ( acme::to_boolean(v1) != true ) { return false; }
        }

        {
            auto v1 = acme::script_value{acme::number{10}};
            auto v2 = acme::script_value{acme::number{20}};

            if ( v1 == v2 )       { return false; }
            if ( not (v1 != v2) ) { return false; }
        }

        {
            auto v1 = acme::script_value{acme::number{1}};
            auto v2 = acme::script_value{acme::number{0}};

            if ( v1.as<acme::number>() != acme::number{1} )         { return false; }
            if ( v2.as<acme::number>() != acme::number{0} )         { return false; }
        }

        {
            auto v1 = acme::script_value{10};
            auto v2 = acme::script_value{acme::string{"10"sv}};

            if ( v1 == v2 )       { return false; }
            if ( not (v1 != v2) ) { return false; }
        }

        return true;

    }() == true, "[ACME acme::script_value] should default construct as undefined"
);

#if 0
static_assert(

    []() constexpr
    {
        using namespace acme;

        auto v1 = acme::script_value{acme::number{10}};
        auto v2 = acme::script_value{acme::number{10.1}};

        if ( is_undefined(v2) or v2.as<acme::number>() != acme::number{10.1} )
        {
            return false;
        }

        {
            auto v3 = op_add(v1, v2);

            if ( is_undefined(v3) or v3.as<acme::number>() != acme::number{20.1} )
            {
                return false;
            }

            v3 = op_add(op_add(v1, v2), v1);
            if ( is_undefined(v3) or v3.as<acme::number>() != acme::number{30.1} )
            {
                return false;
            }

            v3 = op_add(op_add(v2, v2), v1);
            if ( is_undefined(v3) or v3.as<acme::number>() != acme::number{30.2} )
            {
                return false;
            }
        }

        return true;

    }() == true, "[ACME acme::script_value] should default construct as undefined"
);
#endif

TTS_CASE("Empty")
{
    TTS_EXPECT(true);
};
