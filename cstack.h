#ifndef CSTACK_H
#define CSTACK_H

typedef int hstack_t;

struct node
{
    const struct node* prev;       
    unsigned int size;
    char data[0];     
};

typedef struct node* stack_t;

typedef struct stack_entry
{
    int reserved;
    stack_t stack;
}stack_entry_t;

struct stack_entries_table
{
    unsigned int size;
    stack_entry_t* entries;
};

hstack_t stack_new(void);
void stack_free(const hstack_t stack);
int stack_valid_handler(const hstack_t stack);
unsigned int stack_size(const hstack_t stack);
void stack_push(const hstack_t stack, const void* data_in, const unsigned int size);
unsigned int stack_pop(const hstack_t stack, void* data_out, const unsigned int size);

#endif /* CSTACK_H */
