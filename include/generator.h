#pragma once
#ifndef __generator_H__
#define __generator_H__

#include "gfc_vector.h"
#include "entity.h"

typedef struct generator_S
{
	int			pickedup;
	Bool		used;
}generatorData;


Entity* generator_new(Vector2D position);


Entity* generator_get();
// draw the sprites
void generator_draw(Entity* ent);

//think - how it thinks
void generator_think(Entity* ent);

//updates sprite
void generator_update(Entity* ent);


#endif // !__grenade_H__
