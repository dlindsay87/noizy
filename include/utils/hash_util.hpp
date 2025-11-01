#ifndef __HASH_UTIL_HPP__
#define __HASH_UTIL_HPP__

#include <cstddef>

namespace HashUtil
{

// Combines hashes
inline void hash_combine(std::size_t &seed, std::size_t value) noexcept
{
	seed ^= value + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

// Variadic hash function
template <typename T, typename... Rest>
inline std::size_t hash_values(const T &v, const Rest &...rest) noexcept
{
	std::size_t seed = std::hash<T>()(v);
	(hash_combine(seed, std::hash<Rest>()(rest)), ...);
	return seed;
}

} // namespace HashUtil

#endif
