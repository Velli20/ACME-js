#pragma once

namespace acme {

template <class... type_sequence>
struct overloaded : type_sequence...
{
    using type_sequence::operator()...;
};

template <class... type_sequence>
overloaded(type_sequence...) -> overloaded<type_sequence...>;

} // namespace acme
