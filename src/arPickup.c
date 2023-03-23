#include "simple_logger.h"
#include "gf2d_graphics.h"
#include "arPickup.h"
#include "camera.h"
#include "gf2d_draw.h"
#include "gfc_input.h"
#include "level.h"
#include "player.h"

void arPickup_free(Entity* ent);
void arPickup_think(Entity* ent);

static Entity* assaultRifle = NULL;

Entity* arPickup_new(Vector2D position)
{
	Entity* ent;
	ent = entity_new();
	if (!ent)return NULL;
	ent->sprite = gf2d_sprite_load_all(
		"images/assaultRifle-sprite.png",
		32,
		32,
		16,
		0);
	ent->think = arPickup_think;
	vector2d_copy(ent->position, position);
	ent->drawOffset = vector2d(16, 32);
	ent->free_entity = arPickup_free;
	ent->shape = gfc_shape_rect(-8, -31, 16, 31);
	ent->shape = ent->shape;

	ARData* gun = gfc_allocate_array(sizeof(ARData), 1);

	gun->grab = 1;
	gun->grabbed = false;

	ent->data = gun;
	assaultRifle = ent;
	return ent;
}

void arPickup_think(Entity* ent)
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
			slog("pressed");
	
			
			if (((ARData*)(ent->data))->grabbed == false)
			{
				((PlayerData*)(player_get()->data))->ar = ((PlayerData*)(player_get()->data))->ar + ((ARData*)(ent->data))->grab;
				slog("ayo you got gun %i", ((PlayerData*)(player_get()->data))->ar);
				((ARData*)(ent->data))->grabbed = true;
			}
			if (((ARData*)(ent->data))->grabbed == true)
			{
				((ARData*)(ent->data))->grab = 0;
			}
			entity_free(ent);
		}
	}
}

void arPickup_free(Entity* ent)
{
	if (!ent)return;
	/*ent->sprite = NULL;*/
	assaultRifle = NULL;
}