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

//define bool
typedef int bool;
enum { false, true };


struct command_stream
{
  //pointer to command pointer
  struct node* root;
};

struct node
{
  int flag;
  char* word;  
  struct node *next;
  
};

int isOperator(char c)
{
  switch(c)
    {
    case '|':
      {
	return 1;
	break;
      }
    case '(':
      {
	return 1;
	break;
      }
    case ')':
      {
	return 1;
	break;
      }
    case ':':
      {
	return 1;
	break;
      }
    case '<':
      {
	return 1;
	break;
      }
    case '>':
      {
	return 1;
	break;
      }
    default:
      return 0;
    }
}

int isalphaNum(char c)
{
  if(c>=48 && c<=57)
    return 1;
  if(c>=65 && c<=90)
    return 1;
  if(c>=97 && c<=122)
    return 1;
  if(c == '!' || c == '%' || c == '+' || c == ',' || c == '-' || c == '.' || c == '/' || c == '@' || c == '^' || c == '_')
    return 1;
  return 0;
}

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
  enum { COMMAND,
	 OPERATOR,
	 COMPOUND,
	 NEWLINE_SEMICOLON
  };
  command_stream_t stream = (command_stream_t)checked_malloc(sizeof(struct command_stream));
  struct node *head = checked_malloc(sizeof(struct node));
  struct node *current = checked_malloc(sizeof(struct node));
  current = head;
  int current_byte;
  //char* buffer = (char*) checked_malloc(bufferSize);
  char* str = (char*)checked_malloc(sizeof(char));
  int i=0;
  int wordSize=0;
  char current_char;
  int line = 1;
  while((current_byte = get_next_byte(get_next_byte_argument))!= EOF){
    current_char = current_byte;

    if(isOperator(current_char))
      {
	wordSize++;
	current->word = (char*)checked_malloc(wordSize*sizeof(char));
	(current->word)[0]=current_char;
	current->flag = OPERATOR;
	wordSize=0;
	//printf(current->word);
	//printf("%i", current->flag);
	current = current->next;
	current = malloc(sizeof(struct node));
	continue;
	
      }

    if(isalphaNum(current_char) || current_char==' ')
      {
	wordSize++;
	str = (char*)realloc(str,wordSize*sizeof(char));
	str[wordSize-1] = current_byte;
	//printf(str);
	//printf("\n");
      }
    
     
    if((strncmp(str,"if",2)==0) || (strncmp(str,"while",5)==0) || (strncmp(str,"until",2)==0))
      {
	current->word = (char*)checked_malloc(wordSize*sizeof(char));
	int x = 0;
	for(x=0; x<wordSize; x++)
	  {
	    (current->word)[x] = str[x];
	  }
	current->flag = COMPOUND;
	current = current->next;
	current = malloc(sizeof(struct node));
	free(str);
	wordSize = 0;
	continue;
      }
    
    if(current_char=='\n' || current_char==';')
      {
          printf(str);
	  printf("\n");
	if(!(str[0]=='\0'))
	  {
	    current->word = (char*)checked_malloc(wordSize*sizeof(char));
	    int x = 0;
	    for(x=0; x<wordSize; x++)
	      {
		(current->word)[x] = str[x];
	      }
	    current->flag = COMMAND;
	    //printf(current->word);
	    current = current->next;
	    current = malloc(sizeof(struct node));
	    wordSize = 0;
	  }
	wordSize++;
	current->word = (char*)checked_malloc(wordSize*sizeof(char));
	 int x = 0;
	    for(x=0; x<wordSize; x++)
	      {
		(current->word)[x] = str[x];
	      }
	    current->flag = NEWLINE_SEMICOLON;
	    current = current->next;
	    current = malloc(sizeof(struct node));
	free(str);
	wordSize = 0;
	continue;
      }
  }
	
	return stream;
}

command_t
read_command_stream (command_stream_t s)
{
  
  return 0;
}


