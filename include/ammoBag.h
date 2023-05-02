#pragma once
#ifndef __ammoBag_H__
#define __ammoBag_H__

#include "gfc_vector.h"
#include "entity.h"

typedef struct ammoBag_S
{
	int			carryIncrease;
}ammoBagData;

Entity* ammoBag_new(Vector2D position);

Entity* ammoBag_Get();

void ammoBag_draw(Entity* ent);

void ammoBag_think(Entity* ent);

void ammoBag_update(Entity* ent);
#endif