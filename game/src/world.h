#pragma once
#include "raylib.h"
#include "Body.h"

typedef struct ncBody ncBody;

extern ncBody* bodies;
extern ncBody* fireworks;
extern int bodyCount;
extern Vector2 ncGravity;

ncBody* CreateBody(Vector2 position, float mass, ncBodyType bodyType);
void AddBody(ncBody* body);
void DestroyBody(ncBody* body);
void DestroyAllBodies();

