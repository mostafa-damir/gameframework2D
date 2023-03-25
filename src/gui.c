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
void gui_draw_percent_bar(Rect rect, float percent, Color fgColor, Color bgColor, int left)
{
    SDL_Rect r;
    Rect t;
    r = gfc_rect_to_sdl_rect(rect);
    t = gfc_rect_from_sdl_rect(r);
    Color x;
    Color y;
    x = fgColor;
    y = bgColor;
    gfc_color_to_vector4(x);
    gfc_color_to_vector4(y);
    gf2d_draw_rect_filled(t, y);
    if (left)
    {
        r = gfc_rect_to_sdl_rect(gfc_rect(rect.x, rect.y, (float)rect.w * percent, rect.h));
    }
    else
    {
        r = gfc_rect_to_sdl_rect(gfc_rect(rect.x + (1 - percent) * rect.w, rect.y, (float)rect.w * percent, rect.h));
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
    gui_draw_percent_bar(gfc_rect(50, 0, 145, 10), gui.health, gfc_color((1 - gui.health) * 255, gui.health * 255, 0, 255), gfc_color(128, 0, 0, 128), 0);

}

void gui_set_health(int health)
{
    gui.health = health;
}
