#include "simple_logger.h"
#include "gf2d_graphics.h"
#include "healthKit.h"
#include "camera.h"
#include "gf2d_draw.h"
#include "gfc_input.h"
#include "level.h"
#include "player.h"

void healthKit_free(Entity* ent);
void healthKit_think(Entity* ent);

static Entity* HealthKit = NULL;

Entity* healthKit_new(Vector2D position)
{
	Entity* ent;
	ent = entity_new();
	if (!ent)return NULL;
	ent->sprite = gf2d_sprite_load_all(
		"images/health-sprite.png",
		32,
		32,
		16,
		0);
	ent->think = healthKit_think;
	vector2d_copy(ent->position, position);
	ent->drawOffset = vector2d(16, 32);
	ent->free_entity = healthKit_free;
	ent->shape = gfc_shape_rect(-8, -32, 16, 32);
	ent->shape = ent->shape;

	healthKitData* healthData = gfc_allocate_array(sizeof(healthKitData), 1);

	healthData->healthPlus = 25;
	healthData->used = false;
	ent->data = healthData;
	HealthKit = ent;
	return ent;
}

void healthKit_think(Entity* ent)
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
	//slog("%f", distance);
	if (distance < 16)
	{
		if (gfc_input_command_pressed("collect"))
		{
			
			if (((PlayerData*)(player_get()->data))->health < ((PlayerData*)(player_get()->data))->maxHealth && ((healthKitData*)(ent->data))->used == false)
			{
				((PlayerData*)(player_get()->data))->health = min(((PlayerData*)(player_get()->data))->health + ((healthKitData*)(ent->data))->healthPlus, ((PlayerData*)(player_get()->data))->maxHealth);
				slog("%i", ((PlayerData*)(player_get()->data))->health);
				((healthKitData*)(ent->data))->used = true;
				//entity_free(ent);
			}
			if (((healthKitData*)(ent->data))->used == true)
			{
				((healthKitData*)(ent->data))->healthPlus = 0;
			}
			entity_free(ent);
		}
		
		
	}
}

void healthKit_free(Entity* ent)
{
	if (!ent)return;
	/*ent->sprite = NULL;*/
	HealthKit = NULL;
}