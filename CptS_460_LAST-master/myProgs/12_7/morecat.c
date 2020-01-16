#include "util.c"

// make file descriptor global to use inside function:

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

  } // end while loop

  return bytesRead;

} // end printScreen function

int main (int argc, char * argv[])
{
  //int theFD, n = 0, i = 0;
  int n = 0, i = 0;
  int bufSize = 80, bytesRead;
  char buffer[bufSize]; // max size of all direct blocks
  char readChar; // for stdin
  int theWriteFD;
  char myBuf[1];
  char userInput;

  printf ("\t\t\t~~~ my morecat ~~~%c\n", ' ');

    // if there is no argument, read from stdin:

  if (argc == 1)
  {
    theFD = dup (0);

    if (theFD < 0)
    {
      printf ("cannot open file %s\n", argv[1]);
      return 0;
    } // end if open fails

    /*
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
    */
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

  } // end there is no argument

      // if there is an argument, it must be a file to cat :

  if (argc == 2)
  {
      theFD = open (argv[1], 0);

      if (theFD < 0)
      {
        printf ("cannot open file %s\n", argv[1]);
        exit (0);
      } // end if file fails to open
      else
      {
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
        
      } // end file opened successfully

      close (theFD);

  } // end if there is an argument,cat a file

    // if there is a re-direction from stdin:

  if (argc == 3)
  {
    if (strcmp(argv[1], ">") == 0)
    {
        // duplicate the file descriptor:
      theFD = dup (0);
        // create the file:
      //creat (argv[2]);
        // open the file for writing:
      theWriteFD = open (argv[2], O_WRONLY | O_CREAT);

        // read one char at a time to properly handle carriage returns:

      while (read (theFD, &readChar, 1) > 0)
      {
        if (readChar == '\r')
        {
          //printf ("%c", '\n');
          mputc ('\n');
          write (theWriteFD, "\n", 1);
        } // end add new line and carriage return
        else
        {
          mputc (readChar);
          buffer[0] = readChar;
          write (theWriteFD, buffer, 1);
          buffer[0] = 0;
        } // end if not CR

      } // end while loop
      close (theWriteFD);
      close (theFD);

    } // end redirection from stdin

      // if redirection from file:

    if (strcmp(argv[1], "<") == 0)
    {
        // open the file for read:

      theFD = open (argv[2], O_RDONLY);

        // read one char at a time to properly handle carriage returns:

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

      close (theFD);

    } // end redirection in from file

    if (strcmp(argv[1], ">>") == 0)
    {
      theFD = dup (0);
      theWriteFD = open (argv[2], O_RDWR | O_APPEND);


      // read until end of file:
      bytesRead = read (theWriteFD, buffer, bufSize);
      //buffer[bytesRead] = 0;
      while (bytesRead > 0)
      {
        printf ("%s", buffer);
        mybzero (buffer, bufSize);

        bytesRead = read (theWriteFD, buffer, bufSize);

      } // end while loop


      while (read (theFD, &readChar, 1) > 0)
      {
        if (readChar == '\r')
        {
          //printf ("%c", '\n');
          mputc ('\n');
          write (theWriteFD, "\n", 1);
        } // end add new line and carriage return
        else
        {
          mputc (readChar);
          buffer[0] = readChar;
          write (theWriteFD, buffer, 1);
          buffer[0] = 0;
        } // end if not CR

      } // end while loop
      close (theWriteFD);
      close (theFD);

    } // end redirection, append  from stdin

  } // end if 3 arguments

  /////
  if (argc == 4)
  {
    if (strcmp(argv[2], ">") == 0)
    {
      theFD = open (argv[1], 0);
      creat (argv[3]);
      theWriteFD = open (argv[3], 1);

        // read until end of file:

      bytesRead = read (theFD, buffer, 1);

      while (bytesRead > 0)
      {
        write (theWriteFD, buffer, 1);
        mybzero (buffer, 1);
        bytesRead = read (theFD, buffer, 1);

      } // end while loop

      close (theWriteFD);
      close (theFD);

    } // end redirection from file

    if (strcmp(argv[2], ">>") == 0)
    {
      theFD = open (argv[1], 0);
      //creat (argv[3]);
      theWriteFD = open (argv[3], O_RDWR | O_APPEND);

      // move to the end of the writefile:
      bytesRead = read (theWriteFD, buffer, 1);

      while (bytesRead > 0)
      {
        bytesRead = read (theWriteFD, buffer, 1);
        mybzero (buffer, 1);
      } // end move to end of write file

        // now, write the contents of the first file to
        // the end of the second file:

        // read until end of file:

      bytesRead = read (theFD, buffer, 1);

      while (bytesRead > 0)
      {
        write (theWriteFD, buffer, 1);
        mybzero (buffer, 1);
        bytesRead = read (theFD, buffer, 1);

      } // end while loop

      close (theWriteFD);
      close (theFD);

    } // end redirection from file

  } // end if 4 arguments
  /////

  printf ("\t\t\t~~~ end my morecat ~~~\n%c", ' ');

  return 0;

} // end main function
