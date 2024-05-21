#include "Body.h"
#include "world.h"
#include "integrator.h"

void Step(ncBody* body, float timestep)
{
	body->force = Vector2Add(body->force, Vector2Scale(Vector2Scale(ncGravity, body->gravityScale), body->mass));
	body->acceleration = Vector2Scale(body->force, body->inversMass);

	SemiImplicitEuler(body, timestep);
	float damping = 1 / (1 + (body->damping * timestep));
	body->velocity = Vector2Scale(body->velocity, damping);

	ClearForce(body);
}
