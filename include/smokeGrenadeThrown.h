#pragma once
#ifndef __smokeGrenadeThrown_H__
#define __smokeGrenadeThrown_H__

#include "gfc_vector.h"
#include "entity.h"

typedef struct smokeGrenadeThrown_S
{
	int			active;
	int			health;
}smgtData;

Entity* smokeGrenadeThrown_new(Vector2D position);

Entity* smokeGrenadeThrown_get();

Vector2D smokeGrenadeThrown_get_position();


void smokeGrenadeThrown_think(Entity* ent);

void smokeGrenadeThrown_update(Entity* ent);

#endif