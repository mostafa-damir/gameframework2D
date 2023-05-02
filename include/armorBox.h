#pragma once
#ifndef __armorBox_H__
#define __armorBox_H__

#include "gfc_vector.h"
#include "entity.h"

typedef struct armorBox_S
{
	int		armorUp;

}armorBoxData;

Entity* armorBox_new(Vector2D position);

Entity* armorBox_get();

void armorBox_draw(Entity* ent);


void armorBox_think(Entity* ent);


void armorBox_update(Entity* ent);


#endif // __armorBox_H__
