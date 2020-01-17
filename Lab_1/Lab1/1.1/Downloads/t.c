/*********  t.c file *********************/



int prints(char *s)
{
  int i = 0;
  while (c[i] != '\0')
  {
    putchar(c[i++]);
  } // end while to print all characters in the string

  return 0;
} // end prints function

int gets(char *s)
{
  int i = 0;
  while (c[i] != '\0')
  {
    getchar(c[i++]);
  } // end while loop

} // end gets function

char ans[64];

main()
{
  while(1)
  {
    //prints("What's your name? ");
    prints("Please enter your designation: ");
    gets(ans);  prints("\n\r");

    if (ans[0]==0)
    {
      prints("return to assembly and hang\n\r");
      return;
    } // end if

    //prints("Welcome "); prints(ans); prints("\n\r");
    prints("Greetings "); prints(ans); prints("\n\r");
  } // end while true loop

} // end main
