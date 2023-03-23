#pragma once
#ifndef __ITEMS_H_
#define __ITEMS_H_

#include "gfc_types.h"
#include "gfc_vector.h"
#include "entity.h"

#include "gf2d_sprite.h"

typedef enum {
	armorBox,
	healthKit,
	ammoBox,
	money
}ItemType;

typedef struct Item_S {
	ItemType type;
	Entity* owner;
	Sprite* sprite;
	float	stat;
	int		armor;
	int		ammo;
	int		health;
	int		cash;
}ItemData;

/**
*@Brief initialize  the internal manager for the weapon entity
**/

Entity* item_new(ItemData type);


void item_free(Entity* ent);


void item_update(Entity* self);

void item_think(Entity* self);

#endif