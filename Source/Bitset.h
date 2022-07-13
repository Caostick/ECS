#pragma once

#include <bitset>

namespace df {
	template<int NumBits>
	using Bitset = std::bitset<NumBits>;
}