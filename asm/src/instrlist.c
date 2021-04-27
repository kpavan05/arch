#include<stdlib.h>
#include <stdio.h>
#include <string.h>
#include "instruction.h"

void listrelease(struct instruction* head)
{
    struct instruction* item = NULL;
    while (head != NULL)
    {
        item = head;
        head = head->next;
        free(item->opstr);
        free(item);
    }
}
/* store the newly created instruction in the linked list*/
void write2list(struct instruction* head, struct instruction* instr)
{
   struct instruction* item = NULL;
   struct instruction* prev = NULL;
   struct instruction* next = NULL;

   if (head == NULL)
   {
       head = instr;
       head->next = NULL;
   }
   else
   {
       item = instr;
       prev = head;
       next = head->next;
       while (next != NULL)
       {
           prev = next;
           next = next->next;
       }
       prev->next = item;
   }
}
