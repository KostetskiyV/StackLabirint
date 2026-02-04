#ifndef LABIRINT_H__
#define LABIRINT_H__

#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include "list.h"

#define EMPTY_FIELD 0
#define BARRIER_FIELD -1
#define START_FIELD 1
#define END_FIELD 2
#define DEADLOCK_FIELD -2
#define STACK_FIELD -3

typedef void* (*pop_func_t)(void *stack);
typedef int (*push_func_t)(void *stack, const void *new_el);

typedef struct
{
    size_t x;
    size_t y;
} point_t;

struct labirint_t
{
    size_t n;
    size_t m;
    int **fields;
    point_t start;
    point_t end;
};

void print_point(void *point);
bool is_points_eq(void *va, void *vb);

int labirint_init(struct labirint_t *labirint, size_t n, size_t m);
void labirint_free(struct labirint_t *labirint);
int labirint_input(FILE *f_in, struct labirint_t *labirint);
void labirint_print(const struct labirint_t *labirint);

int find_path(struct labirint_t *labirint, struct list_node_t **path,
    void *stack, pop_func_t pop, push_func_t push);

#endif
