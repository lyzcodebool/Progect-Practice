#ifndef _RAND_COUNT_H_
#define _RAND_COUNT_H_

#include <chrono>
#include <random>
#define  seed std::chrono::high_resolution_clock::now().time_since_epoch().count()
std::mt19937 rand_generator(seed);
std::uniform_int_distribution<int>dist(10001,19999);
#endif
