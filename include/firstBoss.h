#ifndef __FIRSTBOSS_H__
#define __FIRSTBOSS_H__

#include "gfc_vector.h""
#include "entity.h"

typedef enum {
	Tier1,
	Tier2
}BossTiers;


typedef struct firstBoss_S
{
	int		damage;
	int		armor;
	int		health;
	BossTiers tiers;
}firstBossData;

Entity* firstBoss_new(Vector2D position);

Entity* firstBoss_get();

Vector2D firstBoss_get_position();

void firstBoss_think(Entity* ent);

void firstBoss_update(Entity* ent);

#endif // !__FIRSTBOSS_H__
