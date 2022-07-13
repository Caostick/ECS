#pragma once

#include <stdint.h>

namespace ecs {
	class ThreadManager {
	public:
		virtual ~ThreadManager() {}
		virtual auto GetThreadCount() const -> uint32_t { return 1; }
		virtual auto GetThreadIndex() const -> uint32_t { return 0; }
	};
}