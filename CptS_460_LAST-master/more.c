// more.c
// prints one screen of data from a file, or stdin,
// then promts user for input,
// if user enter's <space>, prints another full screen,
// if user enter's <enter>, prints one line of data
// redirects are handled inside this program

#include "util.c"

int theFD, theErr;

  // print a screen of information:

int printScreen ()
{
  int bytesRead, i = 0, j = 0;
  int bufSize = 80;
  int crCount = 0;
  char buffer[bufSize];

  bytesRead = read (theFD, buffer, 1);

    // print 24 lines:

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

  // printLine function:

int printLine ()
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

} // end printLine function

  // main function

int main (int argc, char * argv[])
{
  int bytesRead, lineNum, i = 0, j = 0;
  int bufSize = 80;
  char buffer[bufSize], userInput;
  char readChar;

  printf ("\t\t\t~~~ Marcus' more called ~~~%c", '\n');

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

        // if user enters <space>, print one screen:

      if (userInput == ' ')
      {
        bytesRead = printScreen ();
      } // end if user presses spacebar

        // if user enters <enter>, print one line:

      if (userInput == '\r')
      {
        printLine ();

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

    printScreen ();

    userInput = getc ();
    if (userInput == ' ')
    {
      printScreen ();
    } // end print one screen
    else
    {
      // print a line from stdin
      printLine ();
    } // end print one line

  } // end there is no argument

  close (theFD);

  return 0;

} // end main function
