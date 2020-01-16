/* sh.c */
// this is the main control program,
// it prompts the user for a command and then
// handles that command
// I am using my util.c because I thought we were supposed to write it
// it does include "ucode.c"
// it only contains my tokenize function and my 'mybzero' function
// since I already had it written and my code uses it, I have elected to keep it

//#include "ucode.c"
#include "util.c"

// use a global variable to set a "more flag"
// if more is called after a cat, then handle it differently
int moreFlag = 0;

  // do_pipe function
  // multiple pipes are handled recursively from right to left:

int do_pipe (char *cmd, int *pd)
{
  int pid, pd2[2];
  int hasPipe = 0;
  char *head = "", *tail = "";
  char *tokens[16];
  char buf[1024];
  int i = 0;

  printf ("do_pipe cmd: %s\n", cmd);

    // if a pipe was passed in, we are writer on pipe pd
  if (pd)
  {
    //printf ("\t*** pipe passed in: pd[0]: %d - pd[1]: %d ***\n", pd[0], pd[1]);
    close (pd[0]);    // close pipe READ end
    dup2 (pd[1], 1);  // duplicate pipe descriptor write to stdout
    //close (pd[1]);    // close duplicate WRITE end (?) // this was causing problems but works if commented out

  } // end if pipe was passed in

    // divide cmd into head, tail by rightmost pipe symbol:
  hasPipe = scan (cmd, &head, &tail);
  printf ("\t~~~ head: %s : tail: %s : hasPipe: %d ~~~\n", head, tail, hasPipe);

    // I am handling the pipe to more by replacing it by
    // morecat to handle userinput at the beginning of the pipe
    // thread rather than the end so:
    // if there is a more at the end of a pipe,
    // set the moreFlag, remove the more and call
    // hasPipe on head

  if (strcmp (tail, "more") == 0)
  {
    moreFlag = 1;
    tail = "";
    hasPipe = scan (head, &head, &tail);
    printf ("\t*** more: head: %s : tail: %s : hasPipe: %d ***\n", head, tail, hasPipe);
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
      //printf ("\t~~~ new pipe created: pd2[0]: %d - pd2[1]: %d ~~~\n", pd2[0], pd2[1]);
      close (pd2[1]);     // close pipe WRITE end
      dup2 (pd2[0], 0);   // duplicate pipe descriptor read to stdin
      close (pd2[0]);     // close duplicate READ end (?)

      exec (tail);

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
      // if we are here, there are no more pipes:

      // tokenize cmd, check if command is cat:
    strcpy (buf, cmd);
    tokenize (buf, tokens, ' ');

    if (moreFlag == 1 && strcmp (tokens[0], "cat") == 0)
    {
      strcpy (head, "morecat");

      i = 1;

      while (tokens[i] != 0)
      {
        strcat (head, " ");
        strcat (head, tokens[i++]);

      } // end rebuild cmd with morecat

      //strcat (head, 0);

      moreFlag = 0;
      exec (head);

    } // end if cat and moreFlag
    else
    {
      exec (cmd);
    } // end else, run unmodified command (because no cat)

  } // end if no pipe, execute the command

} // end do_pipe function

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

main (int argc, char *argv[])
{
  int pid, status;
  char cmd[128], theCmd[128];
  char cmdPrefix[5] = {"/bin/"};
  char *tokens[16];
  char buf[32];
  int pipeFlag = 0;

  while (1)
  {
      // get a command from the user:

    printf("Select a command: ");

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
      pid = wait(&status);
      continue;
    } // end if pid was successful
    else
    {
      do_pipe (cmd, 0);
    } // end do_pipe

  } // end while

} // end main
