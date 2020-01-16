// login.c
// prompts user for a username and password
// opens the /etc/passwd file for read,
// searches through file for line corresponding to username:
// checks that the password provided matches the password stored,
// if match, changes user id and group id to that of the authenticated user,
// changes to the user's home directory,
// and executes the 'sh' program

//#include "ucode.c"
#include "util.c"

int in, out, err, etcPassword;
char uname[128], upassword[128];
char buf[1024];
char *tokens[16];

// readLine function

int readLine (int theFile)
{
  int i = 0, bytesRead;
  char myChar;
  buf[0] = 0;

  bytesRead = read (theFile, &myChar, 1);

  while (myChar != '\n' && bytesRead > 0)
  {
    buf[i++] = myChar;
    bytesRead = read (theFile, &myChar, 1);

  } // end while loop to read one line from the file, one char at a time

    // close the string:
  buf[i] = 0;

} // end readLine function

int main (int argc, char *argv[])
{
  int i;
  int uid, gid;

    // 1) close file descriptors 0 and 1:
  close (0);
  close (1);

    // 2) open argv[1] 3 times as in(0), out(1), err(2)

  in = open (argv[1], O_RDONLY);
  out = open (argv[1], O_WRONLY);
  err = open (argv[1], O_WRONLY);

    // 3) settty (argv[1]);  // set tty name string in PROC.tty
  settty (argv[1]);

    // 4) open /etc/passwd file for READ;
  etcPassword = open ("/etc/passwd", O_RDONLY);

  while (1)
  {
    printf ("\t\t\t~~~ Marcus Login ~~~%c", '\n');
    printf ("%cPlease login: ", '\n');
    gets (uname);

    printf ("%cEnter password: ",'\n');
    gets (upassword);

      // for each line: need to load one line at a time and tokenize it
      // until we find the one that matches "name":

    i = 0;

      // move to the correct line, the one that matches the user:

    while (strcmp (tokens[0], uname) != 0)
    {
      if (readLine (etcPassword) )
      {
        tokenize (buf, tokens, ':');
      }
      else
      {
        printf ("user %s not found, please try again\n", uname);
      }

    } // end move to correct line:

      // compare password:

    if (strcmp (tokens[1], upassword) == 0)
    {
      printf ("password accepted%c", '\n');
      printf ("Welcome user: %s\n\n", uname);
      uid = ((int) tokens[2][0]) - 48;
      gid = ((int) tokens[3][0]) - 48;

      chuid (uid, gid);
      chdir (tokens[5]);
      close (etcPassword);
      exec (tokens[6]);

    } // end if password is valid
    else
    {
      printf ("incorrect password%c", '\n');
    } // end if password doesn't match

  } // end while loop

} // end main function
