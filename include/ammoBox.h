#pragma once
#ifndef __ammoBox_H__
#define __ammoBox_H__

#include "gfc_vector.h"
#include "entity.h"

typedef struct ammoBox_S
{
	int			ammoPlus;
}ammoBoxData;

Entity* ammoBox_new(Vector2D position);

Entity* ammoBox_Get();

void ammoBox_draw(Entity* ent);

void ammoBox_think(Entity* ent);

void ammoBox_update(Entity* ent);
#endif