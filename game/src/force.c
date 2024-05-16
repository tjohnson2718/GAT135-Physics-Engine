#include "force.h"
#include "Body.h"

void ApplyGravitation(ncBody* bodies, float strength)
{
	for (ncBody* body1 = bodies; body1; body1 = body1->next)
	{
		for (ncBody* body2 = bodies; body2; body2 = body2->next)
		{
			if (body1 == body2) continue;
			Vector2 direction = Vector2Subtract(body1->position, body2->position);
			float distance = Vector2Length(direction);

			distance = fmaxf(distance, 1.0f);
			float force = (body1->mass * body2->mass / (distance * distance)) * strength;

			direction = Vector2Scale(Vector2Normalize(direction), force);

			ApplyForce(body1, Vector2Negate(direction), FM_FORCE);
			ApplyForce(body2, direction, FM_FORCE);
		}
	}
}