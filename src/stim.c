#include "simple_logger.h"
#include "gf2d_graphics.h"
#include "stim.h"
#include "camera.h"
#include "gf2d_draw.h"
#include "gfc_input.h"
#include "level.h"
#include "player.h"

void stim_free(Entity* ent);
void stim_think(Entity* ent);

static Entity* Stim = NULL;

Entity* stim_new(Vector2D position)
{
	Entity* ent;
	ent = entity_new();
	if (!ent)return NULL;
	ent->sprite = gf2d_sprite_load_all(
		"images/stim.png",
		32,
		32,
		16,
		0);
	ent->think = stim_think;
	vector2d_copy(ent->position, position);
	ent->drawOffset = vector2d(16, 32);
	ent->free_entity = stim_free;
	ent->shape = gfc_shape_rect(-8, -32, 16, 32);
	ent->shape = ent->shape;

	stimData* Data = gfc_allocate_array(sizeof(stimData), 1);
	Data->healthPlus = 5;
	Data->pickedup = 1;
	Data->used = false;
	ent->data = Data;
	Stim = ent;
	return ent;
}

void stim_think(Entity* ent)
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
		if (((PlayerData*)(player_get()->data))->currentStims != ((PlayerData*)(player_get()->data))->maxStims && ((PlayerData*)(player_get()->data))->generator == 0)
		{
			if (gfc_input_command_pressed("collect"))
			{
				if (((PlayerData*)(player_get()->data))->currentStims < ((PlayerData*)(player_get()->data))->maxStims && ((stimData*)(ent->data))->used == false)
				{
					((PlayerData*)(player_get()->data))->currentStims = ((PlayerData*)(player_get()->data))->currentStims + ((stimData*)(ent->data))->pickedup;
					slog("%i", ((PlayerData*)(player_get()->data))->currentStims);

				}
				if (((stimData*)(ent->data))->used == true)
				{
					((stimData*)(ent->data))->pickedup = 0;
				}
				entity_free(ent);
			}
		}

	}

}

Entity* stim_get()
{
	return Stim;
}

void stim_free(Entity* ent)
{
	if (!ent)return;
	/*ent->sprite = NULL;*/
	Stim = NULL;
}