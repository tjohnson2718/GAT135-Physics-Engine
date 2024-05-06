#pragma once
#include "raylib.h"

typedef struct ncBody ncBody;

extern ncBody* bodies;
extern int bodyCount;
extern Vector2 ncGravity;

ncBody* CreateBody();
void DestroyBody(ncBody* body);
