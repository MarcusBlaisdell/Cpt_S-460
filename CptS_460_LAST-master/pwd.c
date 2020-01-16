// pwd.c

#include "util.c"

  // main function

  int main (int argc, char *argv[])
  {
    char cwd[1024];
    getcwd (cwd);
    printf ("%s%c", cwd, '\n');

  } // end main function
