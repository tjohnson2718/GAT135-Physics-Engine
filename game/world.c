#include "world.h"
#include <assert.h>

Body* head = NULL;
Body* bodies = NULL;
int bodyCount = 0;

/*
Assert if provided Body is not NULL
If 'prev' is not NULL, set 'prev->next' to 'body->next'
If 'next' is not NULL, set 'next->prev' to 'body->prev'
If body is the head, update head to 'body->next'
Decrement body count
Free the body
*/
Body* CreateBody()
{
    
    Body* newBody = (Body*)malloc(sizeof(Body));
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

void DestroyBody(Body* body)
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

    body--;
    free(body);
}