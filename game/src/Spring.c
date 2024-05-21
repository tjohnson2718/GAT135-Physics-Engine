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
    spring->body1 = body1;
    spring->body2 = body2;

    spring->restLength = restLength;
    spring->k = k;

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


void DestroyAllSprings()
{

}

void ApplySpringForcePosition(Vector2 position, ncBody* body, float restLength, float k, float damping)
{
    // Check if the body pointer is null; if so, exit the function
    if (!body) return;

    // Calculate the direction vector from the body's position to the given position
    Vector2 direction = Vector2Subtract(position, body->position);

    // If the direction vector is zero (i.e., positions are the same), exit the function
    if (direction.x == 0 && direction.y == 0) return;

    // Calculate the length of the direction vector (distance between the two positions)
    float length = Vector2Length(direction);

    // Calculate the displacement from the rest length
    float x = length - restLength;

    // Apply Hooke's Law (f = -kx) to determine the spring force
    float force = -k * x;

    // Normalize the direction vector
    Vector2 ndirection = Vector2Normalize(direction);

    // Calculate the damping force (opposing force due to velocity)
    float dampingForce = damping * Vector2DotProduct(body->velocity, ndirection);

    // Calculate the total force by combining spring force and damping force
    float totalForce = force + dampingForce;

    // Apply the total force to the body in the direction of the normalized vector
    ApplyForce(body, Vector2Scale(ndirection, -totalForce), FM_FORCE);
}

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
