#include "Utils.h"

#include <stdlib.h>

auto RndRange(float min, float max) -> float {
	return ((rand() % 10000) / 10000.0f) * (max - min) + min;
}