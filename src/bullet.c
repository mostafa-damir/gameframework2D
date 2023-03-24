#include "simple_logger.h"

#include "gf2d_draw.h"
#include "gfc_input.h"
#include "camera.h"
#include "level.h"
#include "collision.h"
#include "bullet.h"
#include "player.h"
#include "grunt.h"

void bullet_think(Entity* ent);
void bullet_draw(Entity* ent);
Entity* bullet_new(Vector2D position, int team, float speed, int damage)
{
    Entity* ent;
    ent = entity_new();
    if (!ent)return NULL;
    ent->sprite = gf2d_sprite_load_all(
        "images/bullet.png",
        32,
        32,
        16,
        0);
    ent->think = bullet_think;
    ent->draw = bullet_draw;
    ent->shape = gfc_shape_rect(-8, -31, 16, 31);
    vector2d_copy(ent->position, position);
    ent->speed = speed;
    ent->damage = damage;
    ent->team = team;
    return ent;
}

void bullet_draw(Entity* ent)
{
    Vector2D drawPosition, camera;
    if (!ent)return;
    camera = camera_get_draw_offset();
    vector2d_add(drawPosition, ent->position, camera);
}

void bullet_think(Entity* ent)
{
    Vector2D dir;
    if (ent->team == 1)
    {
        ent->velocity.x += ent->speed;
    }
    else
    {
        ent->velocity.x -= ent->speed;
    }
    if (gfc_input_command_pressed("attack") && ((PlayerData*)(player_get()->data))->ammo != 0 && ent->team == 1)
    {
        ((PlayerData*)(player_get()->data))->ammo = ((PlayerData*)(player_get()->data))->ammo - 1;
        slog("%i", ((PlayerData*)(player_get()->data))->ammo);
    }
    if (level_shape_clip(level_get_active_level(), entity_get_shape_after_move(ent)) != 0) { //if touching a platform
        entity_free(ent);
    }
  /*  Vector2D  a, b;
    a = grunt_get_position();
    b = ent->position;
    float distance = vector2d_magnitude_between(a, b);
    if (distance < 16)
    {
        slog("hit");
        ((GruntData*)(grunt_get()->data))->health = ((GruntData*)(grunt_get()->data))->health - ent->damage;
        slog("%i", ((GruntData*)(grunt_get()->data))->health);
    }*/
   
   

}


/*eol@eof*/