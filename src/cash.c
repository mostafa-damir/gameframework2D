#include "simple_logger.h"
#include "gf2d_graphics.h"
#include "cash.h"
#include "camera.h"
#include "gf2d_draw.h"
#include "gfc_input.h"
#include "level.h"
#include "player.h"

void cash_free(Entity* ent);
void cash_think(Entity* ent);

static Entity* Money = NULL;

Entity* cash_new(Vector2D position)
{
	Entity* ent;
	ent = entity_new();
	ent->sprite = gf2d_sprite_load_all(
		"images/money-sprite.png",
		32,
		32,
		16,
		0);
	ent->think = cash_think;
	vector2d_copy(ent->position, position);
	ent->drawOffset = vector2d(16, 32);
	ent->free_entity = cash_free;
	ent->shape = gfc_shape_rect(-8, -31, 16, 31);
	ent->shape = ent->shape;

	cashData* cashData = gfc_allocate_array(sizeof(cashData), 1);

	cashData->cash = 100;
	cashData->used = false;
	ent->data = cashData;
	Money = ent;
	return ent;
}

void cash_think(Entity* ent)
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

			if (((cashData*)(ent->data))->used == false)
			{
				((PlayerData*)(player_get()->data))->cash =((PlayerData*)(player_get()->data))->cash + ((cashData*)(ent->data))->cash;
				slog("%i", ((PlayerData*)(player_get()->data))->cash);
				((cashData*)(ent->data))->used = true;
			}
			if (((cashData*)(ent->data))->used == true)
			{
				((cashData*)(ent->data))->cash = 0;
			}
			entity_free(ent);
		}

	}
}


void cash_free(Entity* ent)
{
	if (!ent)return;
	/*ent->sprite = NULL;*/
	Money = NULL;
}