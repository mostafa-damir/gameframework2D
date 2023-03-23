//#include "simple_logger.h"
//
//#include "gf2d_graphics.h"
//#include "gf2d_draw.h"
//#include "weapons.h"
//#include "level.h"
//
//
//
//Entity* weapon_new(WeaponType type, Vector2D position) {
//	Entity* weapon = entity_new();
//	if (!weapon) {
//		return NULL;
//	}
//	WeaponData* data = gfc_allocate_array(sizeof(WeaponData), 1);
//	if (!data) {
//		memset(weapon, 0, sizeof(Entity));
//		return NULL;
//	}
//	data->type = type;
//	switch (type) {
//	case Pistol:
//		weapon->sprite = gf2d_sprite_load_all("images/pistol-sprite.png",
//			32,
//			32,
//			1,
//			0);
//		data->damage = 4;
//		data->range = -1;
//		data->firerate = 0.6;
//		data->projectileVelocity = 1;
//		break;
//	case AssaultRifle:
//		weapon->sprite = gf2d_sprite_load_all("images/assaultrifle-sprite.png",
//			32,
//			32,
//			1,
//			0);
//		data->damage = 3;
//		data->range = -1;
//		data->firerate = 0.2;
//		data->projectileVelocity = 1;
//		break;
//	case Minigun:
//		weapon->sprite = gf2d_sprite_load_all("images/chaingun-sprite.png",
//			32,
//			32,
//			1,
//			0);
//		data->damage = 2;
//		data->range = -1;
//		data->firerate = 0.1;
//		data->projectileVelocity = 1;
//		break;
//	case Revolver:
//		weapon->sprite = gf2d_sprite_load_all("images/revolver-sprite.png",
//			32,
//			32,
//			1,
//			0);
//		data->damage = 6;
//		data->range = -1;
//		data->firerate = 1;
//		data->projectileVelocity = 1;
//		break;
//	case Scythe:
//		weapon->sprite = gf2d_sprite_load_all("images/scythe-sprite.png",
//			32,
//			32,
//			1,
//			0);
//		data->damage = 20;
//		data->range = 1.2;
//		data->firerate = 3;
//		data->projectileVelocity = 0;
//		break;
//	case Knife:
//		weapon->sprite = gf2d_sprite_load_all("images/knive-sprite.png",
//			32,
//			32,
//			1,
//			0);
//		data->damage = 2;
//		data->range = 0.5;
//		data->firerate = 0.75;
//		data->projectileVelocity = 0;
//		break;
//	case Smg:
//		weapon->sprite = gf2d_sprite_load_all("images/smg-sprite.png",
//			32,
//			32,
//			1,
//			0);
//		data->damage = 4;
//		data->range = -1;
//		data->firerate = 0.2;
//		data->projectileVelocity = 1;
//		break;
//	case Shotgun:
//		weapon->sprite = gf2d_sprite_load_all("images/shotgun-sprite.png",
//			32,
//			32,
//			1,
//			0);
//		data->damage = 6;
//		data->range = -1;
//		data->firerate = 1;
//		data->projectileVelocity = 1;
//		break;
//	case BurstRifle:
//		weapon->sprite = gf2d_sprite_load_all("images/burstrifle-sprite.png",
//			32,
//			32,
//			1,
//			0);
//		data->damage = 3;
//		data->range = -1;
//		data->firerate = 0.4;
//		data->projectileVelocity = 1;
//		break;
//	case Sniper:
//		weapon->sprite = gf2d_sprite_load_all("images/sniper-sprite.png",
//			32,
//			32,
//			1,
//			0);
//		data->damage = 10;
//		data->range = -1;
//		data->firerate = 1.5;
//		data->projectileVelocity = 1;
//		break;
//
//	}
//}
//void weapon_free(Entity* ent)
//{
//	if (!ent)return;
//}
//void weapon_update(Entity* ent);
//
//void weapon_think(Entity* weapon)
//{
//	if (!weapon)
//	{
//		return;
//	}
//	if (level_shape_clip(level_get_active_level(), entity_get_shape_after_move(weapon)) == 1)
//	{
//		weapon->velocity.y = 0;
//	/*	if (weapon == data(Pistol))
//		{
//
//		}*/
//
//	}
//}
//
