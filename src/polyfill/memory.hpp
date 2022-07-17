#pragma once

#if not defined cmake_compile_test_std_construct_at

namespace xstd {

template <typename T, typename... Args, typename = decltype(::new (std::declval<void*>()) T(std::declval<Args>()...))>
[[nodiscard]] constexpr auto construct_at(T* p, Args&&... args) -> T*
{
    assert(p != nullptr);
    return ::new (static_cast<void*>(p)) T(std::forward<Args>(args)...);
}

} // namespace xstd

namespace std {

using xstd::construct_at;

} // namespace std

#endif /* not cmake_compile_test_std_construct_at */
