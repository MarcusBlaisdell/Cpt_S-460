#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
//#include "util.c"

char *t1 = "xwrxwrxwr-------";
char *t2 = "----------------";

struct stat mystat, *sp;

// ls_file function

int ls_file (char *fname)
{
  struct stat fstat, *sp = &fstat;
  int r, i;
  char sbuf[4096];
  r = lstat (fname, sp);

  if (S_ISDIR(sp->st_mode))
    printf ("%c", 'd');
  if (S_ISREG(sp->st_mode))
    printf ("%c", '-');
  if (S_ISLNK(sp->st_mode))
    printf ("%c", 'l');

  for (i = 8; i >= 0; i--)
  {
    if (sp->st_mode & (1<<i))
      printf ("%c", t1[i]);
    else
      printf ("%c", t2[i]);
  }

  printf ("%4d ", sp->st_nlink);
  printf ("%4d ", sp->st_uid);
  printf ("%8d ", sp->st_size);
  //strcpy (ftime, ctime (&sp->st_ctime) );
  //ftime[strlen(ftime) - 1] = 0;
  //printf ("%s ", ftime);
  printf ("%s", basename(fname));
  /*
  if (S_ISLNK(sp->st_mode))
  {
    r = readlink (fname, sbuf, 4096);
    printf (" -> %s", sbuf);
  }
  */
  printf ("\n");
} // end ls_file function

  // ls_dir function: list the contents of a directory

int ls_dir (char *dname)
{
    // ext2 filename 1-255 chars
  char name[256];
  DIR *dp;
  struct dirent *ep;

  // open DIR to read names:

  dp = opendir (dname); // opendir () syscall

  while (ep = readdir (dp))
  {
    strcpy (name, ep->d_name);

      // skip over . and ..
    if (!strcmp (name, ".") || !strcmp (name, "..") )
    {
      continue;
    } // end skip . and ..

  strcpy (name, dname);
  strcat (name, "/");
  strcat (name, ep->d_name);

    // call list file:
  ls_file (name);

  } // end while

} // end ls_dir function

  // main function

  int main (int argc, char *argv[])
  {

    struct stat mystat, *sp;
    int r;
    char *s;
    char filename[1024], cwd[1024] = {"/home/marcus/CS460/FinalProject"};

    s = argv[1];    // ls filename

      // if no parameters, ls CWD:
    if (argc == 1)
    {
      s = "./";

    } // end if no parameters

    sp = &mystat;

    if ((r = stat (s, sp)) < 0)
    {
      perror ("ls");

      exit (1);

    } // end if

    strcpy (filename, s);

    if (s[0] != '/')
    {
        // get CWD path
      //getcwd (cwd, 1024);
      strcpy (filename, cwd);
      strcat (filename, "/");
      strcat (filename, s); // construct $CWD/filename

    } // end if not absolute

    if (S_ISDIR (sp->st_mode) )
    {
      ls_dir (filename);

    } // enf if is dir
    else
    {
      ls_file (filename);

    } // end else

  } // end main function
