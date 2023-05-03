#pragma once
#ifndef __stunGrenadeThrown_H__
#define __stunGrenadeThrown_H__

#include "gfc_vector.h"
#include "entity.h"

typedef struct stunGrenadeThrown_S
{
	int			active;
	int			health;
}sgtData;

Entity* stunGrenadeThrown_new(Vector2D position);

Entity* stunGrenadeThrown_get();

Vector2D stunGrenadeThrown_get_position();


void stunGrenadeThrown_think(Entity* ent);

void stunGrenadeThrown_update(Entity* ent);

#endif