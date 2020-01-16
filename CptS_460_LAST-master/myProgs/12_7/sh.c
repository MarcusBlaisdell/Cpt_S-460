/* sh.c */

//#include "ucode.c"
#include "util.c"

// use a global variable to set a "more flag"
// if more is called after a cat, then handle it differently
int moreFlag = 0;

  // do_pipe function
  // multiple pipes are handled recursively from right to left:

int do_pipe (char *cmd, int *pd)
{
  int pid, pid2, pd2[2], pd3[2], fd;
  int hasPipe = 0;
  char *head = "", *tail = "";
  char *tokens[16];
  char buf[1024];
  int i = 0;

  printf ("do_pipe cmd: %s\n", cmd);

    // if a pipe was passed in, we are writer on pipe pd
  if (pd)
  {
    close (pd[0]);    // cose pipe READ end
    dup2 (pd[1], 1);  // duplicate pipe descriptor write to stdout
    //close (pd[1]);    // close duplicate WRITE end (?)
  } // end if pipe was passed in

    // divide cmd into head, tail by rightmost pipe symbol:
  hasPipe = scan (cmd, &head, &tail);
  printf ("head: %s : tail: %s : hasPipe: %d\n", head, tail, hasPipe);

    // if there is a more at the end of a pipe,
    // set the moreFlag, remove the more and call
    // hasPipe on head

  if (strcmp (tail, "more") == 0)
  {
    //printf ("more in tail, setting moreFlag%c", '\n');
    moreFlag = 1;
    hasPipe = scan (head, &head, &tail);
  } // end filter out more if it's at the end of a pipe

    // if there is a pipe, handle it:
  if (hasPipe == 1)
  {
      // create a new pipe:
    pipe (pd2);
      // fork a new process:
    pid = fork ();

      // assign parent as the reader:
    if (pid)
    {
      close (pd2[1]);     // close pipe WRITE end
      dup2 (pd2[0], 0);   // duplicate pipe descriptor read to stdin
      close (pd2[0]);     // close duplicate READ end (?)

      exec (tail);
      //do_command (tail);

    } // end if pid

    // child:
    // if there are more pipes, recursively call head:
    else
    {
      do_pipe (head, pd2);

    } // end else, recursively call head

  } // end if hasPipe flag is yes
  else
  {
    // tokenize cmd, check if commnd is cat:
    strcpy (buf, cmd);
    tokenize (buf, tokens, ' ');
    //printf ("tokens[0]: %s%c", tokens[0], '\n');
    if (moreFlag == 1 && strcmp (tokens[0], "cat") == 0)
    {
      strcpy (head, "morecat");
      i = 1;
      while (tokens[i] != 0)
      {
        strcat (head, " ");
        strcat (head, tokens[i++]);
      } // end rebuild cmd with morecat

      moreFlag = 0;
      exec (head);

    } // end if cat and moreFlag
    else
    {
      exec (cmd);
    }

    //do_command (cmd);

  } // end if no pipe, execute the command

} // end do_pipe function

// do_command function:

int do_command (char * cmd)
{
  char *tokens[16];
  char buf[1024];
  int i = 0, fd;
  char head[1024];
  char buffer[1024];
  int bytesRead;

  printf ("do_command called for %s%c", cmd, '\n');

    // look for re-directs:
    // tokenize buf by ' ', search tokens for re-direct symbols:
    // (use token to get file name)

  strcpy (buf, cmd);
  tokenize (buf, tokens, ' ');

  i = 0;

  while (tokens[i] != 0)
  {
    //printf ("tokens[%d]: %s\n", i, tokens[i]);
      // if redirect in:

    if (strcmp (tokens[i], "<") == 0)
    {
        // close stdin
      close (0);
        // open the file to be read from for read:
      printf ("opening %s for read\n", tokens[i + 1]);
      fd = open (tokens[i + 1], O_RDONLY);
      printf ("opened %s for read\n", tokens[i + 1]);

    } // end if redirect in "<"

      // if redirect out:

    if (strcmp (tokens[i], ">") == 0)
    {
        // close stdout
      close (1);

        // create the file:

      //creat (tokens[i + 1]);
        // open file for writing:
      //printf ("opening %s for write\n", tokens[i + 1]);
      fd = open (tokens[i + 1], O_WRONLY | O_CREAT);
      printf ("opened %s for write\n", tokens[i + 1]);

    } // end if redirect out ">"

    // if redirect append:
    if (strcmp (tokens[i], ">>") == 0)
    {
        // close stdout:
      close (1);

        // open file for write:
      printf ("opening %s for append\n", tokens[i + 1]);
      fd = open (tokens[i + 1], O_RDWR | O_APPEND);
      printf ("opened %s for append\n", tokens[i + 1]);

        // move to the end of the writefile:
      bytesRead = read (fd, buffer, 1);

      while (bytesRead > 0)
      {
        bytesRead = read (fd, buffer, 1);
        mybzero (buffer, 1);
      } // end move to end of write file

    } // end if redirect append ">>"

    i++;

  } // end look for redirects

    // head = cmd BEFORE redirections:

  i = 0;

  // copy everything up to the redirect into head:

  strcpy (head, tokens[i++]);

  /*
  while (tokens[i] != "<" && tokens[i] != ">" && tokens[i] != ">>" && tokens[i] != 0)
  while ( (strcmp(tokens[i], "<") != 0) && (strcmp(tokens[i], ">") != 0) \
            && (strcmp(tokens[i], ">>") != 0) && tokens[i] != 0)
  {
    printf ("concatenating: %s\n", tokens[i]);
    strcat (head, " ");
    strcat (head, tokens[i]);
    i++;
  } // end copy cmd before redirects to head
  */

  i = 0;

  while (cmd[i] != '<' && cmd[i] != '>' && cmd[i] != 0)
  {
    head[i] = cmd[i];
    i++;
  }
  head[i] = 0;
  //head[i - 1] = 0; // don't include the trailing space

  printf ("head is now: %s\n", head);
  exec (head);

  return 1;

} // end do_command function

// scan function:

int scan (char * cmd, char ** head, char ** tail)
{
  int cmdLength, i = 0, j = 0;

  // we have to divide the command into the pre-pipe part and the post-pipe part:
  // since there may be multiple pipes, and we have to call them recursively
  // from the right to the left, the pre-pipe part will be every thing that
  // comes before the last pipe and the post-pipe part will be everything
  // that comes after the last pipe, so, we need to find the last pipe,
  // use pointers to variables outside the function:

  char *pointer = cmd;
  cmdLength = 0;

    // move pointer to the end of the command:
  while (*pointer)
  {
    *pointer++;
    cmdLength++;
  } // end move to end of command

  //while (*pointer != '|' && *pointer != *cmd)
  while (*pointer != '|' && cmdLength > -1)
  {
    *pointer--;
    cmdLength--;
  } // end look for last pipe

  if (*pointer == '|')
  {
    // close the string at the pipe,
    // this way, we can set the whole thing equal to head:

    *pointer = 0;
    *head = cmd;

    // and set the remainder to tail:
    *pointer++; // move past pointer:
    *pointer++; // move past space after pointer:
    *tail = pointer;

      // if we found a pipe, return 1
    return 1;
  } // end we found a pipe
  // if we did not find a pipe
  // set head to the full command and return 0:
  else
  {
    *head = cmd;
    return 0;
  } // end no pipe found

    // we shouldn't get here, if we do, return an error:

  return -1;

} // end scan function

  // testForPipe function:

int testForPipe (char *cmd)
{
  int i = 0;

  while (cmd[i] != 0)
  {
      // if a pipe is found, return 1
    if (cmd[i] == '|')
    {
      return 1;
    } // end if we find a pipe, return 1 for yes
    i++;

  } // end while loop to look for pipe

  // if we get here, no pipe was found, return 0:

  return 0;

} // end testForPipe function

main (int argc, char *argv[])
{
  int pid, status;
  char cmd[32], theCmd[32];
  char cmdPrefix[5] = {"/bin/"};
  char *tokens[16];
  char buf[32];
  int pipeFlag = 0;

  while (1)
  {
      // get a command from the user:

    printf("Select a command: ");
    //getline(cmd);
    gets(cmd);
    //printf ("You entered: %s\n", cmd);

      // tokenize input:

    strcpy (buf, cmd);
    tokenize (buf, tokens, ' ');

      // if logout, just logout, don't do any more evaluation:

    if (strcmp (tokens[0], "logout") == 0)
    {
      printf ("Logging out\n%d",0);
      exit (0);
    } // end if exit

      // if cd, call chdir:

    if (strcmp (tokens[0], "cd") == 0)
    {
      printf ("tokens[1]: %s\n", tokens[1]);
      chdir (tokens[1]);
      continue;
    } // end if cd

      // if pwd, call pwd, it's in ucode.c, not /bin/

    if (strcmp (tokens[0], "pwd") == 0)
    {
      pwd ();
      continue;
    } // end if pwd

      // for binary executable command:

    pid = fork ();

    if (pid)
    {
      //printf ("waiting%c", '\n');
      pid = wait(&status);
      //printf ("done waiting%c",'\n');
      continue;
    } // end if pid was successful
    else
    {
      //printf ("calling do_pipe%c", '\n');
      do_pipe (cmd, 0);
    } // end do_pipe

  } // end while

} // end main
