#include "parse_syscall.h"

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

int is_number(char *str)
{
    for (int i = 0; str[i + 1]; i++)
    {
        if (str[i] < '0' || str[i] > '9')
            return 0;
    }
    return 1;
}

struct hash_map *parse_syscall(int arch_type)
{
    FILE *f = NULL;
    if (arch_type == 64)
        f = fopen("/usr/include/asm/unistd_64.h", "r");
    else
        f = fopen("/usr/include/asm/unistd_32.h", "r");
    if (NULL == f)
    {
        err(1, "error while opening the file.");
        return NULL;
    }

    struct hash_map *res = hash_map_init(450);

    char *line = NULL;
    size_t len = 0;
    ssize_t r;
    int i = 0;

    while ((r = getline(&line, &len, f)) != -1)
    {
        if (i++ < 3)
            continue;

        int start = 0;
        while (*(line + start) != '\0' && *(line + start) != '\n'
               && *(line + start) != 'R')
            start++;
        if (*(line + start) == '\0' || *(line + start) == '\n')
            continue;

        start += 2;

        int end = start + 1;
        while (*(line + end) != '\0' && *(line + end) != '\n'
               && *(line + end) != ' ')
            end++;

        char *value = strndup(line + start, end - start);

        start = end + 1;

        while (*(line + end) != '\n')
            end++;

        char *key = strndup(line + start, end - start);

        bool update;
        hash_map_insert(res, key, value, &update);
    }
    free(line);
    fclose(f);
    return res;
}
