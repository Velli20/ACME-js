macro(precompiled_header TARGET)

    target_precompile_headers(${TARGET}
        PRIVATE
            <algorithm>
            <array>
            <atomic>
            <bit>
            <bitset>
            <chrono>
            <charconv>
            <compare>
            <concepts>
            <functional>
            <filesystem>
            <fstream>
            <limits>
            <initializer_list>
            <iterator>
            <iostream>
            <optional>
            <memory>
            <span>
            <sstream>
            <string>
            <system_error>
            <vector>
            <variant>
            <tuple>
            <type_traits>
            <utility>

            $<$<COMPILE_LANGUAGE:CXX>:${PROJECT_SOURCE_DIR}/src/polyfill/concepts.hpp>
            $<$<COMPILE_LANGUAGE:CXX>:${PROJECT_SOURCE_DIR}/src/polyfill/functional.hpp>
            $<$<COMPILE_LANGUAGE:CXX>:${PROJECT_SOURCE_DIR}/src/polyfill/bit.hpp>
            $<$<COMPILE_LANGUAGE:CXX>:${PROJECT_SOURCE_DIR}/src/polyfill/memory.hpp>
            $<$<COMPILE_LANGUAGE:CXX>:${PROJECT_SOURCE_DIR}/src/polyfill/algorithm.hpp>
            $<$<COMPILE_LANGUAGE:CXX>:${PROJECT_SOURCE_DIR}/src/polyfill/utility.hpp>
            $<$<COMPILE_LANGUAGE:CXX>:${PROJECT_SOURCE_DIR}/src/polyfill/pmr.hpp>

            <cstdlib>
            <cstddef>
            <cstdint>
            <ctime>
            <cstring>
            <cassert>
            <cstdio>
            <cmath>
    )

endmacro()