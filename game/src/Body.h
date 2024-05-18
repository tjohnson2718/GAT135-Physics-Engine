#pragma once
#include "raylib.h"
#include "raymath.h"

typedef enum
{
	BT_DYNAMIC,
	BT_KINEMATIC,
	BT_STATIC
} ncBodyType;

typedef enum
{
	FM_FORCE,
	FM_IMPULSE,
	FM_VELOCITY
} ncForceMode;

typedef struct ncBody
{
	ncBodyType type;
	// acceleration -> veclocity -> position
	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;
	Vector2 force;

	float mass;
	float inversMass; // 1 / mass (static = 0)
	float gravityScale;
	float damping;

	float restitution;

	Color color;

	struct ncBody* next;
	struct ncBody* prev;
} ncBody;

inline void ApplyForce(ncBody* body, Vector2 force, ncForceMode forceMode)
{
	if (body->type != BT_DYNAMIC) return;
	
	switch (forceMode)
	{
	case FM_FORCE:
		body->force = Vector2Add(body->force, force);
		break;
	case FM_IMPULSE:
		// applies a sudden change in momentum
		body->velocity = Vector2Add(body->velocity, Vector2Scale(force, body->inversMass));
		break;
	case FM_VELOCITY:
		body->velocity = force;
		break;
	}

}

inline void ClearForce(ncBody* body)
{
	body->force = Vector2Zero();
}

void Step(ncBody* body, float timestep);
