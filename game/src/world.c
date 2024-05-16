#include "world.h"
#include "Body.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

ncBody* bodies = NULL;
int bodyCount = 0;
Vector2 ncGravity;

/*
Assert if provided Body is not NULL
If 'prev' is not NULL, set 'prev->next' to 'body->next'
If 'next' is not NULL, set 'next->prev' to 'body->prev'
If body is the head, update head to 'body->next'
Decrement body count
Free the body
*/
ncBody* CreateBody(Vector2 position, float mass, ncBodyType bodyType)
{
    ncBody* body = (ncBody*)malloc(sizeof(ncBody));
    assert(body);

    memset(body, 0, sizeof(ncBody));
    body->position = position;
    body->mass = mass;
    body->inversMass = (bodyType == BT_DYNAMIC) ? 1 / mass : 0;
    body->type = bodyType;
    
    return body;
}

void AddBody(ncBody* body)
{
    assert(body);

    // add element to linked list
    body->prev = NULL;
    body->next = bodies;

    if (bodies != NULL) bodies->prev = body;

    bodies = body;

    bodyCount++;
}

/*
* Assert if provided Body is not NULL
If 'prev' is not NULL, set 'prev->next' to 'body->next'
If 'next' is not NULL, set 'next->prev' to 'body->prev'
If body is the head, update head to 'body->next'
Decrement body count
Free the body
*/

void DestroyBody(ncBody* body)
{
    assert(body);

    if (body->prev != NULL)
    {
        body->prev->next = body->next;
    }

    if (body->next != NULL)
    {
        body->next->prev = body->prev;
    }

    if (body == bodies)
    {
        bodies = body->next;
    }

    bodyCount--;
    free(body);
}




void DestroyAllBodies()
{

}
