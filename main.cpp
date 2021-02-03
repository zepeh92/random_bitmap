#include <iostream>
#include <cstdlib>
#include <map>
#include <random>
#include <ctime>
#include "bitmap_image.hpp"

#include "c_rand.h"
#include "well512.h"

// xoshiro256 series
#include "xoshiro256p.h"
#include "xoshiro256pp.h"
#include "xoshiro256starstar.h"

// xoshiro512 series
#include "xoshiro512p.h"
#include "xoshiro512pp.h"
#include "xoshiro512starstar.h"

// xoroshiro128 series
#include "xoroshiro128p.h"
#include "xoroshiro128pp.h"
#include "xoroshiro128starstar.h"

using namespace std;

inline namespace GlobalState
{
	int Loop = 1'000'000;
	int BmpWidth = 1000;
	int BmpHeight = 1000;
	int MaxLevel = 10;
	int Range = 0;
}

template < typename Engine >
void WriteBitmap(const char* path, Engine& e)
{
	const int range = BmpWidth * BmpHeight;
	uniform_int_distribution<int> d(0, Range - 1);

	map<int, int> vcnt;
	for (int i = 0; i < Loop; ++i)
	{
		auto v = d(e);
		vcnt[v] += 1;
	}

	bitmap_image image(BmpWidth, BmpHeight);
	image.clear(0);
	for (int y = 0; y < BmpHeight; ++y)
	{
		for (int x = 0; x < BmpWidth; ++x)
		{
			image.set_pixel(x, y, 255, 255, 255);
		}
	}

	for (auto [val, cnt] : vcnt)
	{
		const int x = val % BmpWidth;
		const int y = val / BmpWidth;

		int level = std::min(MaxLevel, cnt);
		int rgbv = 255 - static_cast<int>(255 * level / (float)MaxLevel);

		image.set_pixel(x, y, rgbv, rgbv, rgbv);
	}

	image.save_image(path);
}

#define WRITE_BITMAP(name, engine) WriteBitmap(#name##".bmp", engine);

void SetupEnv(int argc, const char* argv[])
{
	if (1 < argc)
	{
		const char* widthStr = argv[1];
		BmpWidth = std::stoi(widthStr);
	}

	if (2 < argc)
	{
		const char* heightStr = argv[2];
		BmpHeight = std::stoi(heightStr);
	}

	if (3 < argc)
	{
		const char* loopStr = argv[3];
		Loop = std::stoi(loopStr);
	}

	if (4 < argc)
	{
		const char* levelStr = argv[4];
		MaxLevel = std::stoi(levelStr);
	}

	Range = (BmpWidth * BmpHeight) - 1;

	cout << "WIDTH: " << BmpWidth << endl;
	cout << "HEIGHT: " << BmpHeight << endl;
	cout << "LOOP_COUNT: " << Loop << endl;
	cout << "MAX_LEVEL: " << MaxLevel << endl;
	cout << "RANDOM_RANGE: " << 0 << "~" << Range << endl << endl;

	if (Range <= 0)
	{
		throw std::invalid_argument("Range too small");
	}
}

int main(int argc, const char* argv[])
{
	cout << endl;

	try
	{
		SetupEnv(argc, argv);
	}
	catch (const std::exception& e)
	{
		cerr << e.what() << endl;
		cerr << "<width> <height> <loop_count> <level>" << endl;
		std::exit(1);
	}

	{
		std::random_device rd;
		c_rand<100'000> e(rd());
		WRITE_BITMAP(c_rand_max_100000, e);
	}

	{
		std::random_device rd;
		c_rand<1'000'000> e(rd());
		WRITE_BITMAP(c_rand_max_1000000, e);
	}

	{
		std::random_device rd;
		c_rand<10'000'000> e(rd());
		WRITE_BITMAP(c_rand_max_10000000, e);
	}

	{
		std::random_device rd;
		std::mt19937 e(rd());
		WRITE_BITMAP(mt19937, e);
	}

	{
		std::random_device rd;
		std::vector<well512::result_type> seeds;
		std::generate_n(std::back_inserter(seeds), well512::state_size, std::ref(rd));
		std::seed_seq seq(std::begin(seeds), std::end(seeds));
		well512 e(seq);
		WRITE_BITMAP(well512, e);
	}

	// xoshiro256
	{
		std::random_device rd;
		std::vector<xoshiro256p::result_type> seeds;
		std::generate_n(std::back_inserter(seeds), xoshiro256p::state_size, std::ref(rd));
		std::seed_seq seq(std::begin(seeds), std::end(seeds));
		xoshiro256p e(seq);
		WRITE_BITMAP(xoshiro256p, e);
	}

	{
		std::random_device rd;
		std::vector<xoshiro256pp::result_type> seeds;
		std::generate_n(std::back_inserter(seeds), xoshiro256pp::state_size, std::ref(rd));
		std::seed_seq seq(std::begin(seeds), std::end(seeds));
		xoshiro256pp e(seq);
		WRITE_BITMAP(xoshiro256pp, e);
	}

	{
		std::random_device rd;
		std::vector<xoshiro256starstar::result_type> seeds;
		std::generate_n(std::back_inserter(seeds), xoshiro256starstar::state_size, std::ref(rd));
		std::seed_seq seq(std::begin(seeds), std::end(seeds));
		xoshiro256starstar e(seq);
		WRITE_BITMAP(xoshiro256starstar, e);
	}

	// xoshiro512
	{
		std::random_device rd;
		std::vector<xoshiro512p::result_type> seeds;
		std::generate_n(std::back_inserter(seeds), xoshiro512p::state_size, std::ref(rd));
		std::seed_seq seq(std::begin(seeds), std::end(seeds));
		xoshiro512p e(seq);
		WRITE_BITMAP(xoshiro512p, e);
	}

	{
		std::random_device rd;
		std::vector<xoshiro512pp::result_type> seeds;
		std::generate_n(std::back_inserter(seeds), xoshiro512pp::state_size, std::ref(rd));
		std::seed_seq seq(std::begin(seeds), std::end(seeds));
		xoshiro512pp e(seq);
		WRITE_BITMAP(xoshiro512pp, e);
	}

	{
		std::random_device rd;
		std::vector<xoshiro512starstar::result_type> seeds;
		std::generate_n(std::back_inserter(seeds), xoshiro512starstar::state_size, std::ref(rd));
		std::seed_seq seq(std::begin(seeds), std::end(seeds));
		xoshiro512starstar e(seq);
		WRITE_BITMAP(xoshiro512starstar, e);
	}

	// xoroshiro128
	{
		std::random_device rd;
		std::vector<xoroshiro128p::result_type> seeds;
		std::generate_n(std::back_inserter(seeds), xoroshiro128p::state_size, std::ref(rd));
		std::seed_seq seq(std::begin(seeds), std::end(seeds));
		xoroshiro128p e(seq);
		WRITE_BITMAP(xoroshiro128p, e);
	}

	{
		std::random_device rd;
		std::vector<xoroshiro128pp::result_type> seeds;
		std::generate_n(std::back_inserter(seeds), xoroshiro128pp::state_size, std::ref(rd));
		std::seed_seq seq(std::begin(seeds), std::end(seeds));
		xoroshiro128pp e(seq);
		WRITE_BITMAP(xoroshiro128pp, e);
	}

	{
		std::random_device rd;
		std::vector<xoroshiro128starstar::result_type> seeds;
		std::generate_n(std::back_inserter(seeds), xoroshiro128starstar::state_size, std::ref(rd));
		std::seed_seq seq(std::begin(seeds), std::end(seeds));
		xoroshiro128starstar e(seq);
		WRITE_BITMAP(xoroshiro128starstar, e);
	}

	return 0;
}