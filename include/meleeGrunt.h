#ifndef __MELEEGRUNT_H__
#define __MELEEGRUNT_H__

#include "gfc_vector.h"
#include "entity.h"

typedef struct meleeGrunt_S
{
	int		health;
	int		speed;
	int		damage;
}meleeGruntData;


Entity* meleeGrunt_new(Vector2D position);

Entity* meleeGrunt_get();

//void grunt_draw(Entity* ent);
Vector2D meleeGrunt_get_position();

void meleeGrunt_think(Entity* ent);

void meleeGrunt_update(Entity* ent);


#endif // !__GRUNT_H__
