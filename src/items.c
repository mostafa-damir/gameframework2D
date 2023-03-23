#include "simple_logger.h"
#include "gf2d_draw.h"
#include "items.h"


Entity* item_new(ItemType type) {
	Entity* item = entity_new();
	if (!item)return NULL;
	ItemData* data = gfc_allocate_array(sizeof(ItemData), 1);
	if (!data) {
		memset(item, 0, sizeof(Entity));
		return NULL;
	}
	data->type = type;
	switch (type) {
	case healthKit:
		data->armor = 0;
		data->ammo = 0;
		data->health = 10;
		data->cash = 0;
		break;
	case armorBox:
		data->armor = 25;
		data->ammo = 0;
		data->health = 0;
		data->cash = 0;
		break;
	case money:
		data->armor = 0;
		data->ammo = 0;
		data->health = 0;
		data->cash = 5;
		break;
	case ammoBox:
		data->armor = 0;
		data->ammo = 50;
		data->health = 0;
		data->cash = 0;
		break;
	}
}
void item_free(Entity* ent);

void item_update(Entity* self);

void item_think(Entity* self);