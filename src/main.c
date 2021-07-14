#include <assert.h>
#include <stdio.h>

#include "hash_map.h"

int main(void)
{
    int a = 2;
    if (a == 2 || a >= 3)
        printf("Hello\n");
    struct hash_map *hash_map = hash_map_init(7);
    if (NULL == hash_map)
        return 1;
    bool updated = false;
    hash_map_insert(hash_map, "TARAN", "W", &updated);
    assert(updated == true);
    hash_map_insert(hash_map, "TARAN", "W", &updated);
    assert(updated == false);
    hash_map_insert(hash_map, "TARAN", "w", &updated);
    assert(updated == true);
    hash_map_insert(hash_map, "123456789", "51", &updated);
    assert(updated == true);
    hash_map_insert(hash_map, "SKI", "winter", &updated);
    assert(updated == true);
    hash_map_insert(hash_map, "C", "42", &updated);
    assert(updated == true);
    hash_map_insert(hash_map, "42", "life", &updated);
    assert(updated == true);
    hash_map_insert(hash_map, "test", "42", &updated);
    assert(updated == true);
    // hash_map_insert(hash_map, "123456789", "51", &updated);
    hash_map_dump(hash_map);
    printf("\n");
    // bool b = hash_map_remove(hash_map, "SKI");
    hash_map_remove(hash_map, "SKI");
    hash_map_remove(hash_map, "test");
    hash_map_remove(hash_map, "42");
    hash_map_remove(hash_map, "C");
    hash_map_remove(hash_map, "TARAN");
    hash_map_remove(hash_map, "123456789");
    // printf("%s\n", hash_map_get(hash_map, "SKI"));
    // printf("%d\n", b);
    hash_map_dump(hash_map);
    hash_map_free(hash_map);
    return 0;
}