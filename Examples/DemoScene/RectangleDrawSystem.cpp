#include "RectangleDrawSystem.h"

#include <ECS/Query.h>

#include <gl/glew.h>

void RectangleDrawSystem::Update(WorldView& world, [[maybe_unused]] float dt) {

	using Query = ecs::Query<>
		::Include<RectangleComponent>
		::Include<PositionComponent>;

	glBegin(GL_QUADS);

	for (auto e : Query::Iterate(world)) {
		auto& rectangleComponent = e.Get<RectangleComponent>();
		auto& positionComponent = e.Get<PositionComponent>();

		const float size = rectangleComponent.Size;
		const float colorFade = rectangleComponent.Size;

		glColor3f(colorFade * 0.4f, colorFade * 0.4f, colorFade * 1.0f);

		glVertex2f(positionComponent.Position[0] - size, positionComponent.Position[1] - size);
		glVertex2f(positionComponent.Position[0] + size, positionComponent.Position[1] - size);
		glVertex2f(positionComponent.Position[0] + size, positionComponent.Position[1] + size);
		glVertex2f(positionComponent.Position[0] - size, positionComponent.Position[1] + size);
	}

	glEnd();
}