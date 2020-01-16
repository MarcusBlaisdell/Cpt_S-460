#include <stdio.h>
#include <string.h>

//char buf[128] = {"marcus:life:1:1:other"};
//char *tokens[8];

char * tokenize (char *buf, char delimiter)
{
  static char *mybuf;
  static int * len;
  char * word = malloc (128);
  char tempWord[128];
  static int i = 0;
  int j, k = 0;

  if (buf != NULL)
  {
    mybuf = buf;
    len = strlen(mybuf);
  }

  if (i == len)
  {
    return NULL;
  } // end if at end, return null

  if (i > 0)
  {
    i++;
  } // end if i is not zero, increment to move past delimiter

  for (j = i; j < len; j++)
  {
    if (mybuf[j] == delimiter)
    {
      tempWord[k] = 0;
      strcpy (word, tempWord);
      return word;

    } // end if delimiter, return word
    else
    {
      tempWord[k] = mybuf[j];
      k++;
    } // end else
    i++;
  } // end for loop
  // if we get here, we are at the end of the buffer, close word and return:
  tempWord[k] = 0;
  strcpy (word, tempWord);
  return word;

} // end tokenize function


int main ()
{
  int i, j;
  char buf[128] = {"marcus:life:1:1:other"};
  char *token;
  //tokens[0] = 0;

  //tokenize ();
  token = tokenize (buf, ':');

  while (token != NULL)
  {
    printf ("token: %s\n", token);
    token = tokenize (NULL, ':');
  }

} // end main
