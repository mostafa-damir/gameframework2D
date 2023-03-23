#ifndef __ARPICKUP_H__
#define __ARPICKUP_H__

#include "gfc_vector.h"
#include "entity.h"

typedef struct arPickup_S
{
	int		grab;
	Bool	grabbed;
}ARData;

Entity* arPickup_new(Vector2D position);

Entity* arPickup_get();

void arPickup_draw(Entity* ent);

void arPickup_think(Entity* ent);


void arPickup_update(Entity* ent);

#endif // !__ARPICKUP_H__
