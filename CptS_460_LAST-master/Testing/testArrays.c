#include <stdio.h>
#include <string.h>

//char buf[128] = {"marcus:life:1:1:other"};
//char *tokens[8];

void tokenize (char *buf, char *tokens[])
{
  int i = 0, j = 0, k = 0;
  char temp[16][128];
  char *word;
  word = temp[k];

  while (buf[i] != 0)
  {
    if (buf[i] == ':')
    {
      *word = 0;
      word = temp[k++];
      tokens[j] = word;
      printf ("j: %d\n", j);

      word = temp[k];

      j++;
      i++;
    } // end if colon
    else
    {
      *word++ = buf[i++];

    } // end else, add to word

  } // end read to end of buf

  *word = 0;
  word = temp[k++];
  tokens[j] = word;
  tokens[++j] = 0;
  word = temp[k];

  i = 0;
  while (tokens[i] != 0)
  {
    printf ("xtokens[%d]: %s\n",i , tokens[i]);
    i++;
  } // end print words

} // end tokenize function

int main ()
{
  int i, j;
  char test1[8][8];
  char *test2;
  char test3[8] = {"stuff"};
  char test4[8];
  char *test5;
  char test6[8];
  int a, b;
  char *c = "1", *d = "2";
  char buf[128] = {"marcus:life:1:1:other"};
  char *tokens[8];

  printf ("c: %s - d: %s\n", c, d);
  a = ((int) c[0]) - 48;
  b = (int) d[0] - 48;
  printf ("a: %d - b: %d\n", a, b);

  test5 = test6;
  *test5++ = 'a';
  *test5++ = 'a';
  *test5++ = 'a';
  *test5 = 0;
  test5 = test6;
  printf ("test5: %s\n", test5);
  *test5++ = 'b';
  *test5++ = 'b';
  *test5++ = 'b';
  *test5 = 0;
  test5 = test6;
  printf ("test5: %s\n", test5);

  //test2[0] = "text";

  test2 = test3;

  //strcpy (test2, test3);
  printf ("test2: %s\n", test2);
  strcpy (test4,test3);
  printf ("test4: %s\n", test4);

  test1[0][0] = 'o';
  test1[0][1] = 'n';
  test1[0][2] = 'e';
  test1[0][3] = 0;
  test1[1][0] = 't';
  test1[1][1] = 'w';
  test1[1][2] = 'o';
  test1[1][3] = 0;
  test1[2][0] = 't';
  test1[2][1] = 'h';
  test1[2][2] = 'r';
  test1[2][3] = 'e';
  test1[2][4] = 'e';
  test1[2][5] = 0;

  for (i = 0; i < 3; i++)
  {
    j = 0;

    while (test1[i][j] != 0)
    {
      printf ("%c", test1[i][j]);
      j++;
    } // end print each word
    printf ("\n");

  } // end print each line

  tokenize (buf, tokens);

  i = 0;
  printf (" - tokens - \n");
  while (tokens[i] != 0)
  {
    printf ("tokens[%d]: %s\n",i , tokens[i]);
    i++;
  } // end print words

} // end main
