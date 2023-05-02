#pragma once
#ifndef __laser_H__
#define __laser_H__

#include "gfc_vector.h"
#include "entity.h"

Entity* laser_new(Vector2D position);

Entity* laser_get();

void laser_draw(Entity* ent);

void laser_think(Entity* ent);

void laser_update(Entity* ent);


#endif