#include "simple_logger.h"
#include "gf2d_graphics.h"
#include "ammoBox.h"
#include "camera.h"
#include "gf2d_draw.h"
#include "gfc_input.h"
#include "level.h"
#include "player.h"

void ammoBox_free(Entity* ent);
void ammoBox_think(Entity* ent);

static Entity* AmmoBox = NULL;


Entity* ammoBox_new(Vector2D position)
{
	Entity* ent;
	ent = entity_new();
	if (!ent)return NULL;
	ent->sprite = gf2d_sprite_load_all(
		"images/ammo-sprite.png",
		32,
		32,
		16,
		0);
	ent->think = ammoBox_think;
	vector2d_copy(ent->position, position);
	ent->drawOffset = vector2d(16, 32);
	ent->free_entity = ammoBox_free;
	ent->shape = gfc_shape_rect(-8, -31, 16, 31);
	ent->shape = ent->shape;

	ammoBoxData* ammoData = gfc_allocate_array(sizeof(ammoBoxData), 1);

	ammoData->ammoPlus = 50;
	ent->data = ammoData;
	AmmoBox = ent;
	return ent;
}

void ammoBox_think(Entity* ent)
{
	if (!ent)return;
	ent->velocity.y += 0.2;
	if (level_shape_clip(level_get_active_level(), entity_get_shape_after_move(ent)) == 1) {
		ent->velocity.y = 0;
	}
	Vector2D  a, b;
	a = player_get_position();
	b = ent->position;
	float distance = vector2d_magnitude_between(a, b);
	if (distance < 16)
	{
		if (gfc_input_command_pressed("collect"))
		{

			if (((PlayerData*)(player_get()->data))->ammo < ((PlayerData*)(player_get()->data))->maxAmmo)
			{
				((PlayerData*)(player_get()->data))->ammo = min(((PlayerData*)(player_get()->data))->ammo + ((ammoBoxData*)(ent->data))->ammoPlus, ((PlayerData*)(player_get()->data))->maxAmmo);
				slog(" this the ammo %i", ((PlayerData*)(player_get()->data))->ammo);
			}
			entity_free(ent);
		}

	}
}

void ammoBox_free(Entity* ent)
{
	if (!ent)return;
	AmmoBox = NULL;
}