#pragma once
#ifndef __stim_H__
#define __stim_H__

#include "gfc_vector.h"
#include "entity.h"

typedef struct stim_S
{
	int			healthPlus;
	int			pickedup;
	Bool		used;
}stimData;

Entity* stim_new(Vector2D position);


Entity* stim_get();
// draw the sprites
void stim_draw(Entity* ent);

//think - how it thinks
void stim_think(Entity* ent);

//updates sprite
void stim_update(Entity* ent);

#endif