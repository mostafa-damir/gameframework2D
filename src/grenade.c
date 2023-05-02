#include "simple_logger.h"
#include "gf2d_graphics.h"
#include "grenade.h"
#include "camera.h"
#include "gf2d_draw.h"
#include "gfc_input.h"
#include "grunt.h"
#include "level.h"
#include "player.h"

void grenade_free(Entity* ent);
void grenade_think(Entity* ent);

static Entity* Grenade = NULL;

Entity* grenade_new(Vector2D position)
{
	Entity* ent;
	ent = entity_new();
	if (!ent)return NULL;
	ent->sprite = gf2d_sprite_load_all(
		"images/grenade.png",
		32,
		32,
		16,
		0);
	ent->think = grenade_think;
	vector2d_copy(ent->position, position);
	ent->drawOffset = vector2d(16, 32);
	ent->free_entity = grenade_free;
	ent->shape = gfc_shape_rect(-8, -32, 16, 32);
	//ent->draw = grenade_draw;
	grenadeData* Data = gfc_allocate_array(sizeof(grenadeData), 1);
	Data->pickedup = 1;
	Data->used = false;
	ent->data = Data;
	Grenade = ent;
	return ent;
}

void grenade_think(Entity* ent)
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
				if (((PlayerData*)(player_get()->data))->grenade < ((PlayerData*)(player_get()->data))->maxGrenade && ((grenadeData*)(ent->data))->used == false)
				{
					((PlayerData*)(player_get()->data))->grenade = ((PlayerData*)(player_get()->data))->grenade + ((grenadeData*)(ent->data))->pickedup;
					slog("%i", ((PlayerData*)(player_get()->data))->grenade);
					((grenadeData*)(ent->data))->used = true;


				}
				if (((grenadeData*)(ent->data))->used == true)
				{
					((grenadeData*)(ent->data))->pickedup = 0;
				}
				entity_free(ent);
			}
		}

	}

}

void grenade_draw(Entity* ent)
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
Entity* grenade_get()
{
	return Grenade;
}
void grenade_free(Entity* ent)
{
	if (!ent)return;
	/*ent->sprite = NULL;*/
	Grenade = NULL;
}