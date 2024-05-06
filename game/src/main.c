#include "Body.h"
#include "mathf.h"
#include "world.h"
#include "integrator.h"

#include "raylib.h"
#include "raymath.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define MAX_BODIES 10000

int main(void)
{
	InitWindow(1280, 720, "Physics Engine");
	SetTargetFPS(120);

	// initialize world
	ncGravity = (Vector2){ 0, 30 };

	ncBody* bodies = (ncBody*)malloc(sizeof(ncBody) * MAX_BODIES);
	assert(bodies);

	memset(bodies, 0, sizeof(ncBody));

	// add body to linked list
	bodies->prev = NULL;
	bodies->next = bodies->prev;
	int bodyCount = 0;

	// game loop
	while (!WindowShouldClose())
	{
		// update
		float dt = GetFrameTime();
		float fps = (float)GetFPS();

		Vector2 position = GetMousePosition();
		if (IsMouseButtonDown(0))
		{
			ncBody* body = CreateBody();
			body->position = position;
			body->mass = GetRandomFloatValue(1, 10);
			body->inversMass = 1 / body->mass;
			body->type = BT_DYNAMIC;
			body->damping = 0.5f;
			body->gravityScale = 1;
			ApplyForce(body, (Vector2) { GetRandomFloatValue(-100, 100), GetRandomFloatValue(-100, 100) }, FM_VELOCITY);
		}

		// apply force
		ncBody* body = bodies;
		while (body)
		{
			//ApplyForce(body, CreateVector2(0, -50), FM_FORCE);
			body = body->next;
		}

		// update bodies
		body = bodies;
		while (body)
		{
			Step(body, dt);
			body = body->next;
		}
		

		// render
		BeginDrawing();
		ClearBackground(BLACK);
		//stats
		DrawText(TextFormat("FPS: %.2f (%.2fms)", fps, 1000/fps), 10, 10, 20, LIME);
		DrawText(TextFormat("Frame: %.4f", dt), 10, 30, 20, LIME);

		DrawCircle((int)position.x, (int)position.y, 10, DARKPURPLE);

		//draw bodies
		body = bodies;
		while (body)
		{
			DrawCircle(body->position.x, body->position.y, 10.0f, PURPLE);
			body = body->next;
		}
		

		EndDrawing();
	}

	

	CloseWindow();
	free(bodies);

	return 0;
}