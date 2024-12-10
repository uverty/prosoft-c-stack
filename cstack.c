#include "cstack.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct stack_entries_table g_table = {0u, NULL};

//0 - соответствующий хэндлеру стек существует, 1 - нет.
int stack_valid_handler(const hstack_t hstack)
{
    if(hstack >= g_table.size)
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
        printf("invalid handler");
        exit(1);
    }
    if(!data_in || !size)
    {
        printf("data_in or size incorrect\n");
        exit(1);
    }
    stack_t next_node = malloc(sizeof(struct node) + size);
    if(next_node == NULL)
    {
        printf("malloc incorrect\n");
        exit(1);
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
        printf("invalid handler");
        return 0;
    }
    if(!data_out || !size)
    {
        printf("data_out or size incorrect\n");
        exit(1);
    }
    memcpy(data_out, g_table.entries[stack].stack->data, size);
    stack_t del_list = g_table.entries[stack].stack;
    g_table.entries[stack].stack = g_table.entries[stack].stack->prev;
    free(del_list);
    return size;
}

size_t stack_size(const hstack_t stack)
{
    if(stack_valid_handler(stack))
    {
        printf("invalid handler");
        return 0;
    }
    size_t i = 0;

    while(g_table.entries[stack].stack  != NULL)
    {
        g_table.entries[stack].stack = g_table.entries[stack].stack->prev;

        i++;
    }

    return i;
}



hstack_t stack_new(void)
{
    if(g_table.size == 1024){
        printf("Кол-во стеков равно 1024!");
        return -1;
    }
    int realloc_need = 1;
    int free_cell = 0;
    for(int i = 0; i < g_table.size; i++)
    {
        if(g_table.entries[i].reserved == 0)
        {
            realloc_need = 0;
            free_cell = i;
            break;
        }
    }
    if(realloc_need == 1)
    {
        g_table.entries =  realloc(g_table.entries, (g_table.size + 1) * sizeof(stack_entry_t));
        if(g_table.entries == NULL)
        {
            printf("realloc incorrect\n");
            return -1;
        }
    }
    stack_entry_t new_stack;
    new_stack.reserved = 1u;
    new_stack.stack = NULL;
    g_table.entries[realloc_need ? g_table.size : free_cell] = new_stack;
    g_table.size++;
    return g_table.size - 1;
}

void stack_free(const hstack_t hstack)
{   while(g_table.entries[hstack].stack != NULL)
    {
        stack_t del_list = g_table.entries[hstack].stack;
        g_table.entries[hstack].stack = g_table.entries[hstack].stack->prev;
        free(del_list);
    }
    g_table.entries[hstack].reserved = 0;
}
