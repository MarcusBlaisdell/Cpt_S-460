// pipeTest

#include "util.c"

int main ()
{
  int pid, pd[2], i, n, fd;
  char buf[1024];

  pipe(pd);

  pid = fork ();

  //if (fork () )
  if (pid)
  // parent as writer:
  {
    printf ("child %d close pd[1]\n", getpid () );
    close (pd[1]);
    dup2 (pd[0], 0);
    close (pd[0]);

    printf ("child %d reading from pipe\n", getpid () );

    exec ("l2u");


  } // end parent
  // child as reader:
  else
  {
    printf ("parent %d close pd[0]\n", getpid () );
    close (pd[0]);
    dup2 (pd[1], 1);
    close (pd[1]);

    //exec ("cat testFile");
    exec ("cat testFile");

  } // end child

} // end main function
