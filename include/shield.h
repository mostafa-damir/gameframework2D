#pragma once
#ifndef __shield_H__
#define __shield_H__

#include "gfc_vector.h"
#include "entity.h"

typedef struct shield_S
{
	int			active;
	int			health;
}shieldData;

Entity* shield_new(Vector2D position);

Entity* shield_get();

Vector2D shield_get_position();


void shield_think(Entity* ent);

void shield_update(Entity* ent);

#endif