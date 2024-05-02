#include "Body.h"
#include "mathf.h"
#include "raylib.h"
#include "raymath.h"

#include <stdlib.h>
#include <assert.h>

#define MAX_BODIES 10000

int main(void)
{
	InitWindow(1280, 720, "Physics Engine");
	SetTargetFPS(120);

	Body* bodies = (Body*)malloc(sizeof(Body) * MAX_BODIES);
	assert(bodies);
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
			bodies[bodyCount].position = position;
			bodies[bodyCount].velocity = CreateVector2(GetRandomFloatValue(-5, 5), GetRandomFloatValue(-5, 5));
			bodyCount++;
		}

		// render
		BeginDrawing();
		ClearBackground(BLACK);
		DrawText(TextFormat("FPS: %.2f (%.2fms)", fps, 1000/fps), 10, 10, 20, LIME);
		DrawText(TextFormat("Frame: %.4f", dt), 10, 30, 20, LIME);

		DrawCircle((int)position.x, (int)position.y, 10, DARKPURPLE);

		// update bodies
		// update / draw bodies
        Body* body = bodies;
        while (body) // do while we have a valid pointer, will be NULL at the end of the list
        {
            // update body position
            // draw body
			body->position = Vector2Add(body->position, body->velocity);
			DrawCircle(body->position.x, body->position.y, 10.0f, PURPLE);

            body = body->next; // get next body
        }

		/*for (int i = 0; i < bodyCount; i++)
		{
			bodies[i].position = Vector2Add(bodies[i].position, bodies[i].velocity);
			DrawCircle(bodies[i].position.x, bodies[i].position.y, 10.0f, PURPLE);
		}*/

		EndDrawing();
	}

	CloseWindow();
	free(bodies);

	return 0;
}