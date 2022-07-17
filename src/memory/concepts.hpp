#pragma once

namespace acme::concepts {

template <typename T>
concept nullable_type = requires(T value)
{
    { value == nullptr } -> std::same_as<bool>;
    { value != nullptr } -> std::same_as<bool>;
};

template <typename T>
concept smart_pointer = requires(T value)
{
    value.get();

    nullable_type<decltype(value.get())>;
};

} // acme::concepts
