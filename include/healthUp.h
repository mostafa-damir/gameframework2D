#pragma once
#ifndef __healthUp_H__
#define __healthUp_H__

#include "gfc_vector.h"
#include "entity.h"

typedef struct healthUp_S
{
	int		healthIncrease;

}healthUpData;

Entity* healthUp_new(Vector2D position);

Entity* healthUp_get();

void healthUp_draw(Entity* ent);


void healthUp_think(Entity* ent);


void healthUp_update(Entity* ent);


#endif // __healthUp_H__
