#include "hash_map.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct hash_map *hash_map_init(size_t size)
{
    struct hash_map *hash_map = malloc(sizeof(struct hash_map));
    struct pair_list **data = calloc(size, sizeof(struct pair_list));
    if (NULL == hash_map || NULL == data)
    {
        free(hash_map);
        free(data);
        return NULL;
    }
    hash_map->size = size;
    hash_map->data = data;
    return hash_map;
}

static void list_free(struct pair_list *list)
{
    if (list)
    {
        free(list->value);
        free(list->key);
        list_free(list->next);
    }
    free(list);
}

static int my_strlen(char *s)
{
    int len = 0;
    for (; len[s]; len++)
        continue;
    return len;
}

static int my_strlen2(const char *s)
{
    int len = 0;
    for (; len[s]; len++)
        continue;
    return len;
}

static int my_strcmp(char *str1, char *str2)
{
    int len = my_strlen(str1);
    if (len != my_strlen(str2))
        return 0;
    int i = 0;
    for (; i < len && str1[i] == i[str2]; i++)
        continue;
    return i == len;
}

static int my_strcmp2(const char *str1, const char *str2)
{
    int len = my_strlen2(str1);
    if (len != my_strlen2(str2))
        return 0;
    int i = 0;
    for (; i < len && str1[i] == i[str2]; i++)
        continue;
    return i == len;
}

void hash_map_free(struct hash_map *hash_map)
{
    if (NULL == hash_map)
        return;
    for (size_t i = 0; i < hash_map->size; i++)
    {
        list_free(hash_map->data[i]);
    }
    free(hash_map->data);
    free(hash_map);
}
bool hash_map_insert(struct hash_map *hash_map, char *key, char *value,
                     bool *updated)
{
    if (hash_map_get(hash_map, key) != NULL)
    {
        free(key);
        free(value);
        return false;
    }
    if (NULL == hash_map)
        return false;
    struct pair_list *add = malloc(sizeof(struct pair_list));
    if (NULL == add)
        return false;

    add->key = key;
    add->value = value;
    add->next = NULL;
    size_t index = hash(key);

    struct pair_list *pair_list = hash_map->data[index];
    if (NULL == pair_list)
    {
        hash_map->data[index] = add;
        if (NULL != updated)
            *updated = true;
        return true;
    }
    for (; pair_list->next && strcmp(pair_list->key, key);
         pair_list = pair_list->next)
        continue;
    if (!my_strcmp(pair_list->value, value) && !strcmp(pair_list->key, key))
    {
        if (NULL != updated)
            *updated = true;
        free(add);
        pair_list->value = value;
        return true;
    }
    else if (my_strcmp2(pair_list->key, key))
    {
        if (NULL != updated)
            *updated = false;
    }
    else if (NULL != updated)
        *updated = false;
    pair_list->next = add;
    return true;
}

const char *hash_map_get(const struct hash_map *hashh, char *key)
{
    if (NULL == hashh)
        return NULL;
    size_t index = hash(key);
    struct pair_list *pair_list = hashh->data[index];
    for (; pair_list; pair_list = pair_list->next)
    {
        if (!strcmp(key, pair_list->key))
            return pair_list->value;
    }
    return NULL;
}

bool hash_map_remove(struct hash_map *hashh, char *key)
{
    if (NULL == hashh)
        return false;
    size_t index = hash(key);
    struct pair_list *pair_list = hashh->data[index];
    if (NULL == pair_list)
        return false;
    if (!strcmp(pair_list->key, key))
    {
        hashh->data[index] = pair_list->next;
        free(pair_list);
        return true;
    }
    for (; pair_list->next && strcmp(pair_list->next->key, key);
         pair_list = pair_list->next)
        continue;
    if (NULL == pair_list->next)
        return false;
    struct pair_list *rm = pair_list->next;
    pair_list->next = rm->next;
    free(rm);
    return true;
}
