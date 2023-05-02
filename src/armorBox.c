#include "simple_logger.h"
#include "gf2d_graphics.h"
#include "armorBox.h"
#include "camera.h"
#include "gf2d_draw.h"
#include "gfc_input.h"
#include "level.h"
#include "player.h"


void armorBox_free(Entity* ent);
void armorBox_think(Entity* ent);

static Entity* ArmorBox = NULL;


Entity* armorBox_new(Vector2D position)
{
	Entity* ent;
	ent = entity_new();
	if (!ent)return NULL;
	ent->sprite = gf2d_sprite_load_all(
		"images/armor-sprite.png",
		32,
		32,
		16,
		0);
	ent->think = armorBox_think;
	vector2d_copy(ent->position, position);
	ent->drawOffset = vector2d(16, 32);
	ent->free_entity = armorBox_free;
	ent->shape = gfc_shape_rect(-8, -31, 16, 31);
	ent->shape = ent->shape;

	armorBoxData* armorData = gfc_allocate_array(sizeof(armorBoxData), 1);

	armorData->armorUp = 25;
	ent->data = armorData;
	ArmorBox = ent;
	return ent;
}

void armorBox_think(Entity* ent)
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
	if (distance < 16 && ((PlayerData*)(player_get()->data))->armor < ((PlayerData*)(player_get()->data))->maxArmor)
	{
		if (gfc_input_command_pressed("collect"))
		{

			if (((PlayerData*)(player_get()->data))->armor < ((PlayerData*)(player_get()->data))->maxArmor)
			{
				((PlayerData*)(player_get()->data))->armor = min(((PlayerData*)(player_get()->data))->armor + ((armorBoxData*)(ent->data))->armorUp, ((PlayerData*)(player_get()->data))->maxArmor);
				slog(" this the armor %i", ((PlayerData*)(player_get()->data))->armor);
			}
			entity_free(ent);
		}

	}
}

void armorBox_free(Entity* ent)
{
	if (!ent)return;
	ArmorBox = NULL;
}