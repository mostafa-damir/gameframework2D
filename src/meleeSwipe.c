#include "simple_logger.h"

#include "gf2d_draw.h"
#include "gfc_input.h"
#include "camera.h"
#include "level.h"
#include "collision.h"
#include "meleeSwipe.h"
#include "player.h"
#include "grunt.h"

void meleeSwipe_think(Entity* ent);
void meleeSwipe_draw(Entity* ent);
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
    ent->draw = meleeSwipe_draw;
    ent->shape = gfc_shape_rect(-8, -31, 16, 31);
    vector2d_copy(ent->position, position);
    ent->damage = damage;
    ent->team = team;
    return ent;
}

void meleeSwipe_draw(Entity* ent)
{
    Vector2D drawPosition, camera;
    if (!ent)return;
    camera = camera_get_draw_offset();
    vector2d_add(drawPosition, ent->position, camera);
}

void meleeSwipe_think(Entity* ent)
{
    Vector2D dir;
    ent->velocity.x += 0.1;
    if (level_shape_clip(level_get_active_level(), entity_get_shape_after_move(ent)) != 0) { //if touching a platform
        entity_free(ent);
    }

}


/*eol@eof*/