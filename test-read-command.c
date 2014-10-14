#include "command.h"
#include "command-internals.h"
#include "alloc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <stdarg.h>
#include <ctype.h>

enum { COMMAND,
	 OPERATOR,
	 COMPOUND,
	 NEWLINE_SEMICOLON
  };

struct command_stream
{
  //pointer to command pointer
  struct node* root;
};

struct node
{
  int flag;
  char* word;  
  struct node* next;
};

struct node* saveToNode(char* str, struct node* current, int flag, int size)
{
   current->word = (char*)checked_malloc(size*sizeof(char));
   int x = 0;
   for(x=0; x<size; x++)
     {
       // printf(str);
       (current->word)[x] = str[x];
     }
   current->flag = flag;
   printf("Inserted: "); printf(current->word); printf(", ");printf("%i", current->flag);printf("\n");
   current->next = malloc(sizeof(struct node)); 
   current = current->next;
   return current;
}

command_stream_t
make_command_stream (int (*get_next_byte) (void *),
		     void *get_next_byte_argument)
{
  struct node* head;
  struct node* current;
  
  current = (struct node *)malloc(sizeof(struct node));
  head = current;
  char arr1[3] = "yes";
  char arr2[2] = "no";
  char arr3[4] = "goat";
  current = saveToNode(arr1, current, OPERATOR, 3);
  current = saveToNode(arr2, current, OPERATOR, 2);
  current = saveToNode(arr3, current, OPERATOR, 4);
  current = head;
  printf(head->word);
  return 0;
}

command_t
read_command_stream (command_stream_t s)
{
  
  return 0;
}
