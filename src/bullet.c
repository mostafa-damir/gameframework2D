#include "simple_logger.h"

#include "gf2d_draw.h"
#include "gfc_input.h"
#include "camera.h"
#include "collision.h"
#include "level.h"
#include "collision.h"
#include "bullet.h"
#include "player.h"
#include "firstBoss.h"
#include "grunt.h"
#include "meleeGrunt.h"
#include "sniper.h"
#include "gunner.h"


void bullet_think(Entity* ent);
void bullet_draw(Entity* ent);
void bullet_free(Entity* ent);

static Entity* Bullet = NULL;


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
    //ent->drawOffset = vector2d(16, 32);
    ent->shape = gfc_shape_rect(0, 0, 16, 31);
    //ent->draw = bullet_draw;
    ent->free_entity = bullet_free;
    vector2d_copy(ent->position, position);
    BulletData* data = gfc_allocate_array(sizeof(BulletData), 1);
    ent->speed = speed;
    ent->damage = damage;
    ent->team = team;
    ent->hit = 0;
    Bullet = ent;
    return ent;
}

void bullet_draw(Entity* ent)
{
    Vector2D drawPosition, camera;
    if (!ent)return;
    Rect toDraw = ent->shape.s.r;
    toDraw.x += ent->position.x;
    toDraw.y += ent->position.y;
    vector2d_add(toDraw, toDraw, camera_get_draw_offset());
    gf2d_draw_rect(toDraw, GFC_COLOR_GREEN);
}

void bullet_think(Entity* ent)
{
    Vector2D dir;
    if (ent->team == 1)
    {
        ent->velocity.x += ent->speed;
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
            //slog("%i", ((SniperData*)(sniper_get()->data))->health
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
  

   
}

Vector2D bullet_get_position()
{
    Vector2D v = { 0 };
    if (!Bullet)return v;
}

Entity* bullet_get()
{
    return Bullet;
}

void bullet_free(Entity* ent)
{
    if (!ent)return;
    /*ent->sprite = NULL;*/
    Bullet = NULL;
}


/*eol@eof*/