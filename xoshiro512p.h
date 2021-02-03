#pragma once

#include <cstdint>
#include <limits>
#include <iterator>
#include <random>

// @ref: http://prng.di.unimi.it/xoshiro512plus.c
/* This is xoshiro512+ 1.0, our generator for floating-point numbers with
   increased state size. We suggest to use its upper bits for
   floating-point generation, as it is slightly faster than xoshiro512**.
   It passes all tests we are aware of except for the lowest three bits,
   which might fail linearity tests (and just those), so if low linear
   complexity is not considered an issue (as it is usually the case) it
   can be used to generate 64-bit outputs, too.

   We suggest to use a sign test to extract a random Boolean value, and
   right shifts to extract subsets of bits.

   The state must be seeded so that it is not everywhere zero. If you have
   a 64-bit seed, we suggest to seed a splitmix64 generator and use its
   output to fill s.*/
class xoshiro512p
{
public:
	using result_type = std::uint64_t;
	static constexpr std::size_t state_size = 8;
	static constexpr result_type min() { return 0; }
	static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }
	explicit xoshiro512p(std::seed_seq& seq)
	{
		seq.generate(std::begin(state), std::end(state));
	}
	inline result_type operator()()
	{
		const result_type result = state[0] + state[2];
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