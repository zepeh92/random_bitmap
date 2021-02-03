#pragma once

#include <cstdint>
#include <limits>
#include <iterator>
#include <random>

// @ref: http://prng.di.unimi.it/xoroshiro128plusplus.c
/* This is xoroshiro128++ 1.0, one of our all-purpose, rock-solid,
   small-state generators. It is extremely (sub-ns) fast and it passes all
   tests we are aware of, but its state space is large enough only for
   mild parallelism.

   For generating just floating-point numbers, xoroshiro128+ is even
   faster (but it has a very mild bias, see notes in the comments).

   The state must be seeded so that it is not everywhere zero. If you have
   a 64-bit seed, we suggest to seed a splitmix64 generator and use its
   output to fill s. */

class xoroshiro128pp
{
public:
	using result_type = std::uint64_t;
	static constexpr std::size_t state_size = 2;
	static constexpr result_type min() { return 0; }
	static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }
	explicit xoroshiro128pp(std::seed_seq& seq)
	{
		seq.generate(std::begin(state), std::end(state));
	}
	inline result_type operator()()
	{
		const result_type s0 = state[0];
		result_type s1 = state[1];
		const result_type result = rotl(s0 + s1, 17) + s0;
		s1 ^= s0;
		state[0] = rotl(s0, 49) ^ s1 ^ (s1 << 21); // a, b
		state[1] = rotl(s1, 28); // c
		return result;
	}
private:
	static inline result_type rotl(const result_type x, int k) { return (x << k) | (x >> (64 - k)); } // rotate left
	result_type state[state_size];
};