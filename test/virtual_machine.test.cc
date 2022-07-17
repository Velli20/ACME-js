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

TTS_CASE("Virtual machine")
{
    using namespace acme;
    using namespace acme::literals;
    using namespace std::string_view_literals;

    constexpr auto k_numbers = std::to_array<acme::number_constant>
    ({
        number_constant{ .m_hash = "var1"_id },
        number_constant{ .m_i32  = 1 },
        number_constant{ .m_i32  = 3 },
        number_constant{ .m_i32  = 9 },
    });

    constexpr auto k_instructions = std::to_array<acme::instruction>
    ({
        instruction::make(opcode::constant_identifier, 0u),
        instruction::make(opcode::constant_i32,        1u),
        instruction::make(opcode::constant_i32,        2u),
        instruction::make(opcode::binary_add,          0u),
        instruction::make(opcode::initialize,          0u),
    });

    virtual_machine vm{};

    vm.execute(bytecode{std::span{k_instructions}, std::span{k_numbers}});

    TTS_EXPECT(vm.locals().get("var1"_id) == acme::script_value{4});
};

TTS_CASE("Virtual machine 2")
{
    using namespace acme;
    using namespace acme::literals;
    using namespace std::string_view_literals;

    constexpr auto k_numbers = std::to_array<acme::number_constant>
    ({
        number_constant{ .m_hash = "var1"_id },
        number_constant{ .m_i32  = 1 },
        number_constant{ .m_i32  = 3 },
        number_constant{ .m_i32  = 9 },
    });

    constexpr auto k_instructions = std::to_array<acme::instruction>
    ({
        instruction::make(opcode::constant_identifier, 0u),
        instruction::make(opcode::constant_i32,        1u),
        instruction::make(opcode::constant_i32,        2u),
        instruction::make(opcode::binary_add,          0u),
        instruction::make(opcode::initialize,          0u),


        instruction::make(opcode::constant_identifier, 0u),
        instruction::make(opcode::load_var,            0u),
        instruction::make(opcode::constant_i32,        3u),
        instruction::make(opcode::binary_add,          0u),
        instruction::make(opcode::constant_identifier, 0u),
        instruction::make(opcode::store_var,           0u),
    });

    virtual_machine vm{};

    vm.execute(bytecode{std::span{k_instructions}, std::span{k_numbers}});

    TTS_EXPECT(vm.locals().get("var1"_id) == acme::script_value{13});
};

