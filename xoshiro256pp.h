#pragma once

#include <cstdint>
#include <limits>
#include <iterator>
#include <random>

// @ref: http://prng.di.unimi.it/xoshiro256plusplus.c
/* This is xoshiro256++ 1.0, one of our all-purpose, rock-solid generators.
   It has excellent (sub-ns) speed, a state (256 bits) that is large
   enough for any parallel application, and it passes all tests we are
   aware of.

   For generating just floating-point numbers, xoshiro256+ is even faster.

   The state must be seeded so that it is not everywhere zero. If you have
   a 64-bit seed, we suggest to seed a splitmix64 generator and use its
   output to fill s. */
class xoshiro256pp
{
public:
	using result_type = std::uint64_t;
	static constexpr std::size_t state_size = 4;
	static constexpr result_type min() { return 0; }
	static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }
	explicit xoshiro256pp(std::seed_seq& seq)
	{
		seq.generate(std::begin(state), std::end(state));
	}
	inline result_type operator()()
	{
		const result_type result = rotl(state[0] + state[3], 23) + state[0]; // xoshiro256p와의 차이점
		const result_type t = state[1] << 17;
		state[2] ^= state[0];
		state[3] ^= state[1];
		state[1] ^= state[2];
		state[0] ^= state[3];
		state[2] ^= t;
		state[3] = rotl(state[3], 45);
		return result;
	}
private:
	static inline result_type rotl(const result_type x, int k) { return (x << k) | (x >> (64 - k)); }
	result_type state[state_size];
};