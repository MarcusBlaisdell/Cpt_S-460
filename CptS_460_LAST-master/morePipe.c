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

// read from stdin

int readSTDIN ()
{
  char userInput;

  printf ("Enter a character: ");

  userInput = getc ();

  printf ("you entered: %c\n", userInput);

} // end readSTDIN function

  // main function

int main (int argc, char * argv[])
{
  int bytesRead, lineNum, i = 0, j = 0;
  int bufSize = 80;
  char buffer[bufSize], userInput;
  char readChar;
  int pid, pd[2];

  printf ("\t\t\t~~~ my morePipe called ~~~%c", '\n');

    // morePipe is only called by do_pipe from sh
    // arg2 is the pipe
    // we want to print the initial full screen, then switch the input
    // to another process to handle the user input
    // and then switch back to the original to get the next line/page

    // arg,[2] = *int (int[2])

    printScreen ();

    // close writer:
    //close (argv[2][1]);
    // create a new pipe:
    pipe (pd);
    // fork a new process:
    pid = fork ();

    if (pid)
    {
      close(pd[0]);
      dup2 (pd[1], 1);

      userInput = getc ();

      if (userInput == ' ')
      {
        close(pd[1]);
        dup2 (argv[2][1], 1);
        exec ("morePipe(argv[2])" );
      } // end if new print screen called
      else
      {
        while (userInput != ' ')
        {
          close(pd[0]);
          dup2 (argv[2][1], 1);

          while (buffer[0] != '\n' && bytesRead > 0)
          {
            mputc (buffer[0]);
            buffer[0] = 0;
            bytesRead = read (theFD, buffer, 1);
          }
          mputc ('\n');

          close(pd[1]);
          dup2 (pd[1], 1);

          userInput = getc ();

        } // end loop for line-at-a-time

      } // end else, not space

      exec ("morePipe(argv[2])" );

    } // end if pid is valid
    else
    {
      printf ("else%c", '\n');
    }

  close (theFD);

  return 0;

} // end main function
