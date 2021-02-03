#pragma once

#include <cstdlib>
#include <cstdint>
#include <limits>

// https://codeforces.com/blog/entry/61587
// https://en.wikipedia.org/wiki/Linear_congruential_generator
template < std::uint32_t _Max = 10000000 >
class c_rand
{
public:
	using result_type = std::uint32_t; // unsigned int 가 아니면 함수가 끝나지 않는 버그가 있다.
	static constexpr std::size_t state_size = 0;
	static constexpr result_type min() { return 0; }
	static constexpr result_type max() { return _Max; }
	explicit c_rand(unsigned int seed)
	{
		std::srand(seed);
	}
	inline result_type operator()()
	{
		float v = std::rand() / (float)RAND_MAX; // 스케일링 하면 값 간 정밀도가 떨어지게 됨.
		if (1.0f <= v)
		{
			return c_rand::max();
		}
		result_type ret = static_cast<result_type>(v * c_rand::max());
		return ret;
	}
};