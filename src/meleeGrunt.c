#include "simple_logger.h"
#include "gf2d_graphics.h"
#include "gf2d_draw.h"
#include "level.h"
#include "bullet.h"
#include "player.h"
#include "meleeGrunt.h"

void meleeGrunt_think(Entity* ent);
//void grunt_draw(Entity* ent);
void meleeGrunt_free(Entity* ent);

static Entity* meleeGrunt = NULL;


Entity* meleeGrunt_new(Vector2D position)
{
	Entity* ent;
	ent = entity_new();
	if (!ent)return NULL;
	ent->sprite = gf2d_sprite_load_all("images/meleeGrunt-sprite.png",
		64,
		64,
		16,
		0);
	ent->think = meleeGrunt_think;
	//ent->draw = grunt_draw;
	ent->drawOffset = vector2d(32, 64);
	ent->free_entity = meleeGrunt_free;
	ent->shape = gfc_shape_rect(-10, -37, 20, 37);
	ent->shape = ent->shape;
	vector2d_copy(ent->position, position);

	meleeGruntData* stat = gfc_allocate_array(sizeof(meleeGruntData), 1);
	stat->health = 100;
	stat->speed = 2.5;
	stat->damage = 10;
	ent->data = stat;
	meleeGrunt = ent;
	return ent;
}

Entity* meleeGrunt_get()
{
	return meleeGrunt;
}
//void grunt_draw(Entity* ent)
//{
//	Vector2D drawPosition;
//	if (!ent)return;
//	vector2d_add(drawPosition, ent->position);
//	Rect toDraw = ent->shape.s.r;
//	toDraw.x += ent->position.x;
//	toDraw.y += ent->position.y;
//	vector2d_add(toDraw, toDraw);
//	gf2d_draw_rect(toDraw, GFC_COLOR_YELLOW);
//}

void meleeGrunt_think(Entity* ent)
{
	if (!ent) return;
	ent->velocity.x = 0;
	ent->velocity.y += 0.2;
	if (level_shape_clip(level_get_active_level(), entity_get_shape_after_move(ent)) == 1) {
		ent->velocity.y = 0;
	}
	Vector2D  a, b;
	a = player_get_position();
	b = ent->position;
	float distance = vector2d_magnitude_between(a, b);
	if (distance < 100 && distance != 16/* && player_get_position().y == ent->position.y*/)
	{
		ent->velocity.x -= 1;
	}

}
void meleeGrunt_free(Entity* ent)
{
	if (!ent)return;
	/*ent->sprite = NULL;*/
	meleeGrunt = NULL;
}