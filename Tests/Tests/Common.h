#pragma once

#include <catch2/catch.hpp>

#include <ECS/World.h>
#include <ECS/Query.h>
#include <ECS/EntityHandle.h>

#include <unordered_set>

namespace {
	struct ComponentA {
		ComponentA() = default;
		ComponentA(uint32_t v0a) : v0(v0a) {}
		ComponentA(ComponentA&&) = default;

		ComponentA(const ComponentA&) = delete;
		ComponentA& operator = (const ComponentA&) = delete;
		ComponentA& operator = (ComponentA&&) = delete;

		bool operator == (const ComponentA& other) const { return v0 == other.v0; }

		uint32_t v0 = 0;
	};

	struct ComponentB {
		ComponentB() = default;
		ComponentB(uint32_t v0a, uint32_t v1a) : v0(v0a), v1(v1a) {}
		ComponentB(ComponentB&&) = default;

		ComponentB(const ComponentB&) = delete;
		ComponentB& operator = (const ComponentB&) = delete;
		ComponentB& operator = (ComponentB&&) = delete;

		bool operator == (const ComponentB& other) const { return (v0 == other.v0) && (v1 == other.v1); }

		uint32_t v0 = 0;
		uint32_t v1 = 0;
	};

	struct ComponentC {
		ComponentC() = default;
		ComponentC(uint32_t v0a, uint32_t v1a, uint32_t v2a) : v0(v0a), v1(v1a), v2(v2a) {}
		ComponentC(ComponentC&&) = default;

		ComponentC(const ComponentC&) = delete;
		ComponentC& operator = (const ComponentC&) = delete;
		ComponentC& operator = (ComponentC&&) = delete;

		bool operator == (const ComponentC& other) const { return (v0 == other.v0) && (v1 == other.v1) && (v2 == other.v2); }

		uint32_t v0 = 0;
		uint32_t v1 = 0;
		uint32_t v2 = 0;
	};

	struct ComponentD {
		ComponentD() = default;
		ComponentD(uint32_t v0a, uint32_t v1a, uint32_t v2a, uint32_t v3a) : v0(v0a), v1(v1a), v2(v2a), v3(v3a) {}
		ComponentD(ComponentD&&) = default;

		ComponentD(const ComponentD&) = delete;
		ComponentD& operator = (const ComponentD&) = delete;
		ComponentD& operator = (ComponentD&&) = delete;

		bool operator == (const ComponentD& other) const { return (v0 == other.v0) && (v1 == other.v1) && (v2 == other.v2) && (v3 == other.v3); }

		uint32_t v0 = 0;
		uint32_t v1 = 0;
		uint32_t v2 = 0;
		uint32_t v3 = 0;
	};

	struct ComponentE {
		ComponentE() : m_SelfPtr(this) {}
		ComponentE(ComponentE&&) : m_SelfPtr(this) {}

		ComponentE(const ComponentE&) = delete;
		ComponentE& operator = (const ComponentE&) = delete;
		ComponentE& operator = (ComponentE&&) = delete;

		ComponentE* m_SelfPtr;
	};

	struct ComponentF {
		ComponentF() = default;
		ComponentF(uint32_t v0a) : v0(v0a) {}
		ComponentF(ComponentF&&) = default;

		ComponentF(const ComponentF&) = delete;
		ComponentF& operator = (const ComponentF&) = delete;
		ComponentF& operator = (ComponentF&&) = delete;

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