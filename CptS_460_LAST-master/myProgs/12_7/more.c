#include "util.c"

int theFD, theErr;

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
    
  } // end while loop

  return bytesRead;

} // end printScreen function

// printScreenSTDIN function:

int printScreenSTDIN ()
{
  int i = 0, bytesRead;
  char readChar;

  bytesRead = read (theFD, &readChar, 1);

  while (i < 24 && bytesRead > 0)
  {
    if (readChar == '\r')
    {
      mputc ('\n');
      //printf ("%c", '\n');
      i++;

    } // end add new line and carriage return
    else
    {
      mputc (readChar);
      //print2f (&readChar);
    } // end if not CR

    bytesRead = read (theFD, &readChar, 1);

  } // end while loop

} // end printScreenSTDIN function

// printLineSTDIN function:

int printLineSTDIN ()
{
  int i = 0, bytesRead;
  char readChar;

  bytesRead = read (theFD, &readChar, 1);

  while (readChar != '\n' && bytesRead > 0)
  {
    mputc (readChar);
    bytesRead = read (theFD, &readChar, 1);
  }

  mputc ('\n');

  return 0;

} // end printLineSTDIN function

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
    theFD = open (argv[1], O_RDONLY);

      // error checking:

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
          // print characters one at a time to handle
          // carriage returns properly:

        while (buffer[0] != '\n' && bytesRead > 0)
        {
          mputc (buffer[0]);
          buffer[0] = 0;
          bytesRead = read (theFD, buffer, 1);
        }
        mputc ('\n');

        bytesRead = read (theFD, buffer, 1);

      } // end user pushes a different key

    } // end while loop

  } // end if there is an argument
  else
  {
    theFD = dup(0);

      // error checking

    if (theFD < 0)
    {
      printf ("cannot open file %s\n", argv[1]);
      return 0;
    } // end if open fails

    printScreenSTDIN ();

    userInput = getc ();
    if (userInput == ' ')
    {
      // print a screen from stdin
      printScreenSTDIN ();
    } // end print one screen
    else
    {
      // print a line from stdin
      printLineSTDIN ();
    } // end print one line

  } // end there is no argument

  close (theFD);

  return 0;

} // end main function
