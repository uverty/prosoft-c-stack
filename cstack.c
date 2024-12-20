#include "cstack.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct node* stack_t;

struct node
{
    const struct node* prev;       
    unsigned int size;
    char data[0];     
};

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

struct stack_entries_table g_table = {0u, NULL};

//0 - соответствующий хэндлеру стек существует, 1 - нет.
int stack_valid_handler(const hstack_t hstack)
{   
    if((unsigned)hstack >= g_table.size  || hstack < 0)
    {
        return 1;
    }    

    if (g_table.entries[hstack].reserved == 1)
    {
        return 0;
    }

    return 1; 
}

 void stack_push(const hstack_t stack, const void* data_in, const unsigned int size)
{   
    if(stack_valid_handler(stack))
    {
        //printf("invalid handler");
        return;
    }
    if(!data_in || !size)
    {
        //printf("data_in or size incorrect\n");
        return;
    }
    stack_t next_node = malloc(sizeof(struct node) + size);
    if(next_node == NULL)
    {
        //printf("malloc incorrect\n");
        return;
    }
    next_node->prev = g_table.entries[stack].stack;
    next_node->size = size;
    memcpy(next_node->data, data_in, next_node->size);
    g_table.entries[stack].stack = next_node;
}

unsigned int stack_pop(const hstack_t stack, void* data_out, const unsigned int size)
{
    if(stack_valid_handler(stack))
    {
        //printf("invalid handler");
        return 0;
    } 
    if(!data_out || !size)
    {
        //printf("data_out or size incorrect\n");
        return 0;
    }
    if (g_table.entries[stack].stack == NULL)
    {
        return 0;
    }
    if(size != g_table.entries[stack].stack->size)
    {
        //printf("bad size");
        return 0;
    }
    void* res = memcpy(data_out, g_table.entries[stack].stack->data, size);
    if (res == NULL)
    {
        return 0;
    }

    stack_t del_list = g_table.entries[stack].stack;
    g_table.entries[stack].stack = (stack_t)g_table.entries[stack].stack->prev;
    free(del_list);
    return size;
}

unsigned int stack_size(const hstack_t stack)
{
    if(stack_valid_handler(stack))
    {
        //printf("invalid handler");
        return 0;
    }

    stack_t head = g_table.entries[stack].stack;
    unsigned int i = 0;
    while(head != NULL)
    {
        head = (stack_t)head->prev;
        i++;
    }

    return i;
}



hstack_t stack_new(void)
{
    int realloc_need = 1;
    unsigned int free_cell = 0;
    for(unsigned int i = 0; i < g_table.size; i++)
    {
        if(g_table.entries[i].reserved == 0)
        {
            realloc_need = 0;
            free_cell = i;
            break;
        }
    }
    stack_entry_t new_stack;
    new_stack.reserved = 1u;
    new_stack.stack = NULL;
    if(realloc_need == 1)
    {   
        stack_entry_t* entries_safe;
        entries_safe = realloc(g_table.entries, (g_table.size + 1) * sizeof(stack_entry_t));
        if(entries_safe == NULL)
        {
            //printf("realloc incorrect\n");
            return -1;
        }
        g_table.entries = entries_safe;
        g_table.entries[g_table.size] = new_stack;
        g_table.size++;
        return g_table.size - 1;
    }
    
    g_table.entries[free_cell]= new_stack;
    return free_cell;
    
}

void stack_free(const hstack_t hstack)
{   
    if(stack_valid_handler(hstack))
    {
        //printf("invalid handler\n");
        return;
    }
    while(g_table.entries[hstack].stack != NULL)
    {
        stack_t del_list = g_table.entries[hstack].stack;
        g_table.entries[hstack].stack = (stack_t)g_table.entries[hstack].stack->prev;
        free(del_list);
    }
    g_table.entries[hstack].reserved = 0;
}
