#include "util.c"

int theFD;

  // print a screen of information:

int printScreen ()
{
  int bytesRead, i = 0, j = 0;
  int bufSize = 80;
  int crCount = 0;
  char buffer[bufSize];

  //bytesRead = read (theFD, buffer, bufSize);
  bytesRead = read (theFD, buffer, 1);
  //buffer[bytesRead] = 0;
  while (i < 24 && bytesRead > 0)
  {
    if (buffer[0] == '\n')
    {
      mputc ('\n');
      i++;

    } // end if we encounter a CR
    else
    {
      mputc (buffer[0]);
    } // end if not CR, print character

    buffer[0] = 0;
    bytesRead = read (theFD, buffer, 1);
    /*
      // print characters one at a time to handle
      // carriage returns properly:

    for (j = 0; j < 80; j++)
    {
      if (buffer[j] == '\r')
      {
        //printf ("%c", '\n');
        mputc ('\n');
      } // end if carriage return, print appropriately
      else
      {
        //printf ("%c", buffer[j]);
        mputc (buffer[j]);
      } // end if not carriage return print what we see
    }// end print characters
    //printf ("%s", buffer);
    mybzero (buffer, bufSize);

    bytesRead = read (theFD, buffer, bufSize);
    i++;
    */

  } // end while loop

  return bytesRead;

} // end printScreen function

  // main function

int main (int argc, char * argv[])
{
  int bytesRead, lineNum, i = 0, j = 0;
  int bufSize = 80;
  char buffer[bufSize], userInput;
  char readChar;

  printf ("\t\t\t~~~ my more called ~~~%c", '\n');

  if (argc > 1)
  {
    theFD = open (argv[1], 0);

    if (theFD < 0)
    {
      printf ("cannot open file %s\n", argv[1]);
      return 0;
    } // end if open fails

    printScreen ();

    bytesRead = read (theFD, buffer, 1);

    while (bytesRead > 0)
    {
      userInput = getc ();

      if (userInput == ' ')
      {
        bytesRead = printScreen ();
      } // end if user presses spacebar
      else
      {
        // If i print the whole buffer, it will encounter \r's and
        // they don't print as CR, they print as some odd character

        //printf ("%s", buffer);
        /////
        // print characters one at a time to handle
        // carriage returns properly:

      while (buffer[0] != '\n' && bytesRead > 0)
      {
        mputc (buffer[0]);
        buffer[0] = 0;
        bytesRead = read (theFD, buffer, 1);
      }
      mputc ('\n');
      /*
      for (j = 0; j < 80; j++)
      {
        if (buffer[j] == '\r')
        {
          //printf ("%c", '\n');
          mputc ('\n');
        } // end if carriage return, print appropriately
        else
        {
          //printf ("%c", buffer[j]);
          mputc (buffer[j]);
        } // end if not carriage return print what we see
      }// end print characters
        /////
        mybzero (buffer, bufSize);
        */
        bytesRead = read (theFD, buffer, 1);

      } // end user pushes a different key

    } // end while loop

  } // end if there is an argument
  else
  {
    theFD = dup(0);
    if (theFD < 0)
    {
      printf ("cannot open file %s\n", argv[1]);
      return 0;
    } // end if open fails
    while (read (theFD, &readChar, 1) > 0)
    {
      if (readChar == '\r')
      {
        printf ("%c\n", ' ');

      } // end add new line and carriage return
      else
      {
        mputc (readChar);
      } // end if not CR

    } // end while loop

  } // end there is no argument



  close (theFD);

  return 0;

} // end main function
