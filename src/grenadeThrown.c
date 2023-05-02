#include "simple_logger.h"

#include "gf2d_draw.h"
#include "gfc_input.h"
#include "camera.h"
#include "collision.h"
#include "level.h"
#include "collision.h"
#include "grenadeThrown.h"
#include "grenade.h"
#include "player.h"
#include "firstBoss.h"
#include "grunt.h"
#include "meleeGrunt.h"
#include "sniper.h"
#include "gunner.h"

void grenadeThrown_think(Entity* ent);
void grenadeThrown_draw(Entity* ent);
void grenadeThrown_free(Entity* ent);

static Entity* GrenadeThrown = NULL;


Entity* grenadeThrown_new(Vector2D position)
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
    ent->think = grenadeThrown_think;
    //ent->drawOffset = vector2d(16, 32);
    ent->shape = gfc_shape_rect(0, 0, 16, 31);
    //ent->draw = bullet_draw;
    ent->free_entity = grenadeThrown_free;
    vector2d_copy(ent->position, position);
    GrenadeThrown = ent;
    return ent;
}

void grenadeThrown_think(Entity* ent)
{
    ent->velocity.x += 0.050;
    Vector2D  a, b, c, d, e, f;
    a = grunt_get_position();
    b = ent->position;
    float distance = vector2d_magnitude_between(a, b);
    if (distance < 90 && ((GruntData*)(grunt_get()->data))->health > 0)
    {
        slog("hit");
        ((GruntData*)(grunt_get()->data))->health = ((GruntData*)(grunt_get()->data))->health - 10;
        slog("%i", ((GruntData*)(grunt_get()->data))->health);
        entity_free(ent);

    }
    if (level_shape_clip(level_get_active_level(), entity_get_shape_after_move(ent)) != 0) { //if touching a platform
        entity_free(ent);
    }
}

void grenadeThrown_draw(Entity* ent)
{
    Vector2D drawPosition, camera;
    if (!ent)return;
    Rect toDraw = ent->shape.s.r;
    toDraw.x += ent->position.x;
    toDraw.y += ent->position.y;
    vector2d_add(toDraw, toDraw, camera_get_draw_offset());
    gf2d_draw_rect(toDraw, GFC_COLOR_GREEN);
}

Vector2D grenadeThrown_get_position()
{
    Vector2D v = { 0 };
    if (!GrenadeThrown)return v;
}

Entity* grenadeThrown_get()
{
    return GrenadeThrown;
}

void grenadeThrown_free(Entity* ent)
{
    if (!ent)return;
    /*ent->sprite = NULL;*/
    GrenadeThrown = NULL;
}