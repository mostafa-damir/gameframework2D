#pragma once
#ifndef __sachel_H__
#define __sachel_H__

#include "gfc_vector.h""
#include "entity.h"

typedef struct sachel_S
{
	int carryIncrease;
}sachelData;

Entity* sachel_new(Vector2D position);

Entity* sachel_get();

void sachel_draw(Entity* ent);

void sachel_think(Entity* ent);

void sachel_update(Entity* ent);


#endif //__sachel_H__