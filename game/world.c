#include "world.h"
#include "Body.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

ncBody* head = NULL;
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
ncBody* CreateBody()
{
    ncBody* newBody = (ncBody*)malloc(sizeof(ncBody));
    asset(newBody);

    newBody->prev = NULL;
    newBody->next = head;

    if (head != NULL) {
        head->prev = newBody;
    }

    head = newBody;

    bodyCount++;

    return newBody;
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
    if (body != NULL)
    {
        assert(body);
    }

    if (body->prev != NULL)
    {
        body->prev->next = body->next;
    }

    if (body->next != NULL)
    {
        body->next->prev = body->prev;
    }

    if (body == head)
    {
        head = body->next;
    }

    bodyCount--;
    free(body);
}