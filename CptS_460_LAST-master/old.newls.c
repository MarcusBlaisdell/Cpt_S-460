#include "util.c"
#include "type.h"

#define MAX 256
#define mputc putchar

//MINODE minode[NMINODE];
//MINODE root;
//MINODE testTest;
PROC proc[NPROC];
PROC *running;
//MTABLE mtable[4];

SUPER *sp;
GROUP *gp;
INODE *ip;
DIR *dp;
INODE *myip;

int dev;
int nblocks; // from superblock
int ninodes; // from superblock
int bmap; // bmap block
int imap; // imap block
int iblock; // inodes begin block
int Inodes_Per_Block;

u32 InodesBeginBlock;

char buf[BLKSIZE];
int fd, blk, offset, blksize, inodesize, curNode, rootNode;
int dot = 0, dotdot = 0;

char *device;
//char *pathname;
char temp[256];
char * name[16];
int name_n;

struct stat mystat, *newsp;

  // ** FUNCTION mylseek
/*
int mylseek (char * theFD, char * position)
{
  int fileSize;
  int FD = (int) strtol (theFD, (char **) 0, 10);
  int pos = (int) strtol (position, (char **) 0, 10);
  printf ("FD = %d, pos = %d, position = %s\n", FD, pos, position);

  fileSize = running->fd[FD]->mptr->INODE.i_size;

  if (fileSize < pos)
  {
    printf ("Position is outside the file size\n");
    return;
  } // end if outside fileSize

  running->fd[FD]->offset = pos;

} // end function mylseek
*/
// ** FUNCTION get_block()
// reads a disk block into a buf[ ]

int get_block(int fd, int blk, char *buf)
{
  lseek(fd, (long)blk*BLKSIZE, 0);
  return read(fd, buf, BLKSIZE);
} // end function get_block
/*
// ** FUNCTION put_block()

int put_block (int fd, int blk, char *buf)
{
  lseek (fd, (long)blk*BLKSIZE, SEEK_SET);
  if (write (fd, buf, BLKSIZE) )
  {
    return 1;
  } // end return 1 if successful

  return 0;

} // end put_block function
*/
// ** FUNCTION super
/*
//int super(char *device)
int super(int fd)
{
  get_block (fd, 1, buf);

  sp = (SUPER *)buf;           // as a super block structure

    // check for EXT2 FS magic number:

  printf("%-30s = %8x \n", "s_magic", sp->s_magic);


    // if not ext2, print an error
  if (sp->s_magic != 0xEF53)
  {
    printf("NOT an EXT2 FS\n");
    exit(2);
  } // end if not EXT2
  else
  {
    printf("EXT2 FS OK\n");
    blksize = 1024 * (1 << sp->s_log_block_size);

    ninodes = sp->s_inodes_count;
    nblocks = sp->s_blocks_count;
    Inodes_Per_Block = blksize / sp->s_inode_size;

  } // end if it is EXT2

} // end super function

// ** FUNCTION group

int group(int fd)
{
    // lseek to group descriptor block and read it into the buffer
  get_block (fd, (sp->s_first_data_block + 1), buf);

  gp = (GROUP *)buf;           // as a super block structure

  imap = gp->bg_inode_bitmap;
  bmap = gp->bg_block_bitmap;

  InodesBeginBlock = gp->bg_inode_table;

} // end group function
*/
// FUNCTION ** mailman

void mailman (int theInode)
{
  int i = 0;

  blk = (theInode - 1) / Inodes_Per_Block + InodesBeginBlock;
  offset = (theInode -1) % Inodes_Per_Block;

  get_block (fd, blk, buf);
  ip = (INODE * ) buf;

  for (i = 0; i < offset; i++)
  {
    ip++;
  } // end increment to correct inode in correct block

} // end function mailman

// ** FUNCTION ls
// list contents of a directory


//int myls (char *thePath, int theFD)
int myls (int theFD)
{
  char * token;
  int i = 0, n = 0;
  char *myArgs[16];
  char temp[256];

  printf ("myls called%c", '\n');
  /*
  if (thePath == 0)
  {
    printf ("no path provided, using /\n");
    thePath = "/";
  }
  printf ("ls called for %s\n", thePath);

    mycd (thePath);
*/
    //printf ("curNode = %d\n", curNode);
    //mailman (curNode);
    printf ("theFD = %d\n", theFD);
    mailman (theFD);


  get_block (fd, ip->i_block[0], buf);
  dp = (DIR *) buf;

  for (i = 0; i < 12; i++)
  {
    strncpy (temp, dp->name, dp->name_len);
    temp[dp->name_len] = 0;

    //myls_file (dp->inode);
    printf ("   %10s  %6d\n", temp, dp->rec_len);

    dp = (DIR *)((char *) dp + dp->rec_len);
  } // end loop through dir

  printf ("done\n");

  return 1;

} // end ls function

/*
// ** FUNCTION myls_file

int myls_file (int theInode)
{
  INODE *newIP;
  int newBlock = 0, newOffset = 0, i = 0;
  char myBuf[BLKSIZE];
  char *t1 = "xwrxwrxwr-------";
  char *t2 = "----------------";
  char ftime[64];

  newBlock = (theInode - 1) / Inodes_Per_Block + InodesBeginBlock;
  newOffset = (theInode - 1) % Inodes_Per_Block;

  get_block (fd, newBlock, myBuf);
  newIP = (INODE *) myBuf;

  for (i = 0; i < newOffset; i++)
  {
    newIP++;
  } // end move to correct newIP

  if ((newIP->i_mode & 0xF000) == 0x8000 )
  {
    printf ("%c", '-');
  } // end if

  if ((newIP->i_mode & 0xF000) == 0x4000 )
  {
    printf ("%c", 'd');
  } // end if directory

  if ((newIP->i_mode & 0xF000) == 0xA000 )
  {
    printf ("%c",'l');
  } // end if linked

  for (i = 8; i >= 0; i--)
  {
    if (newIP->i_mode & (1 << i) )
    {
      printf ("%c", t1[i]);
    }
    else
    {
      printf ("%c", t2[i]);
    }
  } // end for loop to print file permissions


} // end myls_file function
*/


// ** FUNCTION main

int main(int argc, char *argv[])
{
  int i = 0, j = 0;
  int theFD;
  char *token;
  char *name [16]; // hopefully nobody goes deeper than that
  char line[256];
  char * myArgs[3];
  char buffer[1024];
  int r;

  struct stat fstat;
  *newsp = &fstat;

  printf ("ls called%c", '\n');

  //*sp = &fstat;
  r = lstat ("file2", newsp);

  printf ("newsp->st_ino: %d\n", newsp->st_ino);

  //printf ("s_magic: %x\n", sp->s_magic);
} // end main function
