#include "simple_logger.h"
#include "gfc_hashmap.h"

Uint32 gfc_hash(HashMap *map,const char *key)
{
    Uint32 h;
    const char *p;
    if (!map)return 0;
    h = map->seed;
    for (p = key;*p != 0;p++)
    {
        h = h * 33 + *p;
    }
    return h;
}

void gfc_hashmap_set_seed(HashMap *map,Uint32 seed)
{
    if (!map)return;
    map->seed = seed;
}

HashMap *gfc_hashmap_new_size(Uint32 size)
{
    HashMap *map = NULL;
    map = (HashMap *)gfc_allocate_array(sizeof(HashMap),1);
    if (!map)return NULL;
    map->seed = 5381;   //re: Glib did it
    map->map = gfc_list_new_size(size);
    map->size = size;
    return map;
}

HashMap *gfc_hashmap_new()
{
    return gfc_hashmap_new_size(4);
}

void gfc_hashmap_free(HashMap *map)
{
    HashElement *element;
    int i;
    if (!map)return;
    //free all the keys
    // TODO redo this based on how we are using the list
    if (map->map)
    {
        for (i = 0; i < map->map->size; i++)
        {
            element = map->map->elements[i].data;
            if (!element)continue;
            free(element);
            map->map->elements[i].data = NULL;
        }
        gfc_list_delete(map->map);
    }
    free(map);
}

void gfc_hashmap_rehash(HashMap *map)
{
    int i;
    HashMap *newmap;
    HashElement*element;
    if (!map)return;
    newmap = gfc_hashmap_new_size(map->size * 2);
    for (i = 0; i < map->map->size;i++)
    {
        if (map->map->elements[i].data == NULL)continue;
        
        element = map->map->elements[i].data;
        gfc_hashmap_insert(newmap,element->key,element->data);
        free(element);
    }
    gfc_list_delete(map->map);
    map->map = newmap->map;
    map->size = newmap->size;
    map->seed = newmap->seed;
    newmap->map = NULL;
    gfc_hashmap_free(newmap);
}

void gfc_hashmap_insert(HashMap *map,const char *key,void *data)
{
    Uint32 h,i;
    HashElement *element = NULL;
    HashElement *collision;
    if (!map)return;
    if (!key)
    {
        slog("cannot insert into hashmap, no key provided");
        return;
    }
    element = gfc_allocate_array(sizeof(HashElement),1);
    if (!element)return;
    h = gfc_hash(map,key);
    h = h %map->size;
    i = h;
    while (map->map->elements[i].data != NULL)
    {   //collision
        collision = (HashElement *)map->map->elements[i].data;
        if (collision->hashValue != h)
        {
            //no room to chain, we need to expand and rehash.
            gfc_hashmap_rehash(map);
            h = gfc_hash(map,key);
            h = h %map->size;
            i = h;
            if (map->map->elements[i].data == NULL)break;
        }
        i++;
        if (i >= map->map->size)
        {
            gfc_hashmap_rehash(map);
            h = gfc_hash(map,key);
            h = h %map->size;
            i = h;
            if (map->map->elements[i].data == NULL)break;
        }
    }
    element->hashValue = h;
    element->data = data;
    gfc_line_cpy(element->key,key);
    map->map->elements[i].data = element;
}

Sint64 gfc_hashmap_get_index(HashMap *map,const char *key)
{
    Uint32 h,i;
    HashElement *element;
    if (!map)return -1;
    if (!map->map)
    {
        slog("hashmap missing map of values");
        return -1;
    }
    if (!key)return -1;
    h = gfc_hash(map,key);
    h = h %map->size;
    i = h;
    element = (HashElement *)map->map->elements[h].data;
    if (!element)return -1;
    do
    {
        if ((strcmp(key,element->key)==0)&&(strlen(key) == strlen(element->key)))
        {
            return i;
        }
        //keys don't match, so look at the next element in the array
        if ((i +1) >= map->size)return -1;// we seeked too far
        element = (HashElement *)map->map->elements[++i].data;
    }
    while ((element)&&(element->hashValue == h));
    return -1;//not found
}

void *gfc_hashmap_get(HashMap *map,const char *key)
{
    HashElement *element;
    Sint64 index;
    if ((!map)||(!map->map)||(!map->map->elements))return NULL;
    index = gfc_hashmap_get_index(map,key);
    if (index < 0)return NULL;
    if (index >= map->size)return NULL;
    element = (HashElement *)map->map->elements[index].data;
    return element->data;
}

void gfc_hashmap_slog(HashMap *map)
{
    int i;
    HashElement *element;
    if ((!map) || (!map->map))return;
    slog("Hashmap:");
    for (i = 0; i < map->map->size; i++)
    {
        if (map->map->elements[i].data == NULL)continue;
        element = (HashElement*)map->map->elements[i].data;
        slog("Hash key: '%s' hashValue: %i, hashIndex: %i",element->key,element->hashValue,i);
    }
}

void gfc_hashmap_delete_by_key(HashMap *map,const char *key)
{
    HashElement *element, *target;
    Sint64 index;
    Uint32 hashValue;
    if (!map)return;
    index = gfc_hashmap_get_index(map,key);
    if (index < 0)return; // not found, nothing to do
    target = (HashElement *)map->map->elements[index].data;
    map->map->elements[index].data = NULL;
    hashValue = target->hashValue;
    free(target);
    while ((index + 1) < map->map->size)
    {
        element = (HashElement *)map->map->elements[index + 1].data;
        if (!element)return; // chain has ended;
        if (element->hashValue != hashValue)return; // done, the chain has ended
        map->map->elements[index].data = map->map->elements[index + 1].data;
        map->map->elements[index + 1].data = NULL;
        index++;
    }
}

List *gfc_hashmap_get_all_values(HashMap *map)
{
    int i;
    HashElement *element;
    List *valueList = NULL;
    if ((!map) || (!map->map))return NULL;
    valueList = gfc_list_new();
    for (i = 0; i < map->map->size; i++)
    {
        element = (HashElement *)map->map->elements[i].data;
        if (!element)continue;
        valueList = gfc_list_append(valueList,element);
    }
    return valueList;
}

void gfc_hashmap_foreach(HashMap *map, gfc_work_func func)
{
    int i,c;
    void *item;
    List *items;
    if ((!map)||(!func))return;
    items = gfc_hashmap_get_all_values(map);
    if (!items)return;
    c = gfc_list_get_count(items);
    for (i = 0;i < c;i++)
    {
        item = gfc_list_get_nth(items,i);
        if (!item)continue;
        func(item);
    }
    gfc_list_delete(item);
}

void gfc_hashmap_foreach_context(HashMap *map, gfc_work_func_context func,void *context)
{
    int i,c;
    void *item;
    List *items;
    if ((!map)||(!func))return;
    items = gfc_hashmap_get_all_values(map);
    if (!items)return;
    c = gfc_list_get_count(items);
    for (i = 0;i < c;i++)
    {
        item = gfc_list_get_nth(items,i);
        if (!item)continue;
        func(item,context);
    }
    gfc_list_delete(item);
}


/**/
