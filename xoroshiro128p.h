#pragma once

#include <cstdint>
#include <limits>
#include <iterator>
#include <random>

// @ref: http://prng.di.unimi.it/xoroshiro128plus.c
/* This is xoroshiro128+ 1.0, our best and fastest small-state generator
   for floating-point numbers. We suggest to use its upper bits for
   floating-point generation, as it is slightly faster than
   xoroshiro128**. It passes all tests we are aware of except for the four
   lower bits, which might fail linearity tests (and just those), so if
   low linear complexity is not considered an issue (as it is usually the
   case) it can be used to generate 64-bit outputs, too; moreover, this
   generator has a very mild Hamming-weight dependency making our test
   (http://prng.di.unimi.it/hwd.php) fail after 5 TB of output; we believe
   this slight bias cannot affect any application. If you are concerned,
   use xoroshiro128++, xoroshiro128** or xoshiro256+.

   We suggest to use a sign test to extract a random Boolean value, and
   right shifts to extract subsets of bits.

   The state must be seeded so that it is not everywhere zero. If you have
   a 64-bit seed, we suggest to seed a splitmix64 generator and use its
   output to fill s.

   NOTE: the parameters (a=24, b=16, b=37) of this version give slightly
   better results in our test than the 2016 version (a=55, b=14, c=36).
*/
class xoroshiro128p
{
public:
	using result_type = std::uint64_t;
	static constexpr std::size_t state_size = 2;
	static constexpr result_type min() { return 0; }
	static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }
	explicit xoroshiro128p(std::seed_seq& seq)
	{
		seq.generate(std::begin(state), std::end(state));
	}
	inline result_type operator()()
	{
		const result_type s0 = state[0];
		result_type s1 = state[1];
		const result_type result = s0 + s1;
		s1 ^= s0;
		state[0] = rotl(s0, 24) ^ s1 ^ (s1 << 16); // a, b
		state[1] = rotl(s1, 37); // c
		return result;
	}
private:
	static inline result_type rotl(const result_type x, int k) { return (x << k) | (x >> (64 - k)); }
	result_type state[state_size];
};