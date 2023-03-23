#ifndef __CASH_H__
#define __CASH_H__

#include "gfc_vector.h"
#include "entity.h"

typedef struct cash_S
{
	int			cash;
	Bool		used;
}cashData;

Entity* cash_new(Vector2D position);

Entity* cash_get();

void cash_draw(Entity* ent);

void cash_think(Entity* ent);

void cash_update(Entity* ent);
#endif // !__CASH_H__
