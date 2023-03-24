#ifndef __GUNNER_H__
#define __GUNNER_H__

#include "gfc_vector.h"
#include "entity.h"

typedef struct Gunner_S
{
	int		health;
	int		speed;
	int		damage;
}GunnerData;


Entity* gunner_new(Vector2D position);

Entity* gunner_get();

//void grunt_draw(Entity* ent);
Vector2D gunner_get_position();

void gunner_think(Entity* ent);

void gunner_update(Entity* ent);


#endif // !__GRUNT_H__
