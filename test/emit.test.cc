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
#include "memory/fixed_buffer_resource.hpp"
#include "base/base.hpp"
#include "string_pool/string_pool.hpp"

#include "tokenizer/tokenizer.hpp"
#include "var/script_value.hpp"
#include "parse/parser_context.hpp"
#include "ast/ast.hpp"
#include "parse/parse.hpp"
#include "bytecode/bytecode.hpp"
#include "virtual_machine/virtual_machine.hpp"
#include "emit/emit.hpp"

namespace {

auto do_test(const auto script, acme::emit_context& context)
{
    using namespace acme;
    using namespace acme::literals;
    using namespace std::string_view_literals;

    std::byte buffer[8192];
    acme::fixed_buffer_resource mbr{buffer, sizeof(buffer)};

    acme::parser script_parser{script, std::addressof(mbr)};
    script_parser.parse_all();

    auto& ast_nodes = script_parser.ast_nodes();
    TTS_EXPECT(ast_nodes.empty() == false);

    acme::emit(ast_nodes, context);
}

} // namespace

TTS_CASE("Evaluate 1+1")
{
    using namespace acme::literals;
    using namespace std::string_view_literals;

    acme::emit_context context{};

    static constexpr std::string_view k_script =
    R"(
        var i= 1 + 9;
        var foo= 110 + i;
        i=20;

        var n = i + foo;
        var s = i - foo;
    )";

    do_test(k_script, context);

    acme::virtual_machine vm{};
    vm.execute(context.bytecode());

    TTS_EXPECT(vm.locals().get(acme::identifier{"i"sv}) == acme::script_value{20});
    std::cout << "i: " << vm.locals().get(acme::identifier{"foo"sv})->get().as<acme::number>().value() << '\n';
    TTS_EXPECT(vm.locals().get(acme::identifier{"foo"sv}) == acme::script_value{120});
    TTS_EXPECT(vm.locals().get(acme::identifier{"n"sv}) == acme::script_value{140});
    TTS_EXPECT(vm.locals().get(acme::identifier{"s"sv}) == acme::script_value{-100});
};

TTS_CASE("Evaluate assign")
{
    using namespace acme::literals;
    using namespace std::string_view_literals;

    acme::emit_context context{};

    static constexpr std::string_view k_script =
    R"(
        var i= 1;

        i+=10;
    )";

    do_test(k_script, context);

    acme::virtual_machine vm{};
    vm.execute(context.bytecode());

    TTS_EXPECT(vm.locals().get(acme::identifier{"i"sv}) == acme::script_value{11});
};

TTS_CASE("Evaluate assign")
{
    using namespace acme::literals;
    using namespace std::string_view_literals;

    acme::emit_context context{};

    static constexpr std::string_view k_script =
    R"(
        var i= 3;

        i*=10;
        i = i - 5;

        var n= 3;

        n*=10;
        n = 5 - n;

        var s = n + i + 10 + 20 * 2 / 2;
    )";

    do_test(k_script, context);

    acme::virtual_machine vm{};
    vm.execute(context.bytecode());

    TTS_EXPECT(vm.locals().get(acme::identifier{"i"sv}) == acme::script_value{25});
    TTS_EXPECT(vm.locals().get(acme::identifier{"n"sv}) == acme::script_value{-25});
    TTS_EXPECT(vm.locals().get(acme::identifier{"s"sv}) == acme::script_value{(10 + 20 * 2 / 2)});
};

TTS_CASE("Evaluate comp")
{
    using namespace acme::literals;
    using namespace std::string_view_literals;

    acme::emit_context context{};

    static constexpr std::string_view k_script =
    R"(
        var i= 3;

        var n = 3;

        var s = n == i;
        var a = n != i;
    )";

    do_test(k_script, context);

    std::byte buffer[1024];
    acme::fixed_buffer_resource mbr{buffer, sizeof(buffer)};

    // FIXME: String pool memory buffer.

    acme::virtual_machine vm{std::addressof(mbr)};
    vm.execute(context.bytecode());

    TTS_EXPECT(vm.locals().get(acme::identifier{"s"sv}) == acme::script_value{acme::boolean{true}});
    TTS_EXPECT(vm.locals().get(acme::identifier{"a"sv}) == acme::script_value{acme::boolean{false}});
};

TTS_CASE("Evaluate comp2")
{
    using namespace acme::literals;
    using namespace std::string_view_literals;

    acme::emit_context context{};

    static constexpr std::string_view k_script =
    R"(
        var i= 3;

        var n = "3.0";

        var s = n == i;
        var a = n != i;
    )";

    do_test(k_script, context);

    std::byte buffer[1024];
    acme::fixed_buffer_resource mbr{buffer, sizeof(buffer)};

    // FIXME: String pool memory buffer.

    acme::virtual_machine vm{std::addressof(mbr)};
    vm.execute(context.bytecode());

    TTS_EXPECT(vm.locals().get(acme::identifier{"s"sv}) == acme::script_value{acme::boolean{true}});
    TTS_EXPECT(vm.locals().get(acme::identifier{"a"sv}) == acme::script_value{acme::boolean{false}});
};

TTS_CASE("Post inc")
{
    using namespace acme::literals;
    using namespace std::string_view_literals;

    acme::emit_context context{};

    static constexpr std::string_view k_script =
    R"(
        var i= 3;

        i++;
        i++;
        i++;
        i--;
    )";

    do_test(k_script, context);

    acme::virtual_machine vm{};
    vm.execute(context.bytecode());

    TTS_EXPECT(vm.locals().get(acme::identifier{"i"sv}) == acme::script_value{5});
};

TTS_CASE("Unary")
{
    using namespace acme::literals;
    using namespace std::string_view_literals;

    acme::emit_context context{};

    static constexpr std::string_view k_script =
    R"(
        var i= 3;
        var b= i == 3;
        var u;

        var s = typeof(i);
        var sb = typeof(b);
        var t = true;
        var st = typeof(t);
        var su = typeof(u);
    )";

    do_test(k_script, context);

    acme::virtual_machine vm{};
    vm.execute(context.bytecode());


    TTS_EXPECT(vm.locals().get(acme::identifier{"s"sv}) == acme::script_value{acme::string{"number"sv}});
    TTS_EXPECT(vm.locals().get(acme::identifier{"sb"sv}) == acme::script_value{acme::string{"boolean"sv}});
    TTS_EXPECT(vm.locals().get(acme::identifier{"st"sv}) == acme::script_value{acme::string{"boolean"sv}});
    TTS_EXPECT(vm.locals().get(acme::identifier{"su"sv}) == acme::script_value{acme::string{"undefined"sv}});
};

TTS_CASE("if else")
{
    using namespace acme::literals;
    using namespace std::string_view_literals;

    acme::emit_context context{};

    static constexpr std::string_view k_script =
    R"(
        var i, n;

        if ( true )
        {
            i = 10;
        }

        else
        {
            i = 40;
        }

        if ( false )
        {
            n = undefined;
        }

        else
        {
            n = 20;
        }

    )";

    do_test(k_script, context);

    acme::virtual_machine vm{};
    vm.execute(context.bytecode());

    TTS_EXPECT(vm.locals().get(acme::identifier{"i"sv}) == acme::script_value{10});
    TTS_EXPECT(vm.locals().get(acme::identifier{"n"sv}) == acme::script_value{20});
};

TTS_CASE("if string")
{
    using namespace acme::literals;
    using namespace std::string_view_literals;

    acme::emit_context context{};

    static constexpr std::string_view k_script =
    R"(
        var i= "hello ";
        var s= "hello2";
        var a= "hello2 ";
        var q= i + a;
        var f= q + " " + s;
    )";

    do_test(k_script, context);

    std::byte buffer[1024];
    acme::fixed_buffer_resource mbr{buffer, sizeof(buffer)};

    acme::virtual_machine vm{std::addressof(mbr)};
    vm.execute(context.bytecode());

    TTS_EXPECT(vm.locals().get(acme::identifier{"s"sv}) == acme::script_value{ acme::string {std::string_view{"hello2"}}});
};

TTS_CASE("for loop")
{
    using namespace acme::literals;
    using namespace std::string_view_literals;

    acme::emit_context context{};

    static constexpr std::string_view k_script =
    R"(
        var foo = 0;

        for ( var i = 0; i < 10; i++ )
        {
            foo += (1+1);
        }
    )";

    do_test(k_script, context);

    std::byte buffer[1024];
    acme::fixed_buffer_resource mbr{buffer, sizeof(buffer)};

    acme::virtual_machine vm{std::addressof(mbr)};
    vm.execute(context.bytecode());

    TTS_EXPECT(vm.locals().get(acme::identifier{"foo"sv}) == acme::script_value{20});
};

TTS_CASE("while loop")
{
    using namespace acme::literals;
    using namespace std::string_view_literals;

    acme::emit_context context{};

    static constexpr std::string_view k_script =
    R"(
        var foo = 0;
        let i = 0;
        while ( i < 10 )
        {
            foo += (1+1);
            i++;
        }
    )";

    do_test(k_script, context);

    acme::virtual_machine vm{};
    vm.execute(context.bytecode());

    std::cout << "foo: " << vm.locals().get(acme::identifier{"foo"sv})->get().as<acme::number>().value() << '\n';

    TTS_EXPECT(vm.locals().get(acme::identifier{"foo"sv}) == acme::script_value{20});
};

TTS_CASE("Break statement")
{
    using namespace acme::literals;
    using namespace std::string_view_literals;

    acme::emit_context context{};

    static constexpr std::string_view k_script =
    R"(
        var foo = 0;
        while ( true )
        {
            let i = 0;

            while ( i < 5 )
            {
                foo += (1+1);
                i++;
            }

            if ( foo == 20 )
            {
                break;
            }
        }
    )";

    do_test(k_script, context);

    acme::virtual_machine vm{};
    vm.execute(context.bytecode());

    std::cout << "foo: " << vm.locals().get(acme::identifier{"foo"sv})->get().as<acme::number>().value() << '\n';

    TTS_EXPECT(vm.locals().get(acme::identifier{"foo"sv}) == acme::script_value{20});
};

TTS_CASE("ternary")
{
    using namespace acme::literals;
    using namespace std::string_view_literals;

    acme::emit_context context{};

    static constexpr std::string_view k_script =
    R"(
        const foo = 120;

        let x = foo > 140 ? "success" : "fail";
        let n = foo < 140 ? "success" : "fail";
    )";

    do_test(k_script, context);

    std::byte buffer[1024];
    acme::fixed_buffer_resource mbr{buffer, sizeof(buffer)};

    acme::virtual_machine vm{std::addressof(mbr)};
    vm.execute(context.bytecode());

    //std::cout << "i: " << vm.locals().get(acme::identifier{"x"sv})->get().as<acme::string>().value() << '\n';

    TTS_EXPECT(vm.locals().get(acme::identifier{"n"sv}) == acme::script_value{ acme::string {std::string_view{"success"}}});
    TTS_EXPECT(vm.locals().get(acme::identifier{"x"sv}) == acme::script_value{ acme::string {std::string_view{"fail"}}});
};

