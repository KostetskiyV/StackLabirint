#include <stdlib.h>
#include "array_stack.h"
#include "error_codes.h"

struct array_stack_t *astack_init(size_t size)
{
    static struct array_stack_t stack = {0, 0, NULL};
    stack.size = size;
    stack.content = calloc(size, sizeof(void*));
    if (!stack.content)
        return NULL;
    return &stack;
}

void astack_free(struct array_stack_t *stack)
{
    free(stack->content);
    stack->length = 0;
    stack->size = 0;
    stack->content = NULL;
}

int astack_push(struct array_stack_t *stack, void *new_el)
{
    if (stack->length == stack->size)
        return ERR_STACK_OVERFLOW;
    stack->content[stack->length++] = new_el;
    return OK;
}

void *astack_pop(struct array_stack_t *stack)
{
    if (stack->length == 0)
        return NULL;
    return stack->content[--(stack->length)];
}
