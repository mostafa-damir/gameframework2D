#include <simple_json.h>
#include "simple_logger.h"
#include "gfc_list.h"
#include "gfc_input.h"

typedef struct
{
    List *input_list;
    const Uint8 * input_keys;
    Uint8 * input_old_keys;
    int input_key_count;
    int mouse_wheel_x;
    int mouse_wheel_y;
    int mouse_wheel_x_old;
    int mouse_wheel_y_old;
    List *controllers;
    SJson *controller_button_map;
    SJson *controller_axis_map;
}GFC_InputData;

static GFC_InputData gfc_input_data = {0};


void gfc_input_close();
Input *gfc_input_get_by_name(const char *name);

void gfc_controller_update(GFC_InputController *controller)
{
    int i;
    Uint8 button;
    if (!controller)return;
    if (!controller->controller)return;//nothing to do
    memcpy(controller->old_buttons,controller->buttons,sizeof(Uint8)*controller->num_buttons);// backup the old
    memcpy(controller->old_axis,controller->axis,sizeof(Sint16)*controller->num_axis);// backup the old
    for (i = 0; i < controller->num_buttons;i++)
    {
        button = SDL_JoystickGetButton(controller->controller,i);
        controller->buttons[i] = button;
//        if (controller->buttons[i])slog("controller button %i is %i",i,controller->buttons[i]);
    }
    for (i = 0; i < controller->num_axis;i++)
    {
        controller->axis[i] = SDL_JoystickGetAxis(controller->controller,i);
//        if (controller->axis[i])slog("controller axis %i is %i",i,controller->axis[i]);
    }
}

void gfc_controller_free(GFC_InputController *controller)
{
    if (!controller)return;
    if (controller->buttons)free(controller->buttons);
    if (controller->old_buttons)free(controller->old_buttons);
    if (controller->axis)free(controller->axis);
    if (controller->axis_maxes)free(controller->axis_maxes);
    if (controller->old_axis)free(controller->old_axis);
    if (controller->controller)
    {
        SDL_JoystickClose(controller->controller);
    }
    free(controller);
}

GFC_InputController *gfc_controller_new()
{
    GFC_InputController *controller;
    controller = gfc_allocate_array(sizeof(GFC_InputController),1);
    return controller;
}

void gfc_input_controller_load_mappings(const char *config)
{
    SJson *file,*mappings;
    file = sj_load(config);
    if (!file)return;
    mappings = sj_object_get_value(file,"controller_map");
    if (!mappings)
    {
        sj_free(file);
        return;
    }
    gfc_input_data.controller_button_map = sj_copy(sj_object_get_value(mappings,"buttons"));
    gfc_input_data.controller_axis_map = sj_copy(sj_object_get_value(mappings,"axes"));
    sj_free(file);
}

void gfc_input_init(char *configFile)
{
    SDL_Joystick *joystick;
    GFC_InputController *controller;
    int i;
    if (gfc_input_data.input_list != NULL)
    {
        slog("gfc_input_init: error, gfc_input_data.input_list not NULL");
        return;
    }
    gfc_input_data.input_list = gfc_list_new();
    gfc_input_data.controllers = gfc_list_new();

    gfc_input_data.input_keys = SDL_GetKeyboardState(&gfc_input_data.input_key_count);
    if (!gfc_input_data.input_key_count)
    {
        slog("failed to get keyboard count!");
    }
    else
    {
        gfc_input_data.input_old_keys = (Uint8*)malloc(sizeof(Uint8)*gfc_input_data.input_key_count);
        memcpy(gfc_input_data.input_old_keys,gfc_input_data.input_keys,sizeof(Uint8)*gfc_input_data.input_key_count);
    }
    //controller support
    gfc_input_controller_load_mappings(configFile);
    for (i = 0; i < SDL_NumJoysticks(); ++i)
    {
        if (!SDL_IsGameController(i))continue;
        joystick = SDL_JoystickOpen(i);
        if (!joystick)continue;
        controller = gfc_controller_new();
        if (controller)
        {
            controller->num_buttons = SDL_JoystickNumButtons(joystick);
            if (controller->num_buttons)
            {
                controller->buttons = gfc_allocate_array(sizeof(Uint8),controller->num_buttons);
                controller->old_buttons = gfc_allocate_array(sizeof(Uint8),controller->num_buttons);
            }
            controller->num_axis = SDL_JoystickNumAxes(joystick);
            if (controller->num_axis)
            {
                controller->axis = gfc_allocate_array(sizeof(Sint16),controller->num_axis);
                controller->axis_maxes = gfc_allocate_array(sizeof(Sint16),controller->num_axis);
                controller->old_axis = gfc_allocate_array(sizeof(Sint16),controller->num_axis);
            }
            controller->controller = joystick;
            gfc_input_data.controllers = gfc_list_append(gfc_input_data.controllers,controller);
            break;
        }
    }
    atexit(gfc_input_close);
    gfc_input_commands_load(configFile);
}

int gfc_input_controller_get_button_index(const char *button)
{
    int i,c;
    int index;
    const char *name;
    SJson *item;
    if (!button)return -1;
    c = sj_array_get_count(gfc_input_data.controller_button_map);
    for (i = 0; i < c; i++)
    {
        item = sj_array_get_nth(gfc_input_data.controller_button_map,i);
        if (!item)continue;
        name = sj_get_string_value(sj_object_get_value(item,"button"));
        if (!name)continue;
        if (strcmp(name,button)==0)
        {
            if (sj_get_integer_value(sj_object_get_value(item,"index"),&index))
            {
                return index;
            }
        }
    }
    return -1;
}

Uint8 gfc_input_controller_button_state(Uint8 controllerId, const char *button)
{
    int index = gfc_input_controller_get_button_index(button);
    if (index < 1)return 0;
    return gfc_input_controller_button_state_by_index(controllerId,index);
}

Uint8 gfc_input_controller_button_held(Uint8 controllerId, const char *button)
{
    int index = gfc_input_controller_get_button_index(button);
    if (index < 1)return 0;
    return gfc_input_controller_button_held_by_index(
        controllerId,
        index);
}

Uint8 gfc_input_controller_button_pressed(Uint8 controllerId, const char *button)
{
    int index = gfc_input_controller_get_button_index(button);
    if (index < 1)return 0;
    return gfc_input_controller_button_pressed_by_index(
        controllerId,
        index);
}

Uint8 gfc_input_controller_button_released(Uint8 controllerId, const char *button)
{
    int index = gfc_input_controller_get_button_index(button);
    if (index < 1)return 0;
    return gfc_input_controller_button_released_by_index(
        controllerId,
        index);
}

Uint8 gfc_input_controller_button_state_by_index(Uint8 controllerId, Uint32 button)
{
    GFC_InputController *controller;
    controller = gfc_list_get_nth(gfc_input_data.controllers,controllerId);
    if (!controller)return 0;
    if (button >= controller->num_buttons)return 0;
    return controller->buttons[button];
}

Uint8 gfc_input_controller_old_button_state_by_index(Uint8 controllerId, Uint32 button)
{
    GFC_InputController *controller;
    controller = gfc_list_get_nth(gfc_input_data.controllers,controllerId);
    if (!controller)return 0;
    if (button >= controller->num_buttons)return 0;
    return controller->old_buttons[button];
}

Uint8 gfc_input_controller_button_held_by_index(Uint8 controller, Uint32 button)
{
    if ((gfc_input_controller_old_button_state_by_index(controller, button))&&
        (gfc_input_controller_button_state_by_index(controller,button)))
        return 1;
    return 0;
}

Uint8 gfc_input_controller_button_pressed_by_index(Uint8 controller, Uint32 button)
{
    if ((!gfc_input_controller_old_button_state_by_index(controller, button))&&
        (gfc_input_controller_button_state_by_index(controller,button)))
        return 1;
    return 0;
}

Uint8 gfc_input_controller_button_released_by_index(Uint8 controller, Uint32 button)
{
    if ((gfc_input_controller_old_button_state_by_index(controller, button))&&
        (!gfc_input_controller_button_state_by_index(controller,button)))
        return 1;
    return 0;
}

//mouse section

Uint8 gfc_input_mouse_wheel_up()
{
    if (gfc_input_data.mouse_wheel_y > 0)return 1;
    return 0;
}

Uint8 gfc_input_mouse_wheel_down()
{
    if (gfc_input_data.mouse_wheel_y < 0)return 1;
    return 0;
}

Uint8 gfc_input_mouse_wheel_left()
{
    if (gfc_input_data.mouse_wheel_x < 0)return 1;
    return 0;
}

Uint8 gfc_input_mouse_wheel_right()
{
    if (gfc_input_data.mouse_wheel_x > 0)return 1;
    return 0;
}

void gfc_input_delete(Input *in)
{
    if (!in)return;
    gfc_list_delete(in->keyCodes);// data in the list is just integers
    gfc_list_delete(in->buttons);// data in the list is just integers
    gfc_list_delete(in->axes);// data in the list is just integers
    free(in);
}

Input *gfc_input_new()
{
    Input *in = NULL;
    in = (Input *)gfc_allocate_array(sizeof(Input),1);
    in->keyCodes = gfc_list_new();
    in->buttons = gfc_list_new();
    in->axes = gfc_list_new();
    return in;
}

void gfc_input_set_callbacks(
    char *command,
    void (*onPress)(void *data),
    void (*onHold)(void *data),
    void (*onRelease)(void *data),
    void *data
)
{
    Input *in;
    if (!command)return;
    in = gfc_input_get_by_name(command);
    if (!in)return;
    in->onPress = onPress;
    in->onHold = onHold;
    in->onRelease = onRelease;
    in->data = data;
}

void gfc_input_close()
{
    GFC_InputController *controller;
    int i,c;
    gfc_input_commands_purge();
    gfc_input_data.input_list = NULL;
    if (gfc_input_data.input_old_keys)
    {
        free(gfc_input_data.input_old_keys);
    }
    if (gfc_input_data.controller_button_map)sj_free(gfc_input_data.controller_button_map);
    if (gfc_input_data.controller_axis_map)sj_free(gfc_input_data.controller_axis_map);
    c = gfc_list_get_count(gfc_input_data.controllers);
    for (i = 0; i < c; i++)
    {
        controller = gfc_list_get_nth(gfc_input_data.controllers,i);
        if (!controller)continue;
        gfc_controller_free(controller);
    }
    gfc_list_delete(gfc_input_data.controllers);
    memset(&gfc_input_data,0,sizeof(GFC_InputData));
}

void gfc_input_commands_purge()
{
    Uint32 c,i;
    void *data;
    if (!gfc_input_data.input_list)return;
    c = gfc_list_get_count(gfc_input_data.input_list);
    for (i = 0;i < c;i++)
    {
        data = gfc_list_get_nth(gfc_input_data.input_list,i);
        if (!data)continue;
        gfc_input_delete((Input*)data);
    }
    gfc_list_delete(gfc_input_data.input_list);
}

void gfc_input_update_controller(Input *command)
{
    GFC_InputController *controller;
    Uint32 index,controllerId;
    int c,i;
    int old = 0, new = 0;
    if (!command)return;
    if (!command->controller)return;
    if (!gfc_input_data.controllers)return;
    controllerId = command->controller - 1;
    controller = gfc_list_get_nth(gfc_input_data.controllers,controllerId);
    if (!controller)return;
    
    c = gfc_list_get_count(command->buttons);
    if (!c)
    {
        return;// no buttons configured
    }
    for (i = 0; i < c; i++)
    {
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
        index = (Uint32)gfc_list_get_nth(command->buttons,i);
        if (index >= controller->num_buttons)continue; // bad index
        if (gfc_input_controller_button_state_by_index(controllerId, index))new++;
        if (gfc_input_controller_old_button_state_by_index(controllerId, index))old++;
    }
    if ((old == c)&&(new == c))
    {
        command->state = IET_Hold;
        if (command->onHold)
        {
            command->onHold(command->data);
        }
    }
    else if ((old == c)&&(new != c))
    {
        command->state = IET_Release;
        if (command->onRelease)
        {
            command->onRelease(command->data);
        }
    }
    else if ((old != c)&&(new == c))
    {
        command->state = IET_Press;
        command->pressTime = SDL_GetTicks();
        if (command->onPress)
        {
            command->onPress(command->data);
        }
    }
    else
    {
        command->state = IET_Idle;
    }
}

void gfc_input_update_command(Input *command)
{
    Uint32 c,i;
    Uint32 kc;
    int old = 0, new = 0;
    if (!command)return;
    c = gfc_list_get_count(command->keyCodes);
    if (!c)return;// no commands to update this with, do nothing
    command->downCount = 0;
    for (i = 0; i < c; i++)
    {
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
        kc = (Uint32)gfc_list_get_nth(command->keyCodes,i);
        if (!kc)continue;
        if (kc == EMK_Shift)
        {
            //deal with mod keys
            if (gfc_input_data.input_old_keys[SDL_SCANCODE_LSHIFT]||gfc_input_data.input_old_keys[SDL_SCANCODE_RSHIFT])old++;
            if (gfc_input_data.input_keys[SDL_SCANCODE_LSHIFT]||gfc_input_data.input_keys[SDL_SCANCODE_RSHIFT])
            {
                new++;
                command->downCount++;
            }
        }
        else if (kc == EMK_Alt)
        {
            if (gfc_input_data.input_old_keys[SDL_SCANCODE_LALT]||gfc_input_data.input_old_keys[SDL_SCANCODE_RALT])old++;
            if (gfc_input_data.input_keys[SDL_SCANCODE_LALT]||gfc_input_data.input_keys[SDL_SCANCODE_RALT])
            {
                new++;
                command->downCount++;
            }
        }
        else if (kc == EMK_Ctrl)
        {
            if (gfc_input_data.input_old_keys[SDL_SCANCODE_LCTRL]||gfc_input_data.input_old_keys[SDL_SCANCODE_RCTRL])old++;
            if (gfc_input_data.input_keys[SDL_SCANCODE_LCTRL]||gfc_input_data.input_keys[SDL_SCANCODE_RCTRL])
            {
                command->downCount++;
                new++;
            }
        }
        else if (kc == EMK_Super)
        {
            if (gfc_input_data.input_old_keys[SDL_SCANCODE_LGUI]||gfc_input_data.input_old_keys[SDL_SCANCODE_RGUI])old++;
            if (gfc_input_data.input_keys[SDL_SCANCODE_LGUI]||gfc_input_data.input_keys[SDL_SCANCODE_RGUI])
            {
                command->downCount++;
                new++;
            }
        }
        else
        {
            if(gfc_input_data.input_old_keys[kc])old++;
            if(gfc_input_data.input_keys[kc])
            {
                command->downCount++;
                new++;
            }
        }
    }
    if ((old == c)&&(new == c))
    {
        command->state = IET_Hold;
        if (command->onHold)
        {
            command->onHold(command->data);
        }
    }
    else if ((old == c)&&(new != c))
    {
        command->state = IET_Release;
        if (command->onRelease)
        {
            command->onRelease(command->data);
        }
    }
    else if ((old != c)&&(new == c))
    {
        command->state = IET_Press;
        command->pressTime = SDL_GetTicks();
        if (command->onPress)
        {
            command->onPress(command->data);
        }
    }
    else
    {
        command->state = IET_Idle;
        gfc_input_update_controller(command);
    }
}

Input *gfc_input_get_by_name(const char *name)
{
    Uint32 c,i;
    Input *in;
    if (!name)
    {
        return NULL;
    }
    c = gfc_list_get_count(gfc_input_data.input_list);
    for (i = 0;i < c;i++)
    {
        in = (Input *)gfc_list_get_nth(gfc_input_data.input_list,i);
        if (!in)continue;
        if (gfc_line_cmp(in->command,name)==0)
        {
            return in;
        }
    }
    return 0;
}

Uint8 gfc_input_command_pressed(const char *command)
{
    Input *in;
    in = gfc_input_get_by_name(command);
    if (!in)return 0;
    if (in->state == IET_Press)return 1;
    return 0;
}

Uint8 gfc_input_command_held(const char *command)
{
    Input *in;
    in = gfc_input_get_by_name(command);
    if ((in)&&(in->state == IET_Hold))return 1;
    return 0;
}

Uint8 gfc_input_command_released(const char *command)
{
    Input *in;
    in = gfc_input_get_by_name(command);
    if ((in)&&(in->state == IET_Release))return 1;
    return 0;
}

Uint8 gfc_input_command_down(const char *command)
{
    Input *in;
    in = gfc_input_get_by_name(command);
    if (in)
    {
        if((in->state == IET_Press)||(in->state == IET_Hold))return 1;
    }
    return 0;
}

InputEventType gfc_input_command_get_state(const char *command)
{
    Input *in;
    in = gfc_input_get_by_name(command);
    if (!in)return 0;
    return in->state;
}

List *gfc_input_get_by_scancode(SDL_Scancode keysym)
{
    int i,c,kc,ki;
    Input *in;
    List *keylist = NULL;
    keylist = gfc_list_new();
    
    c = gfc_list_get_count(gfc_input_data.input_list);
    for (i = 0;i < c;i++)
    {
        in = (Input *)gfc_list_get_nth(gfc_input_data.input_list,i);
        if (!in)continue;
        kc = gfc_list_get_count(in->keyCodes);
        if (!kc)continue;
        for (ki = 0;ki < kc;ki++)
        {
            if ((SDL_Scancode)gfc_list_get_nth(in->keyCodes,ki) == keysym)
            {
                keylist = gfc_list_append(keylist,in);
                break;
            }
        }
    }
    
    if (gfc_list_get_count(keylist) == 0)
    {
        gfc_list_delete(keylist);
        return NULL;
    }
    return keylist;
}

void gfc_input_update()
{
    Input *in = NULL;
    GFC_InputController *controller;
    Uint32 c,i;
    SDL_Event event = {0};
    
    memcpy(gfc_input_data.input_old_keys,gfc_input_data.input_keys,sizeof(Uint8)*gfc_input_data.input_key_count);
    gfc_input_data.mouse_wheel_x_old = gfc_input_data.mouse_wheel_x;
    gfc_input_data.mouse_wheel_y_old = gfc_input_data.mouse_wheel_y;
    gfc_input_data.mouse_wheel_x = 0;
    gfc_input_data.mouse_wheel_y = 0;

    SDL_PumpEvents();   // update SDL's internal event structures
    //grab all the input from SDL now
    c = gfc_list_get_count(gfc_input_data.controllers);
    for (i = 0; i < c; i++)
    {
        controller = gfc_list_get_nth(gfc_input_data.controllers,i);
        if (!controller)continue;
        gfc_controller_update(controller);
    }

    gfc_input_data.input_keys = SDL_GetKeyboardState(&gfc_input_data.input_key_count);

    c = gfc_list_get_count(gfc_input_data.input_list);
    for (i = 0;i < c;i++)
    {
        in = gfc_list_get_nth(gfc_input_data.input_list,i);
        if (!in)continue;
        gfc_input_update_command(in);
    }
    while(SDL_PollEvent(&event))
    {
        if (event.type == SDL_WINDOWEVENT)
        {
            if (event.window.event == SDL_WINDOWEVENT_CLOSE)
            {
                in = gfc_input_get_by_name("exit");
                if (in)
                {
                    in->state = IET_Press;
                }
            }
        }
        if (event.type == SDL_MOUSEWHEEL)
        {
            if(event.wheel.y > 0) // scroll up
            {
                gfc_input_data.mouse_wheel_y = 1;
            }
            else if(event.wheel.y < 0) // scroll down
            {
                gfc_input_data.mouse_wheel_y = -1;
            }

            if(event.wheel.x > 0) // scroll right
            {
                gfc_input_data.mouse_wheel_x = 1;
            }
            else if(event.wheel.x < 0) // scroll left
            {
                gfc_input_data.mouse_wheel_x = -1;
            }
        }
    }

}

InputModKey gfc_input_key_mod_check(const char * buffer)
{
    if (!buffer)return EMK_None;
    if (strcmp(buffer,"SHIFT")==0)return EMK_Shift;
    if (strcmp(buffer,"ALT")==0)return EMK_Alt;
    if (strcmp(buffer,"CTRL")==0)return EMK_Ctrl;
    if (strcmp(buffer,"SUPER")==0)return EMK_Super;
    return EMK_None;
}

SDL_Scancode gfc_input_key_to_scancode(const char * buffer)
{
    int F = 0;
    SDL_Scancode kc = -1;
    if (strlen(buffer) == 1)
    {
        //single letter code
        if ((buffer[0] >= 'a')&&(buffer[0] <= 'z'))
        {
            kc = SDL_SCANCODE_A + buffer[0] - 'a';
        }
        else if (buffer[0] == '0')
        {
            kc = SDL_SCANCODE_0;
        }
        else if (buffer[0] == '1')
        {
            kc = SDL_SCANCODE_1;
        }
        else if (buffer[0] == '2')
        {
            kc = SDL_SCANCODE_2;
        }
        else if (buffer[0] == '3')
        {
            kc = SDL_SCANCODE_3;
        }
        else if (buffer[0] == '4')
        {
            kc = SDL_SCANCODE_4;
        }
        else if (buffer[0] == '5')
        {
            kc = SDL_SCANCODE_5;
        }
        else if (buffer[0] == '6')
        {
            kc = SDL_SCANCODE_6;
        }
        else if (buffer[0] == '7')
        {
            kc = SDL_SCANCODE_7;
        }
        else if (buffer[0] == '8')
        {
            kc = SDL_SCANCODE_8;
        }
        else if (buffer[0] == '9')
        {
            kc = SDL_SCANCODE_9;
        }
        else if (buffer[0] == '-')
        {
            kc = SDL_SCANCODE_MINUS;
        }
        else if (buffer[0] == '=')
        {
            kc = SDL_SCANCODE_EQUALS;
        }
        else if (buffer[0] == '[')
        {
            kc = SDL_SCANCODE_LEFTBRACKET;
        }
        else if (buffer[0] == ']')
        {
            kc = SDL_SCANCODE_RIGHTBRACKET;
        }
        else if (buffer[0] == '.')
        {
            kc = SDL_SCANCODE_PERIOD;
        }
        else if (buffer[0] == ',')
        {
            kc = SDL_SCANCODE_COMMA;
        }
        else if (buffer[0] == ';')
        {
            kc = SDL_SCANCODE_SEMICOLON;
        }
        else if (buffer[0] == '\\')
        {
            kc = SDL_SCANCODE_BACKSLASH;
        }
        else if (buffer[0] == '/')
        {
            kc = SDL_SCANCODE_SLASH;
        }
        else if (buffer[0] == '\'')
        {
            kc = SDL_SCANCODE_APOSTROPHE;
        }
        else if (buffer[0] == ';')
        {
            kc = SDL_SCANCODE_SEMICOLON;
        }
        else if (buffer[0] == '`')
        {
            kc = SDL_SCANCODE_GRAVE;
        }
        else if ((buffer[0] >= ' ')&&(buffer[0] <= '`'))
        {
            kc = SDL_SCANCODE_SPACE + buffer[0] - ' ';
        }
    }
    else
    {
        if (buffer[0] == 'F')
        {
            F = atoi(&buffer[1]);
            if (F <= 12)
            {
                kc = SDL_SCANCODE_F1 + F - 1; 
            }
            else if (F <= 24)
            {
                kc = SDL_SCANCODE_F13 + F - 1; 
            }
        }
        else if (strcmp(buffer,"BACKSPACE") == 0)
        {
            kc = SDL_SCANCODE_BACKSPACE;
        }
        else if (strcmp(buffer,"RIGHT") == 0)
        {
            kc = SDL_SCANCODE_RIGHT;
        }
        else if (strcmp(buffer,"LEFT") == 0)
        {
            kc = SDL_SCANCODE_LEFT;
        }
        else if (strcmp(buffer,"UP") == 0)
        {
            kc = SDL_SCANCODE_UP;
        }
        else if (strcmp(buffer,"DOWN") == 0)
        {
            kc = SDL_SCANCODE_DOWN;
        }
        else if (strcmp(buffer,"LALT") == 0)
        {
            kc = SDL_SCANCODE_LALT;
        }
        else if (strcmp(buffer,"RALT") == 0)
        {
            kc = SDL_SCANCODE_RALT;
        }
        else if (strcmp(buffer,"LSHIFT") == 0)
        {
            kc = SDL_SCANCODE_LSHIFT;
        }
        else if (strcmp(buffer,"RSHIFT") == 0)
        {
            kc = SDL_SCANCODE_RSHIFT;
        }
        else if (strcmp(buffer,"LCTRL") == 0)
        {
            kc = SDL_SCANCODE_LCTRL;
        }
        else if (strcmp(buffer,"RCTRL") == 0)
        {
            kc = SDL_SCANCODE_RCTRL;
        }
        else if (strcmp(buffer,"TAB") == 0)
        {
            kc = SDL_SCANCODE_TAB;
        }
        else if (strcmp(buffer,"RETURN") == 0)
        {
            kc = SDL_SCANCODE_RETURN;
        }
        else if (strcmp(buffer,"DELETE") == 0)
        {
            kc = SDL_SCANCODE_DELETE;
        }
        else if (strcmp(buffer,"ESCAPE") == 0)
        {
            kc = SDL_SCANCODE_ESCAPE;
        }
    }
    if (kc == -1)
    {
        slog("no input mapping available for %s",buffer);
    }
    return kc;
}

Uint8 gfc_input_key_pressed(const char *key)
{
    SDL_Scancode kc;
    kc = gfc_input_key_to_scancode(key);
    if (kc == -1)return 0;
    if ((!gfc_input_data.input_old_keys[kc])&&(gfc_input_data.input_keys[kc]))return 1;
    return 0;
}

Uint8 gfc_input_key_released(const char *key)
{
    SDL_Scancode kc;
    kc = gfc_input_key_to_scancode(key);
    if (kc == -1)return 0;
    if ((gfc_input_data.input_old_keys[kc])&&(!gfc_input_data.input_keys[kc]))return 1;
    return 0;
}

Uint8 gfc_input_key_held(const char *key)
{
    SDL_Scancode kc;
    kc = gfc_input_key_to_scancode(key);
    if (kc == -1)return 0;
    if ((gfc_input_data.input_old_keys[kc])&&(gfc_input_data.input_keys[kc]))return 1;
    return 0;
}

Uint8 gfc_input_key_down(const char *key)
{
    SDL_Scancode kc;
    kc = gfc_input_key_to_scancode(key);
    if (kc == -1)return 0;
    if (gfc_input_data.input_keys[kc])
    {
        return 1;
    }
    return 0;
}

void gfc_input_parse_command_json(SJson *command)
{
    SJson *value,*list;
    const char * buffer;
    Input *in;
    int count,i,index;
    Sint32 kc = 0;
    if (!command)return;
    value = sj_object_get_value(command,"command");
    if (!value)
    {
        slog("input command missing 'command' key");
        return;
    }
    in = gfc_input_new();
    if (!in)return;
    buffer = sj_get_string_value(value);
    gfc_line_cpy(in->command,buffer);
    list = sj_object_get_value(command,"keys");
    count = sj_array_get_count(list);
    for (i = 0; i< count; i++)
    {
        value = sj_array_get_nth(list,i);
        if (!value)continue;
        buffer = sj_get_string_value(value);
        if (strlen(buffer) == 0)
        {
            slog("error in key list, empty value");
            continue;   //error
        }
        kc =  gfc_input_key_mod_check(buffer);
        if (kc == EMK_None)
        {
            //not a meta mod key
            kc = gfc_input_key_to_scancode(buffer);
        }
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
        if (kc != -1)
        {
            in->keyCodes = gfc_list_append(in->keyCodes,(void *)kc);
        }
    }
    index = 0;
    value = sj_object_get_value(command,"controller");
    if (value != NULL)
    {
        sj_get_integer_value(value,&index);
        in->controller = index + 1;
        list = sj_object_get_value(command,"buttons");
        if (list)
        {
            count = sj_array_get_count(list);
            for (i = 0; i < count; i++)
            {
                value = sj_array_get_nth(list,i);
                if (!value)continue;
                buffer = sj_get_string_value(value);
                if (strlen(buffer) == 0)continue;
                index = gfc_input_controller_get_button_index(buffer);
                if (index >= 0)
                {
                    in->buttons = gfc_list_append(in->buttons,(void *)index);
                }
                else
                {
                    // check if it is an axis
                }
            }
        }
    }
    gfc_input_data.input_list = gfc_list_append(gfc_input_data.input_list,(void *)in);
}

void gfc_input_commands_load(char *configFile)
{
    SJson *json;
    SJson *commands;
    SJson *value;
    int count,i;
    if (!configFile)return;
    json = sj_load(configFile);
    if (!json)return;
    commands = sj_object_get_value(json,"commands");
    if (!commands)
    {
        slog("config file %s does not contain 'commands' object",configFile);
        sj_free(json);
        return;
    }
    count = sj_array_get_count(commands);
    for (i = 0; i< count; i++)
    {
        value = sj_array_get_nth(commands,i);
        if (!value)continue;
        gfc_input_parse_command_json(value);
    }
    sj_free(json);
}


/*eol@eof*/
