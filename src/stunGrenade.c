#include "simple_logger.h"
#include "gf2d_graphics.h"
#include "stunGrenade.h"
#include "camera.h"
#include "gf2d_draw.h"
#include "gfc_input.h"
#include "grunt.h"
#include "level.h"
#include "player.h"

void stunGrenade_free(Entity* ent);
void stunGrenade_think(Entity* ent);

static Entity* StunGrenade = NULL;

Entity* stunGrenade_new(Vector2D position)
{
	Entity* ent;
	ent = entity_new();
	if (!ent)return NULL;
	ent->sprite = gf2d_sprite_load_all(
		"images/stunGrenade.png",
		32,
		32,
		16,
		0);
	ent->think = stunGrenade_think;
	vector2d_copy(ent->position, position);
	ent->drawOffset = vector2d(16, 32);
	ent->free_entity = stunGrenade_free;
	ent->shape = gfc_shape_rect(-8, -32, 16, 32);
	//ent->draw = grenade_draw;
	stunGrenadeData* Data = gfc_allocate_array(sizeof(stunGrenadeData), 1);
	Data->pickedup = 1;
	Data->used = false;
	ent->data = Data;
	StunGrenade = ent;
	return ent;
}

void stunGrenade_think(Entity* ent)
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
		if (((PlayerData*)(player_get()->data))->stunGrenade != ((PlayerData*)(player_get()->data))->maxStunGrenade)
		{
			if (gfc_input_command_pressed("collect"))
			{
				if (((PlayerData*)(player_get()->data))->stunGrenade < ((PlayerData*)(player_get()->data))->maxStunGrenade && ((stunGrenadeData*)(ent->data))->used == false)
				{
					((PlayerData*)(player_get()->data))->stunGrenade = ((PlayerData*)(player_get()->data))->stunGrenade + ((stunGrenadeData*)(ent->data))->pickedup;
					slog("%i", ((PlayerData*)(player_get()->data))->stunGrenade);
					((stunGrenadeData*)(ent->data))->used = true;


				}
				if (((stunGrenadeData*)(ent->data))->used == true)
				{
					((stunGrenadeData*)(ent->data))->pickedup = 0;
				}
				entity_free(ent);
			}
		}

	}

}

void stunGrenade_draw(Entity* ent)
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
Entity* stunGrenade_get()
{
	return StunGrenade;
}
void stunGrenade_free(Entity* ent)
{
	if (!ent)return;
	/*ent->sprite = NULL;*/
	StunGrenade = NULL;
}