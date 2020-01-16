// ls.c
// this program provides the same behaviour as linux 'ls -l'
// except that it does not print the date and time information

#include "util.c"
#include "type.h"

char *t1 = "xwrxwrxwr-------";
char *t2 = "----------------";

int blk, offset;
char buf[1024];
DIR *dp;

  // basename function:
  // tokenizes the provided name which contains a
  // full path, and prints only the last token
  // which is the name of the file/directory

void basename (char * fname)
{
  char *tokens[16];
  char buf[32];
  char theBasename[32];
  int i = 0;

  strcpy (buf, fname);

  tokenize (buf, tokens, '/');

  while (tokens[i] != 0)
  {
    strcpy (theBasename, tokens[i]);
    i++;
  } // end get last token as basename

  printf (" %s\n", theBasename);

} // end basename function

  // ls_file function

int ls_file (char *fname)
{
  struct stat fstat, *mysp = &fstat;
  int r, i;
  char sbuf[4096];

  r = stat (fname, mysp);

  if ((mysp->st_mode & 0xF000) == 0x8000 )
  {
    mputc ('-');
  } // end if regular file

  if ((mysp->st_mode & 0xF000) == 0x4000 )
  {
    mputc ('d');
  } // end if directory

  if ((mysp->st_mode & 0xF000) == 0xA000 )
  {
    mputc ('l');
  } // end if linked

  for (i = 8; i >= 0; i--)
  {
    if (mysp->st_mode & (1<<i))
    {
      mputc (t1[i]);
    } // end print proper permission if set
    else
    {
      mputc (t2[i]);
    } // end else print a dash

  } // end print permissions

  printf (" %d ", mysp->st_nlink);
  printf (" %d ", mysp->st_uid);
  printf (" %d ", mysp->st_gid);
  printf (" %d ", mysp->st_size);
  basename (fname);

} // end ls_file function

  // ls_dir function: list the contents of a directory

int ls_dir (int theFD, int theIno, char *filename)
{
    // ext2 filename 1-255 chars
  char name[256];
  DIR *dp;
  char buf[1024];
  char temp[256];
  int i;
  char * cp;
  int myFD;

    // open DIR to read names:
  myFD = open (filename, O_RDONLY);

  read (myFD, buf, 1024);
  dp = (DIR *) buf;
  cp = buf;

    // loop to print all records:

  while (cp < buf + 1024)
  {
    strncpy (temp, dp->name, dp->name_len);
    temp[dp->name_len] = 0;

    ls_file (temp);

    cp += dp->rec_len;
    dp = (DIR *) cp;

  } // end while loop to print all records

  close (myFD);

} // end ls_dir function

int main (int argc, char *argv[])
{
  struct stat mystat, *mysp;
  int r;
  char *s;
  char filename[1024], cwd[1024];
  char fileType[32];
  int theFD, theIno;

  printf ("*** Marcus' ls function: ***%c%c", '\n', '\n');
  s = argv[1];

    // if no argument is provide, use current directory:

  if (argc == 1)
  {
    getcwd (cwd);
    s = cwd;
  } // end if no argument is provided

  mysp = &mystat;

  if ((r = stat (s, mysp)) < 0)
  {
    printf ("error%c", '\n');
    exit (1);
  } // end if can't stat the struct

  strcpy (filename, s);

  if (s[0] != '/')
  {
    getcwd (cwd);
    strcpy (filename, cwd);
    strcat (filename, "/");
    strcat (filename, s);
  }

  theFD = mysp->st_dev;
  theIno = mysp->st_ino;

  printf ("mysp->st_dev: %d - mysp->st_ino: %d\n",theFD, theIno);
  printf ("mysp->st_mode: %x\n", mysp->st_mode);

  if ((mysp->st_mode & 0xF000) == 0x8000 )
  {
    strcpy (fileType, "regular file");
  } // end if regular file

  if ((mysp->st_mode & 0xF000) == 0x4000 )
  {
    strcpy (fileType, "directory");
  } // end if directory

  if ((mysp->st_mode & 0xF000) == 0xA000 )
  {
    strcpy (fileType, "linked");
  } // end if linked

  printf ("fileType: %s\n", fileType);

  if (strcmp (fileType, "directory") == 0 )
  {
    ls_dir (theFD, theIno, filename);

  } // enf if is dir
  if (strcmp (fileType, "regular file") == 0 )
  {
    printf ("ls_file call%c", '\n');
    ls_file (filename);

  } // end else

} // end main
