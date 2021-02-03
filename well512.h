#pragma once

#include <cstdint>
#include <limits>
#include <random>

class well512
{
public:
	using result_type = unsigned int;
	static constexpr unsigned int state_size = 16;
	static constexpr result_type min() { return 0; }
	static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }

	explicit well512(std::seed_seq& seq)
	{
		seq.generate(std::begin(state), std::end(state));
	}

	inline result_type operator()()
	{
		const result_type z0 = state[(index + 15) & 0x0fu];
		const result_type z1 = xsl(16, state[index]) ^ xsl(15, state[(index + 13) & 0x0fu]);
		const result_type z2 = xsr(11, state[(index + 9) & 0x0fu]);
		state[index] = z1 ^ z2;
		const result_type t = xslm(5, 0xda442d24u, state[index]);
		index = (index + state_size - 1) & 0x0fu;
		state[index] = xsl(2, z0) ^ xsl(18, z1) ^ (z2 << 28) ^ t;
		return state[index];
	}
private:
	// xor-shift-right
	static inline result_type xsr(unsigned int shift, result_type value) { return value ^ (value >> shift); }
	// xor-shift-left
	static inline result_type xsl(unsigned int shift, result_type value) { return value ^ (value << shift); }
	// xor-shift-left and mask
	static inline result_type xslm(unsigned int shift, result_type mask, result_type value) { return value ^ ((value << shift) & mask); }

	unsigned int index = 0;
	result_type state[state_size];
};