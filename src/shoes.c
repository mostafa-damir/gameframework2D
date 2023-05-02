#include "simple_logger.h"
#include "gf2d_graphics.h"
#include "shoes.h"
#include "camera.h"
#include "gf2d_draw.h"
#include "gfc_input.h"
#include "level.h"
#include "player.h"

void shoes_free(Entity* ent);
void shoes_think(Entity* ent);

static Entity* Shoes = NULL;

Entity* shoes_new(Vector2D position)
{
	Entity* ent;
	ent = entity_new();
	if (!ent)return NULL;
	ent->sprite = gf2d_sprite_load_all(
		"images/shoes.png",
		32,
		32,
		16,
		0);
	ent->think = shoes_think;
	vector2d_copy(ent->position, position);
	ent->drawOffset = vector2d(16, 32);
	ent->free_entity = shoes_free;
	ent->shape = gfc_shape_rect(-8, -31, 16, 31);
	ent->shape = ent->shape;

	shoesData* Data = gfc_allocate_array(sizeof(shoesData), 1);

	Data->speedUp = 5;
	ent->data = Data;
	Shoes = ent;
	return ent;
}

void shoes_think(Entity* ent)
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

			((PlayerData*)(player_get()->data))->speed = ((shoesData*)(ent->data))->speedUp;
			slog(" your speed has doubled to %i ", ((PlayerData*)(player_get()->data))->speed);
			entity_free(ent);
		}

	}
}

void shoes_free(Entity* ent)
{
	if (!ent)return;
	Shoes = NULL;
}