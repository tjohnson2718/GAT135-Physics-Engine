#include "collision.h"
#include "contact.h"
#include "Body.h"
#include "mathf.h"
#include "raymath.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

bool Intersects(ncBody* body1, ncBody* body2)
{
	// if distance <= radius
	float distance = Vector2Distance(body1->position, body2->position);
	float radius = body1->mass + body2->mass;

	return (distance <= radius);
}

void CreateContacts(ncBody* bodies, ncContact_t** contacts)
{
	for (ncBody* body1 = bodies; body1; body1 = body1->next)
	{
		for (ncBody* body2 = body1->next; body2; body2 = body2->next)
		{
			if (body1 == body2) continue;
			if (body1->type != BT_DYNAMIC && body2->type != BT_DYNAMIC) continue;

			if (Intersects(body1, body2))
			{
				ncContact_t* contact = GenerateContact(body1, body2);
				AddContact(contact, contacts);
			}
		}
	}
}

ncContact_t* GenerateContact(ncBody* body1, ncBody* body2)
{
	ncContact_t* contact = (ncContact_t*)malloc(sizeof(ncContact_t));
	assert(contact);

	memset(contact, 0, sizeof(ncContact_t));

	contact->body1 = body1;
	contact->body2 = body2;

	Vector2 direction = Vector2Subtract(body1->position, body2->position);
	float distance = Vector2Length(direction);
	if (distance == 0)
	{
		direction = (Vector2){ GetRandomFloatValue(-0.05f, 0.05f), GetRandomFloatValue(-0.05f, 0.05f) };
	}

	float radius = (body1->mass + body2->mass);

	contact->depth = radius - distance;
	contact->normal = Vector2Normalize(direction);
	contact->restitution = (body1->restitution + body2->restitution) * 0.5f;

	return contact;
}

void SeparateContacts(ncContact_t* contacts)
{

}

void ResolveContacts(ncContact_t* contacts)
{

}