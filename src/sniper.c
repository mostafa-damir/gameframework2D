#include "simple_logger.h"
#include "gf2d_graphics.h"
#include "gf2d_draw.h"
#include "level.h"
#include "bullet.h"
#include "player.h"
#include "sniper.h"

void sniper_think(Entity* ent);
void sniper_free(Entity* ent);

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
	ent->data = Stat;
	SniperDude = ent;
	return ent;
}

Entity* sniper_get()
{
	return Sniper;
}

void sniper_think(Entity* ent)
{
	if (!ent) return;
	ent->velocity.x = 0;
	ent->velocity.y += 3;
	if (level_shape_clip(level_get_active_level(), entity_get_shape_after_move(ent)) == 1) {
		ent->velocity.y = 0;
	}
	Vector2D  a, b;
	a = player_get_position();
	b = ent->position;
	float distance = vector2d_magnitude_between(a, b);
	if (distance < 150)
	{
		ent->velocity.x += 0.5;
		//Vector2D place = vector2d(0, 0);
		//place.x = ent->position.x;
		//place.y = ent->position.y - 47;
		//bullet_new(vector2d(place.x, place.y), 2, 0.1, 10);
	}

}
void sniper_free(Entity* ent)
{
	if (!ent)return;
	/*ent->sprite = NULL;*/
	SniperDude = NULL;
}