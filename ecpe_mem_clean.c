#define _GNU_SOURCE    // Required to use getline() function
#include <stdlib.h>
#include <stdio.h>

#include "main.h"

// In this program, a "document" is a linked list of objects.
// Each object is a "line_obj" structure with two items:
//  (1) A pointer to an array of characters, allocated separately
//      (This is the line of text)
//  (2) A pointer to the next linked-list item, or NULL if the end


// Example:
//
// ------------- 
// | Line*: ==========>"Chapter 1: A Long-Expected Party\n\0"
// | Next*: || |
// ---------||-- 
//          ||
//          \/
// ------------- 
// | Line*: ==========>"\n\0"
// | Next*: || |
// ---------||-- 
//          ||
//          \/
// ------------- 
// | Line*: ==========>"When Mr. Bilbo Baggins of Bag End .... etc.....\n\0"
// | Next*: || |
// ---------||-- 
//          ||
//          ...


int main()
{
  char filename[]="data.txt";
  char* line;           // Pointer to line of text
  FILE *fp;             // File pointer
  size_t length = 0;    // # of characters read in
  ssize_t read;         // Return value for getline()
  struct line_obj *document = NULL;  // Head of linked list

  fp = fopen(filename, "r");
  
  if(fp == NULL)
    {
      printf("Error: Unable to open input file\n");
      return 1;
    }

  // Read in the input file one line at a time using the
  // GNU project's getline() function
  // Tip: Read the manpage to learn what getline() does:
  //  unix>  man getline
  line = NULL; // Force getline() to allocate new buffer for line
  read = getline(&line, &length, fp);

  while(read != -1)
    {
      // Keep looping until getline() returns -1,
      // which indicates end-of-file

      // Add each line to a linked-list
      // of lines. "document" will always stay
      // pointing to the first line.
      document = line_append(document, line);

      // Get another line
      line = NULL; // Force getline() to allocate new buffer for line
      read = getline(&line, &length, fp);
    }
  free(line); // getline() allocates an empty buffer
              // at end-of-file, which is useless to us.

  // Close the file - done with input
  fclose(fp);

  // Print out the original document
  printf("Document read from file: \n");
  printf("-------------------------\n");
  document_print(document);
  printf("-------------------------\n");

  // Delete the linked-list
  // (calls free() for each "line_obj" structure in the list)
  document_delete(document);

  return 0;
}


// Append line to the end of a linked-list of lines
// Argument 1: Pointer to an existing linked-list
//   (or NULL, if no existing linked-list)
// Argument 2: Line to append to end of list
// Return: Pointer to the start of the modified linked-list
struct line_obj* line_append(struct line_obj* document, char* line)
{
  struct line_obj* ptr;

  // Allocate heap space for 1 additional line object
  // (which is a pointer to a line  + pointer to the next object).
  // Obviously, we'll want to free() this heap space later
  // on when the program is finished with it.
  struct line_obj* object = malloc(sizeof (struct line_obj)); 

  // Set member variables in the new object
  object->line = line;
  object->next = NULL;

  // Add the new object to the end of the existing list
  if(document == NULL)
    {
      // There is no existing list
      // Return a pointer to just this new object
      return object;
    }
  else
    {
      // There is an existing list.  Walk it till the end
      ptr = document;
      while(ptr->next != NULL)
	ptr = ptr->next;

      // ptr now is at the end of original string
      // Add the new object to the end
      ptr->next = object;
      // Return a pointer to the original head of the string

      return document;
    }
}

// Print out a document, line-by-line
// Argument 1: Pointer to start of linked list
void document_print(struct line_obj* document)
{
  while(document != NULL)
    {
      printf("%s", document->line);
      document = document->next;
    }

  return;
}


// Delete a document by free()ing the linked list
// Argument 1: Pointer to start of linked list
void document_delete(struct line_obj* object)
{
  struct line_obj* next_object;

  while(object != NULL)
    {
      next_object = object->next;
      free(object);
      object = next_object;
    }
free(next_object);
  return;
}