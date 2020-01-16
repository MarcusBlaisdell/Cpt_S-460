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

// mytokenize function:

void mytokenize ()
{
  int i = 0, j = 0, k = 0;
  char temp[16][128];
  char *word;
  word = temp[k];

  while (buf[i] != '\n')
  {
    if (buf[i] == ':')
    {
      *word = 0;
      word = temp[k++];
      tokens[j] = word;
      word = temp[k];


      j++;
      i++;
    } // end if colon
    else
    {
      *word++ = buf[i++];

    } // end else, add to word

  } // end read to end of buf

  *word = 0;
  word = temp[k++];
  tokens[j] = word;
  word = temp[k];

} // end mytokenize function

int main (int argc, char *argv[])
{
  int i;
  int uid, gid;

  /*
  1) close file descriptors 0, 1 inherited from INIT.
  2) open argv[1] 3 times as in(0), out(1), err(2).
  3) settty (argv[1]);  // set tty name string in PROC.tty
  4) open /etc/passwd file for READ;
    while (1)
    {
  5)  printf ("login: ");
      gets (name);
      printf ("password: ");
      gets (password);
      for each line in /etc/passwd file do:
      {
        tokenize user account line;
  6)    if (user has a valid account)
        {
          change uid, gid to user's uid, gid;   // chuid ()
          change cwd to user's home DIR          // chdir ()
          close opened /etc/passwd              // close ()
          exec to program in user account       // exec ()
        } // end if user has a valid account
      } // end for each line in /etc/passwd
      printf ("login failed, try again\n");

    } // end while
  */

  // 1) close file descriptors 0 and 1:
  close (0);
  close (1);
  //close (2);

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
    printf ("Please login: ");
    gets (uname);

    printf ("Enter password: ");
    gets (upassword);
    /*
      // format of /etc/passwd:
    root:12345:0:0:super user:/:sh
    kcw:abcde:1:1:kcw user:/user/kcw:sh
    username:password:uid?:gid?:group:home directory:program to execute on startup
    */
    // for each line: need to load one line at a time and tokenize it
    // until we find the one that matches "name":

    i = 0;

    //while (tokens[0] != uname)
      // move to the correct line, the one that matches the user:

    while (strcmp (tokens[0], uname) != 0)
    {
      //readLine (etcPassword);
      //mytokenize ();

      if (readLine (etcPassword) )
      {
        mytokenize ();
      }
      else
      {
        printf ("user %s not found, please try again\n", uname);
      }

    } // end move to correct line:

    if (strcmp (tokens[1], upassword) == 0)
    {
      printf ("password accepted%d\n", 0);
      uid = ((int) tokens[2][0]) - 48;
      gid = ((int) tokens[3][0]) - 48;

      chuid (uid, gid);
      chdir (tokens[5]);
      close (etcPassword);
      printf ("tokens[6]: %s\n", tokens[6]);
      exec (tokens[6]);

    } // end if password is valid
    else
    {
      printf ("incorrect password%d\n",0);
    } // end if password doesn't match

  } // end while loop

} // end main function
