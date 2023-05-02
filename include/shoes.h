#pragma once
#ifndef __shoes_H__
#define __shoes_H__

#include "gfc_vector.h"
#include "entity.h"

typedef struct shoes_S
{
	int		speedUp;
}shoesData;

Entity* shoes_new(Vector2D position);

Entity* shoes_get();

void shoes_draw(Entity* ent);

void shoes_think(Entity* ent);

void shoes_update(Entity* ent);

#endif