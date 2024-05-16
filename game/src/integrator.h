#pragma once
#include "Body.h"


inline void ExplicitEuler(ncBody* body, float timestep)
{
	body->position = Vector2Add(body->position, Vector2Scale(body->velocity, timestep));
	body->velocity = Vector2Add(body->velocity, Vector2Scale(body->force, timestep));
}

inline void SemiImplicitEuler(ncBody* body, float timestep)
{
	body->velocity = Vector2Add(body->velocity, Vector2Scale(body->acceleration, timestep));
	body->position = Vector2Add(body->position, Vector2Scale(body->velocity, timestep));
}