#pragma once

namespace acme::concepts {

template <typename T>
concept container_like = requires(T& c)
{
    requires std::default_initializable<T>;
    requires std::destructible<T>;
    requires std::signed_integral<typename T::difference_type>;
    requires std::unsigned_integral<typename T::size_type>;

    c.begin();
    c.end();
    c.cbegin();
    c.cend();
    c.size();
};

} // namespace acme::concepts

