#include "simple_logger.h"
#include "gf2d_graphics.h"
#include "stunMine.h"
#include "camera.h"
#include "gf2d_draw.h"
#include "gfc_input.h"
#include "level.h"
#include "player.h"

void stunMine_free(Entity* ent);
void stunMine_think(Entity* ent);

static Entity* StunMine = NULL;

Entity* stunMine_new(Vector2D position)
{
	Entity* ent;
	ent = entity_new();
	if (!ent)return NULL;
	ent->sprite = gf2d_sprite_load_all(
		"images/stunMine.png",
		32,
		32,
		16,
		0);
	ent->think = stunMine_think;
	vector2d_copy(ent->position, position);
	ent->drawOffset = vector2d(16, 32);
	ent->shape = gfc_shape_rect(-8, -31, 16, 31);
	//ent->draw = stunMine_draw;
	stunMineData* Data = gfc_allocate_array(sizeof(stunMineData), 1);

	Data->active = 0;
	Data->health = 250;
	ent->data = Data;
	StunMine = ent;
	return ent;
}

void stunMine_think(Entity* ent)
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
	if (distance < 10)
	{

		((stunMineData*)(ent->data))->active = 1;
		if (((stunMineData*)(ent->data))->active == 1)
		{
			((PlayerData*)(player_get()->data))->stunned = 1;
			((stunMineData*)(ent->data))->health --;
			//slog(" you are stunned ");
		}
		if (((stunMineData*)(ent->data))->health == 0)
		{
			((PlayerData*)(player_get()->data))->stunned = 0;
			((stunMineData*)(ent->data))->active = 0;
			slog(" you are no longer stunned");
			entity_free(ent);
		}
	}

}

Entity* stunMine_get()
{
	return StunMine;
}

void stunMine_draw(Entity* self)
{
	Vector2D drawPosition, camera;
	if (!self)return;
	//vector2d_add(drawPosition, self->position, camera);
	Rect toDraw = self->shape.s.r;
	toDraw.x += self->position.x;
	toDraw.y += self->position.y;
	vector2d_add(toDraw, toDraw, camera_get_draw_offset());
	gf2d_draw_rect(toDraw, GFC_COLOR_YELLOW);
}

void stunMine_free(Entity* ent)
{
	if (!ent)return;
	StunMine = NULL;
}