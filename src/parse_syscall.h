#ifndef PARSE_SYSCALL_H
#define PARSE_SYSCALL_H

#include "hash_map.h"

struct hash_map *parse_syscall(int arch_type);

#endif