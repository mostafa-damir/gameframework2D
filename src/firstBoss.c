#include "simple_logger.h"
#include "gf2d_graphics.h"
#include "gf2d_draw.h"
#include "level.h"
#include "bullet.h"
#include "player.h"
#include "firstBoss.h"

void firstBoss_think(Entity* self);
void firstBoss_free(Entity* ent);

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
	//ent->draw = grunt_draw;
	ent->drawOffset = vector2d(32, 64);
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

void firstBoss_think(Entity* ent)
{
	if (!ent) return;
	ent->velocity.x = 0;
	ent->velocity.y += 0.2;
	if (level_shape_clip(level_get_active_level(), entity_get_shape_after_move(ent)) == 1) {
		ent->velocity.y = 0;
	}
	/*if (gfc_input_command_down("half"))
	{
		ent->health = 500;
	}
	if (ent->health <= 500)
	{
		((firstBossData*)(ent->data))->tiers = 1;
		ent->sprite = gf2d_sprite_load_all("images/bossPhase2.png",
			128,
			128,
			16,
			0);
	}*/
}
void firstBoss_free(Entity* ent)
{
	if (!ent)return;
	Boss = NULL;
}