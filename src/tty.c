#include "tty.h"

static const char format[13] =
{
  [1] = 'f',
  [2] = 'c',
  [4] = 'd',
  [6] = 'b',
  [8] = '-',
  [10] = 's',
  [12] = 'l',
};

static const s_command commands[MAX_FUNCS] =
{
  {"help", "Display the commands and their descriptions.", &help},
  {"ls", "List all the files and directories of the actual directory.", &ls},
  {"linode", "Display the informations of the inode specified.", &linode},
  {"cd", "Change of directory.", &cd},
  {"cat", "Print the file to the ouput.", &cat},
};

int help (s_disk *disk,
          int    argc,
          char   *argv[5])
{
  printf ("Commands:\n");
  for (int i = 0; i< MAX_FUNCS; i++)
    printf ("\t%s\t: %s\n", commands[i].name, commands[i].comment);
  return 0;
}

void print_mode (uint mode)
{
  if (format[(mode & S_IFMT) >> 12] != '\0')
    printf (&format[(mode & S_IFMT) >> 12]);
  else
    printf ("-");
  if (mode & S_IRUSR)
    printf ("r");
  else
    printf ("-");
  if (mode & S_IWUSR)
    printf ("w");
  else
    printf ("-");
  if (mode & S_IXUSR)
    printf ("x");
  else
    printf ("-");

  if (mode & S_IRGRP)
    printf ("r");
  else
    printf ("-");
  if (mode & S_IWGRP)
    printf ("w");
  else
    printf ("-");
  if (mode & S_IXGRP)
    printf ("x");
  else
    printf ("-");

  if (mode & S_IROTH)
    printf ("r");
  else
    printf ("-");
  if (mode & S_IWOTH)
    printf ("w");
  else
    printf ("-");
  if (mode & S_IXOTH)
    printf ("x");
  else
    printf ("-");
  printf (" ");
}

int ls (s_disk *disk,
        int    argc,
        char   *argv[5])
{
  s_directory *dir = NULL;
  s_inode     *tmp = NULL;
  int         *p = NULL;
  int         *pp = NULL;
  int         *ppp = NULL;

  for (int i = 0; i < 12 && g_tty.curr_dir->block[i]; i++)
  {
    dir = get_dir (disk, g_tty.curr_inode, i);
    while (dir && dir->name_len
        && !(!strcmp (dir->name, ".") && dir->inode != g_tty.curr_inode))
    {
      tmp = get_inode (disk, dir->inode);

      print_mode (tmp->mode);
      printf ("%u ", tmp->mode);
      printf ("%5.u ", tmp->size);
      printf ("%3.u ", dir->inode);
      printf ("%.*s\n", (char)dir->name_len, dir->name);
      dir = (void*)((char*)dir + dir->rec_len);
    }
  }

  if (g_tty.curr_dir->block[12] != 0)
  {
    p = get_block (disk, g_tty.curr_dir->block[12]);
    for (int i = 0; i < disk->block_size / 4 && p[i]; i++)
    {
      dir = get_block (disk, p[i]);
      while (dir && dir->name_len
          && !(!strcmp (dir->name, ".") && dir->inode != g_tty.curr_inode))
      {
        tmp = get_inode (disk, dir->inode);

        print_mode (tmp->mode);
        printf ("%u ", tmp->mode);
        printf ("%5.u ", tmp->size);
        printf ("%3.u ", dir->inode);
        printf ("%.*s\n", (char)dir->name_len, dir->name);
        dir = (void*)((char*)dir + dir->rec_len);
      }
    }
  }
  if (g_tty.curr_dir->block[13] != 0)
  {
    p = get_block (disk, g_tty.curr_dir->block[13]);
    for (int i = 0; i < disk->block_size / 4 && p[i]; i++)
    {
      pp = get_block (disk, p[i]);
      for (int j = 0; j < disk->block_size / 4 && pp[j]; j++)
      {
        dir = get_block (disk, pp[j]);
        while (dir && dir->name_len
            && !(!strcmp (dir->name, ".") && dir->inode != g_tty.curr_inode))
        {
          tmp = get_inode (disk, dir->inode);

          print_mode (tmp->mode);
          printf ("%u ", tmp->mode);
          printf ("%5.u ", tmp->size);
          printf ("%3.u ", dir->inode);
          printf ("%.*s\n", (char)dir->name_len, dir->name);
          dir = (void*)((char*)dir + dir->rec_len);
        }
      }
    }
  }

  if (g_tty.curr_dir->block[14] != 0)
  {
    p = get_block (disk, g_tty.curr_dir->block[14]);
    for (int i = 0; i < disk->block_size / 4 && p[i]; i++)
    {
      pp = get_block (disk, p[i]);
      for (int j = 0; j < disk->block_size / 4 && pp[j]; j++)
      {
        ppp = get_block (disk, pp[i]);
        for (int k = 0; k < disk->block_size / 4 && ppp[k]; k++)
        {
          dir = get_block (disk, ppp[k]);
          while (dir && dir->name_len
              && !(!strcmp (dir->name, ".") && dir->inode != g_tty.curr_inode))
          {
            tmp = get_inode (disk, dir->inode);

            print_mode (tmp->mode);
            printf ("%u ", tmp->mode);
            printf ("%5.u ", tmp->size);
            printf ("%3.u ", dir->inode);
            printf ("%.*s\n", (char)dir->name_len, dir->name);
            dir = (void*)((char*)dir + dir->rec_len);
          }
        }
      }
    }
  }

  return 0;
}

int cat (s_disk  *disk,
         int     argc,
         char    *argv[5])
{
  s_directory *dir = NULL;
  s_inode     *tmp = NULL;
  int         *p = NULL;
  int         *pp = NULL;
  int         *ppp = NULL;

  if (argc < 2)
    return printf ("You must specify a file.\n");

  for (int i = 0; i < 12 && g_tty.curr_dir->block[i]; i++)
  {
    dir = get_dir (disk, g_tty.curr_inode, i);
    while (dir && !(!strcmp (dir->name, ".")
          && dir->inode != g_tty.curr_inode))
    {
      tmp = get_inode (disk, dir->inode);

      if (!strncmp (argv[1], dir->name, (char)dir->name_len)
          && argv[1][(char)dir->name_len] == '\0')
      {
        print_file (disk, dir->inode);
        return 0;
      }
      dir = (void*)((char*)dir + dir->rec_len);
    }
  }

  if (g_tty.curr_dir->block[12] != 0)
  {
    p = get_block (disk, g_tty.curr_dir->block[12]);
    for (int i = 0; i < disk->block_size / 4 && p[i]; i++)
    {
      dir = get_block (disk, p[i]);
      while (dir && !(!strcmp (dir->name, ".")
            && dir->inode != g_tty.curr_inode))
      {
        tmp = get_inode (disk, dir->inode);

        if (!strncmp (argv[1], dir->name, (char)dir->name_len)
            && argv[1][(char)dir->name_len] == '\0')
        {
          print_file (disk, dir->inode);
          return 0;
        }
        dir = (void*)((char*)dir + dir->rec_len);
      }
    }
  }
  if (g_tty.curr_dir->block[13] != 0)
  {
    p = get_block (disk, g_tty.curr_dir->block[13]);
    for (int i = 0; i < disk->block_size / 4 && p[i]; i++)
    {
      pp = get_block (disk, p[i]);
      for (int j = 0; j < disk->block_size / 4 && pp[j]; j++)
      {
        dir = get_block (disk, pp[j]);
        while (dir && !(!strcmp (dir->name, ".")
              && dir->inode != g_tty.curr_inode))
        {
          tmp = get_inode (disk, dir->inode);

          if (!strncmp (argv[1], dir->name, dir->name_len)
              && argv[1][(char)dir->name_len] == '\0')
          {
            print_file (disk, dir->inode);
            return 0;
          }
          dir = (void*)((char*)dir + dir->rec_len);
        }
      }
    }
  }

  if (g_tty.curr_dir->block[14] != 0)
  {
    p = get_block (disk, g_tty.curr_dir->block[14]);
    for (int i = 0; i < disk->block_size / 4 && p[i]; i++)
    {
      pp = get_block (disk, p[i]);
      for (int j = 0; j < disk->block_size / 4 && pp[j]; j++)
      {
        ppp = get_block (disk, pp[i]);
        for (int k = 0; k < disk->block_size / 4 && ppp[k]; k++)
        {
          dir = get_block (disk, ppp[k]);
          while (dir && !(!strcmp (dir->name, ".")
                && dir->inode != g_tty.curr_inode))
          {
            tmp = get_inode (disk, dir->inode);

            if (!strncmp (argv[1], dir->name, dir->name_len)
                && argv[1][(char)dir->name_len] == '\0')
            {
              print_file (disk, dir->inode);
              return 0;
            }
            dir = (void*)((char*)dir + dir->rec_len);
          }
        }
      }
    }
  }
  return 1;
}
int cd (s_disk  *disk,
        int     argc,
        char    *argv[5])
{
  s_directory *dir = NULL;
  s_inode     *tmp = NULL;
  int         *p = NULL;
  int         *pp = NULL;
  int         *ppp = NULL;

  if (argc < 2)
    return printf ("You must specify a directory.\n");

  for (int i = 0; i < 12 && g_tty.curr_dir->block[i]; i++)
  {
    dir = get_dir (disk, g_tty.curr_inode, i);
    while (dir && !(!strcmp (dir->name, ".")
          && dir->inode != g_tty.curr_inode))
    {
      tmp = get_inode (disk, dir->inode);

      if (!strncmp (argv[1], dir->name, dir->name_len)
          && argv[1][(char)dir->name_len] == '\0')
      {
        if (!(tmp->mode & S_IFDIR))
          return printf ("%s is not a directory.\n", argv[1]);
        g_tty.curr_inode = dir->inode;
        g_tty.curr_dir = tmp;
        return 0;
      }
      dir = (void*)((char*)dir + dir->rec_len);
    }
  }

  if (g_tty.curr_dir->block[12] != 0)
  {
    p = get_block (disk, g_tty.curr_dir->block[12]);
    for (int i = 0; i < disk->block_size / 4 && p[i]; i++)
    {
      dir = get_block (disk, p[i]);
      while (dir && !(!strcmp (dir->name, ".")
            && dir->inode != g_tty.curr_inode))
      {
        tmp = get_inode (disk, dir->inode);

        if (!strncmp (argv[1], dir->name, dir->name_len)
            && argv[1][(char)dir->name_len] == '\0')
        {
          if (!(tmp->mode & S_IFDIR))
            return printf ("%s is not a directory.\n", argv[1]);
          g_tty.curr_inode = dir->inode;
          g_tty.curr_dir = tmp;
          return 0;
        }
        dir = (void*)((char*)dir + dir->rec_len);
      }
    }
  }
  if (g_tty.curr_dir->block[13] != 0)
  {
    p = get_block (disk, g_tty.curr_dir->block[13]);
    for (int i = 0; i < disk->block_size / 4 && p[i]; i++)
    {
      pp = get_block (disk, p[i]);
      for (int j = 0; j < disk->block_size / 4 && pp[j]; j++)
      {
        dir = get_block (disk, pp[j]);
        while (dir && !(!strcmp (dir->name, ".")
              && dir->inode != g_tty.curr_inode))
        {
          tmp = get_inode (disk, dir->inode);

          if (!strncmp (argv[1], dir->name, dir->name_len)
              && argv[1][(char)dir->name_len] == '\0')
          {
            if (!(tmp->mode & S_IFDIR))
              return printf ("%s is not a directory.\n", argv[1]);
            g_tty.curr_inode = dir->inode;
            g_tty.curr_dir = tmp;
            return 0;
          }
          dir = (void*)((char*)dir + dir->rec_len);
        }
      }
    }
  }

  if (g_tty.curr_dir->block[14] != 0)
  {
    p = get_block (disk, g_tty.curr_dir->block[14]);
    for (int i = 0; i < disk->block_size / 4 && p[i]; i++)
    {
      pp = get_block (disk, p[i]);
      for (int j = 0; j < disk->block_size / 4 && pp[j]; j++)
      {
        ppp = get_block (disk, pp[i]);
        for (int k = 0; k < disk->block_size / 4 && ppp[k]; k++)
        {
          dir = get_block (disk, ppp[k]);
          while (dir && !(!strcmp (dir->name, ".")
                && dir->inode != g_tty.curr_inode))
          {
            tmp = get_inode (disk, dir->inode);

            if (!strncmp (argv[1], dir->name, dir->name_len)
                && argv[1][(char)dir->name_len] == '\0')
            {
              if (!(tmp->mode & S_IFDIR))
                return printf ("%s is not a directory.\n", argv[1]);
              g_tty.curr_inode = dir->inode;
              g_tty.curr_dir = tmp;
              return 0;
            }
            dir = (void*)((char*)dir + dir->rec_len);
          }
        }
      }
    }
  }
  return 1;
}

int linode (s_disk  *disk,
            int     argc,
            char    *argv[5])
{
  char*   arg = NULL;
  s_inode *inode = NULL;
  uint    in;

  if (argc < 2)
    return printf ("You have to specify the inode.\n");

  arg = argv[1];

  in = atoi (arg);
  if (in < 2)
    return printf ("%s is not a valid inode.\n", arg);

  inode = get_inode (disk, in);
  printf ("mode:\t\t%u\n", inode->mode);
  printf ("owner uid:\t%u\n", inode->uid);
  printf ("size:\t\t%u\n", inode->size);
  printf ("atime:\t\t%u\n", inode->atime);
  printf ("ctime:\t\t%u\n", inode->ctime);
  printf ("dtime:\t\t%u\n", inode->dtime);
  printf ("gid:\t\t%u\n", inode->gid);
  printf ("links_count:\t%u\n", inode->links_count);
  printf ("blocks:\t\t%u\n", inode->blocks);
  printf ("flags:\t\t%u\n", inode->flags);
  printf ("reserved1:\t%u\n", inode->reserved1);
  printf ("version:\t%u\n", inode->version);
  printf ("file_acl:\t%u\n", inode->file_acl);
  printf ("dir_acl:\t%u\n", inode->dir_acl);
  printf ("faddr:\t\t%u\n", inode->faddr);
  printf ("blocks:\t\t%u", inode->block[0]);
  for (int i = 1; i < 15; i++)
    printf (" %u", inode->block[i]);
  printf ("\n");

  return 0;
}

void launch_tty (s_disk *disk)
{
  char        *line = NULL;
  char        PS1[4] = "$> ";

  g_tty.curr_dir = get_inode (disk, EXT2_ROOT_INO);
  g_tty.curr_inode = EXT2_ROOT_INO;

  while ((line = readline (PS1)) != NULL)
  {
    char  *l = strtok (line, " ");
    int   argc = 1;
    char  *argv[5];
    while (l = strtok (NULL, " "),
        l != NULL && argc < 6)
    {
      argv[argc] = l;
      argc++;
    }
    for (int i = 0; i < MAX_FUNCS; i++)
    {
      if (!strcmp (commands[i].name, line))
      {
        argv[0] = commands[i].name;
        (*commands[i].function)(disk, argc, argv);
        break;
      }
    }
    free (line);
  }
}
