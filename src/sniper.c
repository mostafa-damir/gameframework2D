#include "simple_logger.h"
#include "gf2d_graphics.h"
#include "gf2d_draw.h"
#include "level.h"
#include "bullet.h"
#include "player.h"
#include "camera.h"
#include "sniper.h"

void sniper_think(Entity* ent);
void sniper_free(Entity* ent);
void sniper_draw(Entity* ent);

static Entity* SniperDude = NULL;

Entity* sniper_new(Vector2D position)
{
	Entity* ent;
	ent = entity_new();
	if (!ent)return NULL;
	ent->sprite = gf2d_sprite_load_all("images/sniperGrunt-sprite.png",
		64,
		64,
		16,
		0);
	ent->think = sniper_think;
	//ent->draw = sniper_draw;
	ent->drawOffset = vector2d(32, 64);
	ent->free_entity = sniper_free;
	ent->shape = gfc_shape_rect(-10, -37, 20, 37);
	ent->shape = ent->shape;
	vector2d_copy(ent->position, position);

	SniperData* Stat = gfc_allocate_array(sizeof(SniperData), 1);
	Stat->health = 150;
	Stat->speed = 2.5;
	Stat->damage = 10;
	Stat->stunned = 0;
	Stat->stunTime = 250;
	ent->data = Stat;
	SniperDude = ent;
	return ent;
}

Entity* sniper_get()
{
	return SniperDude;
}

void sniper_think(Entity* ent)
{
	if (!ent) return;
	ent->velocity.x = 0;
	ent->velocity.y += 3;
	if (level_shape_clip(level_get_active_level(), entity_get_shape_after_move(ent)) == 1) {
		ent->velocity.y = 0;
	}
	if (((SniperData*)(ent->data))->health > 0)
	{
		Vector2D  a, b;
		a = player_get_position();
		b = ent->position;
		float distance = vector2d_magnitude_between(a, b);
		if (distance < 150 && ((SniperData*)(ent->data))->stunned != 1)
		{
			ent->velocity.x += 0.5;
			//Vector2D place = vector2d(0, 0);
			//place.x = ent->position.x;
			//place.y = ent->position.y - 47;
			//bullet_new(vector2d(place.x, place.y), 2, 0.1, 10);
		}
		if (((SniperData*)(ent->data))->stunned == 1)
		{
			((SniperData*)(ent->data))->stunTime--;
			if (((SniperData*)(ent->data))->stunTime == 0)
			{
				((SniperData*)(ent->data))->stunned = 0;
				((SniperData*)(ent->data))->stunTime = 250;
			}
		}
	}
	else if (((SniperData*)(ent->data))->health <= 0)
	{
		ent->sprite = gf2d_sprite_load_all("images/death-sprite.png",
			64,
			64,
			16,
			0);
	}
}

void sniper_draw(Entity* ent)
{
	Vector2D drawPosition, camera;
	if (!ent)return;
	Rect toDraw = ent->shape.s.r;
	toDraw.x += ent->position.x;
	toDraw.y += ent->position.y;
	vector2d_add(toDraw, toDraw, camera_get_draw_offset());
	gf2d_draw_rect(toDraw, GFC_COLOR_GREEN);
}

Vector2D sniper_get_position()
{
	Vector2D v = { 0 };
	if (!SniperDude)return v;
	return SniperDude->position;
}

void sniper_free(Entity* ent)
{
	if (!ent)return;
	/*ent->sprite = NULL;*/
	SniperDude = NULL;
}