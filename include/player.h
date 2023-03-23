#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "gfc_vector.h"
#include "entity.h"

typedef enum {
	knife,
	AssaultRifle,
	Smg,
	Shotgun,
	Minigun,
	Sniper,
	Pistol,
	revolver,
	BurstRifle,
	Scythe
}WeaponType;

typedef struct Player_S
{

	int			armor;
	int			maxArmor;
	int			ammo;
	int			maxAmmo;
	int			cash;
	int			health;
	int			maxHealth;
	int			ar;
	int			knife;
	int			pistol;
	int			smg;
	int			speed;
	WeaponType	selectedWeapon;

}PlayerData;


/**
 * @brief spawn a new player at the given location
 * @param position where to spawn the bug at
 * @return NULL on failure, or a pointer to the new bug
 */
Entity* player_new(Vector2D position);

/**
 * @brief get a pointer to the active player, if available
 * @return the player or NULL if not set
 */
Entity* player_get();

/*
* @brief set the player position to the one specified
* @note risk of solid collision
* @param configure the new position for the player
*/
void player_set_position(Vector2D position);

/**
 * @brief get the player's position (or a zero vector if none is set);
 * @return the player's position
 */
Vector2D player_get_position();

// draw - obtain sprites
void player_draw(Entity* self);

// think - handles player actions
void player_think(Entity* self);

// update - updates player sprites accordingly
void player_update(Entity* self);

#endif