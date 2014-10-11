// UCLA CS 111 Lab 1 command reading

// Copyright 2012-2014 Paul Eggert.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "command.h"
#include "command-internals.h"
#include "alloc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <stdarg.h>
#include <ctype.h>

/* FIXME: You may need to add #include directives, macro definitions,
   static function definitions, etc.  */

/* FIXME: Define the type 'struct command_stream' here.  This should
   complete the incomplete type declaration in command.h.  */

struct command_stream
{
  //pointer to command pointer
  struct node* root;
};

struct node
{
  char* word;  
  struct node *next;
  
};

command_stream_t
make_command_stream (int (*get_next_byte) (void *),
		     void *get_next_byte_argument)
{
  //create a command stream
  /*Parses the commmands and stores it in a command stream pick a data structure to store the shit in

     test-t-ok.sh

    1) Generate inputs(shell commands)
    2) Generate expected output
    3) Run you program against 1
    4) Take diff of output from 3 and 2

*/
  command_stream_t stream = (command_stream_t)checked_malloc(sizeof(struct command_stream));
  struct node *head = checked_malloc(sizeof(struct node));
  struct node *current = checked_malloc(sizeof(struct node));
  current = head;
  size_t read = 0;
  int current_byte;
  //char* buffer = (char*) checked_malloc(bufferSize);
  char* str = (char*)checked_malloc(sizeof(char));
  int i=0;
  int wordSize=0;
  char current_char;
  
  while((current_byte = get_next_byte(get_next_byte_argument))!= EOF){
    
    current_char = current_byte;
    if(current_char!=32 && current_char!=10)
      {
	wordSize++;
	str = (char*)realloc(str,wordSize*sizeof(char));
	str[wordSize-1] = current_byte;
	
      }
    
    else
      {
	current->word = (char*)checked_malloc(wordSize*sizeof(char));
	int x = 0;
	for(x=0; x<wordSize; x++)
	  {
	    (current->word)[x] = str[x];
	  }
	
	free(str);
	str = (char*)checked_malloc(sizeof(char));
	current = current->next;
	current = malloc(sizeof(struct node));
	wordSize=0;
       }
    i++;
   
  }

	current->word = (char*)checked_malloc(wordSize*sizeof(char));
	int x;
       	for(x=0; x<wordSize; x++)
	  {
	    (current->word)[x] = str[x];
	  }
	
	free(str);
	str = (char*)checked_malloc(sizeof(char));
	current = current->next;
	current = malloc(sizeof(struct node));
	wordSize=0;
	stream->root = checked_malloc(sizeof(struct node));
	stream->root = head;
	//printf(stream->root->word);
	return stream;
}

command_t
read_command_stream (command_stream_t s)
{
  printf(s->root->word);
  return 0;
}

