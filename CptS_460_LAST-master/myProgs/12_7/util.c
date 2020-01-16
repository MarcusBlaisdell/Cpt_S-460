/*
  util.c
*/

#include "uart.c"
#include "ucode.c"

//#include "type.h"

// globals defined in main.c file:

/*
extern MINODE minode [NMINODE];
extern MINODE *root;
extern PROC proc[NPROC], *running;
extern char gpath[128];
extern char *name[64];
extern int n;
extern int fd, dev;
extern int nblocks, ninodes, bmap, imap, inode_start;
extern char line[256], cmd[32], pathname[256];
*/

#define BLKSIZE 1024

// clear the buffer:

int mybzero (char buf[], int size)
{
  int i = 0;

  for (i = 0; i < size; i++)
  {
    buf[i] = 0;
  } // end for loop to empty buffer

} // end bzero function


// get_block function

int get_block(int fd, int blk, char *buf)
{
  //lseek(fd, (long)blk*BLKSIZE);
  lseek(fd, (long)blk*BLKSIZE, 0);
  return read(fd, buf, BLKSIZE);
} // end function get_block

/*

// put_block function

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

// tokenize function:

void tokenize (char *buf, char * tokens[], char delimiter)
{
  int i = 0, j = 0, k = 0;
  char temp[16][128];
  char *word;
  word = temp[k];

  while (buf[i] != '\n' && buf[i] != 0)
  {
    if (buf[i] == delimiter)
    {
      *word = 0;          // close out word
      word = temp[k++];   // reset pointer to beginning of the temp array position and increment temp array position
      tokens[j] = word;   // store the word in the pass-by-reference tokens array
      word = temp[k];     // set word to the beginning of the next slot in the temp array

      j++;
      i++;
    } // end if delimiter
    else
    {
        // add the character from the buffer to the word
      *word++ = buf[i++];

    } // end else, add to word

  } // end read to end of buf

  *word = 0;          // close out word
  word = temp[k++];   // reset pointer to beginning of the temp array position and increment temp array position
  tokens[j] = word;   // store the word in the pass-by-reference tokens array
  tokens[j + 1] = 0;  // close out the tokens array
  word = temp[k];     // set word to the beginning of the next slot in the temp array

} // end tokenize function

// iget
/*
MINODE *iget (int dev, int ino)
{
  // return minode pointer to loaded INODE

  MINODE * iget (int device, int inode)
{
  int i = 0, j = 0;

  printf ("iget called\n");

    // check if this inode already has a minode
    // search minode array for inode:

  for (i = 0; i < NMINODE; i++)
  {
    if ((minode[i].dev == device) && (minode[i].ino == inode) )
    {
      printf ("minode already loaded\n");
      minode[i].refCount++;

      return &minode[i];

    } // end if inode is already in memory

  } // end loop check minode memory

      // if inode is not already in memory,
      // put it in memory
      // search for an open minode to assign our inode to

    printf ("minode not loaded, searching for empty minode\n");

    i = 0;

      // find first empty minode

    //while (minode[i].ino != 0)
    while (minode[i].refCount != 0)
    {
      i++;
    } // end loop to first empty minode

    printf ("Empty minode found at %d\n", i);

      // store inode in minode array

    minode[i].dev = device;
    minode[i].ino = inode;
    minode[i].refCount++;
    minode[i].dirty = 0;
    minode[i].mounted = 0;
    minode[i].mptr = 0;


  mailman (inode);

  memcpy (&minode[i].INODE, ip, sizeof (INODE) );

  return &minode[i];

} // end function *iget

// iput function, dispose a used minode by mip

int iput (MINODE *mip)
{
  printf ("mip->ino = %d, mip->dirty = %d, mip->INODE.i_mode = %x\n", mip->ino, mip->dirty, mip->INODE.i_mode);
  mip->refCount--;
  //INODE *tmpNode;

  if (mip->dirty == 1)
  {
    mailman (mip->ino);
    *ip = mip->INODE;
    if (put_block (mip->dev, blk, buf) )
    {
      printf ("put_block successful\n");
    }
    else
    {
      printf ("put_block failed\n");
    }
    //write (mip->dev, mip->INODE, sizeof(INODE));
    //return;
  } // end return if dirty

    // otherwise, write inode back to disk

  printf ("iput: dev = %d, ino = %d\n", mip->dev, mip->ino);
  mailman (mip->ino);

    // copy inode into ip

  *ip = mip->INODE;

    // write updated block back to disk

  put_block (mip->dev, blk, buf);

} // end function iput

// search a directory INODE for entry with given name

//int search (MINODE *mip, char *name)
int search (INODE theInode, char * theName)
{
  int i = 0, xino = 0;
  MINODE * mip;

  printf ("search called for :%s:\n", theName);

  printf ("theInode.i_mode = %x\n", theInode.i_mode);

  get_block (fd, theInode.i_block[0], buf);
  dp = (DIR *) buf;

  for (i = 0; i < 12; i++)
  {
    printf ("i = %d\n", i);
        // convert name

      strncpy (temp, dp->name, dp->name_len);
      temp[dp->name_len] = 0;

      if (strcmp (temp, theName) == 0)
      {
        printf ("match found\n");

          // check if mounted
        xino = dp->inode;
        mip = iget (fd, xino);

        printf ("test mip->INODE.i_mode = %x\n\n", mip->INODE.i_mode);
          // Only do this if we are on the mounted device,
          // if we are on the root device,
        //if (mip->mounted == 1 && fd == root.dev)
        if (mip->mounted == 1)
        int search (INODE theInode, char * theName)
{
  int i = 0, xino = 0;
  MINODE * mip;

  printf ("search called for :%s:\n", theName);

  printf ("theInode.i_mode = %x\n", theInode.i_mode);

  get_block (fd, theInode.i_block[0], buf);
  dp = (DIR *) buf;

  for (i = 0; i < 12; i++)
  {
    printf ("i = %d\n", i);
        // convert name

      strncpy (temp, dp->name, dp->name_len);
      temp[dp->name_len] = 0;

      if (strcmp (temp, theName) == 0)
      {
        printf ("match found\n");

          // check if mounted
        xino = dp->inode;
        mip = iget (fd, xino);

        printf ("test mip->INODE.i_mode = %x\n\n", mip->INODE.i_mode);
          // Only do this if we are on the mounted device,
          // if we are on the root device,
        //if (mip->mounted == 1 && fd == root.dev)
        if (mip->mounted == 1)
        {
          printf ("\n\nMounted\n\n\n");

          if (fd == root.dev)
          {
              // change fd to mounted fd
            fd = mip->mptr->dev;
            //fd = root.dev;
            // return root of mounted fs
            return 2;

          }
          else
          {
            fd = root.dev;
            printf ("\n\n\nhere\n\n\n");
            return xino;
          }

          //return mip->mptr->mountDirPtr->ino;


        } // end if mounted

        //return dp->inode;
        return xino;

      } // end if we found the entry
      else
      {
        dp = (DIR *)((char *)dp + dp->rec_len);
      } // end else, not found

      printf ("not found: %s\n", temp);

  } // end loop

  printf ("not found: %s\n", temp);

  return 0;

} // end function search

// getino function : return inode number of pathname

int getino (char *pathname)
{
  int i = 0, ino, blk, disp, n = 0;
  MINODE *mip;
  dev = root.dev; // only ONE device so far
  char *token;
  char * myArgs[16];

  printf("getino: pathname=%s\n", pathname);

  if (strcmp (pathname, "/") == 0 )
  {
    return 2;
  }

  if (pathname[0] == '/')
  {
    mip = iget(dev, 2);
  }
  else
  {
    mip = iget(dev, running->cwd->ino);
  }

  //strcpy(buf, pathname);

  //tokenize(buf); // n = number of token strings

  i = 0;

  token = tokenize (pathname, "/");
  while (token != NULL)
  {
    myArgs[i] = token;
    token = tokenize (NULL, "/");
    i++;
    n++;
  }

  myArgs[i] = NULL;
      // end tokenize


  for (i = 0; i < n; i++)
  {
    printf("===========================================\n");
    printf("getino: i=%d myArgs[%d]=%s\n", i, i, myArgs[i]);

    ino = search(mip->INODE, myArgs[i]);

    if (ino == 0)
    {
       iput (mip);
       printf ("name %s does not exist\n", myArgs[i]);
       return 0;
    }

    iput (mip);

    mip = iget(dev, ino);
  } // end loop through all args
  iput (mip);

  curNode = ino;
  printf ("getino, curNode = %d\n", curNode);

  return ino;

} // end function getino
*/
// findmyname function
/*
int findmyname (MINODE *parent, u32 myino, char *myname)
{
  // return name string of myino: SAME as search except by myino;
  // copy entry name (string) into myname[];
} // end findmyname function

// findino function

int findino (MINODE *mip, u32 *myino)
{
  // fill myino with ino of .
  // return ino of ..

} // end findino functioni
*/
