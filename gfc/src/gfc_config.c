#include "simple_logger.h"
#include "gfc_config.h"

SJson *sj_vector2d_new(Vector2D input)
{
    SJson *json;
    
    json = sj_array_new();
    if (!json)return NULL;
    sj_array_append(json,sj_new_float(input.x));
    sj_array_append(json,sj_new_float(input.y));
    return json;
}

SJson *sj_vector3d_new(Vector3D input)
{
    SJson *json;
    
    json = sj_array_new();
    if (!json)return NULL;
    sj_array_append(json,sj_new_float(input.x));
    sj_array_append(json,sj_new_float(input.y));
    sj_array_append(json,sj_new_float(input.z));
    return json;
}

SJson *sj_vector4d_new(Vector4D input)
{
    SJson *json;
    
    json = sj_array_new();
    if (!json)return NULL;
    sj_array_append(json,sj_new_float(input.x));
    sj_array_append(json,sj_new_float(input.y));
    sj_array_append(json,sj_new_float(input.z));
    sj_array_append(json,sj_new_float(input.w));
    return json;
}

Color sj_value_as_color(SJson *value)
{
    Color color;
    Vector4D colorv = {255,255,255,255};
    color = gfc_color8(255,255,255,255);
    if (!value)return color;
    sj_value_as_vector4d(value,&colorv);
    color = gfc_color_from_vector4(colorv);
    return color;
}


int sj_value_as_vector2d(SJson *json,Vector2D *output)
{
    const char *text = NULL;
    float numbers[4];
    int i,count;
    SJson *value;
    if (!json)return 0;
    if (sj_is_array(json))
    {
        count = sj_array_get_count(json);
        if (count < 2)return 0;
        if (count > 2)count = 2;
        for (i = 0; i < count;i++)
        {
            value = sj_array_get_nth(json,i);
            sj_get_float_value(value,&numbers[i]);
        }
        if (output)
        {
            output->x = numbers[0];
            output->y = numbers[1];
        }
        return 1;
    }
    if (sj_is_string(json))
    {
        text = sj_get_string_value(json);
        if(sscanf(text,"%f,%f",&numbers[0],&numbers[1]) != 2)
        {
            return 0;
        }
        if (output)
        {
            output->x = numbers[0];
            output->y = numbers[1];
        }
        return 1;
    }
    return 0;
}

int sj_value_as_vector3d(SJson *json,Vector3D *output)
{
    const char *text = NULL;
    float numbers[4];
    int i,count;
    SJson *value;
    if (!json)return 0;
    if (sj_is_array(json))
    {
        count = sj_array_get_count(json);
        if (count < 3)return 0;
        if (count > 3)count = 3;
        for (i = 0; i < count;i++)
        {
            value = sj_array_get_nth(json,i);
            sj_get_float_value(value,&numbers[i]);
        }
        if (output)
        {
            output->x = numbers[0];
            output->y = numbers[1];
            output->z = numbers[2];
        }
        return 1;
    }
    if (sj_is_string(json))
    {
        text = sj_get_string_value(json);
        if(sscanf(text,"%f,%f,%f",&numbers[0],&numbers[1],&numbers[2]) != 3)
        {
            return 0;
        }
        if (output)
        {
            output->x = numbers[0];
            output->y = numbers[1];
            output->z = numbers[2];
        }
        return 1;
    }
    return 0;
}

int sj_value_as_vector4d(SJson *json,Vector4D *output)
{
    const char *text = NULL;
    float numbers[4];
    int i,count;
    SJson *value;
    if (!json)
    {
        return 0;
    }
    if (sj_is_array(json))
    {
        count = sj_array_get_count(json);
        if (count < 4)
        {
            slog("not enough elements in the array (%i) for a Vector4D",count);
            return 0;
        }
        if (count > 4)count = 4;
        for (i = 0; i < count;i++)
        {
            value = sj_array_get_nth(json,i);
            sj_get_float_value(value,&numbers[i]);
        }
        if (output)
        {
            output->x = numbers[0];
            output->y = numbers[1];
            output->z = numbers[2];
            output->w = numbers[3];
        }
        return 1;
    }
    if (sj_is_string(json))
    {
        text = sj_get_string_value(json);
        if(sscanf(text,"%f,%f,%f,%f",&numbers[0],&numbers[1],&numbers[2],&numbers[3]) != 4)
        {
            return 0;
        }
        if (output)
        {
            output->x = numbers[0];
            output->y = numbers[1];
            output->z = numbers[2];
            output->w = numbers[3];
        }
        return 1;
    }
    slog("json is not an array or string");
    sj_echo(json);
    return 0;
}

/*eol@eof*/
