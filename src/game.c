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
#include "sachel.h"
#include "shoes.h"
#include "entity.h"
#include "ammoBox.h"
#include "ammoBag.h"
#include "gui.h"
#include "spikeTrap.h"
#include "armorBox.h"
#include "stunMine.h"
#include "stim.h"
#include "armorUp.h"
#include "arPickup.h"
#include "mimic.h"
#include "grenade.h"
#include "stunGrenade.h"
#include "laser.h"
#include "mine.h"
#include "cash.h"
#include "healthUp.h"
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
    Sprite* sprite, * mainMenu, * gameOver;
    Entity* ent;

    int mx, my;
    int menu = 1;
    int over = 0;
    float mf = 0; 
    //Sprite* mouse;
    //Sprite* pistol;
    //Color mouseColor = gfc_color8(255, 100, 255, 200);

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
    gfc_audio_init(4, 4, 4, 4, 1, 0);
    SDL_ShowCursor(SDL_DISABLE);

    /*demo setup*/
    mainMenu = gf2d_sprite_load_image("images/mainMenu.png");
    gameOver = gf2d_sprite_load_image("images/gameOver.png");
    sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    //sprite = gf2d_sprite_load_image("images/hud-sprite.png");
    //mouse = gf2d_sprite_load_all("images/pointer.png", 32, 32, 16, 0);
  
    //ent = space_bug_new(vector2d(1000, 1000));
  
    Sound* mainSong = gfc_sound_load("audio/contra1.mp3", 1, 1);

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
    

        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
        //UI elements last
        gf2d_graphics_next_frame();// render current draw frame and skip to the next frame
        if (menu)
        {
            gf2d_sprite_draw_image(mainMenu, vector2d(0, 0));
            if (gfc_input_command_pressed("enter"))
            {
                menu = 0;
                level = level_load("config/test.level");
                level_set_active_level(level);
                gui_setup_hud();
                gfc_sound_play(mainSong, 999, 0.5, -1, -1);
                healthKit_new(vector2d(200, 900));
                mimic_new(vector2d(150, 900));
                sachel_new(vector2d(400, 900));
                armorBox_new(vector2d(500, 900));
                cash_new(vector2d(600, 900));
                meleeGrunt_new(vector2d(800, 650));
                arPickup_new(vector2d(700, 900));
                shoes_new(vector2d(800, 900));
                ammoBox_new(vector2d(300, 900));
                stim_new(vector2d(350, 900));
                stim_new(vector2d(375, 900));
                //stim_new(vector2d(450, 900));
                armorUp_new(vector2d(100, 750));
                ammoBag_new(vector2d(600, 1050));
                grenade_new(vector2d(725, 1050));
                grenade_new(vector2d(750, 1050));
                grenade_new(vector2d(775, 1050));
                grenade_new(vector2d(675, 1050));
                grenade_new(vector2d(650, 1050));
                stunGrenade_new(vector2d(450, 900));
                stunGrenade_new(vector2d(450, 750));
                stunGrenade_new(vector2d(500, 750));
                healthKit_new(vector2d(150, 750));
                healthKit_new(vector2d(125, 750));
                mine_new(vector2d(300, 700));
                laser_new(vector2d(200, 750));
                stunMine_new(vector2d(400, 700));
                healthUp_new(vector2d(250, 900));
                grunt_new(vector2d(800, 1050));
                gunner_new(vector2d(1000, 1000));
                firstBoss_new(vector2d(1900, 800));
                sniper_new(vector2d(700, 650));
                player_new(vector2d(100, 900));
                smokeGrenade_new(vector2d(125, 900));
                spikeTrap_new(vector2d(550, 1000));
            }
        }
        else
        {
            entity_think_all();
            entity_update_all();
            camera_world_snap();
            gf2d_graphics_clear_screen();// clears drawing buffers
            gf2d_sprite_draw_image(sprite, vector2d(0, 0));
            level_draw(level_get_active_level());
            entity_draw_all();
            gui_draw_hud();
        }
        if (!menu && ((PlayerData*)(player_get()->data))->health <= 0)
        {
            over = 1;
        }
        if (over)
        {
            gf2d_sprite_draw_image(gameOver, vector2d(0, 0));
            if (gfc_input_command_released("enter") && over)
            {
                entity_free_all();
                menu = 1;
                over = 0;
            }

        }     
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    SJson* json, * a;
    json = sj_load("config/scores.json");
    a = sj_copy(json);

    sj_object_insert(a, "score", sj_new_int(((PlayerData*)(player_get()->data))->cash));
    sj_save(a, "config/scores.json");
    if(level_get_active_level())
    level_free(level_get_active_level());
    entity_free_all();
    slog("---==== END ====---");


   

    return 0;
}
/*eol@eof*/