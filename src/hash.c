#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "hash_map.h"

size_t hash(const char *key)
{
    return atol(key);
}