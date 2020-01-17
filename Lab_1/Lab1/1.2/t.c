/*******************************************************
*                      t.c file                        *
*******************************************************/
typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned long  u32;

#define TRK 18
#define CYL 36
#define BLK 1024

#include "ext2.h"
typedef struct ext2_super_block SUPER;
typedef struct ext2_group_desc  GD;
typedef struct ext2_inode       INODE;
typedef struct ext2_dir_entry_2 DIR;

SUPER *sp;
GD    *gp;
INODE *ip;
DIR   *dp;

//char buf1[BLK], buf2[BLK];
char buf1[BLK];
int color = 0x0A;
u8 ino;
int Inodes_Per_Block, blk, offset;
u16 iblk;
int NSEC = 1;
//u32 BASE = 10;
//char * ctable = "0123456789ABCDEF";

Inodes_Per_Block = BLK / 256;

// FUNCTION ** mailman

void mailman (int theInode)
{
int i = 0;

blk = (theInode - 1) / Inodes_Per_Block + iblk;
offset = (theInode - 1) % Inodes_Per_Block;

getblk (blk, buf1);
ip = (INODE * ) buf1;

for (i = 0; i < offset; i++)
{
  ip++;
} // end increment to correct inode in correct block

} // end function mailman

// code from book, prints integers, modified BASE variable to be global
/*
int rpu (u32 x)
{
  char c;
  if (x)
  {
    c = ctable[x % BASE];

    rpu (x / BASE);

    putc (c);
  } // end if
} // end rpu
*/

main()
{
  //u16    i, iblk;
  u16    i;
  char   c, temp[64];
  int x, y;
  u16 eVal;

    // get group descriptor block:

  prints("read block# 2 (GD)\n\r");
  getblk(2, buf1);

// 1. WRITE YOUR CODE to get iblk = bg_inode_table block number
    // cast the contents of the buffer to a GD type:
  gp = (GD *) buf1;
    // get the bg_inode_table as the Inodes begin block number:
  iblk = gp->bg_inode_table;

  prints("inode_block="); putc(iblk+'0');
  prints("\n\r");

// 2. WRITE YOUR CODE to get root inode
  prints("read inodes begin block to get root inode\n\r");

  getblk (iblk, buf1);
  ip = (INODE *) buf1;

// 3. WRITE YOUR CODE to step through the data block of root inode
   prints("read data block of root DIR\n\r");

   mailman (2);
   prints ("read i_block[1]\n\r");
   getblk (ip->i_block[13], buf1);
   dp = (DIR *) buf1;
   /*
   temp[0] = 'y';
   temp[1] = dp->name[0];
   temp[2] = 'z';
   temp[3] = 0;
   prints ("test\n\r");
   prints ( temp);
   */
   for (x = 0; x < dp->name_len; x++)
   {
     temp[x] = dp->name[x];
     putc (temp[x]);
   }
   temp[dp->name_len] = 0;
   prints ("\n\rx\n\r");
   prints (temp);

   /*
   for (y = 0; y < 10; y++)
   {
     for (x = 0; x < dp->name_len; x++)
     {
       /*
       putc(dp->name[0]);
       prints ("\n\r");
       putc(dp->name[1]);
       prints ("\n\r");
       putc(dp->name[2]);
       prints ("\n\r");
       prints ("here\n\r");

       temp[x] = dp->name[x];
     }
     temp[dp->name_len] = '0';
     prints (temp[0]);
     dp = (DIR *) (char *) dp + dp->rec_len;
   }
   */

// 4. print file names in the root directory /

}

int prints(char *s)
{
  int i = 0;
    // read the string until termination character

  while (s[i] != '\0')
  {
    putc(s[i++]);
  } // end while to print all characters in the string

    // return 0 to hang the program
  return;
}

int gets(char *s)
{
  int i = 0;

    // read input until we see a <return>:
  while (s[i - 1] != '\r')
  {
    s[i] = getc();
    putc(s[i]);
    i++;
  } // end while loop

    // terminate the string:
  s[i - 1] = '\0';

  return;
}

int getblk(u16 blk, char *buf)
{
  // readfd( (2*blk)/CYL, ( (2*blk)%CYL)/TRK, ((2*blk)%CYL)%TRK, buf);
  readfd( blk/18, ((blk)%18)/9, ( ((blk)%18)%9)<<1, buf);
}
