#pragma once
#ifndef __healthKit_H__
#define __healthKit_H__

#include "gfc_vector.h"
#include "entity.h"

typedef struct healthKit_S
{
	int			healthPlus;
	Bool			used;
}healthKitData;

/**
*@brief spawn a new healthKit at the given location
* @param position where to spawn the healthKit at
* @return NULL on failure, or a pointer to the new bug
**/
Entity* healthKit_new(Vector2D position);


Entity* healthKit_get();
// draw the sprites
void healthKit_draw(Entity* ent);

//think - how it thinks
void healthKit_think(Entity* ent);

//updates sprite
void healthKit_update(Entity* ent);
#endif