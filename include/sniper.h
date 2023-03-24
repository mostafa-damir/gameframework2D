#ifndef SNIPER_H__
#define SNIPER_H__

#include "gfc_vector.h"
#include "entity.h"

typedef struct Sniper_S
{
	int		health;
	int		speed;
	int		damage;
}SniperData;


Entity* sniper_new(Vector2D position);

Entity* sniper_get();

void sniper_think(Entity* ent);

void sniper_update(Entity* ent);

#endif // !__SNIPER_H__