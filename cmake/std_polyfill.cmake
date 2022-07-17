
include (CheckCXXSourceCompiles)

set(CXXSTD_CONSTRUCT_AT_TEST_SOURCE
[[

#include <memory>

struct S
{
    S(int v)
        : m_val{v}
    {}

    int m_val{};
};

int main()
{
    alignas(S) unsigned char storage[sizeof(S)];

    S* ptr = std::construct_at(reinterpret_cast<S*>(storage), 42);

    std::destroy_at(ptr);
}

]])

set(CXXSTD_LEXICOGRAPHICAL_COMPARE_THREE_WAY_TEST_SOURCE
[[

#include <algorithm>
#include <cctype>
#include <compare>
#include <string_view>
#include <utility>

int main()
{
    using namespace std::literals;

    auto cmp_icase = [](char x, char y) {
        const auto ux { std::toupper(x) };
        const auto uy { std::toupper(y) };
        return (ux < uy) ? std::strong_ordering::less:
               (ux > uy) ? std::strong_ordering::greater:
                           std::strong_ordering::equal;
    };

    for (const auto& [s1, s2] : { std::pair{"one"sv, "ONE"sv},
                                           {"two"sv, "four"sv},
                                           {"three"sv, "two"sv} }) {
        const auto res = std::lexicographical_compare_three_way(
            s1.cbegin(), s1.cend(), s2.cbegin(), s2.cend(), cmp_icase);
    }
}

]])

macro(std_polyfill_test TARGET)

    check_cxx_source_compiles("${CXXSTD_CONSTRUCT_AT_TEST_SOURCE}"                      CPP20_HAS_CONSTRUCT_AT)
    check_cxx_source_compiles("${CXXSTD_LEXICOGRAPHICAL_COMPARE_THREE_WAY_TEST_SOURCE}" CPP20_HAS_LEXICOGRAPHICAL_COMPARE_THREE_WAY)

    target_compile_definitions(${TARGET}
        PUBLIC
            $<$<BOOL:${CPP20_HAS_CONSTRUCT_AT}>:cmake_compile_test_std_construct_at>
            $<$<BOOL:${CPP20_HAS_LEXICOGRAPHICAL_COMPARE_THREE_WAY}>:cmake_compile_test_std_lexicographical_compare_three_way>
    )

endmacro()
