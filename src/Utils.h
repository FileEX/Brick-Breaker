#pragma once
#include <cstdint>
#include <array>
#include <random>

#define M_PI       3.14159265358979323846f   // pi

static int GetRandomNumber(int min, int max)
{
	static std::random_device rd;
	static std::mt19937 rng(rd());
	std::uniform_int_distribution<int> dist(min, max);

	return dist(rng);
}
