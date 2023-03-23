#pragma once
#ifndef  MELEESWIPE_H__
#define MELEESWIPE_H__

#include "gfc_vector.h"
#include "entity.h"
//typedef struct Swipe_S
//{
//
//	int			damage;
//}meleeData;
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
Entity* meleeSwipe_new(Vector2D position, int team, int damage);
#endif
