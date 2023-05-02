#include "simple_logger.h"
#include "gf2d_graphics.h"
#include "ammoBag.h"
#include "camera.h"
#include "gf2d_draw.h"
#include "gfc_input.h"
#include "level.h"
#include "player.h"

void ammoBag_free(Entity* ent);
void ammoBag_think(Entity* ent);

static Entity* AmmoBag = NULL;


Entity* ammoBag_new(Vector2D position)
{
	Entity* ent;
	ent = entity_new();
	if (!ent)return NULL;
	ent->sprite = gf2d_sprite_load_all(
		"images/ammoBag.png",
		32,
		32,
		16,
		0);
	ent->think = ammoBag_think;
	vector2d_copy(ent->position, position);
	ent->drawOffset = vector2d(16, 32);
	ent->free_entity = ammoBag_free;
	ent->shape = gfc_shape_rect(-8, -31, 16, 31);
	ent->shape = ent->shape;

	ammoBagData* bagData = gfc_allocate_array(sizeof(ammoBagData), 1);

	bagData->carryIncrease = 1000;
	ent->data = bagData;
	AmmoBag = ent;
	return ent;
}

void ammoBag_think(Entity* ent)
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

			((PlayerData*)(player_get()->data))->maxAmmo = ((ammoBagData*)(ent->data))->carryIncrease;
			slog(" you can carry %i bullets", ((PlayerData*)(player_get()->data))->maxAmmo);
			entity_free(ent);
		}

	}
}

void ammoBag_free(Entity* ent)
{
	if (!ent)return;
	AmmoBag = NULL;
}