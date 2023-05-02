#pragma once
#ifndef __stunMine_H__
#define __stunMine_H__

#include "gfc_vector.h"
#include "entity.h"

typedef struct stunMine_S
{
	int			active;
	int			health;
}stunMineData;

Entity* stunMine_new(Vector2D position);

Entity* stunMine_get();

void stunMine_draw(Entity* ent);

void stunMine_think(Entity* ent);

void stunMine_update(Entity* ent);


#endif