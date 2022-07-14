#pragma once

#include <catch2/catch.hpp>

#include <ECS/World.h>
#include <ECS/Query.h>
#include <ECS/EntityHandle.h>

#include <unordered_set>

namespace {
	struct ComponentA {
		ComponentA() = default;
		ComponentA(const ComponentA& other) = delete;
		ComponentA(ComponentA&& other) = default;

		ComponentA& operator = (const ComponentA& other) = delete;
		ComponentA& operator = (ComponentA&& other) = default;

		bool operator == (const ComponentA& other) const { return v0 == other.v0; }

		uint32_t v0 = 0;
	};

	struct ComponentB {
		ComponentB() = default;
		ComponentB(const ComponentB& other) = delete;
		ComponentB(ComponentB&& other) = default;

		ComponentB& operator = (const ComponentB& other) = delete;
		ComponentB& operator = (ComponentB&& other) = default;

		bool operator == (const ComponentB& other) const { return (v0 == other.v0) && (v1 == other.v1); }

		uint32_t v0 = 0;
		uint32_t v1 = 0;
	};

	struct ComponentC {
		ComponentC() = default;
		ComponentC(const ComponentC& other) = delete;
		ComponentC(ComponentC&& other) = default;

		ComponentC& operator = (const ComponentC& other) = delete;
		ComponentC& operator = (ComponentC&& other) = default;

		bool operator == (const ComponentC& other) const { return (v0 == other.v0) && (v1 == other.v1) && (v2 == other.v2); }

		uint32_t v0 = 0;
		uint32_t v1 = 0;
		uint32_t v2 = 0;
	};

	struct ComponentD {
		ComponentD() = default;
		ComponentD(const ComponentD& other) = delete;
		ComponentD(ComponentD&& other) = default;

		ComponentD& operator = (const ComponentD& other) = delete;
		ComponentD& operator = (ComponentD&& other) = default;

		bool operator == (const ComponentD& other) const { return (v0 == other.v0) && (v1 == other.v1) && (v2 == other.v2) && (v3 == other.v3); }

		uint32_t v0 = 0;
		uint32_t v1 = 0;
		uint32_t v2 = 0;
		uint32_t v3 = 0;
	};

	struct ComponentE {
		ComponentE() : m_SelfPtr(this) {}
		ComponentE(const ComponentE& other) = delete;
		ComponentE([[maybe_unused]] ComponentE&& other) : m_SelfPtr(this) {}

		ComponentE& operator = (const ComponentE& other) = delete;

		ComponentE& operator = ([[maybe_unused]]  ComponentE&& other) {
			m_SelfPtr = this;
			return *this;
		}

		ComponentE* m_SelfPtr;
	};

	struct ComponentF {
		ComponentF() = default;
		ComponentF(const ComponentF& other) = delete;
		ComponentF(ComponentF&& other) = default;

		ComponentF& operator = (const ComponentF& other) = delete;
		ComponentF& operator = (ComponentF&& other) = default;

		bool operator == (const ComponentF& other) const { return v0 == other.v0; }

		uint32_t v0 = 0;
	};
}

namespace ecs {
	namespace ut {
		template<typename T, typename ...TRest>
		struct GroupBitmask {
			static ecs::Bitset Get() {
				return ecs::Component<T>::GetTypeBitmask() | GroupBitmask<TRest...>::Get();
			}
		};

		template<typename T>
		struct GroupBitmask<T> {
			static ecs::Bitset Get() {
				return ecs::Component<T>::GetTypeBitmask();
			}
		};
	}
}