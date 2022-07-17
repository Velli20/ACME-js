
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wextra-semi"
#endif /* __clang__ */

#define TTS_MAIN
#include <tts/tts.hpp>

#if defined(__clang__)
#pragma clang diagnostic pop
#endif /* __clang__ */

#include <iostream>

#include "memory/memory.hpp"
#include "memory/fixed_buffer_resource.hpp"
#include "base/base.hpp"
#include "string_pool/string_pool.hpp"

TTS_CASE("Check String pool reference counting")
{
    using namespace std::string_view_literals;

    std::byte buffer[1024];
    acme::fixed_buffer_resource mbr{buffer, sizeof(buffer)};

    acme::string_pool pool{std::addressof(mbr)};

    static constexpr auto k_test_lit = "test-string1"sv;

    // Interns same string literal twice. Expect only one pooled instance.

    [&]()
    {
        auto int1 = pool.intern(k_test_lit);
        TTS_EXPECT(int1.view() == k_test_lit);
        TTS_EXPECT(int1.reference_count() == 1);

        auto int2 = pool.intern(k_test_lit);
        TTS_EXPECT(int2.view() == k_test_lit);
        TTS_EXPECT(int1.reference_count() == 2);

        // Expect only one instance of the interned string literal.

        TTS_EXPECT(pool.count() == 1u);

        std::move(int1);
        std::move(int2);
    }();

    // Out of scope. Expected the interned strings to unreferenced and cleared from the pool.

    TTS_EXPECT(pool.count() == 0u);
    std::cout << "pool.count() " << pool.count() << '\n';

    // Re-insert same literal.

    {
        auto int1 = pool.intern(k_test_lit);
        TTS_EXPECT(int1.view() == k_test_lit);
        TTS_EXPECT(int1.reference_count() == 1);
    }

    std::cout << "pool.count() " << pool.count() << '\n';
    TTS_EXPECT(pool.count() == 0u);
};

TTS_CASE("Check String pool generic operations")
{
    using namespace std::string_view_literals;
    using string_array_type = typename std::vector<acme::string_pool::string_ref>;

    std::byte buffer[1024];
    acme::fixed_buffer_resource mbr{buffer, sizeof(buffer)};

    acme::string_pool pool{std::addressof(mbr)};

    static constexpr auto k_test_literals = std::array
    {
        "123"sv,
        "1234"sv,
        "12345"sv,
        "123456"sv
    };

    // Test container for interned string references.

    string_array_type strings{};
    strings.reserve(k_test_literals.size());

    // Do intern strings to the test container.

    for ( const auto& lit : k_test_literals )
    {
        strings.emplace_back(pool.intern(lit));
    }

    TTS_EXPECT(pool.count() == 4u);

    // Check that interned strings match original string literals. Expect
    // reference count to be 1 for each interned string.

    for ( std::size_t i{}; i != k_test_literals.size(); ++i )
    {
        TTS_EXPECT(strings[i].view() == k_test_literals[i]);
        TTS_EXPECT(strings[i].reference_count() == 1u);
    }

    // Create a copy of the test container.

    auto strings_copy = strings;

    // Expect the reference count to be 2 for each string.

    for ( std::size_t i{}; i != k_test_literals.size(); ++i )
    {
        TTS_EXPECT(strings_copy[i].view() == k_test_literals[i]);
        TTS_EXPECT(strings_copy[i].reference_count() == 2u);
    }

    // Clear original container. Expect the data to remain.

    strings.clear();

    for ( std::size_t i{}; i != k_test_literals.size(); ++i )
    {
        TTS_EXPECT(strings_copy[i].view() == k_test_literals[i]);
        TTS_EXPECT(strings_copy[i].reference_count() == 1u);
    }

    // Clear copy container. Expect all strings to be uninterned.

    strings_copy.clear();

    std::cout << "pool.count() " << pool.count() << '\n';
    TTS_EXPECT(pool.count() == 0u);
};


TTS_CASE("Concatanate")
{
    using namespace std::string_view_literals;
    using string_array_type = typename std::vector<acme::string_pool::string_ref>;

    std::byte buffer[1024];
    acme::fixed_buffer_resource mbr{buffer, sizeof(buffer)};

    acme::string_pool pool{std::addressof(mbr)};

    static constexpr struct
    {
        std::string_view s1;
        std::string_view s2;
        std::string_view s3;
    }
    k_test_literals[] =
    {
        { "123"sv,    "456"sv,   "123456"sv },
        { "1234"sv,   "56"sv,    "123456"sv },
        { "1"sv,      "23456"sv, "123456"sv }
    };

    string_array_type strings{};

    for ( const auto& [s1, s2, s3] : k_test_literals )
    {
        auto& s = strings.emplace_back(pool.concatanate(s1, s2));
        TTS_EXPECT(s.view() == s3);

        std::cout << "s \"" << s.view() << "\" != \"" << s3 << "\"\n";
    }

    TTS_EXPECT(pool.count() == 1u);
    strings.clear();
    TTS_EXPECT(pool.count() == 0u);

    auto s1 = pool.intern("123"sv);
    auto s2 = pool.intern("456"sv);
    TTS_EXPECT(pool.count() == 2u);
    TTS_EXPECT(s1.view() == "123"sv);
    TTS_EXPECT(s2.view() == "456"sv);

    auto s3 = pool.concatanate(s1, s2);
    TTS_EXPECT(s3.view() == "123456"sv);
    TTS_EXPECT(pool.count() == 3u);
};
