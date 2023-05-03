#include "simple_logger.h"
#include "gf2d_graphics.h"
#include "smokeGrenade.h"
#include "camera.h"
#include "gf2d_draw.h"
#include "gfc_input.h"
#include "grunt.h"
#include "level.h"
#include "player.h"

void smokeGrenade_free(Entity* ent);
void smokeGrenade_think(Entity* ent);

static Entity* Grenade = NULL;

Entity* smokeGrenade_new(Vector2D position)
{
	Entity* ent;
	ent = entity_new();
	if (!ent)return NULL;
	ent->sprite = gf2d_sprite_load_all(
		"images/smokeGrenade.png",
		32,
		32,
		16,
		0);
	ent->think = smokeGrenade_think;
	vector2d_copy(ent->position, position);
	ent->drawOffset = vector2d(16, 32);
	ent->free_entity = smokeGrenade_free;
	ent->shape = gfc_shape_rect(-8, -32, 16, 32);
	//ent->draw = grenade_draw;
	smokeGrenadeData* Data = gfc_allocate_array(sizeof(smokeGrenadeData), 1);
	Data->pickedup = 1;
	Data->used = false;
	ent->data = Data;
	Grenade = ent;
	return ent;
}

void smokeGrenade_think(Entity* ent)
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
	//slog("%f", distance);
	if (distance < 16)
	{
		if (((PlayerData*)(player_get()->data))->grenade != ((PlayerData*)(player_get()->data))->maxGrenade)
		{
			if (gfc_input_command_pressed("collect"))
			{
				if (((PlayerData*)(player_get()->data))->smokeGrenade < ((PlayerData*)(player_get()->data))->maxSmokeGrenade && ((smokeGrenadeData*)(ent->data))->used == false)
				{
					((PlayerData*)(player_get()->data))->smokeGrenade = ((PlayerData*)(player_get()->data))->smokeGrenade + ((smokeGrenadeData*)(ent->data))->pickedup;
					slog("%i", ((PlayerData*)(player_get()->data))->grenade);
					((smokeGrenadeData*)(ent->data))->used = true;


				}
				if (((smokeGrenadeData*)(ent->data))->used == true)
				{
					((smokeGrenadeData*)(ent->data))->pickedup = 0;
				}
				entity_free(ent);
			}
		}

	}

}

void smokeGrenade_draw(Entity* ent)
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
Entity* smokeGrenade_get()
{
	return Grenade;
}
void smokeGrenade_free(Entity* ent)
{
	if (!ent)return;
	/*ent->sprite = NULL;*/
	Grenade = NULL;
}