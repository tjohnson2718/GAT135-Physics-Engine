#include "Spring.h"
#include "Body.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

ncSpring_t* ncSprings = NULL;

ncSpring_t* CreateSpring(struct ncBody* body1, struct ncBody* body2, float restLength, float k)
{
    ncSpring_t* spring = (ncSpring_t*)malloc(sizeof(ncSpring_t));
    assert(spring);

    memset(spring, 0, sizeof(ncSpring_t));

    return spring;
}

void AddSpring(ncSpring_t* spring)
{
    assert(spring);

    // add element to linked list
    spring->prev = NULL;
    spring->next = ncSprings;

    if (ncSprings) ncSprings->prev = spring;

    ncSprings = spring;
}

void DestroySpring(ncSpring_t* spring)
{
    assert(spring);

    if (spring->prev) spring->prev->next = spring->next;
    
    if (spring->next) spring->next->prev = spring->prev;
    
    if (spring == ncSprings) ncSprings = spring->next;
    
    free(spring);
}


//void DestroyAllSprings()
//{
//
//}

void ApplySpringForce(ncSpring_t* springs)
{
    for (ncSpring_t* spring = springs; spring; spring = spring->next)
    {
        Vector2 direction = Vector2Subtract(spring->body1->position, spring->body2->position);
        if (direction.x == 0 && direction.y == 0) continue;

        float length = Vector2Length(direction);
        float x = length - spring->restLength;
        float force = -spring->k * x;

        Vector2 ndirection = Vector2Normalize(direction);

        ApplyForce(spring->body1, Vector2Scale(ndirection, force), FM_FORCE);
        ApplyForce(spring->body2, Vector2Scale(Vector2Negate(ndirection), force), FM_FORCE);
    }
}
