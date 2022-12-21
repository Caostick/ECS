#pragma once

#include <bitset>

namespace ecs {
	template<int NumBits>
	using TBitset = std::bitset<NumBits>;
}