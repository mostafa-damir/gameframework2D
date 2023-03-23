#pragma once
#ifndef  __BULLET_H__
#define __BULLET_H__

#include "gfc_vector.h"
#include "entity.h"
typedef struct Bullet_S
{

	int			damage;
}BulletData;
/**
 * @brief spawn a new bullet at the given location
 * @param parent the entity that fired the bullet
 * @param position where to spawn the bullet at
 * @param dir the direction to move in
 * @param speed how fast we move
 * @param damage how much damage we will
 * @param actor which actor file to used for this projectile
 * @return NULL on failure, or a pointer to the new projectile
 */
Entity* bullet_new( Vector2D position, int team, float speed, int damage);
#endif
