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
typedef struct node *node_t;

enum{IF,THEN,ELSE,FI,WHILE,DO,DONE,UNTIL,GREATER_THAN,LESS_THAN,OPEN_PAREN,CLOSED_PAREN,PIPE,SIMPLE,NEWLINE};
struct command_node
{
  command_t command;
  command_node_t next;
};

struct command_stream
{
  command_node_t* commands;
};

struct node
{
  int lineNum;
  int flag;
  char* simpleCommand;
  node_t next;
};

int isOperator(char c)
{
  switch(c)
    {
    case '|':{return 1;break;}
    case '(':{return 1;break;}
    case ')':{return 1;break;}
    case ':':{return 1;break;}
    case '<':{return 1;break;}
    case '>':{return 1;break;}
    default:return 0;
    }
}

int isalphaNum(char c)
{
  if(c>=48 && c<=57) return 1;
  if(c>=65 && c<=90) return 1;
  if(c>=97 && c<=122) return 1;
  if(c == '!' || c == '%' || c == '+' || c == ',' || c == '-' || c == '.' || c == '/' || c == '@' || c == '^' || c == '_' || c==' ' || c==':')
    return 1;
  return 0;
}
int isalphaNumNoSpace(char c)
{
  if(c>=48 && c<=57) return 1;
  if(c>=65 && c<=90) return 1;
  if(c>=97 && c<=122) return 1;
  if(c == '!' || c == '%' || c == '+' || c == ',' || c == '-' || c == '.' || c == '/' || c == '@' || c == '^' || c == '_' || c==':')
    return 1;
  return 0;
}

int isSimpleCommand(char* c)
{
  int x=0;
  char cur;
  cur = c[0];
    
  if(cur=='\0')
    return 0;
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
int isSimpleCommandNoSpace(char* c)
{
  int x=0;
  char cur;
  cur = c[0];
    
  if(cur=='\0')
    return 0;
  while(cur!='\0')
    {
      if(!isalphaNumNoSpace(c[x]))
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
  int ifCount = 0;
  int whileCount = 0;
  int untilCount = 0;
  int blockSize = 0;
  int x = *start;
  for(x;x<(*size);x++)
    {
      blockSize++;
      if(c[x] == 'i' && c[x+1] == 'f')
	{
	  printf("found an if\n");
	ifCount++;
	}
      if(c[x] == 'f' && c[x+1] == 'i')
	{
	  printf("found an fi\n");
	  ifCount--;
	}
      str = (char*)realloc(str, blockSize*sizeof(char));
      str[blockSize-1] = c[x];
      if(c[x+1] == '\n' && c[x+2] == '\n' && ifCount==0)
	{
	  str = (char*)realloc(str, (blockSize+1)*sizeof(char));
	  str[blockSize] = '\n';
	  blockSize+=2;
	  str = (char*)realloc(str, blockSize*sizeof(char));
	  str[blockSize-1] = '\0';

	  //blockSize+=1;
	  *start = *start + blockSize;
	  return str;
	}
    }
  return '\0'; 
}

void tokenizeBlock(char* block, int* lineNum, node_t head)
{
  printf("here yo\n");
  int i = 0;
  char* temp;
  int wordSize = 0;
  node_t current = head;
  
  while(block[i]!='\0')
    {
      
      if(wordSize==0)
	temp = calloc(1,sizeof(char));
      else
	{
	  printf("reallocated\n");
	  temp = realloc(temp, (wordSize+1)*sizeof(char));
	}
      wordSize++;
      printf("\nTOP OF BLOCK\n");
      printf("wordSize is: ");printf("%i",wordSize);printf("\n");
       printf("block[i] is: ");printf("%c",block[i]);printf("\n");
      temp[wordSize-1] = block[i];
      //printf("%i",sizeof(*temp));printf("\n");
      printf("temp is: ");printf(temp);printf("\n");
      if(block[i]=='#')
	{
	  while(block[i+1]!='\n')
	    {
	      printf("in here\n");
	      i++;
	    }
	  i++;
	  current->lineNum = *lineNum;
	  *lineNum++;
	  memset(temp,0,wordSize);
	  
	  current->flag = NEWLINE;
	  current->next = checked_malloc(sizeof(struct node));
	  current = current->next;
	  //memset(temp,0,wordSize);
	  free(temp);
	  wordSize = 0;
	  i++;
	  continue;
	}
      //check for leading whitespaces
      if(block[i]==' ' && !isSimpleCommandNoSpace(temp))
	{
	  printf("jumped the space\n");
	  i++;
	  wordSize=0;
	  continue;
	}
      if(strncmp(temp,"if",2)==0)
	{
	  printf("found an if\n");
	  current->lineNum = *lineNum;
	  
	  current->flag = IF;
	  current->next = checked_malloc(sizeof(struct node));
	  current = current->next;
	  //memset(temp,0,wordSize);
	  free(temp);
	  wordSize = 0;
	  i++;
	  continue;
	}
      if(strncmp(temp,"then",4)==0)
	{
	  printf("found an then\n");
	  current->lineNum = *lineNum;

	  current->flag = THEN;
	  current->next = checked_malloc(sizeof(struct node));
	  current = current->next;
	  //memset(temp,0,wordSize);
	  free(temp);
	  wordSize = 0;
	  i++;
	  continue;
	}
       if(strncmp(temp,"else",4)==0)
	{
	  printf("found an else\n");
	  current->lineNum = *lineNum;

	  current->flag = ELSE;
	  current->next = checked_malloc(sizeof(struct node));
	  current = current->next;
	  free(temp);
	  wordSize = 0;
	  i++;
	  continue;
	}
       if(strncmp(temp,"fi",2)==0)
	{
	  printf("found a fi\n");
	  current->lineNum = *lineNum;

	  current->flag = FI;
	  current->next = checked_malloc(sizeof(struct node));
	  current = current->next;
	  //memset(temp,0,wordSize);
	  free(temp);
	  wordSize = 0;
	  i++;
	  continue;
	}
      if(strncmp(temp,"while",5)==0)
	{
	  printf("found a while\n");
	  current->lineNum = *lineNum;
	  current->flag = WHILE;
	  current->next = checked_malloc(sizeof(struct node));
	  current = current->next;
	  //memset(temp,0,wordSize);
	  free(temp);
	  wordSize = 0;
	  i++;
	  continue;
	}
      if(strncmp(temp,"done",4)==0)
	{
	  printf("found a done\n");
	  current->lineNum = *lineNum;
	  current->flag = DONE;
	  current->next = checked_malloc(sizeof(struct node));
	  current = current->next;
	  //memset(temp,0,wordSize);
	  free(temp);
	  wordSize = 0;
	  i++;
	  continue;
	}
      if(strncmp(temp,"do",4)==0)
	{
	  printf("found a do\n");
	  if(block[i+1]=='n')
	    {
	      i++;
	      continue;
	    }
	  current->lineNum = *lineNum;
	  current->flag = DO;
	  current->next = checked_malloc(sizeof(struct node));
	  current = current->next;
	  //memset(temp,0,wordSize);
	  free(temp);
	  wordSize = 0;
	  i++;
	  continue;
	}

      if(strncmp(temp,"until",5)==0)
	{
	  current->lineNum = *lineNum;
	  current->flag = UNTIL;
	  current->next = checked_malloc(sizeof(struct node));
	  current = current->next;
	  //memset(temp,0,wordSize);
	  free(temp);
	  wordSize = 0;
	  i++;
	  continue;
	}
      if(block[i]=='>')
	{	 
	  printf("found >\n");
	    current->simpleCommand = malloc((wordSize-1)*sizeof(char));
	    strncpy(current->simpleCommand,temp,wordSize-1);
	    current->lineNum = *lineNum;
	    current->flag = SIMPLE;
	    current->next = checked_malloc(sizeof(struct node));
	    current = current->next;

	  current->lineNum = *lineNum;
	  current->flag = GREATER_THAN;
	  current->next = checked_malloc(sizeof(struct node));
	  current = current->next;
	  //memset(temp,0,wordSize);
	  free(temp);
	  wordSize = 0;
	  i++;
	  continue;
	}
      if(block[i]=='<')
	{
	  printf("found <\n");
	    current->simpleCommand = malloc((wordSize-1)*sizeof(char));
	    strncpy(current->simpleCommand,temp,wordSize-1);
	    current->lineNum = *lineNum;
	    current->flag = SIMPLE;
	    current->next = checked_malloc(sizeof(struct node));
	    current = current->next;
       
	  current->lineNum = *lineNum;

	  current->flag = LESS_THAN;
	  current->next = checked_malloc(sizeof(struct node));
	  current = current->next;
	  //memset(temp,0,wordSize);
	  free(temp);
	  wordSize = 0;
	  i++;
	  continue;	  
	  }
      if(block[i]=='(')
	{
	  current->lineNum = *lineNum;

	  current->flag = OPEN_PAREN;
	  current->next = checked_malloc(sizeof(struct node));
	  current = current->next;
	  //memset(temp,0,wordSize);
	  free(temp);
	  wordSize = 0;
	  i++;
	  continue;
	}
      if(block[i]==')')
	{
	  // memset(temp,'\0',(wordSize-1)*sizeof(char));
	  if(isSimpleCommand(temp))
	  {
	    current->simpleCommand = malloc((wordSize-1)*sizeof(char));
	    strncpy(current->simpleCommand,temp,wordSize-1);
	    current->lineNum = *lineNum;

	    current->flag = SIMPLE;
	    current->next = checked_malloc(sizeof(struct node));
	    current = current->next;
	  }
	  else
	    {
	      printf("You've got an issue on line ");printf("%i",*lineNum);
	    }
	  current->lineNum = *lineNum;

	  current->flag = CLOSED_PAREN;
	  current->next = checked_malloc(sizeof(struct node));
	  current = current->next;
	  //memset(temp,0,wordSize);
	  free(temp);
	  wordSize = 0;
	  i++;
	  continue;	  
	}
      if(block[i]=='|')
	{
	  printf("found a pipe\n");
	  // memset(temp,'\0',(wordSize-1)*sizeof(char));
	    current->simpleCommand = malloc((wordSize-1)*sizeof(char));
	    strncpy(current->simpleCommand,temp,(wordSize-1));
	    printf("Simple command stored was: ");printf(current->simpleCommand);printf("\n");
	    current->lineNum = *lineNum;
	    current->flag = SIMPLE;
	    current->next = checked_malloc(sizeof(struct node));
	    current = current->next;

	  current->lineNum = *lineNum;

	  current->flag = PIPE;
	  current->next = checked_malloc(sizeof(struct node));
	  current = current->next;
	  //memset(temp,0,wordSize);
	  free(temp);
	  wordSize = 0;
	  i++;
	  continue;
	}
      if(block[i]==';')
	{
	  printf("found a semi colon\n");
	  char* semi = calloc(wordSize-1, sizeof(char)*(wordSize-1));
	  
	  strncpy(semi,temp,wordSize-1);
	  printf("semi is: ");printf(semi);printf("\n");
	  if(isSimpleCommand(semi))
	  {
	    printf("twas a simple command\n");
	    current->simpleCommand = malloc((wordSize-1)*sizeof(char));
	    strncpy(current->simpleCommand,temp,wordSize-1);
	    current->lineNum = *lineNum;
	    current->flag = SIMPLE;
	    current->next = checked_malloc(sizeof(struct node));
	    current = current->next;
	  }
	  // memset(semi,0,wordSize);
	  free(semi);
	  free(temp);
	  wordSize = 0;
	  //check for whitespaces afterwords
	  while(block[i+1]==' ')
	    i++;
	  i++;
	  continue;
	}
      if(block[i]=='\n')
	{
	  printf("found a new line\n");
	  char* newL = malloc(sizeof(char)*(wordSize-1));
	  strncpy(newL,temp,wordSize-1);
	  if(isSimpleCommand(newL)){
	    printf("twas a simple command!\n");
	    current->simpleCommand = malloc((wordSize-1)*sizeof(char));
	    strncpy(current->simpleCommand,temp,wordSize-1);
	    current->lineNum = *lineNum;
	    current->flag = SIMPLE;
	    current->next = checked_malloc(sizeof(struct node));
	    current = current->next;
	  }
	  current->lineNum = *lineNum;
	  *lineNum++;

	  current->flag = NEWLINE;
	  current->next = checked_malloc(sizeof(struct node));
	  current = current->next;
	  free(newL);
	  free(temp);
	  wordSize = 0;
	  i++;
	  continue;  
	  }
      i++;
    }
  
  if(isSimpleCommand(block))
    {
      printf("thats a simple command\n");
	    current->simpleCommand = malloc((wordSize)*sizeof(char));
	    strncpy(current->simpleCommand,temp,wordSize);
	    current->lineNum = *lineNum;

	    current->flag = SIMPLE;
	    current->next = checked_malloc(sizeof(struct node));
	    current = current->next;
	    //memset(temp,0,wordSize);
	    free(temp);
	    wordSize = 0;
    }

current = NULL;
  
  return;
}

void treeMaker(node_t start, node_t end, command_node_t c_command)
{
  while(start!=end)
    {
      switch(start->flag)
	{
	case IF:
	  {
	    c_command->command->type=IF_COMMAND;
	    int ifCount = 0;

	  }
	case WHILE:
	      {

	      }
	case UNTIL:
	    {

	    }
	case GREATER_THAN:
	    {

	    }
	case LESS_THAN:
	    {

	    }
	case PIPE:
	    {

	    }
	case SIMPLE_COMMAND:
	    {

	    }
	  }
	  start = start->next;
    }

  
}

command_stream_t
make_command_stream (int (*get_next_byte) (void *),
		     void *get_next_byte_argument)
{
  //Get the command stream linked list started
  command_stream_t stream = checked_malloc(sizeof(struct command_stream));
  stream->commands = checked_malloc(sizeof(struct command_node));
  command_node_t current = stream->commands;
  //Get the token list started
  node_t head = calloc(1,sizeof(struct node));
  node_t current_node = head;
  
   int current_byte;
  char* buffer = (char*)checked_malloc(sizeof(char));
  char current_char;
  int line = 1;
  int i=1;
  
  //Load the entire script into a buffer
  while((current_byte = get_next_byte(get_next_byte_argument))!= EOF){
    current_char = current_byte;
    buffer = (char*)realloc(buffer,i*sizeof(char));
    buffer[i-1]=current_char;
    i++;
  }
  char* buffer_block;
  int start = 0;
  int size = i;
  int type = 0;
  char** commands;
  char* test;

  while(1) 
  {
    //  printf("%i",head);
    //grab a block
      buffer_block = parseIntoBlocks(buffer,&start,&size);
      if(buffer_block == '\0')
      	break;
      line++;
      printf("buffer block is: ");
      printf(buffer_block);
      printf("\n");
      tokenizeBlock(buffer_block,&line,current_node);
      printf("out of that function\n");
      current_node = head;
      while(current_node!=NULL)
	{
	  printf("%i",current_node->flag);printf("\n");
	  current_node = current_node->next;
	  }
      //Free that mothafuckin list
      current_node = head;
      node_t temp;
      /*while(current_node!=NULL)
	{
	  printf("in here\n");
	  temp = current_node;
	  current_node = current_node->next;
	  free(temp);
	  }*/
      head = calloc(1,sizeof(struct node));
      current_node = head;
    } 

   return 0;
}

command_t
read_command_stream (command_stream_t s)
{
 
  return 0;
}

