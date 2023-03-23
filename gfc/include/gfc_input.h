#ifndef __GFC_INPUT_H__
#define __GFC_INPUT_H__

#include <SDL.h>
#include "gfc_text.h"
#include "gfc_list.h"

typedef enum
{
    EMK_None = 0,
    EMK_Shift = 1000,
    EMK_Alt,
    EMK_Ctrl,
    EMK_Super
}InputModKey;

typedef enum
{
    IET_Idle    = 0,
    IET_Press   = 1,
    IET_Hold    = 2,
    IET_Release = 3
}InputEventType;

typedef struct
{
    Uint32 num_buttons;
    Uint8 *buttons;
    Uint8 *old_buttons;
    Uint32 num_axis;
    Sint16 *axis_maxes;
    Sint16 *axis;
    Sint16 *old_axis;
    SDL_Joystick *controller;
}GFC_InputController;

/**
 * @brief Inputs abstract user input collection.  They can be setup to trigger callbacks and/or polled for current state
 */
typedef struct
{
    TextLine command;
    List *keyCodes;                     /**<list of keys that must be pressed together to count as a single input*/
    Uint8 controller;                   /**<Index of the controller to use to update this input*/
    List *buttons;                      /**<list of buttons that must be pressed together to count as a single input*/
    List *axes;                         /**<list of axes that must be pressed together to count as a single input*/
    int downCount;
    Uint32 pressTime;                   /**<clock ticks when button was pressed*/
    InputEventType state;               /**<updated each frame*/
    void (*onPress)(void *data);        /**<callback for press event*/
    void (*onHold)(void *data);         /**<callback for hold event*/
    void (*onRelease)(void *data);      /**<callback for release event*/
    void *data;                         /**<pointer to be passed to callbacks*/
}Input;

/**
 * @brief initializes the config system
 * @param configFile if not NULL it will load the file for user input configuration
 */
void gfc_input_init(char *configFile);

/**
 * @brief called as often as you want your input data update.  Every Frame, or faster if you like
 * @note must be called or input will be stale
 */
void gfc_input_update();

/**
 * @brief load user configuration from file.
 * @note this operation appends to existing commands
 */
void gfc_input_commands_load(char *configFile);

/**
 * @brief clears all user input configuration
 */
void gfc_input_commands_purge();

/**
 * @brief check if a command was pressed this frame
 * @param command the name of the command to check
 * @returns true if pressed, false otherwise
 */
Uint8 gfc_input_command_pressed(const char *command);
Uint8 gfc_input_command_held(const char *command);
Uint8 gfc_input_command_released(const char *command);
Uint8 gfc_input_command_down(const char *command);

InputEventType gfc_input_command_get_state(const char *command);

/**
 * @brief report if the key provided has been pressed this frame
 * @param key the name of the key to check
 * @returns true if pressed, false otherwise
 * 
 */
Uint8 gfc_input_key_pressed(const char *key);
Uint8 gfc_input_key_released(const char *key);
Uint8 gfc_input_key_held(const char *key);
Uint8 gfc_input_key_down(const char *key);

/**
 * @brief check if the mouse wheel is moving in the indicated direction
 * @return 0 if it is not, 1 if it is
 */
Uint8 gfc_input_mouse_wheel_up();
Uint8 gfc_input_mouse_wheel_down();
Uint8 gfc_input_mouse_wheel_left();
Uint8 gfc_input_mouse_wheel_right();

/**
 * @brief check the state of a controller's button
 * @param controller the controller index to check
 * @param button, the SDL_GameControllerButton to check
 * @return if the event is true for this update frame
 */

Uint8 gfc_input_controller_button_state_by_index(Uint8 controller, Uint32 button);
Uint8 gfc_input_controller_button_held_by_index(Uint8 controller, Uint32 button);
Uint8 gfc_input_controller_button_pressed_by_index(Uint8 controller, Uint32 button);
Uint8 gfc_input_controller_button_released_by_index(Uint8 controller, Uint32 button);

/**
 * @brief get the state of a controller's button by its configured name
 * @param controllerId the index of the controller to check
 * @param button the name of the button
 */
Uint8 gfc_input_controller_button_state(Uint8 controllerId, const char *button);
Uint8 gfc_input_controller_button_held(Uint8 controllerId, const char *button);
Uint8 gfc_input_controller_button_pressed(Uint8 controllerId, const char *button);
Uint8 gfc_input_controller_button_released(Uint8 controllerId, const char *button);

/**
 * @brief get the button index given the "name" of the button in the config file
 * @return -1 on not found or error, or the index of the button otherwise
 */
int gfc_input_controller_get_button_index(const char *button);

/**
 * @brief configure callbacks for an input
 * @param command the name of the input to configure
 * @param onPress the function to call when the input is pressed
 * @param onHold the function to call when the input is held
 * @param onRelease the function to call when the input is released
 * @param data pointer to any custom data you want associated with the input
 */
void gfc_input_set_callbacks(
    char *command,
    void (*onPress)(void *data),
    void (*onHold)(void *data),
    void (*onRelease)(void *data),
    void *data
);


#endif
