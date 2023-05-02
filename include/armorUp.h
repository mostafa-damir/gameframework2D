#pragma once
#pragma once
#ifndef __armorUp_H__
#define __armorUp_H__

#include "gfc_vector.h""
#include "entity.h"

typedef struct armorUp_S
{
	int		carryIncrease;

}armorUpData;

Entity* armorUp_new(Vector2D position);

Entity* armorUp_get();

void armorUp_draw(Entity* ent);


void armorUp_think(Entity* ent);


void armorUp_update(Entity* ent);


#endif // __armorUp_H__
