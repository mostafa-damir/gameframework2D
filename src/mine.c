#include "simple_logger.h"
#include "gf2d_graphics.h"
#include "mine.h"
#include "camera.h"
#include "gf2d_draw.h"
#include "gfc_input.h"
#include "level.h"
#include "player.h"

void mine_free(Entity* ent);
void mine_think(Entity* ent);

static Entity* Mine = NULL;

Entity* mine_new(Vector2D position)
{
	Entity* ent;
	ent = entity_new();
	if (!ent)return NULL;
	ent->sprite = gf2d_sprite_load_all(
		"images/mine.png",
		32,
		32,
		16,
		0);
	ent->think = mine_think;
	vector2d_copy(ent->position, position);
	ent->drawOffset = vector2d(16, 32);
	ent->shape = gfc_shape_rect(-8, -31, 16, 31);
	//ent->draw = mine_draw;
	Mine = ent;
	return ent;
}

void mine_think(Entity* ent)
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

		((PlayerData*)(player_get()->data))->health = ((PlayerData*)(player_get()->data))->health - 10;
		slog(" your health is %i ", ((PlayerData*)(player_get()->data))->health);
		entity_free(ent);

	}
}

void mine_draw(Entity* self)
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

void mine_free(Entity* ent)
{
	if (!ent)return;
	Mine = NULL;
}