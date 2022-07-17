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
#include "render/render.hpp"

namespace {

void do_test(const auto script)
{
    using namespace acme;

    std::byte buffer[8192];
    acme::fixed_buffer_resource mbr{buffer, sizeof(buffer)};

    acme::parser script_parser{script, std::addressof(mbr)};

    script_parser.parse_all();

    const auto& ast_nodes = script_parser.ast_nodes();
    TTS_EXPECT(ast_nodes.empty() == false);

    auto res = acme::to_json(ast_nodes);
    std::cout << res << '\n';
}

} // namespace

TTS_CASE("Parse variables")
{
    static constexpr std::string_view k_script =
    R"(
        /*
        *  comment
        */

        #!/test/hashbang some_prog

        // Variables.

        var i= 1;
        var n;

        var dd = 10;

        var someS = "Test";
        var someBool = false;
        var someS2 = "Test";
        var someS3 = 'Test';
    )";

    do_test(k_script);
};

TTS_CASE("Parse ternary")
{
    static constexpr std::string_view k_script =
    R"(
        var ter = i > 10 ? i : 11;
    )";

    do_test(k_script);
};

TTS_CASE("Parse if else")
{
    static constexpr std::string_view k_script =
    R"(
        if ( (i + 10)++ )
        {
            var n = i;
        }

        else if ( false )
        {
            var n = i;
        }

        else
        {
            var n = i;
        }
    )";

    do_test(k_script);
};

TTS_CASE("Parse for loop")
{
    static constexpr std::string_view k_script =
    R"(
        for ( var i = 0; i < 10; i ++ ) { /* NOP */ }
    )";

    do_test(k_script);
};

TTS_CASE("Parse object")
{
    static constexpr std::string_view k_script =
    R"(
        var obj =
        {
            a: "foo", b: 42,
            c:
            {
                h : 10, j:200,

                set b(c)
                {
                    var a = 10;
                },

                get s() { a; }
            }
        };
    )";

    do_test(k_script);
};

TTS_CASE("Parse member expessions")
{
    static constexpr std::string_view k_script =
    R"(
        blaah (a,b,10)().aa;
    )";

    do_test(k_script);
};

TTS_CASE("Parse function")
{
    static constexpr std::string_view k_script =
    R"(
        function calcRectArea(width, height)
        {
            var blaah = false;
            // expected output: 30
            if ( blaah )
            {
                return 10;
            }

            else if ( !blaah )
            {
                return 9;
            }
            else
            {
                return 11;
            }
        }
    )";

    do_test(k_script);
};

TTS_CASE("Parse new")
{
    static constexpr std::string_view k_script =
    R"(
        var car1 = new Car (123, -1234, "BLAAH" );
    )";

    do_test(k_script);
};

TTS_CASE("Parse new.target")
{
    static constexpr std::string_view k_script =
    R"(
        if ( new.target ) { /* NOP */ }
    )";

    do_test(k_script);
};

TTS_CASE("Parse anonymous function")
{
    static constexpr std::string_view k_script =
    R"(
        function(width, height)
        {
            var blaah = false;
            // expected output: 30
            if ( blaah )
            {
                return 10;
            }

            else if ( !blaah )
            {
                return 9;
            }
            else
            {
                return 11;
            }
        }
    )";

    do_test(k_script);
};

#if 0
TTS_CASE("Parse arrow function and expression")
{
    {
        static constexpr std::string_view k_script =
        R"(
            let myFunction = (a, b) => a * b;
        )";

        do_test(k_script);
    }

    {
        static constexpr std::string_view k_script =
        R"(
            console.log(materials.map(material => material.length));
        )";

        do_test(k_script);
    }
};
#endif

TTS_CASE("Parse array literal")
{
    static constexpr std::string_view k_script =
    R"(
        var arr1 = [10];
        var arr2 = [10, 20, 30];
        var arr3 = [, 20, "some string",, 30];
        var arr4 = [/* Empty */];

        i = 10;
    )";

    do_test(k_script);
};

TTS_CASE("Parse labels")
{
    static constexpr std::string_view k_script =
    R"(
        for ( let i = 0; i < 10; i ++ )
        {
            some_label:

            if ( i > 1 )
            {
                break;
            }

            else if ( i > 2 )
            {
                break some_label;
            }

            else if ( i == 3 )
            {
                continue some_label;
            }
        }
    )";

    do_test(k_script);
};

TTS_CASE("Empty string literal")
{
    static constexpr std::string_view k_script =
    R"(

                var n = 1;

        for ( var i = 0; i < 10; i++ )
        {
            n *= 10;
        }
    )";

    do_test(k_script);
};


