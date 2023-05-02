#include "simple_logger.h"
#include "gf2d_graphics.h"
#include "gf2d_draw.h"
#include "level.h"
#include "bullet.h"
#include "camera.h"
#include "player.h"
#include "gunner.h"

void gunner_think(Entity* ent);
void grunt_draw(Entity* ent);
void gunner_free(Entity* ent);

static Entity* Gunner = NULL;


Entity* gunner_new(Vector2D position)
{
	Entity* ent;
	ent = entity_new();
	if (!ent)return NULL;
	ent->sprite = gf2d_sprite_load_all("images/gunner-sprite.png",
		64,
		64,
		16,
		0);
	ent->think = gunner_think;
	ent->free_entity = gunner_free;
	ent->drawOffset = vector2d(32, 64);
	ent->free_entity = gunner_free;
	ent->shape = gfc_shape_rect(-10, -37, 20, 37);
	//ent->draw = gunner_draw;
	vector2d_copy(ent->position, position);

	GunnerData* gunnerStat = gfc_allocate_array(sizeof(GunnerData), 1);
	gunnerStat->health = 200;
	gunnerStat->speed = 2.5;
	gunnerStat->damage = 10;
	ent->data = gunnerStat;
	Gunner = ent;
	return ent;
}


void gunner_think(Entity* ent)
{
	if (!ent) return;
	ent->velocity.x = 0;
	ent->velocity.y += 0.2;
	if (level_shape_clip(level_get_active_level(), entity_get_shape_after_move(ent)) == 1) {
		ent->velocity.y = 0;
	}
	if (((GunnerData*)(ent->data))->health > 0)
	{
		Vector2D  a, b;
		a = player_get_position();
		b = ent->position;
		float distance = vector2d_magnitude_between(a, b);
		//if (distance < 200)
		//{
		//	//ent->velocity.x -= 1;
		//	Vector2D place = vector2d(0, 0);
		//	place.x = ent->position.x - 47;
		//	place.y = ent->position.y - 47;
		//	bullet_new(vector2d(place.x, place.y), 2, 0.1, 10);
		//}
	}
	else if (((GunnerData*)(ent->data))->health <= 0)
	{
		ent->sprite = gf2d_sprite_load_all("images/death-sprite.png",
			64,
			64,
			16,
			0);
	}

}

void gunner_draw(Entity* ent)
{
	Vector2D drawPosition, camera;
	if (!ent)return;
	Rect toDraw = ent->shape.s.r;
	toDraw.x += ent->position.x;
	toDraw.y += ent->position.y;
	vector2d_add(toDraw, toDraw, camera_get_draw_offset());
	gf2d_draw_rect(toDraw, GFC_COLOR_GREEN);
}

Vector2D gunner_get_position()
{
	Vector2D v = { 0 };
	if (!Gunner)return v;
	return Gunner->position;
}

Entity* gunner_get()
{
	return Gunner;
}


void gunner_free(Entity* ent)
{
	if (!ent)return;
	/*ent->sprite = NULL;*/
	Gunner = NULL;
}