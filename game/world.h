#pragma once
#include "Body.h"

extern Body* bodies;
extern int bodyCount;

Body* CreateBody();
void DestroyBody(Body body);
