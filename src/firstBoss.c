#include "simple_logger.h"
#include "gf2d_graphics.h"
#include "gf2d_draw.h"
#include "level.h"
#include "bullet.h"
#include "camera.h"
#include "player.h"
#include "firstBoss.h"

void firstBoss_think(Entity* self);
void firstBoss_free(Entity* ent);
void firstBoss_draw(Entity* ent);

static Entity* Boss = NULL;

Entity* firstBoss_new(Vector2D position)
{
	Entity* ent;
	ent = entity_new();
	if (!ent)return NULL;
	ent->sprite = gf2d_sprite_load_all("images/bossPhase1.png",
		128,
		128,
		16,
		0);
	ent->think = firstBoss_think;
	//ent->draw = firstBoss_draw;
	ent->drawOffset = vector2d(64, 128);
	ent->free_entity = firstBoss_free;
	ent->shape = gfc_shape_rect(-10, -37, 20, 37);
	ent->shape = ent->shape;
	vector2d_copy(ent->position, position);
	ent->speed = 1;

	firstBossData* stat = gfc_allocate_array(sizeof(firstBossData), 1);
	stat->armor = 500;
	stat->damage = 3;
	stat->health = 1000;
	ent->data = stat;
	Boss = ent;
	return ent;
}
Entity* firstBoss_get()
{
	return Boss;
}

void firstBoss_draw(Entity* ent)
{
	Vector2D drawPosition, camera;
	if (!ent)return;
	Rect toDraw = ent->shape.s.r;
	toDraw.x += ent->position.x;
	toDraw.y += ent->position.y;
	vector2d_add(toDraw, toDraw, camera_get_draw_offset());
	gf2d_draw_rect(toDraw, GFC_COLOR_GREEN);
}

void firstBoss_think(Entity* ent)
{
	if (!ent) return;
	ent->velocity.x = 0;
	ent->velocity.y += 0.2;
	Vector2D  a, b;
	a = player_get_position();
	b = ent->position;
	float distance = vector2d_magnitude_between(a, b);
	if (level_shape_clip(level_get_active_level(), entity_get_shape_after_move(ent)) == 1) {
		ent->velocity.y = 0;
	}
	if (((firstBossData*)(ent->data))->health > 0 )
	{
		if (((firstBossData*)(ent->data))->health <= 500)
		{
			((firstBossData*)(ent->data))->tiers = 1;
			ent->sprite = gf2d_sprite_load_all("images/bossPhase2.png",
				128,
				128,
				16,
				0);
			if (distance < 240/* && player_get_position().y == ent->position.y*/)
			{
				//ent->velocity.x -= 1;
				Vector2D place = vector2d(0, 0);
				place.x = ent->position.x - 60;
				place.y = ent->position.y - 63;
				bullet_new(vector2d(place.x, place.y), 2, 2, 10);

				place.x = ent->position.x - 47;
				place.y = ent->position.y - 85;
				bullet_new(vector2d(place.x, place.y), 2, 2, 10);

			}
		}
		else if (((firstBossData*)(ent->data))->health <= 1000 && (((firstBossData*)(ent->data))->health != 0))
		{
			((firstBossData*)(ent->data))->tiers = 0;
			ent->sprite = gf2d_sprite_load_all("images/bossPhase1.png",
				128,
				128,
				16,
				0);
			if (distance < 240/* && player_get_position().y == ent->position.y*/)
			{
				//ent->velocity.x -= 1;
				Vector2D place = vector2d(0, 0);
				place.x = ent->position.x - 47;
				place.y = ent->position.y - 60;
				bullet_new(vector2d(place.x, place.y), 2, 2, 10);


			}
		}
	}

	else if (((firstBossData*)(ent->data))->health <= 0)
	{
		ent->sprite = gf2d_sprite_load_all("images/deadBoss.png",
			128,
			128,
			16,
			0);
	}
}

Vector2D firstBoss_get_position()
{
	Vector2D v = { 0 };
	if (!Boss)return v;
	return Boss->position;
}

void firstBoss_free(Entity* ent)
{
	if (!ent)return;
	Boss = NULL;
}