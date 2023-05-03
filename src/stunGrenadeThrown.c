#include "simple_logger.h"

#include "gf2d_draw.h"
#include "gfc_input.h"
#include "camera.h"
#include "collision.h"
#include "level.h"
#include "collision.h"
#include "stunGrenadeThrown.h"
#include "stunGrenade.h"
#include "player.h"
#include "firstBoss.h"
#include "grunt.h"
#include "sniper.h"
#include "meleeGrunt.h"
#include "sniper.h"
#include "gunner.h"

void stunGrenadeThrown_think(Entity* ent);
void stunGrenadeThrown_draw(Entity* ent);
void stunGrenadeThrown_free(Entity* ent);

static Entity* StunGrenadeThrown = NULL;


Entity* stunGrenadeThrown_new(Vector2D position)
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
    ent->think = stunGrenadeThrown_think;
    //ent->drawOffset = vector2d(16, 32);
    ent->shape = gfc_shape_rect(0, 0, 16, 31);
    //ent->draw = stunGrendaeThrown_draw;
    ent->free_entity = stunGrenadeThrown_free;
    vector2d_copy(ent->position, position);
    StunGrenadeThrown = ent;
    return ent;
}

void stunGrenadeThrown_think(Entity* ent)
{
    ent->velocity.x += 0.050;
    Vector2D  a, b, c, d, e, f;
    a = grunt_get_position();
    b = ent->position;
    float distance = vector2d_magnitude_between(a, b);
    if (distance < 90 && ((GruntData*)(grunt_get()->data))->health > 0)
    {
        ((GruntData*)(grunt_get()->data))->stunned = 1;
        slog("stunned");
        entity_free(ent);
    }
    e = sniper_get_position();
    float distance4 = vector2d_magnitude_between(e, b);
    if (distance4 < 90 && ((SniperData*)(sniper_get()->data))->health > 0)
    {
        ((SniperData*)(sniper_get()->data))->stunned = 1;
        slog("stunned");
        entity_free(ent);
    }
    if (level_shape_clip(level_get_active_level(), entity_get_shape_after_move(ent)) != 0) {
        entity_free(ent);
    }
}

void stunGrenadeThrown_draw(Entity* ent)
{
    Vector2D drawPosition, camera;
    if (!ent)return;
    Rect toDraw = ent->shape.s.r;
    toDraw.x += ent->position.x;
    toDraw.y += ent->position.y;
    vector2d_add(toDraw, toDraw, camera_get_draw_offset());
    gf2d_draw_rect(toDraw, GFC_COLOR_GREEN);
}

Vector2D stunGrenadeThrown_get_position()
{
    Vector2D v = { 0 };
    if (!StunGrenadeThrown)return v;
}

Entity* stunGrenadeThrown_get()
{
    return StunGrenadeThrown;
}

void stunGrenadeThrown_free(Entity* ent)
{
    if (!ent)return;
    /*ent->sprite = NULL;*/
    StunGrenadeThrown = NULL;
}