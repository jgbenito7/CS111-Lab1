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

enum { COMMAND,
	 OPERATOR,
	 COMPOUND,
	 NEWLINE_SEMICOLON
  };

struct command_node
{
  command_t command;
  command_node* next;
}

struct command_stream
{
  command_node_t* commands;
};

struct node
{
  int flag;
  char* word;  
  struct node* next;  
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
  if(c == '!' || c == '%' || c == '+' || c == ',' || c == '-' || c == '.' || c == '/' || c == '@' || c == '^' || c == '_' || c == '=')
    return 1;
  return 0;
}
struct node* saveToNode(char* str, struct node* current, int flag, int size)
{
  current->word = (char*)checked_malloc(size*sizeof(char));
   int x = 0;
   for(x=0; x<size; x++)
     {
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
  //create a command stream


  struct command_stream_t stream = (command_stream_t)checked_malloc(sizeof(struct command_stream));
  struct node* head;
  struct node* current;
  current = (struct node *)checked_malloc(sizeof(struct node));
  head = current;
  
  int current_byte;
  char* str = (char*)checked_malloc(sizeof(char));
  int wordSize=0;
  char current_char;
  int line = 1;
  int foundComment = 0;
  while((current_byte = get_next_byte(get_next_byte_argument))!= EOF){
    current_char = current_byte;
     if(current_char == '#')
       {
	 foundComment = 1;
	 continue;
       }
    
     if(isOperator(current_char))
      {
	
	if(!(str[0]=='\0'))
	    { 
	      current = saveToNode(str, current, OPERATOR, wordSize);
	      str[0]='\0';
	      memset(str,0,strlen(str));
	      wordSize = 0;
	  }
	wordSize++;
	str = (char*)realloc(str,wordSize*sizeof(char));
	str[wordSize-1] = current_byte;
	current = saveToNode(str, current, OPERATOR, wordSize);
	str[0]='\0';
	memset(str,0,strlen(str));
	wordSize=0;
	continue;
	
	}
    
       
    
    if(isalphaNum(current_char) || current_char==' ')
      {
	if(foundComment)
	  {
	    continue;
	  }
	wordSize++;
	str = (char*)realloc(str,wordSize*sizeof(char));
	str[wordSize-1] = current_byte;
	
      }
    
     
    if((strncmp(str,"if",2)==0) || (strncmp(str,"while",5)==0) || (strncmp(str,"until",2)==0) || (strncmp(str,"else",4)==0) || (strncmp(str,"done",4)==0))
      {
	
	current = saveToNode(str, current, COMPOUND, wordSize);
	str[0]='\0';
	memset(str,0,strlen(str));
	wordSize = 0;
	continue;
      }
    
    
    
    if(current_char=='\n' || current_char==';')
      {
	if(foundComment)
	  {
	    foundComment = 0;
	    continue;
	  }
	  if(!(str[0]=='\0'))
	    { 
	      current = saveToNode(str, current, COMMAND, wordSize);
	      str[0]='\0';
	      memset(str,0,strlen(str));
	      wordSize = 0;
	  }
	wordSize++;
	current = saveToNode(str, current, NEWLINE_SEMICOLON, wordSize);
	str[0]='\0';
	memset(str,0,strlen(str));
	wordSize = 0;
	continue;
	}
  }
  
  current = head;
  command_node* c_node = checked_malloc(sizeof(struct command_node));
  stream->commands = checked_malloc(sizeof(struct command_node));
  while(current->next != NULL)
   {
      printf("Node: ");
      printf(current->word);
      printf(", Operator: ");
      printf("%i", current->flag);
      printf("\n");
      current = current->next;
      }
   
   return *stream;
}

command_t
read_command_stream (command_stream_t s)
{
  
  command_t command;
  switch(s->flag)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    default:
    }
    s = s->next;*/
  return 0;
}


