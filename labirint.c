#include <stdio.h>
#include <stdlib.h>
#include "labirint.h"
#include "list.h"
#include "error_codes.h"

void print_point(void *point)
{
    point_t *p = (point_t*) point;
    printf("%lu %lu\n", p->x, p->y);
}

bool is_points_eq(void *va, void *vb)
{
    point_t *a = (point_t*) va, *b = (point_t*) vb;
    return a->x == b->x && a->y == b->y;
}

int labirint_init(struct labirint_t *labirint, size_t n, size_t m)
{
    int *data = NULL;
    if (labirint->fields)
        labirint_free(labirint);

    data = calloc(n * m, sizeof(int));
    if (!data)
        return ERR_MEM;

    labirint->fields = calloc(n, sizeof(int*));
    if (!labirint->fields)
    {
        free(data);
        return ERR_MEM;
    }

    for (size_t i = 0; i < n; i++)
        labirint->fields[i] = &data[i * m];
    
    labirint->n = n;
    labirint->m = m;
    return OK;
}

void labirint_free(struct labirint_t *labirint)
{
    if (labirint->fields)
    {
        free(*(labirint->fields));
        free(labirint->fields);
    }
    labirint->fields = NULL;
}

int labirint_input(FILE *f_in, struct labirint_t *labirint)
{
    size_t n, m;
    int cur;
    bool start_flag = false, end_flag = false;

    if (f_in == stdin)
        printf("Введите размеры лабиринта:\n");

    if (fscanf(f_in, "%lu%lu", &n, &m) != 2)
        return ERR_IO;
    if (n == 0 || m == 0)
        return ERR_RANGE;
    if (labirint_init(labirint, n, m) != OK)
        return ERR_MEM;

    if (f_in == stdin)
        printf("Введите лабиринт %lu:%lu (1 - старт, 2 - конец, 0 - пустое поле, -1 - препятствие):\n", n, m);
    
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < m; j++)
        {
            if (fscanf(f_in, "%d", &cur) != 1)
            {
                labirint_free(labirint);
                return ERR_IO;
            }

            if (cur == START_FIELD)
            {
                if (start_flag)
                {
                    labirint_free(labirint);
                    return ERR_LABIRINT;
                }
                labirint->start.y = i;
                labirint->start.x = j;
                start_flag = true;
            }
            else if (cur == END_FIELD)
            {
                if (end_flag)
                {
                    labirint_free(labirint);
                    return ERR_LABIRINT;
                }
                labirint->end.y = i;
                labirint->end.x = j;
                end_flag = true;
            }
            else if (cur != EMPTY_FIELD && cur != BARRIER_FIELD)
            {
                labirint_free(labirint);
                return ERR_RANGE;
            }
            labirint->fields[i][j] = cur;
        }
    }

    if (!start_flag || !end_flag)
        return ERR_LABIRINT;
    return OK;
}

void labirint_print(const struct labirint_t *labirint)
{
    if (!labirint->fields)
        return;
    for (size_t i = 0; i < labirint->n; i++)
    {
        for (size_t j = 0; j < labirint->m; j++)
            printf("%d ", labirint->fields[i][j]);
        printf("\n");
    }
}

int find_path(struct labirint_t *labirint, struct list_node_t **path,
    void *stack, pop_func_t pop, push_func_t push)
{
    size_t stack_len = 1;
    point_t *cur_point = NULL;
    cur_point = malloc(sizeof(point_t));
    if (!cur_point)
        return ERR_MEM;
    cur_point->x = labirint->start.x;
    cur_point->y = labirint->start.y;
    int rc = OK;

    while (rc == OK && cur_point && !is_points_eq(cur_point, &labirint->end))
    {
        size_t x = cur_point->x, y = cur_point->y;
        labirint->fields[y][x] = STACK_FIELD;
        
        point_t *new_point = NULL;
        new_point = malloc(sizeof(point_t));
        if (!new_point)
        {
            rc = ERR_MEM;
            continue;
        }
        new_point->x = cur_point->x;
        new_point->y = cur_point->y;
        
        if (x < labirint->m - 1 && (labirint->fields[y][x+1] == EMPTY_FIELD || labirint->fields[y][x+1] == END_FIELD))
            new_point->x++;
        else if (y < labirint->n - 1 && (labirint->fields[y+1][x] == EMPTY_FIELD || labirint->fields[y+1][x] == END_FIELD))
            new_point->y++;
        else if (x > 0 && (labirint->fields[y][x-1] == EMPTY_FIELD || labirint->fields[y][x-1] == END_FIELD))
            new_point->x--;
        else if (y > 0 && (labirint->fields[y-1][x] == EMPTY_FIELD || labirint->fields[y-1][x] == END_FIELD))
            new_point->y--;
        else
        {
            free(new_point);
            free(cur_point);
            if (stack_len == 0)
                break;
            cur_point = pop(stack);
            stack_len--;
            labirint->fields[y][x] = DEADLOCK_FIELD;
            continue;
        }

        rc = push(stack, cur_point);        
        stack_len++;
        cur_point = new_point;
    }

    if (!cur_point || (rc == OK && !is_points_eq(cur_point, &labirint->end)))
        rc = ERR_NO_SOLUTION;
    free(cur_point);

    while ((cur_point = ((point_t*) pop(stack))) && rc == OK)
        rc = appbeg(path, cur_point);
    return rc;
}
