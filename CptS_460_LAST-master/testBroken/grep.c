// grep.c
// searches through a file, or stdin,
// one line at a time looking for a given pattern,
// if the pattern is found, it prints the full line
// it appears in,
// if the pattern is not found, it moves to the next line
// until all lines have been read
// redirects are handled inside this program

#include "util.c"

int main (int argc, char * argv[])
{
  int theFD, i = 0, j = 0;
  int theWriteFD;
  char * pattern;
  int patternSize;
  char line[1024];
  char buf[1024];
  char readChar;
  int bytesRead;
  char match[32];
  int matchFlag = 0;
  int redirectFlag = 0;
  char buffer[1024];
  int bufSize = 80;

    // check for redirects:
  for (i = 0; i < argc; i++)
  {
    if (strcmp (argv[i], "<") == 0)
    {
      redirectFlag = 1;
      theFD = open (argv[i + 1], O_RDONLY);
      break;
    } // end if there is a redirect in

    if (strcmp (argv[i], ">") == 0)
    {
      redirectFlag = 1;

      theWriteFD = open (argv[i + 1], O_WRONLY | O_CREAT);
      break;
    } // end if there is a redirect out

    if (strcmp (argv[i], ">>") == 0)
    {
      redirectFlag = 1;

      theWriteFD = open (argv[i + 1], O_RDWR | O_APPEND);

        // move to the end of the writefile:
      bytesRead = read (theWriteFD, buffer, 1);

      while (bytesRead > 0)
      {
        bytesRead = read (theWriteFD, buffer, 1);
        mybzero (buffer, 1);
      } // end move to end of write file

      break;
    } // end if there is a redirect append

  } // end check args for a redirect

    // arg1 is the pattern we want to match:
  pattern = argv[1];
  patternSize = strlen(pattern);

  printf ("\t\t\t~~~ Marcus' grep function ~~~%c", '\n');

    // if there is an arg2, search that file,
    // if there is no arg2, search from stdin:

  if (argc == 3 || argc == 5)
  {
    // arg2 is the file we are searching:
    theFD = open (argv[2], O_RDONLY);

      // read the file until we find that pattern:
      // we need to print the full line that contains the pattern
      // so read one full line at a time:

    bytesRead = read (theFD, &readChar, 1);

      // read until eof:

    while (bytesRead > 0)
    {
        // read until eol:

      while (readChar != '\n')
      {
        line[i] = readChar;
        bytesRead = read (theFD, &readChar, 1);
        i++;

      } // end read until eol

        // close the line:

      line[i] = 0;

      // now we have one full line, look for our pattern
      // within it, the pattern could appear anywhere so
      // we need to use a rolling window:
      // pattern = "test"
      // line = "this is xtest12 of the file"
      // sliding window of pattern.size():
      // line = "[this] is xtest12 of the file"
      // line = "t[his ]is xtest12 of the file"
      // line = "th[is i]s xtest12 of the file"
      // line = "thi[s is] xtest12 of the file"
      // line = "this[ is ]xtest12 of the file"
      // line = "this [is x]test12 of the file"
      // line = "this i[s xt]est12 of the file"
      // line = "this is[ xte]st12 of the file"
      // line = "this is [xtes]t12 of the file"
      // line = "this is x[test]12 of the file" == pattern match!

        // initialize buf with the first patternSize characters
        // of line:

      for (j = 0; j < patternSize; j++)
      {
        buf[j] = line[j];
      } // end initialize buf

      // close the buf:
      buf[j] = 0;

      // start the search:
      while (line[j] != 0)
      {
        // if we found the pattern, print the line:

        if (strcmp (buf, pattern) == 0)
        {
          if (redirectFlag == 1)
          {
              // write the output to the file:
            write (theWriteFD, line, strlen(line));
            buffer[0] = '\n';
            buffer[1] = 0;
            write (theWriteFD, buffer, 1);
            buffer[0] = 0;
          }
          else
          {
            printf ("%s%c", line, '\n');
          }
          break;
        } // end if we found the pattern, print the line
        else
        {
          for (i = 0; i < patternSize - 1; i++)
          {
            buf[i] = buf[i + 1];
          } // end shift characters one position to the left
          buf[i] = line[j];
          j++;
        } // end advance the buf

        buf[patternSize] = 0;

      } // end while not eol

      bytesRead = read (theFD, &readChar, 1);
      j = 0;
      i = 0;

    } // end while not eof

    printf ("\t\t\t~~~ end of Marcus' grep function ~~~%c", '\n');
    return 0;

  } // end if there is a file to search

  if (argc == 4)
  {
      // we are searching from stdin:
    theFD = dup (0);

    i = 0;

      // read the file until we find that pattern:
      // we need to print the full line that contains the pattern
      // so read one full line at a time:

      // read until eof:

    while (read (theFD, &readChar, 1) > 0)
    {
        // read until eol:

      if (readChar == '\n')
      {
        line[i] = 0;

        // now we have one full line, look for our pattern
        // within it, the pattern could appear anywhere so
        // we need to use a rolling window:
        // pattern = "test"
        // line = "this is xtest12 of the file"
        // sliding window of pattern.size():
        // line = "[this] is xtest12 of the file"
        // line = "t[his ]is xtest12 of the file"
        // line = "th[is i]s xtest12 of the file"
        // line = "thi[s is] xtest12 of the file"
        // line = "this[ is ]xtest12 of the file"
        // line = "this [is x]test12 of the file"
        // line = "this i[s xt]est12 of the file"
        // line = "this is[ xte]st12 of the file"
        // line = "this is [xtes]t12 of the file"
        // line = "this is x[test]12 of the file" == pattern match!

          // initialize buf with the first patternSize characters
          // of line:

        for (j = 0; j < patternSize; j++)
        {
          buf[j] = line[j];
        } // end initialize buf

          // close the buf:
        buf[j] = 0;

          // start the search:
        while (line[j] != 0)
        {
            // if we found the pattern, print the line:

          if (strcmp (buf, pattern) == 0)
          {
            if (redirectFlag == 1)
            {
                // write the output to the file:
              write (theWriteFD, line, strlen(line));
              buffer[0] = '\n';
              buffer[1] = 0;
              write (theWriteFD, buffer, 1);
              buffer[0] = 0;
            }
            else
            {
              printf ("%s%c", line, '\n');
            }
            break;
          } // end if we found the pattern, print the line
          else
          {
            for (i = 0; i < patternSize - 1; i++)
            {
              buf[i] = buf[i + 1];
            } // end shift characters one position to the left

            buf[i] = line[j];
            j++;

          } // end advance the buf

          buf[patternSize] = 0;

        } // end while not eol

        j = 0;
        i = 0;

      } // end if eol, close line and search for pattern
      else
      {
        line[i] = readChar;
        i++;

      } // end if not eol, add to line

    } // end while not eof

    printf ("\t\t\t~~~ end of Marcus' grep function ~~~%c", '\n');

    return 0;

  } // end if there is a redirect and search from stdin

    // if there are only two arguments, search stdin:

  if (argc == 2)
  {
    theFD = dup (0);
    i = 0;
    j = 0;
    match[0] = 0;
    matchFlag = 0;

      // start at the beginning of pattern:

    while (read (theFD, &readChar, 1) > 0)
    {
      if (readChar == '\r')
      {

        if (matchFlag == 1)
        {
          line[j] = 0;

          if (redirectFlag == 1)
          {
            write (theWriteFD, line, strlen(line));
            buffer[0] = '\n';
            buffer[1] = 0;
            write (theWriteFD, buffer, 1);
            buffer[0] = 0;
          }
          else
          {
            printf ("%s%c", line, '\n');
          }

        } // end if we found our pattern, print the line

        match[0] = 0;
        i = 0;
        j = 0;
        matchFlag = 0;
        line[0] = 0;

      } // end add new line and carriage return
      else
      {
        line[j++] = readChar;

        if (readChar == pattern[i])
        {
          match[i] = readChar;
          i++;
          match[i] = 0;
          if (strcmp (match, pattern) == 0)
          {
            matchFlag = 1;
          } // end if we found a match, set matchFlag

        } // end if we have a match, add to our match string
        else
        {
            // if no match, reset:
          i = 0;
        } // end if no match, reset

      } // end if not CR

    } // end while loop

  } // end if there are only 2 args

  printf ("\t\t\t~~~ end of Marcus' grep function ~~~%c", '\n');

  close (theFD);

  if (redirectFlag == 1)
  {
    close (theWriteFD);
  } // end if we opend a write fd, close it

  return 0;

}
