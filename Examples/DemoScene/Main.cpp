#include <ECS/World.h>

#include "PointSimulationSystem.h"
#include "RectangleSimulationSystem.h"
#include "TriangleSimulationSystem.h"
#include "PositionHistorySystem.h"
#include "RectangleDrawSystem.h"
#include "PointDrawSystem.h"
#include "TriangleDrawSystem.h"

#include "Utils.h"

#include <GLFW/glfw3.h>

namespace {
	constexpr float DomainMinX = -10.0f;
	constexpr float DomainMinY = -10.0f;
	constexpr float DomainMaxX = 10.0f;
	constexpr float DomainMaxY = 10.0f;

	const glm::vec2 DomainMins = glm::vec2(DomainMinX, DomainMinY);
	const glm::vec2 DomainMaxs = glm::vec2(DomainMaxX, DomainMaxY);

	using PointEntity = ecs::EntityView<
		PointComponent,
		PositionComponent
	>;

	using TriangleEntity = ecs::EntityView<
		TriangleComponent,
		PositionComponent,
		PositionHistoryComponent
	>;
}


void InitECS(ecs::World& world) {
	world.Init();

	world.RegisterSystem<PointSimulationSystem>(DomainMins, DomainMaxs);
	world.RegisterSystem<RectangleSimulationSystem>(DomainMins, DomainMaxs);
	world.RegisterSystem<TriangleSimulationSystem>(DomainMins, DomainMaxs);
	world.RegisterSystem<RectangleDrawSystem>();
	world.RegisterSystem<PointDrawSystem>();
	world.RegisterSystem<TriangleDrawSystem>();
	world.RegisterSystem<PositionHistorySystem>();
}

void InitEntities(ecs::World& world) {
	// Create points
	for (int i = 0; i < 1000; ++i) {
		const float x = RndRange(DomainMinX, DomainMaxX);
		const float y = RndRange(DomainMinY, DomainMaxY);

		auto e = world.Construct<PointEntity>()
			.Construct<PointComponent>(RndRange(1.0f, 5.0f))
			.Construct<PositionComponent>(glm::vec2(x, y));
	}

	// Create triangles
	for (int i = 0; i < 5; ++i) {
		const float x = RndRange(DomainMinX, DomainMaxX);
		const float y = RndRange(DomainMinY, DomainMaxY);

		auto e = world.Construct<TriangleEntity>()
			.Construct<TriangleComponent>(RndRange(1.0f, 20.0f))
			.Construct<PositionComponent>(glm::vec2(RndRange(DomainMinX, DomainMaxX), RndRange(DomainMinY + 1.0f, DomainMinY + 5.0f)))
			.Construct<PositionHistoryComponent>();
	}
}

void ReleaseECS(ecs::World& world) {
	world.FinishAllCommands();
	world.Release();
}

void UpdateECS(ecs::World& world, float timeDelta) {
	constexpr float rectCreateTime = 1.0f;
	static float rectCreateTimeRemain = rectCreateTime;
	rectCreateTimeRemain -= timeDelta;

	// Create new entities
	if (rectCreateTimeRemain < 0.0f) {
		rectCreateTimeRemain += rectCreateTime;

		const float x = RndRange(DomainMinX, DomainMaxX);
		const float y = 15.0f;

		auto e = world.CreateEntity();
		world.AttachComponent<PositionComponent>(e, glm::vec2(x, y));
		world.AttachComponent<RectangleComponent>(e);

	}

	// Main ECS update methods
	world.Update(timeDelta);
	world.ExecuteCommands();
}


int main(void) {

	ecs::World world;

	InitECS(world);
	InitEntities(world);

	if (!glfwInit())
		return -1;

	GLFWwindow* window = glfwCreateWindow(600, 600, "Demo scene", nullptr, nullptr);
	if (!window) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	double time = glfwGetTime();
	while (!glfwWindowShouldClose(window)) {
		const double newTime = glfwGetTime();
		const float timeDelta = float(newTime - time);
		time = newTime;

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glLoadIdentity();
		glOrtho(DomainMinX, DomainMaxX, DomainMinY, DomainMaxY, -1.0f, 1.0f);

		UpdateECS(world, timeDelta);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	ReleaseECS(world);

	return 0;
}