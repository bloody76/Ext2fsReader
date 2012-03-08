#include "tty.h"

void launch_tty ()
{
  char  *line = NULL;
  char[3] PS1 = "$> ";

  while ((line = readline (PS1)) != NULL)
  {
    printf (line);
    free (line);
  }
}
