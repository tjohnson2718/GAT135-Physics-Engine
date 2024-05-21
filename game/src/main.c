#include "Body.h"
#include "Spring.h"
#include "mathf.h"
#include "world.h"
#include "integrator.h"
#include "force.h"
#include "render.h"
#include "editor.h"
#include "collision.h"
#include "contact.h"

#include "raylib.h"
#include "raymath.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define MAX_BODIES 10000

int main(void)
{
	ncBody* selectedBody = NULL;
	ncBody* connectBody = NULL;
	float fixedTimeStep = 1.0f / 60.0f;
	float timeAccumulator = 0.0f;

	InitWindow(1280, 720, "Physics Engine");
	InitEditor();
	SetTargetFPS(120);

	// initialize world

	// game loop
	while (!WindowShouldClose())
	{
		ncGravity = (Vector2){ 0, ncEditorData.GravitySliderValue };

		// update
		float dt = GetFrameTime();
		float fps = (float)GetFPS();

		Vector2 position = GetMousePosition();
		ncScreenZoom += GetMouseWheelMove() * 0.2f;
		ncScreenZoom = Clamp(ncScreenZoom, 0.1f, 10);

		UpdateEditor(position);

		selectedBody = GetBodyIntersect(bodies, position);
		if (selectedBody)
		{
			Vector2 screen = ConvertWorldToScreen(selectedBody->position);
			DrawCircleLines((int)screen.x, (int)screen.y, ConvertWorldToPixel(selectedBody->mass * 0.5f) + 5, YELLOW);
		}

		// create body
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonDown(MOUSE_BUTTON_LEFT) && IsKeyDown(KEY_LEFT_SHIFT))
		{
			ncBody* body = CreateBody(ConvertScreenToWorld(position), ncEditorData.MassSliderValue, ncEditorData.BodyTypeDropDownActive);
			body->damping = ncEditorData.DampingSliderValue;
			body->gravityScale = ncEditorData.GS_SliderValue;

			body->color = ColorFromHSV(GetRandomFloatValue(0, 360), 1, 1);
			body->restitution = 0.8f;

			AddBody(body);
		}

		// create static body
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && IsKeyDown(KEY_LEFT_CONTROL))
		{
			ncBody* body = CreateBody(ConvertScreenToWorld(position), 3, BT_STATIC);
			body->damping = 0;
			body->gravityScale = 0;
			body->color = ColorFromHSV(GetRandomFloatValue(0, 360), 1, 1);
			body->restitution = 0;

			AddBody(body);
		}

		// connect springs
		if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && selectedBody) connectBody = selectedBody;
		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && connectBody) DrawLineBodyToPosition(connectBody, position);
		if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT) && connectBody)
		{
			if (selectedBody && selectedBody != connectBody)
			{
				ncSpring_t* spring = CreateSpring(connectBody, selectedBody, Vector2Distance(connectBody->position, selectedBody->position), 20);
				AddSpring(spring);
			}
		}

		// Add delta time to time accumulator
		timeAccumulator += dt;

		// Physics simulation
		while (timeAccumulator >= fixedTimeStep)
		{
			// Apply gravitation and other forces
			ApplyGravitation(bodies, ncEditorData.GravitationSliderValue);
			ApplySpringForce(ncSprings);

			// Update bodies
			for (ncBody* body = bodies; body; body = body->next)
			{
				Step(body, fixedTimeStep);
			}

			// Destroy all contacts
			ncContact_t* contacts = NULL;

			// Create contacts
			CreateContacts(bodies, &contacts);

			// Separate contacts
			SeparateContacts(contacts);

			// Resolve contacts
			ResolveContacts(contacts);

			// Subtract fixed time step from time accumulator
			timeAccumulator -= fixedTimeStep;
		}

		//// apply gravitation
		//ApplyGravitation(bodies, ncEditorData.GravitationSliderValue);
		//ApplySpringForce(ncSprings);

		//// update bodies
		//for (ncBody* body = bodies; body; body = body->next)
		//{
		//	Step(body, dt);
		//}

		// collision
		ncContact_t* contacts = NULL;
		CreateContacts(bodies, &contacts);
		SeparateContacts(contacts);
		ResolveContacts(contacts);

		// render
		BeginDrawing();
		ClearBackground(BLACK);

		DrawEditor(position);
		//stats
		DrawText(TextFormat("FPS: %.2f (%.2fms)", fps, 1000/fps), 10, 10, 20, LIME);
		DrawText(TextFormat("Frame: %.4f", dt), 10, 30, 20, LIME);

		//draw bodies
		for (ncBody* body = bodies; body; body = body->next)
		{
			Vector2 screen = ConvertWorldToScreen(body->position);
			DrawCircle((int)screen.x, (int)screen.y, ConvertWorldToPixel(body->mass * 0.5f), body->color);
		}

		//draw contacts
		for (ncContact_t* contact = contacts; contact; contact = contact->next)
		{
			Vector2 screen = ConvertWorldToScreen(contact->body1->position);
			DrawCircle((int)screen.x, (int)screen.y, ConvertWorldToPixel(contact->body1->mass * 0.5f), RED);
		}

		//draw springs
		for (ncSpring_t* spring = ncSprings; spring; spring = spring->next)
		{
			Vector2 screen1 = ConvertWorldToScreen(spring->body1->position);
			Vector2 screen2 = ConvertWorldToScreen(spring->body2->position);
			DrawLine((int)screen1.x, (int)screen1.y, (int)screen2.x, (int)screen2.y, YELLOW);
		}
		
		EndDrawing();
	}

	

	CloseWindow();
	free(bodies);

	return 0;
}