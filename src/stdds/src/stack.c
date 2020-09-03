#include  "stack.h"

extern int stack_new(stack *stack, size_t size_element, size_t initial_capacity,
              dsconf *conf);
extern void stack_delete(stack *stack);
extern bool stack_empty(stack *stack);
extern size_t stack_size(stack *stack);
extern void *stack_top(stack *stack);
extern int stack_push(stack *stack, void *element);
extern void *stack_pop(stack *stack);