#include "gui.h"
#include "gf2d_draw.h"
#include "gf2d_sprite.h"
#include "shape.h"
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

void gui_draw_percent_bar_horizontal(Rect rect, float percent, Vector4D fgColor, Vector4D bgColor, int left)
{
  /*  SDL_Rect r;
    r = gf2d_rect_to_sdl_rect(rect);
    gf2d_draw_solid_rect(r, bgColor);
    if (left)
    {
        r  = gf2d_draw_rect(gf2d_rect(rect.x, rect.y, (float)rect.w * percent, rect.h);
    }
    else
    {
        r = gf2d_rect_to_sdl_rect(gf2d_rect(rect.x + (1 - percent) * rect.w, rect.y, (float)rect.w * percent, rect.h));
    }
    gf2d_draw_solid_rect(r, fgColor);*/
    //r = gf2d_draw_rect_filled;
}
void gui_setup_hud()
{
	memset(&gui, 0, sizeof(GUI));
	gui.hud = gf2d_sprite_load_image("images/hud-sprite.png");
	atexit(gui_close_hud);
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

}

void gui_set_health(int health)
{
    gui.health = health;
}
