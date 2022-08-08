#pragma once

namespace acme {

enum class opcode : std::uint8_t
{
    binary_add = 0u,
    binary_sub,
    binary_mul,
    binary_div,
    binary_mod,
    binary_pow,
    compare_strict_equal,
    compare_equal,
    compare_less_than,
    compare_greater_than,
    compare_not,
    compare_instanceof,
    load_var,
    store_var,
    constant_double,
    constant_i32,
    constant_u32,
    push_bool_true,
    push_bool_false,
    push_undefined,
    push_null,
    duplicate_top,
    push_stack_frame,
    pop_stack_frame,
    constant_string,
    constant_identifier,
    initialize,
    typeof_value,
    unary_delete,
    unary_negate,
    jump_if_false,
    jump_if_true,
    jump_to,
    no_opearation,
};

[[nodiscard]] static constexpr auto to_string(opcode op) noexcept -> std::string_view
{
    using namespace std::string_view_literals;

    constexpr auto k_map = std::to_array<std::pair<opcode, std::string_view>>
    ({
        { opcode::binary_add,           "+"sv                   },
        { opcode::binary_sub,           "-"sv                   },
        { opcode::binary_mul,           "*"sv                   },
        { opcode::binary_div,           "/"sv                   },
        { opcode::binary_mod,           "%"sv                   },
        { opcode::binary_pow,           "**"sv                  },
        { opcode::compare_instanceof,   "INSTANCEOF"sv          },
        { opcode::compare_strict_equal, "==="sv                 },
        { opcode::compare_equal,        "=="sv                  },
        { opcode::compare_less_than,    "<"sv                   },
        { opcode::compare_greater_than, ">"sv                   },
        { opcode::compare_not,          "!"sv                   },
        { opcode::load_var,             "LOAD"sv                },
        { opcode::store_var,            "STORE"sv               },
        { opcode::constant_double,      "CONSTANT double"sv     },
        { opcode::constant_i32,         "CONSTANT i32"sv        },
        { opcode::constant_u32,         "CONSTANT u32"sv        },
        { opcode::push_bool_true,       "CONSTANT bool true"sv  },
        { opcode::push_bool_false,      "CONSTANT bool false"sv },
        { opcode::push_null,            "CONSTANT null"sv       },
        { opcode::duplicate_top,        "DUPLICATE TOP"sv       },
        { opcode::push_undefined,       "UNDEFINED"sv           },
        { opcode::push_stack_frame,     "PUSH STACK FRAME"sv    },
        { opcode::pop_stack_frame,      "POP STACK FRAME"sv     },
        { opcode::constant_string,      "CONSTANT string"sv     },
        { opcode::constant_identifier,  "CONSTANT identifier"sv },
        { opcode::initialize,           "INITIALIZE"sv          },
        { opcode::typeof_value,         "TYPEOF"sv              },
        { opcode::unary_delete,         "DELETE"sv              },
        { opcode::unary_negate,         "NEGATE"sv              },
        { opcode::jump_if_false,        "JUMP IF FALSE"sv       },
        { opcode::jump_if_true,         "JUMP IF TRUE"sv        },
        { opcode::jump_to,              "JUMP TO"sv             },
        { opcode::no_opearation,        "NO OPERATION"sv        },
    });

    for ( auto [o, s] : k_map )
    {
        if ( o == op )
        {
            return s;
        }
    }

    return {};
}

} // namespace acme
