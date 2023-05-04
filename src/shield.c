#include "simple_logger.h"

#include "gf2d_draw.h"
#include "gfc_input.h"
#include "camera.h"
#include "collision.h"
#include "level.h"
#include "shield.h"
#include "generator.h"
#include "player.h"
#include "firstBoss.h"
#include "grunt.h"
#include "sniper.h"
#include "meleeGrunt.h"
#include "sniper.h"
#include "gunner.h"

void shield_think(Entity* ent);
void shield_draw(Entity* ent);
void shield_free(Entity* ent);

static Entity* shield = NULL;


Entity* shield_new(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)return NULL;
    ent->sprite = gf2d_sprite_load_all(
        "images/shield.png",
        32,
        32,
        16,
        0);
    ent->think = shield_think;
    //ent->drawOffset = vector2d(16, 32);
    ent->shape = gfc_shape_rect(0, 0, 16, 31);
    //ent->draw = shield_draw;
    ent->free_entity = shield_free;
    vector2d_copy(ent->position, position);
    shieldData* Data = gfc_allocate_array(sizeof(shieldData), 1);
    Data->active = 0;
    Data->health = 250;
    shield = ent;
    return ent;
}

void shield_think(Entity* ent)
{
    Vector2D  a, b;
    ent->velocity.y += 0.2;
    if (level_shape_clip(level_get_active_level(), entity_get_shape_after_move(ent)) == 1) {
        ent->velocity.y = 0;
    }
    if (((PlayerData*)(player_get()->data))->shieldHealth == 0)
    {
        entity_free(ent);
    }
}

void shield_draw(Entity* ent)
{
    Vector2D drawPosition, camera;
    if (!ent)return;
    Rect toDraw = ent->shape.s.r;
    toDraw.x += ent->position.x;
    toDraw.y += ent->position.y;
    vector2d_add(toDraw, toDraw, camera_get_draw_offset());
    gf2d_draw_rect(toDraw, GFC_COLOR_GREEN);
}

Vector2D shield_get_position()
{
    Vector2D v = { 0 };
    if (!shield) return v;
    return shield->position;

}

Entity* shield_get()
{
    return shield;
}

void shield_free(Entity* ent)
{
    if (!ent)return;
    /*ent->sprite = NULL;*/
    shield = NULL;
}