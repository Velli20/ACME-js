
macro(compiler_options TARGET)

    target_compile_features(${TARGET}
        PRIVATE
            cxx_std_20
    )

    # TODO:
    # add_compile_options($<$<COMPILE_LANG_AND_ID:C,CXX,GNU>:-fmax-errors=1>)
    # add_compile_options($<$<COMPILE_LANG_AND_ID:C,CXX,AppleClang,Clang>:-ferror-limit=1>)

    # -Wpedantic means 'Issue all the
    # warnings demanded by strict ISO C++'.
    target_compile_options(${TARGET}
        PRIVATE
            $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:
                -fconstexpr-steps=1271242
            >

            $<$<CXX_COMPILER_ID:Clang,AppleClang,GNU>:
                -Werror
                -Wfatal-errors
                $<$<CONFIG:Debug>:-g;-O0>
                $<$<CONFIG:Release>:-O3>
            >

            $<$<CXX_COMPILER_ID:Clang,AppleClang>:
                -Wcomma
                -Wextra-semi
                -Wimplicit-fallthrough
                -Wmost
                -Wmove
                -Wnewline-eof
            >

            # MSVC

            $<$<CXX_COMPILER_ID:MSVC>:
                #/W4
                /std:c++latest
                /utf-8
                #/permissive # enforce standards conformance on MSVC
                $<$<CONFIG:Debug>:/Od;/MTd>
                $<$<CONFIG:Release>:/O2;/Ob2;/MT>
            #    $<$<CONFIG:Release>:/GL> # For LTO build
            >

            # Clang LTO

            $<$<AND:$<CXX_COMPILER_ID:Clang>,$<CONFIG:Release>>:
                -flto
            >

            $<$<AND:$<CXX_COMPILER_ID:Clang,AppleClang,GNU>,$<BOOL:${CONFIG_USE_SANITZERS}>>:
                -fsanitize=undefined
                -fsanitize=address
                -fsanitize=bounds
                -fsanitize-address-use-after-scope
                #-fsanitize-memory-track-origins

                -fsanitize=alignment
                -fsanitize=bool
                -fsanitize=bounds
                -fsanitize=enum
                -fsanitize=float-cast-overflow
                -fsanitize=float-divide-by-zero
                -fsanitize=integer-divide-by-zero
                -fsanitize=nonnull-attribute
                -fsanitize=null
                #-fsanitize=object-size
                -fsanitize=return
                -fsanitize=returns-nonnull-attribute
                -fsanitize=shift
                -fsanitize=signed-integer-overflow
                -fsanitize=unreachable
                -fsanitize=vla-bound

            >
            $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<BOOL:${CONFIG_USE_SANITZERS}>>:
                /fsanitize=address
            >
    )

    target_link_libraries(${TARGET}
        PUBLIC
            $<$<CXX_COMPILER_ID:Clang,AppleClang,GNU>:-fsanitize=undefined>
            $<$<CXX_COMPILER_ID:Clang,AppleClang,GNU>:-fsanitize=address>
            $<$<CXX_COMPILER_ID:Clang,AppleClang,GNU>:-fsanitize=bounds>
    )

endmacro()
