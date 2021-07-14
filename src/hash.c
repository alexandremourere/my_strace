#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "hash_map.h"

size_t hash(const char *key)
{
    return atol(key);
    // size_t i = 0;
    // size_t hash = 0;

    // for (i = 0; key[i] != '\0'; ++i)
    //     hash += key[i];
    // hash += i;

    // return hash;
}