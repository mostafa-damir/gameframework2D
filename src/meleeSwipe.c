#include "simple_logger.h"

#include "gf2d_draw.h"
#include "gfc_input.h"
#include "camera.h"
#include "level.h"
#include "collision.h"
#include "meleeSwipe.h"
#include "grunt.h"
#include "meleeGrunt.h"
#include "sniper.h"
#include "gunner.h"
#include "firstBoss.h"
#include "player.h"
#include "grunt.h"

void meleeSwipe_think(Entity* ent);
void meleeSwipe_draw(Entity* ent);
void meleeSwipe_free(Entity* ent);
static Entity* MeleeSwipe = NULL;

Entity* meleeSwipe_new(Vector2D position, int team, int damage)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)return NULL;
    ent->sprite = gf2d_sprite_load_all(
        "images/swipe.png",
        32,
        32,
        16,
        0);
    ent->think = meleeSwipe_think;
    //ent->draw = meleeSwipe_draw;
    ent->free_entity = meleeSwipe_free;
    ent->shape = gfc_shape_rect(0, 0, 16, 31);
    vector2d_copy(ent->position, position);
    ent->damage = damage;
    ent->team = team;
    MeleeSwipe = ent;
    return ent;
}

void meleeSwipe_draw(Entity* ent)
{
    Vector2D drawPosition, camera;
    if (!ent)return;
    Rect toDraw = ent->shape.s.r;
    toDraw.x += ent->position.x;
    toDraw.y += ent->position.y;
    vector2d_add(toDraw, toDraw, camera_get_draw_offset());
    gf2d_draw_rect(toDraw, GFC_COLOR_GREEN);
}

void meleeSwipe_think(Entity* ent)
{
    Vector2D dir;
    ent->velocity.x += 0.1;
    if (level_shape_clip(level_get_active_level(), entity_get_shape_after_move(ent)) != 0) { //if touching a platform
        entity_free(ent);
    }
    Vector2D  a, b, c, d, e, f;
    a = grunt_get_position();
    b = ent->position;
    float distance = vector2d_magnitude_between(a, b);
    if (distance < 90 && ((GruntData*)(grunt_get()->data))->health > 0)
    {
        ((GruntData*)(grunt_get()->data))->health = ((GruntData*)(grunt_get()->data))->health - ent->damage;
        //slog("%i", ((GruntData*)(grunt_get()->data))->health);
        entity_free(ent);
    }
    c = gunner_get_position();
    float distance2 = vector2d_magnitude_between(c, b);
    if (distance2 < 90 && ((GunnerData*)(gunner_get()->data))->health > 0)
    {
        ((GunnerData*)(gunner_get()->data))->health = ((GunnerData*)(gunner_get()->data))->health - ent->damage;
        //slog("%i", ((GunnerData*)(gunner_get()->data))->health);
        entity_free(ent);
    }
    d = meleeGrunt_get_position();
    float distance3 = vector2d_magnitude_between(d, b);
    if (distance3 < 90 && ((meleeGruntData*)(meleeGrunt_get()->data))->health > 0)
    {
        ((meleeGruntData*)(meleeGrunt_get()->data))->health = ((meleeGruntData*)(meleeGrunt_get()->data))->health - ent->damage;
        entity_free(ent);
    }
    e = sniper_get_position();
    float distance4 = vector2d_magnitude_between(e, b);
    if (distance4 < 90 && ((SniperData*)(sniper_get()->data))->health > 0)
    {
        ((SniperData*)(sniper_get()->data))->health = ((SniperData*)(sniper_get()->data))->health - ent->damage;
        //slog("%i", ((SniperData*)(sniper_get()->data))->health);
        entity_free(ent);
    }
    f = firstBoss_get_position();
    float distance5 = vector2d_magnitude_between(f, b);
    if (distance5 < 90 && ((firstBossData*)(firstBoss_get()->data))->health > 0)
    {
        ((firstBossData*)(firstBoss_get()->data))->health = ((firstBossData*)(firstBoss_get()->data))->health - ent->damage;
        //slog("%i", ((firstBossData*)(firstBoss_get()->data))->health);
        entity_free(ent);
    }
}

void meleeSwipe_free(Entity* ent)
{
    if (!ent)return;
    /*ent->sprite = NULL;*/
    MeleeSwipe = NULL;
}


/*eol@eof*/