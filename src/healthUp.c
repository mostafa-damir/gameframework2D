#include "simple_logger.h"
#include "gf2d_graphics.h"
#include "healthUp.h"
#include "camera.h"
#include "gf2d_draw.h"
#include "gfc_input.h"
#include "level.h"
#include "player.h"


void healthUp_free(Entity* ent);
void healthUp_think(Entity* ent);

static Entity* HealthUp = NULL;

Entity* healthUp_new(Vector2D position)
{
	Entity* ent;
	ent = entity_new();
	if (!ent)return NULL;
	ent->sprite = gf2d_sprite_load_all(
		"images/healthUp.png",
		32,
		32,
		16,
		0);
	ent->think = healthUp_think;
	vector2d_copy(ent->position, position);
	ent->drawOffset = vector2d(16, 32);
	ent->free_entity = healthUp_free;
	ent->shape = gfc_shape_rect(-8, -31, 16, 31);
	ent->shape = ent->shape;

	healthUpData* Data = gfc_allocate_array(sizeof(healthUpData), 1);

	Data->healthIncrease = 150;
	ent->data = Data;
	HealthUp = ent;
	return ent;
}

void healthUp_think(Entity* ent)
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

			((PlayerData*)(player_get()->data))->maxHealth = ((healthUpData*)(ent->data))->healthIncrease;
			slog(" your max health is this much %i ", ((PlayerData*)(player_get()->data))->maxHealth);
			entity_free(ent);
		}

	}
}

void healthUp_free(Entity* ent)
{
	if (!ent)return;
	HealthUp = NULL;
}