#pragma once

#if not defined __cpp_lib_bind_front

// https://en.cppreference.com/w/cpp/header/functional

namespace xstd {

template <typename Func, typename BoundArgsTuple, typename... CallArgs>
constexpr auto bind_front_caller(Func&& func, BoundArgsTuple&& boundArgsTuple, CallArgs&&... callArgs) -> decltype(auto)
{
    return std::apply(
        [&func, &callArgs...](auto&&... boundArgs) -> decltype(auto)
        {
            return std::invoke(std::forward<Func>(func),
                std::forward<decltype(boundArgs)>(boundArgs)..., std::forward<CallArgs>(callArgs)...);
        }, std::forward<BoundArgsTuple>(boundArgsTuple)
    );
}

template <typename Func, typename... BoundArgs>
class bind_front_t
{
    public:
    template <typename F, typename... BA, std::enable_if_t<!(sizeof...(BA) == 0 && std::is_base_of_v<bind_front_t, std::decay_t<F>>), bool> = true>
    explicit bind_front_t(F&& f, BA&&... ba) : func_(std::forward<F>(f)), boundArgs_(std::forward<BA>(ba)...)
    {
    }

    template <typename... CallArgs>
    auto operator()(CallArgs&&... callArgs) & -> std::invoke_result_t<Func&, BoundArgs&..., CallArgs...>
    {
        return bind_front_caller(func_, boundArgs_, std::forward<CallArgs>(callArgs)...);
    }

    template <typename... CallArgs>
    auto operator()(
        CallArgs&&... callArgs) const& -> std::invoke_result_t<Func const&, BoundArgs const&..., CallArgs...>
    {
        return bind_front_caller(func_, boundArgs_, std::forward<CallArgs>(callArgs)...);
    }

    template <typename... CallArgs>
    auto operator()(CallArgs&&... callArgs) && -> std::invoke_result_t<Func, BoundArgs..., CallArgs...>
    {
        return bind_front_caller(std::move(func_), std::move(boundArgs_), std::forward<CallArgs>(callArgs)...);
    }

    template <typename... CallArgs>
    auto operator()(CallArgs&&... callArgs) const&& -> std::invoke_result_t<Func const, BoundArgs const..., CallArgs...>
    {
        return bind_front_caller(std::move(func_), std::move(boundArgs_), std::forward<CallArgs>(callArgs)...);
    }

    private:

    Func func_;
    std::tuple<BoundArgs...> boundArgs_;
};

/// \brief The function template bind_front generates a forwarding call wrapper
/// for f. Calling this wrapper is equivalent to invoking f with its first
/// sizeof...(Args) parameters bound to args. In other words, std::bind_front(f,
/// bound_args...)(call_args...) is equivalent to std::invoke(f, bound_args...,
/// call_args....).
///
/// \details Copied implementation from paper:
/// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0356r5.html
template <typename Func, typename... BoundArgs>
constexpr auto bind_front(Func&& func, BoundArgs&&... boundArgs)
{
    return detail::bind_front_t<std::decay_t<Func>, std::unwrap_ref_decay_t<BoundArgs>...> {
        std::forward<Func>(func), std::forward<BoundArgs>(boundArgs)...
    };
}

} // namespace xstd

namespace std {

using xstd::bind_front;

} // namespace std

#endif /* not __cpp_lib_bind_front */
