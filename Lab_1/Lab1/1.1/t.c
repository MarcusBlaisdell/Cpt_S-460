/*********  t.c file *********************/



int prints(char *s)
{
  int i = 0;
  while (s[i] != '\0')
  {
    putc(s[i++]);
  } // end while to print all characters in the string

  return;
} // end prints function

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

} // end gets function

char ans[64];

main()
{
  while(1)
  {
    prints("Please enter your designation: ");

    gets(ans);

      // print new line:
    prints("\n\r");

    if (ans[0] == 0)
    {
      prints("to assembly, return, and lock out\n\r");
        // return non-zero integer to indicate success
      return 1;
    } // end if

      // print the text the user entered:
    prints("Greetings "); prints(ans); prints("\n\r");

  } // end while true loop

} // end main
