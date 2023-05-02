#pragma once
#ifndef __grenade_H__
#define __grenade_H__

#include "gfc_vector.h"
#include "entity.h"

typedef struct grenade_S
{
	int			pickedup;
	Bool		used;
}grenadeData;


Entity* grenade_new(Vector2D position);


Entity* grenade_get();
// draw the sprites
void grenade_draw(Entity* ent);

//think - how it thinks
void grenade_think(Entity* ent);

//updates sprite
void grenade_update(Entity* ent);


#endif // !__grenade_H__
