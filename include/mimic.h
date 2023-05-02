#pragma once
#ifndef __mimic_H__
#define __mimic_H__

#include "gfc_vector.h"
#include "entity.h"

Entity* mimic_new(Vector2D position);

Entity* mimic_get();

void mimic_draw(Entity* ent);

void mimic_think(Entity* ent);

void mimic_update(Entity* ent);


#endif