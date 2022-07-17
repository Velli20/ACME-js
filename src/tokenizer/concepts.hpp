#pragma once

namespace acme::concepts {

template <typename t>
concept character_like = std::integral<t> && (std::same_as<t, char> || std::same_as<t, unsigned char> || std::same_as<t, signed char>);

template <typename T>
concept tokenizer = requires(T tokenizer)
{
    { tokenizer.empty()            } -> std::same_as<bool>;
    { tokenizer.available()        } -> std::convertible_to<std::size_t>;
    { tokenizer.consume()          } -> std::same_as<std::optional<typename T::char_traits::char_type>>;
    { tokenizer.template peek<2>() } -> std::same_as<std::array<typename T::char_traits::char_type, 2>>;
    { tokenizer.peek()             } -> std::same_as<typename T::char_traits::char_type>;
    { tokenizer.eol()              } -> std::same_as<bool>;
    { tokenizer.next()             } -> std::same_as<typename T::token_item_type>;

    requires requires(typename T::size_type index)
    {
        { tokenizer.from(index)        } -> std::same_as<typename T::string_type>;
        { tokenizer.from(index, index) } -> std::same_as<typename T::string_type>;

        tokenizer.eat(index);
    };

    requires requires(typename T::checkpoint_type position)
    {
        { tokenizer.from(position) } -> std::same_as<typename T::string_type>;
    };

    requires requires(const typename T::token_item_type& token)
    {
        tokenizer.eat(token);
    };

};

template <typename F>
concept token_predicate = requires(F functor, acme::token_item input)
{
    { std::invoke(functor, input) } -> std::same_as<bool>;
};


} // namespace acme::concepts
