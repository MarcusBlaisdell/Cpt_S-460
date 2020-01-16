#include <stdio.h>
#include <string.h>

char * testFunc ()
{
  char * word1 = malloc (4);
  char word2[4];

  word2[0] = 'a';
  word2[1] = 0;
  strcpy (word1, word2);

  return word1;
}

int testRef (char ** head, char ** tail, char *command)
{

  char *pointer = command;


    // move pointer to the end of the command:
  while (*pointer)
  {
    *pointer++;
  } // end move to end of command

  while (*pointer != '|' && *pointer != *command)
  {
    *pointer--;
  } // end look for last pipe

  if (*pointer == '|')
  {
    *pointer = 0;
    *head = command;
    *pointer++;
    *pointer++;
    *tail = pointer;
  }

  printf ("infunc: head: %s - tail: %s\n", *head, *tail);

  return 1;
} // end testRef

// testinter:

void testinter (char *command)
{
  char *head = "", *tail = "";

  testRef (&head, &tail, command);
  //printf ("head: %s: tail: %s\n", *head, *tail);
}

int testStatic ()
{
  static int i = 0;
  i++;
  return i;
}

int main ()
{
  char * word1;
  char * test1;
  char test2[16] = {"hello world"};
  char command[32] = {"cat f1 | more"};
  char filename[1024], cwd[1024] = {"/user/marcus"};

  strcpy (filename, cwd);
  printf ("filename: %s\n", filename);

  testinter (command);

  //strcpy (test1, test2);
  test1 = test2;
  printf ("test1: %s - strlen(test1) = %d\n", test1, strlen(test1) );

  word1 = testFunc ();

  printf ("word1 %s\n", word1);

  printf ("print Static: %d\n", testStatic ());
  printf ("print Static: %d\n", testStatic ());
  printf ("print Static: %d\n", testStatic ());
}
