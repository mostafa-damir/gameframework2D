#include "simple_logger.h"
#include "gf2d_graphics.h"
#include "gf2d_draw.h"
#include "level.h"
#include "bullet.h"
#include "player.h"
#include "grunt.h"
#include "gfc_audio.h"
#include "camera.h"

void grunt_think(Entity* ent);
void grunt_draw(Entity* ent);
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
	ent->free_entity = grunt_free;
	vector2d_copy(ent->position, position);
	ent->drawOffset = vector2d(32, 64);
	ent->shape = gfc_shape_rect(-10, -37, 20, 37);
	//ent->draw = grunt_draw;

	GruntData* gruntStat = gfc_allocate_array(sizeof(GruntData), 1);
	gruntStat->health = 100;
	gruntStat->speed = 2.5;
	gruntStat->damage = 10;
	gruntStat->dead = 0;
	ent->data = gruntStat;
	Grunt = ent;
	return ent;
}



void grunt_think(Entity* ent)
{
	if (!ent) return;
	Sound* end = gfc_sound_load("audio/wilhelm.mp3", 1, 1);
	ent->velocity.x = 0;
	ent->velocity.y += 0.2;
	if (level_shape_clip(level_get_active_level(), entity_get_shape_after_move(ent)) == 1) {
		ent->velocity.y = 0;
	}
	if (((GruntData*)(ent->data))->health > 0)
	{
		Vector2D  a, b;
		a = player_get_position();
		b = ent->position;
		float distance = vector2d_magnitude_between(a, b);
		if (distance  < 200/* && player_get_position().y == ent->position.y*/)
		{
			//ent->velocity.x -= 1;
			Vector2D place = vector2d(0, 0);
			place.x = ent->position.x - 47;
			place.y = ent->position.y - 47;
			bullet_new(vector2d(place.x, place.y), 2, 0.1, 10);
		}

	}
	else if (((GruntData*)(ent->data))->health <= 0)
	{
		gfc_sound_play(end, 0, 0.5, 0, -1);
		ent->sprite = gf2d_sprite_load_all("images/death-sprite.png",
			64,
			64,
			16,
			0);
	}

}

void grunt_draw(Entity* ent)
{
	Vector2D drawPosition, camera;
	if (!ent)return;
	Rect toDraw = ent->shape.s.r;
	toDraw.x += ent->position.x;
	toDraw.y += ent->position.y;
	vector2d_add(toDraw, toDraw, camera_get_draw_offset());
	gf2d_draw_rect(toDraw, GFC_COLOR_GREEN);
}

Vector2D grunt_get_position()
{
	Vector2D v = { 0 };
	if (!Grunt)return v;
	return Grunt->position;
}

Entity* grunt_get()
{
	return Grunt;
}

void grunt_free(Entity* ent)
{
	if (!ent)return;
	/*ent->sprite = NULL;*/
	Grunt = NULL;
}