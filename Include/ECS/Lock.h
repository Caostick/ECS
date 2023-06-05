#pragma once

#include <mutex>
#include <shared_mutex>

namespace ecs {
	using Mutex = std::mutex;
	using Lock = std::shared_mutex;
	using WriteLock = std::unique_lock<Lock>;
	using ReadLock = std::shared_lock<Lock>;
	using ReadWriteLock = std::unique_lock<Mutex>;
}