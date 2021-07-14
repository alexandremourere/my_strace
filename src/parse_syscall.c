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

    char *key = NULL;
    char *value = NULL;

    while ((r = getline(&line, &len, f)) != -1)
    {
        if (i++ < 3)
            continue;
        char *saveptr = NULL;
        for (char *str1 = line;; str1 = NULL)
        {
            char *token = strtok_r(str1, " ", &saveptr);
            if (NULL == token)
            {
                break;
            }
            if (!strcmp(token, "#define") || !strcmp(token, " "))
            {
                continue;
            }

            char *saveptrW = NULL;

            for (char *str2 = token;; str2 = NULL)
            {
                char *word = strtok_r(str2, "_", &saveptrW);

                if (NULL == word)
                {
                    break;
                }
                int is_nb = is_number(word);
                if (strcmp(word, "NR"))
                {
                    if (is_nb)
                    {
                        key = word;
                    }
                    else
                    {
                        value = word;
                    }
                }
            }
        }
        char d[strlen(key)];
        for (size_t i = 0; i + 1 < strlen(key); i++)
        {
            d[i] = key[i];
        }
        d[strlen(key) - 1] = '\0';
        bool update = 0;
        hash_map_insert(res, strdup(d), strdup(value), &update);
    }
    free(line);
    fclose(f);
    return res;
}