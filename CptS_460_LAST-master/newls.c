#include "util.c"
#include "type.h"

char *t1 = "xwrxwrxwr-------";
char *t2 = "----------------";

int Inodes_Per_Block;
u32 InodesBeginBlock;
int blk, offset;
char buf[1024];
SUPER *sp;
GROUP *gp;
INODE *ip;
DIR *dp;
INODE *myip;

// FUNCTION ** mailman

void mailman (int theInode)
{
  int i = 0;

  blk = (theInode - 1) / Inodes_Per_Block + InodesBeginBlock;
  offset = (theInode -1) % Inodes_Per_Block;

  get_block (3, blk, buf);
  ip = (INODE * ) buf;

  for (i = 0; i < offset; i++)
  {
    ip++;
  } // end increment to correct inode in correct block

} // end function mailman

// ls_file function

int ls_file (int theIno, char *fname)
{
  struct stat fstat, *mysp = &fstat;
  int r, i;
  char sbuf[4096];

  //printf ("ls_file called%c", '\n');

  r = stat (fname, mysp);

  if ((mysp->st_mode & 0xF000) == 0x8000 )
  {
    mputc ('-');
    //printf (" %c", '-');
  } // end if regular file

  if ((mysp->st_mode & 0xF000) == 0x4000 )
  {
    mputc ('d');
    //printf (" %c", 'd');
  } // end if directory

  if ((mysp->st_mode & 0xF000) == 0xA000 )
  {
    mputc ('l');
    //printf (" %c", 'l');
  } // end if linked

  for (i = 8; i >= 0; i--)
  {
    if (mysp->st_mode & (1<<i))
    {
      mputc (t1[i]);
      //printf (" %c", t1[i]);
    } // end print proper permission if set
    else
    {
      mputc (t2[i]);
      //printf (" %c", t2[i]);
    } // end else print a dash

  } // end print permissions

  printf (" %d ", mysp->st_nlink);
  printf (" %d ", mysp->st_uid);
  printf (" %d ", mysp->st_size);
  //strcpy (ftime, ctime (&sp->st_ctime) );
  //ftime[strlen(ftime) - 1] = 0;
  //printf ("%s ", ftime);
  printf (" %s", fname);
  /*
  if (S_ISLNK(sp->st_mode))
  {
    r = readlink (fname, sbuf, 4096);
    printf (" -> %s", sbuf);
  }
  */
  //printf (" %c", '\n');
} // end ls_file function

// ls_dir function: list the contents of a directory

int ls_dir (int theFD, int theIno)
{
    // ext2 filename 1-255 chars
  char name[256];
  DIR *dp;
  char buf[1024];
  char temp[256];
  int i;

  //printf ("ls_dir called%c", '\n');
  // open DIR to read names:

  mailman (theIno);

  get_block (theFD, theIno, buf);
  dp = (DIR *) buf;

  printf (" - inode: %d\n", dp->inode);

  for (i = 0; i < 2; i++)
  {
    strncpy (name, dp->name, dp->name_len);
    name[dp->name_len] = 0;

    ls_file (dp->inode, name);
    printf ("   %s  %d\n", name, dp->rec_len);

    dp = (DIR *) ((char *) dp + dp->rec_len);
  } // end loop through dir

    // call list file:
  ls_file (dp->inode, name);

} // end ls_dir function

int main (int argc, char *argv[])
{
  struct stat mystat, *mysp;
  int r;
  char *s;
  char filename[1024], cwd[1024];
  char fileType[32];
  int theFD, theIno;

  get_block (3, 1, buf);

  sp = (SUPER *)buf;           // as a super block structure

    // check for EXT2 FS magic number:

  printf ("%s = %x \n", "s_magic", sp->s_magic);
  printf ("sp->s_inode_size: %d\n", sp->s_inode_size);
  printf ("sp->s_first_data_block: %d\n", sp->s_first_data_block);

  s = argv[1];

  if (argc == 1)
  {
    s = "./";
  }

  mysp = &mystat;

  if ((r = stat (s, mysp)) < 0)
  {
    //perror("ls");
    printf ("error%c", '\n');
    exit (1);
  }

  strcpy (filename, s);

  if (s[0] != '/')
  {
    //strcpy (cwd, getcwd () );
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
    printf ("ls_dir call%c", '\n');
    ls_dir (theFD, theIno);

  } // enf if is dir
  if (strcmp (fileType, "regular file") == 0 )
  {
    printf ("ls_file call%c", '\n');
    ls_file (theIno, filename);

  } // end else


} // end main
