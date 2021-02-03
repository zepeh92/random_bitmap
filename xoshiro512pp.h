#pragma once

#include <cstdint>
#include <limits>
#include <iterator>
#include <random>

// @ref: http://prng.di.unimi.it/xoshiro512plusplus.c
/* This is xoshiro512++ 1.0, one of our all-purpose, rock-solid
   generators. It has excellent (about 1ns) speed, a state (512 bits) that
   is large enough for any parallel application, and it passes all tests
   we are aware of.

   For generating just floating-point numbers, xoshiro512+ is even faster.

   The state must be seeded so that it is not everywhere zero. If you have
   a 64-bit seed, we suggest to seed a splitmix64 generator and use its
   output to fill s. */
class xoshiro512pp
{
public:
	using result_type = std::uint64_t;
	static constexpr std::size_t state_size = 8;
	static constexpr result_type min() { return 0; }
	static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }
	explicit xoshiro512pp(std::seed_seq& seq)
	{
		seq.generate(std::begin(state), std::end(state));
	}
	inline result_type operator()()
	{
		const uint64_t result = rotl(state[0] + state[2], 17) + state[2]; // xoshiro512p과의 차이점
		const result_type t = state[1] << 11;
		state[2] ^= state[0];
		state[5] ^= state[1];
		state[1] ^= state[2];
		state[7] ^= state[3];
		state[3] ^= state[4];
		state[4] ^= state[5];
		state[0] ^= state[6];
		state[6] ^= state[7];
		state[6] ^= t;
		state[7] = rotl(state[7], 21);
		return result;
	}
private:
	static inline result_type rotl(const result_type x, int k) { return (x << k) | (x >> (64 - k)); }
	result_type state[state_size];
};