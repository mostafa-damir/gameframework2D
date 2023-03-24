#include <SDL.h>
#include "simple_logger.h"
#include "gfc_list.h"
#include "gfc_audio.h"
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "gfc_input.h"
#include "simple_json_object.h"
#include "level.h"
#include "camera.h"
#include "sniper.h"
#include "entity.h"
#include "ammoBox.h"
#include "gui.h"
#include "armorBox.h"
#include "arPickup.h"
#include "cash.h"
#include "space_bug.h"
#include "player.h"
#include "meleeGrunt.h"
#include "grunt.h"
#include "gunner.h"
#include "firstBoss.h"
#include "weapons.h"
#include "healthKit.h"

int main(int argc, char* argv[])
{
    /*variable declarations*/
    int done = 0;
    Level* level;
    const Uint8* keys;
    Sprite* sprite;
    Entity* ent;

    int mx, my;
    float mf = 0;
    Sprite* mouse;
    Sprite* pistol;
    Color mouseColor = gfc_color8(255, 100, 255, 200);

    /*program initializtion*/
    init_logger("gf2d.log", 0);
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1200,
        720,
        1200,
        720,
        vector4d(0, 0, 0, 255),
        0);
    gfc_audio_init(
        1024,
        16,
        5,
        MIX_MAX_VOLUME,
        1,
        1);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    gfc_input_init("config/input.cfg");
    entity_manager_init(1024);
    SDL_ShowCursor(SDL_DISABLE);
    
    /*demo setup*/
    sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    //sprite = gf2d_sprite_load_image("images/hud-sprite.png");
    mouse = gf2d_sprite_load_all("images/pointer.png", 32, 32, 16, 0);
    level = level_load("config/test.level");
    level_set_active_level(level);
    //ent = space_bug_new(vector2d(1000, 1000));
    gui_setup_hud();
    ent = player_new(vector2d(100, 900));
    ent = healthKit_new(vector2d(200, 900));
    ent = healthKit_new(vector2d(400, 900));
    ent = armorBox_new(vector2d(500, 900));
    ent = cash_new(vector2d(600, 900));
    ent = meleeGrunt_new(vector2d(800, 650));
    ent = arPickup_new(vector2d(700, 900));
    ent = ammoBox_new(vector2d(600, 1050));
    ent = grunt_new(vector2d(800, 1050));
    ent = gunner_new(vector2d(1000, 1000));
    ent = firstBoss_new(vector2d(1100, 800));
    ent = sniper_new(vector2d(700, 650));
    /*main game loop*/
    while (!done)
    {
        gfc_input_update();
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx, &my);
        mf += 0.1;
        if (mf >= 16.0)mf = 0;
        entity_think_all();
        entity_update_all();
        camera_world_snap();

        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
        gf2d_sprite_draw_image(sprite, vector2d(0, 0));
        level_draw(level_get_active_level());
        entity_draw_all();
        //UI elements last
       /* gf2d_sprite_draw(
            mouse,
            vector2d(mx, my),
            NULL,
            NULL,
            NULL,
            NULL,
            &mouseColor,
            (int)mf);*/
        gui_draw_hud();
        gf2d_graphics_next_frame();// render current draw frame and skip to the next frame

        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    level_free(level);
    entity_free(ent);
    slog("---==== END ====---");
    

    SJson* json, * a;
    json = sj_load("config/scores.json");
    a = sj_copy(json);

    sj_object_insert(a, "score", sj_new_int(((PlayerData*)(player_get()->data))->cash));
    sj_save(a, "config/scores.json");
    
    return 0;
}
/*eol@eof*/