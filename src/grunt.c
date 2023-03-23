#include "simple_logger.h"
#include "gf2d_graphics.h"
#include "gf2d_draw.h"
#include "level.h"
#include "bullet.h"
#include "player.h"
#include "grunt.h"

void grunt_think(Entity* ent);
//void grunt_draw(Entity* ent);
void grunt_free(Entity* ent);

static Entity* Grunt = NULL;


Entity* grunt_new(Vector2D position)
{
	Entity* ent;
	ent = entity_new();
	if (!ent)return NULL;
	ent->sprite = gf2d_sprite_load_all("images/grunt-sprite.png",
		64,
		64,
		16,
		0);
	ent->think = grunt_think;
	//ent->draw = grunt_draw;
	ent->drawOffset = vector2d(32, 64);
	ent->free_entity = grunt_free;
	ent->shape = gfc_shape_rect(-16, -63, 32, 63);
	ent->shape = ent->shape;
	vector2d_copy(ent->position, position);

	GruntData* gruntStat = gfc_allocate_array(sizeof(GruntData), 1);
	gruntStat->health = 50;
	gruntStat->speed = 2.5;
	gruntStat->damage = 10;
	ent->data = gruntStat;
	Grunt = ent;
	return ent;
}

Entity* grunt_get()
{
	return Grunt;
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

void grunt_think(Entity* ent)
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
	if (distance > 16 && player_get_position().y == ent->position.y)
	{
		ent->velocity.x -= 0.1;
	}
	//else if (player_get_position().y != ent->position.y)
	//{
	//	ent->velocity.x += 0.1;
	//}

}
void grunt_free(Entity* ent)
{
	if (!ent)return;
	/*ent->sprite = NULL;*/
	Grunt = NULL;
}