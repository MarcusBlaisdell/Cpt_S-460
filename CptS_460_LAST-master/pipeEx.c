// pipeEx.c

#include "util.c"

int main ()
{
  int pid, fd[2];
  int bytesRead;
  char buf[1024];
  int fp;

  pipe (fd);

  pid = fork ();

  if (pid)
  {
    close (fd[0]);
    dup2 (fd[1], 1);
    close (fd[1]);

    //exec ("cat testFile");
    exec ("cat file2");
  }
  else
  {
    close (fd[1]);
    dup2 (fd[0], 0);
    close (fd[0]);

    //exec ("grep three");
    //exec ("grep print");
    exec ("more");
  }

  return 0;

} // end main function
