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

typedef struct command_node *command_node_t;

struct command_node
{
  command_t command;
  command_node_t next;
};

struct command_stream
{
  command_node_t* commands;
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

int isSimpleCommand(char* c)
{
  int x=0;
  char cur;
  cur = c[0];
  while(cur!='\0')
    {
      if(!isalphaNum(c[x]))
	{
	  return 0;
	}
      cur = c[x+1];
      x++;
    }
  return 1;
}
char* parseIntoBlocks(char* c, int *start, int *size)
{
  char* str = malloc(sizeof(char));
  int blockSize = 0;
  int x = *start;
  for(x;x<(*size);x++)
    {
      blockSize++;
      str = (char*)realloc(str, blockSize*sizeof(char));
      str[blockSize-1] = c[x];
      //printf("current string: ");
      //printf(str);
      //printf("\n");
      if(c[x+1] == '\n' && c[x+2] == '\n')
	{
	  blockSize+=2;
	  *start = *start + blockSize;
	  return str;
	}
    }
  return '\0';
  
}
/*
struct node* saveToNode(char* str, struct node* current, int flag, int size)
{
  return NULL;
  }*/

command_stream_t
make_command_stream (int (*get_next_byte) (void *),
		     void *get_next_byte_argument)
{
  command_stream_t stream = checked_malloc(sizeof(struct command_stream));
  command_node_t head = NULL;
  stream->commands = checked_malloc(sizeof(struct command_node));
  head = stream->commands;
  command_node_t current = head;
  int current_byte;
  char* buffer = (char*)checked_malloc(sizeof(char));
  char* str = (char*)checked_malloc(sizeof(char));
  int wordSize=0;
  char current_char;
  int line = 1;
  int i=1;
  while((current_byte = get_next_byte(get_next_byte_argument))!= EOF){
    current_char = current_byte;
    buffer = (char*)realloc(buffer,i*sizeof(char));
    buffer[i-1]=current_char;
    i++;

  }
  int x=0;
  char* buffer_block = (char*)checked_malloc(i*sizeof(char));
  int start = 0;
  int size = i;
  int wordCount = 0;

  while(buffer_block!='\0')
    {

      buffer_block = parseIntoBlocks(buffer,&start,&size);
      
    }
  /*
  for(x=0;x<i;x++)
    {
      
      if(isalphaNum(buffer[x]) || buffer[x]==' ')
      {

	wordSize++;
	buffer_block = (char*)realloc(buffer_block,wordSize*sizeof(char));
	buffer_block[wordSize-1] = buffer[x];
	
      }
    
      
      if((strncmp(buffer_block,"if",2)==0))
      {
	printf(buffer_block);
	printf("\n");
	int k = x+1;
	while(isalphaNum(buffer[k]) || buffer[x]==' ' || isOperator(buffer[k]))
	  {
	    printf("%i",k);
	    printf("\n");
	    wordSize++;
	    buffer_block = (char*)realloc(buffer_block,wordSize*sizeof(char));
	    buffer_block[wordSize-1] = buffer[k];
	    printf(buffer_block);
	    printf("\n");
	    if(buffer[k] == 'i' && buffer[k-1] == 'f' && buffer[k-2] == ' ')
	      {
	     
		return 0;
		break;
		}
	    k++;
	  }
	 x = x+k;
	 }
   
    }*/

   return 0;
}

command_t
read_command_stream (command_stream_t s)
{
  return NULL;
}
