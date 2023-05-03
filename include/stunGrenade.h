#pragma once
#ifndef __stunGrenade_H__
#define __stunGrenade_H__

#include "gfc_vector.h"
#include "entity.h"

typedef struct stunGrenade_S
{
	int			pickedup;
	Bool		used;
}stunGrenadeData;


Entity* stunGrenade_new(Vector2D position);


Entity* stunGrenade_get();
// draw the sprites
void stunGrenade_draw(Entity* ent);

//think - how it thinks
void stunGrenade_think(Entity* ent);

//updates sprite
void stunGrenade_update(Entity* ent);


#endif // !__stunGrenade_H__
