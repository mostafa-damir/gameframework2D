#include "simple_logger.h"

#include "gfc_input.h"
#include "gui.h"
#include "gf2d_graphics.h"
#include "gf2d_draw.h"
#include "camera.h"
#include "healthKit.h"
#include "level.h"
#include "meleeSwipe.h"
#include "bullet.h"
#include "arPickup.h"
#include "player.h"


void player_think(Entity* self);
void player_draw(Entity* self);
void player_free(Entity* self);


static Entity* ThePlayer = NULL;

void player_set_position(Vector2D position)
{
	if (!ThePlayer)
	{
		return;
	}
	vector2d_copy(ThePlayer->position, position);
}

Entity* player_new(Vector2D position)
{
	Entity* self;
	self = entity_new();
	if (!self)return NULL;
	/*gfc_line_cpy(ent->name,"player");*/
	self->sprite = gf2d_sprite_load_all("images/playerKnife.png",
		64,
		64,
		16,
		0);
	//ent->action = gf2d_actor_get_action_by_name(ent->sprite, "idle");
	self->think = player_think;
	//self->update = player_update;
	self->draw = player_draw;
	self->drawOffset = vector2d(32, 64);
	self->free_entity = player_free;
	self->shape = gfc_shape_rect(-10, -37, 20,37);
	self->shape = self->shape;
	vector2d_copy(self->position, position);
	int speed;
	SJson* json,* player;
	json = sj_load("config/playerStats.json");
	if (!json)
	{
		return NULL;
	}
	player = sj_object_get_value(json, "player");

	sj_object_get_value_as_int(player, "speed", &speed);

	PlayerData* playerStat = gfc_allocate_array(sizeof(PlayerData), 1);
	playerStat->ammo = 900;
	playerStat->maxAmmo = 1000;
	playerStat->armor = 25;
	playerStat->maxArmor = 100;
	playerStat->health =  75;
	playerStat->maxHealth = 100;
	playerStat->cash = 0;
	playerStat->ar = 0;
	playerStat->knife = 1;
	playerStat->pistol = 1;
	playerStat->smg = 1;
	playerStat->speed = speed;
	self->data = playerStat;
	ThePlayer = self;
	return self;
}	

Vector2D player_get_position( )
{
	Vector2D v = { 0 };
	if (!ThePlayer)return v;
	return ThePlayer->position;
}

Entity* player_get()
{
	return ThePlayer;
}

void player_draw(Entity* self)
{
	Vector2D drawPosition, camera;
	if (!self)return;
	camera = camera_get_draw_offset();
	vector2d_add(drawPosition, self->position, camera);
	gui_set_health(((PlayerData*)(self->data))->health / (float)((PlayerData*)(self->data))->maxHealth);
	Rect toDraw = self->shape.s.r;
	toDraw.x += self->position.x;
	toDraw.y += self->position.y;
	vector2d_add(toDraw, toDraw, camera_get_draw_offset());
	//gf2d_draw_rect(toDraw, GFC_COLOR_YELLOW);
}

void player_think(Entity* self)
{
	if (!self)return;
	Vector2D dir;
	Vector2D camera;
	Vector2D walk = vector2d(0,0);
	if (!self) return;
	self->velocity.x = 0;
	self->velocity.y += 0.2;
	if (((PlayerData*)(self->data))->health != 0)
	{
		if (level_shape_clip(level_get_active_level(), entity_get_shape_after_move(self)) == 1) {
			if ((gfc_input_command_down("jump")))
			{
				self->velocity.y = -10;
			}
			else
			{
				self->velocity.y = 0;
			}
		}

		if (gfc_input_command_down("walkleft"))
		{
			walk.x = -1;
		}
		if (gfc_input_command_down("walkright"))
		{
			walk.x += 1;
		}
		if (gfc_input_command_down("hurt"))
		{
			((PlayerData*)(self->data))->health -= 10;
		}
		if (gfc_input_command_down("switchToKnife"))
		{
			((PlayerData*)(self->data))->selectedWeapon = 0;
			self->sprite = gf2d_sprite_load_all("images/playerKnife.png",
				64,
				64,
				16,
				0);
		}
		if (((PlayerData*)(self->data))->ar == 1)
		{
			if (gfc_input_command_down("switchToAr"))
			{
				((PlayerData*)(self->data))->selectedWeapon = 1;
				self->sprite = gf2d_sprite_load_all("images/playerAr.png",
					64,
					64,
					16,
					0);
			}
		}
		if (gfc_input_command_down("switchToSmg"))
		{
			((PlayerData*)(self->data))->selectedWeapon = 2;
			self->sprite = gf2d_sprite_load_all("images/playerSmg.png",
				64,
				64,
				16,
				0);
		}
		if (gfc_input_command_down("switchToShotgun"))
		{
			((PlayerData*)(self->data))->selectedWeapon = 3;
			self->sprite = gf2d_sprite_load_all("images/playerShotgun.png",
				64,
				64,
				16,
				0);
		}
		if (gfc_input_command_down("switchToMinigun"))
		{
			((PlayerData*)(self->data))->selectedWeapon = 4;
			self->sprite = gf2d_sprite_load_all("images/playerMinigun.png",
				64,
				64,
				16,
				0);
		}
		if (gfc_input_command_down("switchToSniper"))
		{
			((PlayerData*)(self->data))->selectedWeapon = 5;
			self->sprite = gf2d_sprite_load_all("images/playeSniper.png",
				64,
				64,
				16,
				0);
		}
		if (gfc_input_command_down("switchToPistol"))
		{
			((PlayerData*)(self->data))->selectedWeapon = 6;
			self->sprite = gf2d_sprite_load_all("images/playerPistol.png", 64, 64, 16, 0);
		}
		if (gfc_input_command_down("switchToRevolver"))
		{
			((PlayerData*)(self->data))->selectedWeapon = 7;
			self->sprite = gf2d_sprite_load_all("images/playerRevolver.png",
				64,
				64,
				16,
				0);
		}
		if (gfc_input_command_down("switchToBr"))
		{
			((PlayerData*)(self->data))->selectedWeapon = 8;
			self->sprite = gf2d_sprite_load_all("images/playerBr.png",
				64,
				64,
				5,
				0);
		}
		if (gfc_input_command_down("switchToScythe"))
		{
			((PlayerData*)(self->data))->selectedWeapon = 9;
			self->sprite = gf2d_sprite_load_all("images/playerScythe.png",
				64,
				64,
				16,
				0);
		}

		if (vector2d_magnitude_compare(walk, 0) != 0)
		{
			vector2d_set_magnitude(&walk, ((PlayerData*)(self->data))->speed);
			vector2d_copy(self->velocity, walk);
		}

		if (gfc_input_command_pressed("attack") && ((PlayerData*)(self->data))->ammo != 0)
		{
			Vector2D place = vector2d(0, 0);
			place.x = self->position.x;
			place.y = self->position.y - 47;
			if (((PlayerData*)(self->data))->selectedWeapon == 3)
			{
				bullet_new(vector2d(place.x, place.y + 8), 1, 1, 10);
				bullet_new(vector2d(place.x, place.y),1, 1, 10);
				bullet_new(vector2d(place.x, place.y - 8),1, 1, 10);
			}
			else if (((PlayerData*)(self->data))->selectedWeapon == 5)
			{
				bullet_new(vector2d(place.x, place.y),1, 0.1, 100);
			}
			else if (((PlayerData*)(self->data))->selectedWeapon == 6)
			{
				bullet_new(vector2d(place.x, place.y),1, 0.4, 2);
			}
			else if (((PlayerData*)(self->data))->selectedWeapon == 7)
			{
				bullet_new(vector2d(place.x, place.y),1 , 0.3, 25);
			}
			else if (((PlayerData*)(self->data))->selectedWeapon == 8)
			{
				bullet_new(vector2d(place.x, place.y),1 , 0.5, 10);
				bullet_new(vector2d(place.x, place.y),1 , 0.3, 10);
				bullet_new(vector2d(place.x, place.y),1 , 0.2, 10);
			}


		}
		else if (gfc_input_command_held("attack") && ((PlayerData*)(self->data))->ammo != 0)
		{
			Vector2D place = vector2d(0, 0);
			place.x = self->position.x;
			place.y = self->position.y - 47;
			if (((PlayerData*)(self->data))->selectedWeapon == 1)
			{
				bullet_new(vector2d(place.x, place.y),1 , 0.4, 10);
			}
			else if (((PlayerData*)(self->data))->selectedWeapon == 2)
			{
				bullet_new(vector2d(place.x, place.y),1 , 0.1, 5);
			}
			else if (((PlayerData*)(self->data))->selectedWeapon == 4)
			{
				bullet_new(vector2d(place.x, place.y),1, 1, 2);
			}
		}
		if (gfc_input_command_pressed("melee"))
		{
			Vector2D place = vector2d(0, 0);
			place.x = self->position.x;
			place.y = self->position.y - 47;
			if (((PlayerData*)(self->data))->selectedWeapon == 0)
			{
				meleeSwipe_new(vector2d(place.x, place.y),1, 10);
			}
			else if (((PlayerData*)(self->data))->selectedWeapon == 9)
			{
				meleeSwipe_new(vector2d(place.x, place.y), 1, 100);

			}
		}
	}
	camera_center_at(self->position);
}

void player_free(Entity* self)
{
    if (!self)return;
    ThePlayer = NULL;
}

/*eol@eof*/