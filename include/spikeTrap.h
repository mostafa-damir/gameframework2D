#pragma once
#ifndef __spikeTrap_H__
#define __spikeTrap_H__

#include "gfc_vector.h"
#include "entity.h"

Entity* spikeTrap_new(Vector2D position);

Entity* spikeTrap_get();

void spikeTrap_draw(Entity* ent);


void spikeTrap_think(Entity* ent);

void spikeTrap_update(Entity* ent);


#endif