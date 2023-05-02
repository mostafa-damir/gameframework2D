#pragma once
#ifndef __grenadeThrown_H__
#define __grenadeThrown_H__

#include "gfc_vector.h"
#include "entity.h"


Entity* grenadeThrown_new(Vector2D position);

Entity* grenadeThrown_get();

Vector2D grenadeThrown_get_position();


void grenadeThrown_think(Entity* ent);

void grenadeThrown_update(Entity* ent);

#endif