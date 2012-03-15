#ifndef TTY_H_
# define TTY_H_

# include "ext2.h"
# include <stdlib.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>

# define MAX_FUNCS 5

typedef int (*f_func)(s_disk*, int argc, char*[5]);

typedef struct
{
  char *name;
  char *comment;
  f_func     function;
}            s_command;

typedef struct
{
  s_inode   *curr_dir;
  uint      curr_inode;
}           s_tty;

static s_tty g_tty;

void launch_tty (s_disk*);

int help (s_disk    *disk, int argc, char *argv[5]);
int ls (s_disk      *disk, int argc, char *argv[5]);
int linode (s_disk  *disk, int argc, char *argv[5]);
int cd (s_disk      *disk, int argc, char *argv[5]);
int cat (s_disk     *disk, int argc, char *argv[5]);

#endif /* !TTY_H_ */
