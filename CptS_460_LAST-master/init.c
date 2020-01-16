// init.c
// Modified from original version provided by K.C. Wang
// the original version opens one terminal,
// the modified version opens three terminals that the user can login to and use 

//#include "ucode.c"
#include "util.c"

	// must run on 3 terminals:

int console1, console2, console3;

	// parent function for main terminal

int parent()		// P1's code
{
	int pid, status;

	while (1)
	{
		printf ("INIT : wait for ZOMBIE child%c\n", ' ');
		pid = wait (&status);

			// if console1 login process died
		if (pid == console1)
		{
			printf ("INIT-1: forks a new console1 login%c\n", ' ');
			console1 = fork ();	// fork another one

			if (console1)
			{
				continue;
			} // end if console1 is valid
			else
			{
				exec ("login /dev/tty0");	// new console1 login process
			} // end else

		} // end if pid == console1

			// if console2 login process died
		if (pid == console2)
		{
			printf ("INIT-2: forks a new console1 login%c\n", ' ');
			console2 = fork ();	// fork another one

			if (console2)
			{
				continue;
			} // end if console1 is valid
			else
			{
				exec ("login /dev/ttyS0");	// new console1 login process
			} // end else

		} // end if pid == console2

			// if console3 login process died
		if (pid == console3)
		{
			printf ("INIT-3: forks a new console1 login%c\n", ' ');
			console2 = fork ();	// fork another one

			if (console3)
			{
				continue;
			} // end if console1 is valid
			else
			{
				exec ("login /dev/ttyS1");	// new console1 login process
			} // end else

		} // end if pid == console3

		printf ("INIT: I just buried an orphan child proc %d\n", pid);


	} // end while

} // end parent function

main ()
{
	int in, out, err;						// file descriptors for terminal I/O - console 1
	int in2, out2;						// file descriptors for terminal I/O - console 2
	int in3, out3;						// file descriptors for terminal I/O - console 3


	in = open ("/dev/tty0", O_RDONLY); 	// file descriptor 0
	out = open ("dev/tty0", O_WRONLY);	// for display to console1

	in2 = open ("/dev/ttyS0", O_RDONLY); 	// file descriptor 0
	out2 = open ("dev/ttyS0", O_WRONLY);	// for display to console1

	in3 = open ("/dev/ttyS0", O_RDONLY); 	// file descriptor 0
	out3 = open ("dev/ttyS0", O_WRONLY);	// for display to console1

	printf ("INIT : fork a login proc on console1%c\n", ' ');

	console1 = fork ();

	if (!console1)
	{
		exec ("login /dev/tty0");

	} // end if console1 died, relaunch it

	console2 = fork ();

	if (!console2)
	{
		exec ("login /dev/ttyS0");

	} // end if console2 died, relaunch it

	console3 = fork ();

	if (!console3)
	{
		exec ("login /dev/ttyS1");

	} // end if console3 died, relaunch it


	parent ();

} // end main
