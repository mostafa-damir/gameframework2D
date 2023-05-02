#ifndef __GRUNT_H__
#define __GRUNT_H__

#include "gfc_vector.h"
#include "entity.h"

typedef struct Grunt_S
{
	int		health;
	int		speed;
	int		damage;
	int		dead;
}GruntData;


Entity* grunt_new(Vector2D position);

Entity* grunt_get();

void grunt_draw(Entity* ent);

Vector2D grunt_get_position();

void grunt_think(Entity* ent);

void grunt_update(Entity* ent);


#endif // !__GRUNT_H__
