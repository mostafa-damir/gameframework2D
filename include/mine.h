#pragma once
#ifndef __mine_H__
#define __mine_H__

#include "gfc_vector.h"
#include "entity.h"

Entity* mine_new(Vector2D position);

Entity* mine_get();

void mine_draw(Entity* ent);


void mine_think(Entity* ent);

void mine_update(Entity* ent);


#endif