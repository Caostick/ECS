#include "PointDrawSystem.h"

#include <ECS/Query.h>

#include <gl/glew.h>

void PointDrawSystem::Update(WorldView& world, [[maybe_unused]] float dt) {
	using Query = ecs::Query<>
		::Include<PointComponent>
		::Include<PositionComponent>;

	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POINTS);

	for (auto e : Query::Iterate(world)) {
		auto& positionComponent = e.Get<PositionComponent>();

		glVertex2f(positionComponent.Position[0], positionComponent.Position[1]);
	}

	glEnd();
}