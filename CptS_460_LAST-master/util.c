/*
  util.c
*/
// I started writing this because I thought we were supposed to,
// Since many of my functions already depended on this tokenize function,
// and this mybzero function, I have left it in 

#include "uart.c"
#include "ucode.c"

#define BLKSIZE 1024

  // function mybzero:
  // clears the buffer:

int mybzero (char buf[], int size)
{
  int i = 0;

  for (i = 0; i < size; i++)
  {
    buf[i] = 0;
  } // end for loop to empty buffer

} // end mybzero function

  // get_block function

int get_block(int fd, int blk, char *buf)
{
  //lseek(fd, (long)blk*BLKSIZE);
  lseek(fd, (long)blk*BLKSIZE, 0);
  return read(fd, buf, BLKSIZE);
} // end function get_block

  // tokenize function:
  // requires:
  // char buf[1024];
  // char *tokens[16];

void tokenize (char *buf, char * tokens[], char delimiter)
{
  int i = 0, j = 0, k = 0;
  char temp[16][128];
  char *word;
  word = temp[k];

  while (buf[i] != '\n' && buf[i] != 0)
  {
    if (buf[i] == delimiter)
    {
      *word = 0;          // close out word
      word = temp[k++];   // reset pointer to beginning of the temp array position and increment temp array position
      tokens[j] = word;   // store the word in the pass-by-reference tokens array
      word = temp[k];     // set word to the beginning of the next slot in the temp array

      j++;
      i++;
    } // end if delimiter
    else
    {
        // add the character from the buffer to the word
      *word++ = buf[i++];

    } // end else, add to word

  } // end read to end of buf

  *word = 0;          // close out word
  word = temp[k++];   // reset pointer to beginning of the temp array position and increment temp array position
  tokens[j] = word;   // store the word in the pass-by-reference tokens array
  tokens[j + 1] = 0;  // close out the tokens array
  word = temp[k];     // set word to the beginning of the next slot in the temp array

} // end tokenize function
