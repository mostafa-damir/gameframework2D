#ifndef __GFC_CONFIG_H__
#define __GFC_CONFIG_H__

#include <simple_json.h>

#include "gfc_vector.h"
#include "gfc_color.h"

/**
 * @brief extract a vector from json
 */
int sj_value_as_vector2d(SJson *json,Vector2D *output);
int sj_value_as_vector3d(SJson *json,Vector3D *output);
int sj_value_as_vector4d(SJson *json,Vector4D *output);

/**
 * @brief convert a vector to json
 */
SJson *sj_vector2d_new(Vector2D input);
SJson *sj_vector3d_new(Vector3D input);
SJson *sj_vector4d_new(Vector4D input);


/**
 * @brief parse a color from json
 * @param value the value contianing a color vector
 * @return pure solid white, unless a color was successfully parsed
 */
Color sj_value_as_color(SJson *value);

#endif
