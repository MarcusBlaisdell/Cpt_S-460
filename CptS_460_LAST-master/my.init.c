// init.c
#include "ucode.c"
#include "uart.c"

int console;

	// parent function

int parent()		// P1's code
{
	int pid, status;

	while (1)
	{
		printf ("INIT : wait for ZOMBIE child\n");
		pid = wait (&status);

			// if console login process died
		if (pid == console)
		{
			printf ("INIT: forks a new console login\n");
			console = fork ();	// fork another one

			if (console)
			{
				continue;
			} // end if console is valid
			else
			{
				exec ("login /dev/tty0");	// new console login process
			} // end else

		} // end if pid == console

		printf ("INIT: I just buried an orphan child proc %d\n", pid);

	} // end while

} // end parent function

//UART *up;

main ()
{
	int sh, pid, status;
	int in, out;						// file descriptors for terminal I/O
	int i;
	char theString[32];

	//uart_init ();

	in = open ("/dev/tty0", O_RDONLY); 	// file descriptor 0
	out = open ("dev/tty0", O_WRONLY);	// for display to console

	printf ("INIT : fork a login proc on console\n");

	console = fork ();

	if (console)
	{
		parent ();
	} // end if console is valid
	else
	{
		exec ("login /dev/tty0");

	} // end else

} // end main
