// l2u.c
// converts lowercase letters to uppercase letters

#include "util.c"

int main (int argc, char * argv[])
{
  int theFD, bytesRead;
  char buffer[1024];

  printf ("\t\t\t~~~ Marcus' l2u called ~~~%c", '\n');

  if (argc == 2)
  {
    theFD = open (argv[1], O_RDONLY);
  } // end if there is an argument

  if (argc == 1)
  {
    theFD = dup (0);
  } // end if no arg, read from stdin

  bytesRead = read (theFD, buffer, 1);

    // for each character in the file, if it's
    // ascii code is between 97 and 122,
    // it is lowercase, convert to uppercase
    // by subtracting 32:

  while (buffer[0] != 0 && bytesRead > 0)
  {
    if (buffer[0] == '\r')
    {
      mputc ('\n');
    } // end if carriage return
    else
    {
      if (buffer[0] - 'a' >= 0 && buffer[0] -'a' <= 25)
      {
        mputc (buffer[0] - 32);
      } // end convert to uppercase
      else
      {
        mputc (buffer[0]);
      } // end put the un-changed character

    } // end if not carriage return

    buffer[0] = 0;
    bytesRead = read (theFD, buffer, 1);

  } // end convert lower to upper

  close (theFD);

  return 1;

} // end main
