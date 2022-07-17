#include "TriangleDrawSystem.h"

#include <ECS/Query.h>

#include <gl/glew.h>

void TriangleDrawSystem::Update(WorldView& world, [[maybe_unused]] float dt) {
	using QueryTriangles = ecs::Query<>
		::Include<TriangleComponent>
		::Include<PositionComponent>;

	constexpr float size = 0.2f;

	glColor3f(0.5f, 1.0f, 0.5f);
	glBegin(GL_TRIANGLES);
	for (auto e : QueryTriangles::Iterate(world)) {
		auto& positionComponent = e.Get<PositionComponent>();

		glVertex2f(positionComponent.Position[0] - size, positionComponent.Position[1]);
		glVertex2f(positionComponent.Position[0] + size, positionComponent.Position[1]);
		glVertex2f(positionComponent.Position[0], positionComponent.Position[1] + size * 2.0f);
	}
	glEnd();


	using QueryTrails = ecs::Query<>
		::Include<TriangleComponent>
		::Include<PositionComponent>
		::Include<PositionHistoryComponent>;

	glLineWidth(2.0f);
	glColor3f(1.0f, 0.7f, 0.7f);
	glBegin(GL_LINES);
	for (auto e : QueryTrails::Iterate(world)) {
		auto& positionComponent = e.Get<PositionComponent>();
		auto& positionHistoryComponent = e.Get<PositionHistoryComponent>();

		if (positionComponent.Position[1] > positionHistoryComponent.Position[1]) {
			glVertex2f(positionComponent.Position[0], positionComponent.Position[1]);
			glVertex2f(positionHistoryComponent.Position[0], positionHistoryComponent.Position[1]);
		}
	}
	glEnd();
	glLineWidth(1.0f);
}