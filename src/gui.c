#include "gui.h"
#include "gf2d_draw.h"
#include "gf2d_sprite.h"
#include "gfc_shape.h"
#include "gfc_color.h"

typedef struct
{
	Sprite* hud;
	int		health;
}GUI;

static GUI gui = { 0 };

void gui_close_hud()
{
	gf2d_sprite_free(gui.hud);
}
void gui_setup_hud()
{
    memset(&gui, 0, sizeof(GUI));
    gui.hud = gf2d_sprite_load_image("images/hud-sprite.png");
    atexit(gui_close_hud);
}
void gui_draw_percent_bar(Rect rect, float percent, Vector4D fgColor, Vector4D bgColor, int left)
{
    SDL_Rect r;
    Rect t;
    Shape u;
    r = gfc_rect_to_sdl_rect(rect);
    t = gfc_rect_from_sdl_rect(r);
    Color x;
    Color y;
   /* gfc_color_to_vector4(x);
    gfc_color_to_vector4(y);*/
    y = gfc_color_from_vector4(bgColor);
    x = gfc_color_from_vector4(fgColor);
    gf2d_draw_rect_filled(t, y);
    if (left)
    {
        r = gfc_rect_to_sdl_rect(gfc_rect(rect.x, rect.y, (float)rect.w * percent, rect.h));
    }
    else
    {
        r = gfc_rect_to_sdl_rect(gfc_rect(rect.x + (1 - percent) * rect.w, rect.y, (float)rect.w * percent, rect.h));
        //u = gfc_shape_rect(rect.x, rect.y, (float)rect.w * percent, rect.h);
        //gf2d_draw_shape(u, fgColor, vector2d(60, 0));

    }
    gf2d_draw_rect_filled(t, x);
}

void gui_draw_hud()
{
    Vector4D color = { 255,255,255,255 };
    gf2d_sprite_draw(
        gui.hud, vector2d(0,0),
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        0);
    gui_draw_percent_bar(gfc_rect(50, 0, 145, 10), gui.health, vector4d((1 - gui.health) * 255, gui.health * 255, 0, 255), vector4d(128, 0, 0, 128), 0);

}

void gui_set_health(int health)
{
    gui.health = health;
}