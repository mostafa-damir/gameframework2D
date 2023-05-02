#include "simple_logger.h"
#include "gf2d_graphics.h"
#include "sachel.h"
#include "camera.h"
#include "gf2d_draw.h"
#include "gfc_input.h"
#include "level.h"
#include "player.h"

void sachel_free(Entity* ent);
void sachel_think(Entity* ent);

static Entity* Sachel = NULL;

Entity* sachel_new(Vector2D position)
{
	Entity* ent;
	ent = entity_new();
	if (!ent)return NULL;
	ent->sprite = gf2d_sprite_load_all(
		"images/sachel.png",
		32,
		32,
		16,
		0);
	ent->think = sachel_think;
	vector2d_copy(ent->position, position);
	ent->drawOffset = vector2d(16, 32);
	ent->free_entity = sachel_free;
	ent->shape = gfc_shape_rect(-8, -31, 16, 31);
	ent->shape = ent->shape;

	sachelData* Data = gfc_allocate_array(sizeof(sachelData), 1);

	Data->carryIncrease = 5;
	ent->data = Data;
	Sachel = ent;
	return ent;
}
void sachel_think(Entity* ent)
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

			((PlayerData*)(player_get()->data))->maxGrenade = ((sachelData*)(ent->data))->carryIncrease;
			((PlayerData*)(player_get()->data))->maxStunGrenade = ((sachelData*)(ent->data))->carryIncrease;
			((PlayerData*)(player_get()->data))->maxSmokeGrenade = ((sachelData*)(ent->data))->carryIncrease;
			slog(" you can carry this many %i impact grenades", ((PlayerData*)(player_get()->data))->maxGrenade);
			slog(" you can carry this many %i stun grenades", ((PlayerData*)(player_get()->data))->maxStunGrenade);
			slog(" you can carry this many %i smoke grenades", ((PlayerData*)(player_get()->data))->maxSmokeGrenade);
			entity_free(ent);
		}

	}
}

void sachel_free(Entity* ent)
{
	if (!ent)return;
	Sachel = NULL;
}