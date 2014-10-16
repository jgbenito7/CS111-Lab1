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
  if(c == '!' || c == '%' || c == '+' || c == ',' || c == '-' || c == '.' || c == '/' || c == '@' || c == '^' || c == '_' || c == ' ')
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
      //printf("%c",str[x]);
      //printf("\n");
      if(c[x+1] == '\n' && c[x+2] == '\n')
	{
	  blockSize++;
	  str = (char*)realloc(str, blockSize*sizeof(char));
	  str[blockSize-1] = '\0';

	  blockSize+=1;
	  *start = *start + blockSize;
	  return str;
	}
    }
  return '\0'; 
}

char* findIfBlock(char* c, int k)
{
  int subSize = 0;
  char* sub_block = malloc(sizeof(char));
  char* error_message = "Check you if syntax";
  int ifCount = 1;
  while(c[k]!='\0')
	    {
	      subSize++;
	      sub_block = (char*)realloc(sub_block,subSize*sizeof(char));
	      sub_block[subSize-1] = c[k];
	      if(c[k] == 'i' && c[k+1] == 'f' && (c[k+2] == ' ' || c[k+2] == '\n'))
		 {
		   ifCount++;
		 }
	      if(((c[k] == ' ') || (c[k] == '\n')) && c[k+1] == 'f' && c[k+2] == 'i')
		 {
		   ifCount--;
		   if(ifCount == 0)
		     {
		       return sub_block;
		     }
		   
		 }
	      k++;
	    }
  return error_message;
}
char* findWhileBlock(char* c, int k)
{
  int subSize = 0;
  char* sub_block = malloc(sizeof(char));
  char* error_message = "you fucked up. Quit CS right now";
  int whileCount = 1;
  while(c[k]!='\0')
	    {
	      subSize++;
	      sub_block = (char*)realloc(sub_block,subSize*sizeof(char));
	      sub_block[subSize-1] = c[k];
	      if(c[k] == 'w' && c[k+1] == 'h' && c[k+2] == 'i' && c[k+3] == 'l' && c[k+4] == 'e' && (c[k+5] == ' ' || c[k+5] == '\n'))
		 {
		   whileCount++;
		 }
	      if(((c[k] == ' ') || (c[k] == '\n')) && c[k+1] == 'd' && c[k+2] == 'o' && c[k+3] == 'n' && c[k+4] == 'e' && (c[k+5] == '\0' || c[k+5] == '\n'))
		 {
		   whileCount--;
		   if(whileCount == 0)
		     {
		       printf("Here is the motherfucking sub block: ");
		       printf(sub_block);
		       printf("\n");
		       return sub_block;
		     }
		   
		 }
	      k++;
	    }
  return error_message;
}

char* findUntilBlock(char* c, int k)
{
  int subSize = 0;
  char* sub_block = malloc(sizeof(char));
  char* error_message = "you fucked up. Quit CS right now";
  int untilCount = 1;
  while(c[k]!='\0')
	    {
	      subSize++;
	      sub_block = (char*)realloc(sub_block,subSize*sizeof(char));
	      sub_block[subSize-1] = c[k];
	      if(c[k] == 'u' && c[k+1] == 'n' && c[k+2] == 't' && c[k+3] == 'i' && c[k+4] == 'l' && (c[k+5] == ' ' || c[k+5] == '\n'))
		 {
		   untilCount++;
		 }
	      if(((c[k] == ' ') || (c[k] == '\n')) && c[k+1] == 'd' && c[k+2] == 'o' && c[k+3] == 'n' && c[k+4] == 'e')
		 {
		   untilCount--;
		   if(untilCount == 0)
		     {
		       return sub_block;
		     }
		   
		 }
	      k++;
	    }
  return error_message;
}
char* commands[3];
char** parseBlock(char* c,int* type)
{
  //printf(c);
  int wordSize = 0;
  char* buffer_block = malloc(sizeof(char));
  int x = 0;
  char* sub_block;
  while(c[x]!='\0')
    {	    
	  
      wordSize++;
	  buffer_block = (char*)realloc(buffer_block,wordSize*sizeof(char));
	  buffer_block[wordSize-1] = c[x];

      
      if((strncmp(buffer_block,"if",2)==0))
	{
	  *type = IF_COMMAND;
	  int ifCount = 1;
	  int whichCommand = 0;
	  int k = x+2;
	  

	  sub_block = findIfBlock(c,k);
	  int j = 0;
	  char* temp = malloc(sizeof(char));
	  int tempSize = 0;
	  ifCount = 0;
	  while(sub_block[j]!='\0')
	    {
	      tempSize++;
	      temp = (char*)realloc(temp,tempSize*sizeof(char));
	      temp[tempSize-1] = sub_block[j];
	      if(sub_block[j] == 'i' && sub_block[j+1] == 'f' && (sub_block[j+2] == ' ' || sub_block[j+2] == '\n'))
		 {
		   //printf("found if\n");
		   ifCount++;
		 }
	      if(((sub_block[j] == ' ') || (sub_block[j] == '\n')) && sub_block[j+1] == 'f' && sub_block[j+2] == 'i')
		 {

		   ifCount--;
		   if(ifCount == 0)
		     {
		       tempSize++;
		       temp = (char*)realloc(temp,tempSize*sizeof(char));
		       temp[tempSize-1] = sub_block[j+1];
		       tempSize++;
		       temp = (char*)realloc(temp,tempSize*sizeof(char));
		       temp[tempSize-1] = sub_block[j+2];
		       
		       j+=3;
		       int index = 0;
		       commands[whichCommand] = malloc(tempSize*sizeof(char));
		       for(index=0;index<tempSize;index++)
			 {
			   commands[whichCommand][index] = temp[index];
			 }
		       
		       if(whichCommand == 0)
			 {
			   printf("A: ");
			   printf(commands[whichCommand]);
		       printf("\n");
		       tempSize = 0;
		       free(temp);
		       temp = malloc(sizeof(char));
			   whichCommand++;
			   if((((sub_block[j] == ' ') || (sub_block[j] == '\n')) && sub_block[j+1] == 't' && sub_block[j+2] == 'h' && sub_block[j+3] == 'e' && sub_block[j+4] == 'n') && ((sub_block[j+5] == ' ') || (sub_block[j+5] == '\n')))
			     {
			       j=j+4;
			     }
		       

			 }
		       else if(whichCommand == 1)
			 {
			   
			   printf("B: ");
			   printf(commands[whichCommand]);
			   printf("\n");
		       tempSize = 0;
		       free(temp);
		       temp = malloc(sizeof(char));
			   whichCommand++;
			   if((((sub_block[j] == ' ') || (sub_block[j] == '\n')) && sub_block[j+1] == 'e' && sub_block[j+2] == 'l' && sub_block[j+3] == 's' && sub_block[j+4] == 'e') && ((sub_block[j+5] == ' ') || (sub_block[j+5] == '\n')))
			     {
			       j=j+4;
			     }
			 }
		       else if(whichCommand == 2)
			 {
			   printf("C: ");
			   printf(commands[whichCommand]);
			   printf("\n");
		       tempSize = 0;
		       free(temp);
		       temp = malloc(sizeof(char));
			   whichCommand++;
			   if((((sub_block[j] == ' ') || (sub_block[j] == '\n')) && sub_block[j+1] == 'f' && sub_block[j+2] == 'i' && ((sub_block[j+3] == ' ') || (sub_block[j+3] == '\n'))))
			     {
			       j=j+2;
			     }
			 }
		     }
		   
		 }
	      if(whichCommand == 0)
			 {
	      if((((sub_block[j] == ' ') || (sub_block[j] == '\n')) && sub_block[j+1] == 't' && sub_block[j+2] == 'h' && sub_block[j+3] == 'e' && sub_block[j+4] == 'n') && ((sub_block[j+5] == ' ') || (sub_block[j+5] == '\n')) && ifCount==0)
		{
		  int index = 0;
		  commands[whichCommand] = malloc(tempSize*sizeof(char));
		  for(index=0;index<tempSize;index++)
		  {
		     commands[whichCommand][index] = temp[index];
		  }
		  printf("A: ");
		  printf(commands[whichCommand]);
		  printf("\n");
		  whichCommand++;
		  j=j+4;
		  tempSize = 0;
		  free(temp);
		  temp = malloc(sizeof(char));
		  //char* newString = malloc(sizeof(char));
		  //temp = newString;
		}
	      else
		{
		  if(sub_block[j+1] == '\0' && ifCount==0)
		    {
		      int index = 0;
		  commands[whichCommand] = malloc(tempSize*sizeof(char));
		  for(index=0;index<tempSize;index++)
		  {
		     commands[whichCommand][index] = temp[index];
		  }
		  printf("A: ");
		  printf(commands[whichCommand]);
		  printf("\n");
		  whichCommand++;
		  j=j+4;
		  tempSize = 0;
		  free(temp);
		  temp = malloc(sizeof(char));
		    }
		}
			 }
	      else if(whichCommand == 1)
			 {
	      if((((sub_block[j] == ' ') || (sub_block[j] == '\n')) && sub_block[j+1] == 'e' && sub_block[j+2] == 'l' && sub_block[j+3] == 's' && sub_block[j+4] == 'e') && ((sub_block[j+5] == ' ') || (sub_block[j+5] == '\n')))
			     {
			       
			       int index = 0;
		       commands[whichCommand] = malloc(tempSize*sizeof(char));
		       for(index=0;index<tempSize;index++)
			 {
			   commands[whichCommand][index] = temp[index];
			 }
			       printf("B: ");
			       printf(commands[whichCommand]);
			       printf("\n");
			       whichCommand++;
			       j=j+4;
			       tempSize = 0;
			      free(temp);
		       temp = malloc(sizeof(char));
			     }
	      else
		{
		  if(sub_block[j+1] == '\0' && ifCount==0)
		    {
		      int index = 0;
		      //printf("which command: ");printf("%i",tempSize);
		       commands[whichCommand] = malloc(tempSize*sizeof(char));
		       for(index=0;index<tempSize;index++)
			 {
			   commands[whichCommand][index] = temp[index];
			 }
			       
			       j=j+4;
			       tempSize = 0;
			      free(temp);
			      temp = malloc(sizeof(char));
			      printf("B: ");
			       printf(commands[whichCommand]);
			       printf("\n");
			       whichCommand++;
			       return commands;
		    }
		}

			 }
	      else if(whichCommand == 2)
			 {
	       
	if(sub_block[j+1]=='\0')
			     {		     
		       commands[whichCommand] = malloc(tempSize*sizeof(char));
		       
		       strncpy(commands[whichCommand],temp,tempSize);
			       
			       j=j+2;
			       memset(temp,"\0",tempSize);
			       tempSize = 0;
			       // free(temp);
			       // temp = malloc(sizeof(char));
			      
			      printf("C: ");
			      printf(commands[whichCommand]);
			       printf("\n");
			       whichCommand++;
			     }

			 }
	      j++;
	    }
	  return commands;
	  break;
	  
	  
        }
      //END OF THE IF BLOCK
      if((strncmp(buffer_block,"while",5)==0))
	{
	  *type = WHILE_COMMAND;
	  int whileCount = 1;
	  int until_count = 0;
	  int whichCommand = 0;
	  int k = x+2;
	  sub_block = malloc(sizeof(char));
	  sub_block = findWhileBlock(c,k);
	  int j = 0;
	  char* temp = malloc(sizeof(char));
	  int tempSize = 0;
	  whileCount = 0;
	  while(sub_block[j]!='\0')
	    {
	      tempSize++;
	      temp = (char*)realloc(temp,tempSize*sizeof(char));
	      temp[tempSize-1] = sub_block[j];
	      if(sub_block[j] == 'w' && sub_block[j+1] == 'h' && sub_block[j+2] == 'i' && sub_block[j+3] == 'l'&& sub_block[j+4] == 'e' && (sub_block[j+5] == ' ' || sub_block[j+5] == '\n'))
		 {
		   whileCount++;
		 }
	       if(sub_block[j] == 'u' && sub_block[j+1] == 'n' && sub_block[j+2] == 't' && sub_block[j+3] == 'i'&& sub_block[j+4] == 'l' && (sub_block[j+5] == ' ' || sub_block[j+5] == '\n'))
		 {
		   until_count++;
		 }
	      if(((sub_block[j] == ' ') || (sub_block[j] == '\n')) && sub_block[j+1] == 'd' && sub_block[j+2] == 'o' && sub_block[j+3] == 'n' && sub_block[j+4] == 'e')
		 {
		   if(until_count == 0)
		     { whileCount--;}
		   else
		     {
		       until_count--;
		     }
		   if(whileCount == 0)
		     {
		       tempSize++;
		       temp = (char*)realloc(temp,tempSize*sizeof(char));
		       temp[tempSize-1] = sub_block[j+1];
		       tempSize++;
		       temp = (char*)realloc(temp,tempSize*sizeof(char));
		       temp[tempSize-1] = sub_block[j+2];

		       j+=3;
		       int index = 0;
		       commands[whichCommand] = malloc(tempSize*sizeof(char));
		       for(index=0;index<tempSize;index++)
			 {
			   commands[whichCommand][index] = temp[index];
			 }
		       
		       if(whichCommand == 0)
			 {
			   printf("This is where you done fucked up");
			   printf("\n");
			   printf("A: ");
		       printf(commands[whichCommand]);
		       printf("\n");
		       tempSize = 0;
		       free(temp);
		       temp = malloc(sizeof(char));
			   whichCommand++;
			   if((((sub_block[j] == ' ') || (sub_block[j] == '\n')) && sub_block[j+1] == 'd' && sub_block[j+2] == 'o' && ((sub_block[j+3] == ' ') || (sub_block[j+3] == '\n'))))
			     {
			       
			       j=j+2;
			     }
		       

			 }
		       else if(whichCommand == 1)
			 {
			   
			   printf("B: ");
		       printf(commands[whichCommand]);
		       printf("\n");
		       tempSize = 0;
		       free(temp);
		       temp = malloc(sizeof(char));
			   whichCommand++;
			   if((((sub_block[j] == ' ') || (sub_block[j] == '\n')) && sub_block[j+1] == 'd' && sub_block[j+2] == 'o' && sub_block[j+3] == 'n' && sub_block[j+4] == 'e') && ((sub_block[j+5] == ' ') || (sub_block[j+5] == '\n')))
			     {
			       j=j+4;
			     }
			 }
		     }
		   
		 }
	      if(whichCommand == 0)
			 {
			   if((((sub_block[j] == ' ') || (sub_block[j] == '\n')) && sub_block[j+1] == 'd' && sub_block[j+2] == 'o'  && ((sub_block[j+3] == ' ') || (sub_block[j+3] == '\n')) && whileCount==0))
		{
		  int index = 0;
		  commands[whichCommand] = malloc(tempSize*sizeof(char));
		  for(index=0;index<tempSize;index++)
		  {
		     commands[whichCommand][index] = temp[index];
		  }
		  printf("A: ");
		  printf(commands[whichCommand]);
		  printf("\n");
		  whichCommand++;
		  j=j+2;
		  tempSize = 0;
		  free(temp);
		  temp = malloc(sizeof(char));
		}
			   else
			     {
			       if(sub_block[j+1] == '\0' && whileCount==0)
				 {
				   int index = 0;
				   commands[whichCommand] = malloc(tempSize*sizeof(char));
				   for(index=0;index<tempSize;index++)
				     {
				       commands[whichCommand][index] = temp[index];
				     }
				   printf("A: ");
				   printf(commands[whichCommand]);
				   printf("\n");
				   whichCommand++;
				   j=j+4;
				   tempSize = 0;
				   free(temp);
				   temp = malloc(sizeof(char));
				 }
			     }
			 }
	      else if(whichCommand == 1)
			 {
	      	if(sub_block[j+1]=='\0')
			     {
			       
			       int index = 0;
		       commands[whichCommand] = malloc(tempSize*sizeof(char));
		       for(index=0;index<tempSize;index++)
			 {
			   commands[whichCommand][index] = temp[index];
			 }
			       printf("B: ");
			       printf(commands[whichCommand]);
			       printf("\n");
			       whichCommand++;
			       j=j+4;
			       tempSize = 0;
			      free(temp);
		       temp = malloc(sizeof(char));
			     }

			 }
	      j++;
	      }
	  
	  break;
	  
	  
        }
      //END OF WHILE BLOCK
      
 if((strncmp(buffer_block,"until",5)==0))
	{
	  *type = UNTIL_COMMAND;
	  int untilCount = 1;
	  int whichCommand = 0;
	  int k = x+2;
	  sub_block = malloc(sizeof(char));

	  sub_block = findUntilBlock(c,k);
	  int j = 0;
	  char* temp = malloc(sizeof(char));
	  int tempSize = 0;
	  untilCount = 0;
	  while(sub_block[j]!='\0')
	    {
	      tempSize++;
	      temp = (char*)realloc(temp,tempSize*sizeof(char));
	      temp[tempSize-1] = sub_block[j];
	      if(sub_block[j] == 'u' && sub_block[j+1] == 'n' && sub_block[j+2] == 't' && sub_block[j+3] == 'i'&& sub_block[j+4] == 'l' && (sub_block[j+5] == ' ' || sub_block[j+5] == '\n'))
		 {
		   untilCount++;
		 }
	      if(((sub_block[j] == ' ') || (sub_block[j] == '\n')) && sub_block[j+1] == 'd' && sub_block[j+2] == 'o' && sub_block[j+3] == 'n' && sub_block[j+4] == 'e')
		 {

		   untilCount--;
		   if(untilCount == 0)
		     {
		       tempSize++;
		       temp = (char*)realloc(temp,tempSize*sizeof(char));
		       temp[tempSize-1] = sub_block[j+1];
		       tempSize++;
		       temp = (char*)realloc(temp,tempSize*sizeof(char));
		       temp[tempSize-1] = sub_block[j+2];
		       
		       j+=3;
		       int index = 0;
		       commands[whichCommand] = malloc(tempSize*sizeof(char));
		       for(index=0;index<tempSize;index++)
			 {
			   commands[whichCommand][index] = temp[index];
			 }
		       
		       if(whichCommand == 0)
			 {
			   printf("A: ");
		       printf(commands[whichCommand]);
		       printf("\n");
		       tempSize = 0;
		       free(temp);
		       temp = malloc(sizeof(char));
			   whichCommand++;
			   if((((sub_block[j] == ' ') || (sub_block[j] == '\n')) && sub_block[j+1] == 'd' && sub_block[j+2] == 'o' && ((sub_block[j+3] == ' ') || (sub_block[j+3] == '\n'))))
			     {
			       j=j+2;
			     }
		       

			 }
		       else if(whichCommand == 1)
			 {
			   
			   printf("B: ");
		       printf(commands[whichCommand]);
		       printf("\n");
		       tempSize = 0;
		       free(temp);
		       temp = malloc(sizeof(char));
			   whichCommand++;
			   if((((sub_block[j] == ' ') || (sub_block[j] == '\n')) && sub_block[j+1] == 'd' && sub_block[j+2] == 'o' && sub_block[j+3] == 'n' && sub_block[j+4] == 'e') && ((sub_block[j+5] == ' ') || (sub_block[j+5] == '\n')))
			     {
			       j=j+4;
			     }
			 }
		     }
		   
		 }
	      if(whichCommand == 0)
			 {
			   if((((sub_block[j] == ' ') || (sub_block[j] == '\n')) && sub_block[j+1] == 'd' && sub_block[j+2] == 'o'  && ((sub_block[j+3] == ' ') || (sub_block[j+3] == '\n')) && untilCount==0))
		{
		  int index = 0;
		  commands[whichCommand] = malloc(tempSize*sizeof(char));
		  for(index=0;index<tempSize;index++)
		  {
		     commands[whichCommand][index] = temp[index];
		  }
		  printf("A: ");
		  printf(commands[whichCommand]);
		  printf("\n");
		  whichCommand++;
		  j=j+2;
		  tempSize = 0;
		  free(temp);
		  temp = malloc(sizeof(char));
		}
			   else
			     {
			       if(sub_block[j+1] == '\0' && untilCount==0)
				 {
				   int index = 0;
				   commands[whichCommand] = malloc(tempSize*sizeof(char));
				   for(index=0;index<tempSize;index++)
				     {
				       commands[whichCommand][index] = temp[index];
				     }
				   printf("A: ");
				   printf(commands[whichCommand]);
				   printf("\n");
				   whichCommand++;
				   j=j+4;
				   tempSize = 0;
				   free(temp);
				   temp = malloc(sizeof(char));
				 }
			     }
			 }
	      else if(whichCommand == 1)
			 {
	      	if(sub_block[j+1]=='\0')
			     {
			       
			       int index = 0;
		       commands[whichCommand] = malloc(tempSize*sizeof(char));
		       for(index=0;index<tempSize;index++)
			 {
			   commands[whichCommand][index] = temp[index];
			 }
			       printf("B: ");
			       printf(commands[whichCommand]);
			       printf("\n");
			       whichCommand++;
			       j=j+4;
			       tempSize = 0;
			      free(temp);
		       temp = malloc(sizeof(char));
			     }

			 }
	      j++;
	      }
	  
	  break;
	  
	  
        }
 //END of UNTIL BLOCK
  if((strncmp(c,"(",1)==0))
	{
	  *type = SUBSHELL_COMMAND;
	  int untilCount = 1;
	  int j = 0;
	  char* temp = malloc(sizeof(char));
	  int tempSize = 0;
	  untilCount = 0;
	  while(c[j]!='\0')
	    {
	      if(isalphaNum(c[j]))
		{
	      tempSize++;
	      temp = (char*)realloc(temp,tempSize*sizeof(char));
	      temp[tempSize-1] = c[j];
		}
	      if(c[j+1]==')')
		{
		  int index = 0;
		   commands[0] = malloc(tempSize*sizeof(char));
		  for(index=0;index<tempSize;index++)
		  {
		     commands[0][index] = temp[index];
		  }
		  printf("A: "); printf(commands[0]);
		  return commands;
		}
	     
	      
	      j++;
	    }
	  printf("Something fucked up with the sub shell\n");
	  
	  break;
	  
	  
        }
  //END OF SUBSHELL_COMMAND
 
  if(buffer_block[x]=='<' || buffer_block[x]=='>')
	{
	  
	  *type = SEQUENCE_COMMAND;
	  int index = 0;
	  commands[0] = malloc((x-1)*sizeof(char));
	  for(index=0;index<x;index++)
	  {
	     commands[0][index] = buffer_block[index];
	  }
	  
	  int j = x+1;
	  char* temp = malloc(sizeof(char));
	  int tempSize = 0;
	  while(c[j]!='\0')
	    {
	      tempSize++;
	      temp = (char*)realloc(temp,tempSize*sizeof(char));
	      temp[tempSize-1] = c[j];
	       

	      j++;
	    }
	  
	  index = 0;
		   commands[1] = malloc(tempSize*sizeof(char));
		  for(index=0;index<tempSize;index++)
		  {
		     commands[1][index] = temp[index];
		  }		  
		  return commands;
	  break;
	  
	  
        }
  //END OF SEQUENCE COMMAND
  if(buffer_block[x]=='|')
	{
	  *type = PIPE_COMMAND;
	  int index = 0;
	  commands[0] = malloc((x-1)*sizeof(char));
	  for(index=0;index<x-1;index++)
	  {
	     commands[0][index] = buffer_block[index];
	  }
	  
	  int j = x+1;
	  char* temp = malloc(sizeof(char));
	  int tempSize = 0;
	  while(c[j]!='\0')
	    {
	      
	      tempSize++;
	      temp = (char*)realloc(temp,tempSize*sizeof(char));
	      temp[tempSize-1] = c[j];
	       

	      j++;
	    }
	  
	  index = 0;
		   commands[1] = malloc(tempSize*sizeof(char));
		  for(index=0;index<tempSize;index++)
		  {
		     commands[1][index] = temp[index];
		  }
		  //printf("A: "); printf(commands[0]);
		  //printf("\n");
		  //printf("B: "); printf(commands[1]);
		  return commands;
	  break;
	  
	  
        }
  //END OF PIPE COMMAND
      x++;
    }
  
   if(isSimpleCommand(c))
	{
	  *type = SIMPLE_COMMAND;
	  int index = 0;
	  commands[0] = malloc((x-1)*sizeof(char));
	  for(index=0;index<x;index++)
	  {
	     commands[0][index] = c[index];
	     
	  }
	  printf("A: "); printf(commands[0]);
	  printf("\n");
	}

  return commands;
  
}

void treeMaker(char* block, int* type, command_t c)
{
  
  c->type = *type;
  printf("in here\n");
  if(*type == SIMPLE_COMMAND)
    {
      strcpy(c->u.word,block);
      return;
    }
  char** strArray = parseBlock(block,type);
  treeMaker(strArray[0],type, c->u.command[0]);
  treeMaker(strArray[1],type,c->u.command[1]);
  treeMaker(strArray[2],type,c->u.command[2]);

}

command_stream_t
make_command_stream (int (*get_next_byte) (void *),
		     void *get_next_byte_argument)
{
  command_stream_t stream = checked_malloc(sizeof(struct command_stream));
  stream->commands = checked_malloc(sizeof(struct command_node));
  command_node_t current = stream->commands;

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
    
    //grab a block
      buffer_block = parseIntoBlocks(buffer,&start,&size);
      if(buffer_block == '\0')
      	break;
      int index = 0;
      test = malloc(sizeof(char));
      while(buffer_block[index]!='\0')
	  {
	    index++;
	    test = realloc(test,index*sizeof(char));
	    test[index-1] = buffer_block[index-1]; 
	  }
    printf("Buffer_Block: ");
    printf(test);
    printf("\n");
    printf("\n");
    parseBlock(test,&type);
    //Parse the block and store the values
    treeMaker(test,&type,current->command);
    // command_t test = current->command;
    current->next = checked_malloc(sizeof(struct command_node));
    current = current->next;

    
    /*printf("Commands 0: ");
    printf(commands[0]);
    printf("\n");
    printf("Commands 1: ");
    printf(commands[1]);
    printf("\n");
    printf("Commands 2: ");
    printf(commands[2]);
    printf("\n");*/

    
    
    //reset the commands
    //commands[0] = NULL;  
    //commands[1] = NULL; 
    //commands[2] = NULL; 
    free(test);
    } 
  

   return 0;
}

command_t
read_command_stream (command_stream_t s)
{
  return NULL;
}

