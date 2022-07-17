#pragma once

#if __has_include(<memory_resource>)

#include <memory_resource>
#include <vector>
#include <deque>
#include <string>
#include <map>
#include <forward_list>

namespace platform {

namespace pmr = std::pmr;

} // namespace platform

#elif __has_include(<experimental/memory_resource>)
#include <experimental/memory_resource>
#include <experimental/vector>
#include <experimental/deque>
#include <experimental/string>
#include <experimental/map>
#include <experimental/forward_list>

namespace platform {

namespace pmr = std::experimental::pmr;

} // namespace platform

namespace platform::memory {

auto default_resource() noexcept -> platform::pmr::memory_resource*;

} // namespace platform::memory

#endif /* __has_include <memory_resource> */
