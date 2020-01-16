// pipeTest

#include "util.c"

int main ()
{
  int pid, pd[2], i, n;
  char buf[1024];
  char line[256], *s = "~~~ data from pipe ~~~";
  char *t = "=== more data ===";

  pipe(pd);

  pid = fork ();

  //if (fork () )
  if (pid)
  // parent as writer:
  {
    printf ("parent %d close pd[0]\n", getpid () );
    close (pd[0]);
    //dup2 (pd[1], 1);
    i = 0;
    while (i++ < 5)
    {
      printf ("parent %d writing pipe : %s \n", getpid (), s);
      write (pd[1], s, strlen(s) );
      write (pd[1], t, strlen(t) );
    } // end while loop ten times
    printf ("parent %d exit\n", getpid () );
    exit (0);

  } // end parent
  // child as reader:
  else
  {
    printf ("child %d close pd[1]\n", getpid () );
    close (pd[1]);
    //dup2 (pd[0], 0);
    i = 0;
    while (i++ < 10)
    {
      printf ("child %d reading from pipe\n", getpid () );
      //n = read (pd[0], line, 256);

      if ( (n = read (pd[0], line, 256) ) == 0)
      {
        exit (0);
      } // end if no more reads
//printf ("***** here *****%c", '\n');
      line[n] = 0;

      printf ("%s  n = %d\n", line, n);
    } // end while loop
    exit (0);
  } // end child

  //exit (0);

} // end main function
