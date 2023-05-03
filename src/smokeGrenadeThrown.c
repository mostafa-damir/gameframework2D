#include "simple_logger.h"

#include "gf2d_draw.h"
#include "gfc_input.h"
#include "camera.h"
#include "collision.h"
#include "level.h"
#include "collision.h"
#include "smokeGrenadeThrown.h"
#include "smokeGrenade.h"
#include "player.h"
#include "firstBoss.h"
#include "grunt.h"
#include "sniper.h"
#include "meleeGrunt.h"
#include "sniper.h"
#include "gunner.h"
#include "simple_logger.h"

#include "gf2d_draw.h"
#include "gfc_input.h"
#include "camera.h"
#include "collision.h"
#include "level.h"
#include "collision.h"
#include "smokeGrenadeThrown.h"
#include "smokeGrenade.h"
#include "player.h"
#include "firstBoss.h"
#include "grunt.h"
#include "sniper.h"
#include "meleeGrunt.h"
#include "sniper.h"
#include "gunner.h"

void smokeGrenadeThrown_think(Entity* ent);
void smokeGrenadeThrown_draw(Entity* ent);
void smokeGrenadeThrown_free(Entity* ent);

static Entity* smokeGrenadeThrown = NULL;


Entity* smokeGrenadeThrown_new(Vector2D position)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)return NULL;
    ent->sprite = gf2d_sprite_load_all(
        "images/smoke.png",
        32,
        32,
        16,
        0);
    ent->think = smokeGrenadeThrown_think;
    //ent->drawOffset = vector2d(16, 32);
    ent->shape = gfc_shape_rect(0, 0, 16, 31);
    //ent->draw = smokeGrendaeThrown_draw;
    ent->free_entity = smokeGrenadeThrown_free;
    vector2d_copy(ent->position, position);
    smgtData* Data = gfc_allocate_array(sizeof(smgtData), 1);
    Data->active = 0;
    Data->health = 250;
    smokeGrenadeThrown = ent;
    return ent;
}

void smokeGrenadeThrown_think(Entity* ent)
{
    Vector2D  a, b;
    ent->velocity.y += 0.2;
    if (level_shape_clip(level_get_active_level(), entity_get_shape_after_move(ent)) == 1) {
        ent->velocity.y = 0;
    }
    if (((PlayerData*)(player_get()->data))->smokeHealth == 0)
    {
        entity_free(ent);
    }
}

void smokeGrenadeThrown_draw(Entity* ent)
{
    Vector2D drawPosition, camera;
    if (!ent)return;
    Rect toDraw = ent->shape.s.r;
    toDraw.x += ent->position.x;
    toDraw.y += ent->position.y;
    vector2d_add(toDraw, toDraw, camera_get_draw_offset());
    gf2d_draw_rect(toDraw, GFC_COLOR_GREEN);
}

Vector2D smokeGrenadeThrown_get_position()
{
    Vector2D v = { 0 };
    if (!smokeGrenadeThrown)return v;
}

Entity* smokeGrenadeThrown_get()
{
    return smokeGrenadeThrown;
}

void smokeGrenadeThrown_free(Entity* ent)
{
    if (!ent)return;
    /*ent->sprite = NULL;*/
    smokeGrenadeThrown = NULL;
}