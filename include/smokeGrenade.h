#pragma once
#ifndef __smokeGrenade_H__
#define __smokeGrenade_H__

#include "gfc_vector.h"
#include "entity.h"

typedef struct smokeGrenade_S
{
	int			pickedup;
	Bool		used;
}smokeGrenadeData;


Entity* smokeGrenade_new(Vector2D position);


Entity* smokeGrenade_get();
// draw the sprites
void smokeGrenade_draw(Entity* ent);

//think - how it thinks
void smokeGrenade_think(Entity* ent);

//updates sprite
void smokeGrenade_update(Entity* ent);


#endif // !__grenade_H__
