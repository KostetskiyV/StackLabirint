#ifndef ARRAY_STACK_H__
#define ARRAY_STACK_H__

#include <stddef.h>

struct array_stack_t
{
    size_t length;
    size_t size;
    void **content;
};

struct array_stack_t *astack_init(size_t size);
void astack_free(struct array_stack_t *stack);
int astack_push(struct array_stack_t *stack, void *new_el);
void *astack_pop(struct array_stack_t *stack);

#endif
