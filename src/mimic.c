#include "simple_logger.h"
#include "gf2d_graphics.h"
#include "mimic.h"
#include "camera.h"
#include "gf2d_draw.h"
#include "gfc_input.h"
#include "level.h"
#include "player.h"

void healthUp_free(Entity* ent);
void healthUp_think(Entity* ent);

static Entity* Mimic = NULL;

Entity* mimic_new(Vector2D position)
{
	Entity* ent;
	ent = entity_new();
	if (!ent)return NULL;
	ent->sprite = gf2d_sprite_load_all(
		"images/mimic.png",
		32,
		32,
		16,
		0);
	ent->think = mimic_think;
	vector2d_copy(ent->position, position);
	ent->drawOffset = vector2d(16, 32);
	ent->free_entity = healthUp_free;
	ent->shape = gfc_shape_rect(-8, -31, 16, 31);
	ent->shape = ent->shape;
	//ent->draw = mimic_draw;
	Mimic = ent;
	return ent;
}

void mimic_think(Entity* ent)
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

			((PlayerData*)(player_get()->data))->health = ((PlayerData*)(player_get()->data))->health - 25;
			slog(" you took damage you are at %i ", ((PlayerData*)(player_get()->data))->health);
			entity_free(ent);
		}

	}
}

void mimic_draw(Entity* ent)
{
	Vector2D drawPosition, camera;
	if (!ent)return;
	//vector2d_add(drawPosition, self->position, camera);
	Rect toDraw = ent->shape.s.r;
	toDraw.x += ent->position.x;
	toDraw.y += ent->position.y;
	vector2d_add(toDraw, toDraw, camera_get_draw_offset());
	gf2d_draw_rect(toDraw, GFC_COLOR_YELLOW);
}

void Mimic_free(Entity* ent)
{
	if (!ent)return;
	Mimic = NULL;
}
