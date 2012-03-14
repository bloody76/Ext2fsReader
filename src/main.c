#include "ext2.h"
#include "tty.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>

int main (int argc, char **argv)
{
  int                 fd = -1;
  struct stat         buf;
  void                *fs = 0;
  s_disk              disk;
  s_super_block       *super_block = 0;
  s_inode             *root_inode = 0;


  if (argc < 2)
    return 1; // No args.

  if ((fd = open (argv[1], O_RDONLY)) == -1)
    return 2; // Bad file.

  fstat (fd, &buf);
  if ((fs = mmap (0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == (void*)(-1))
    return 2;

  init_disk (&disk, fs);

  // We check that the magic number is correct.
  if (disk.sblk->magic != MAGIC_SUPER_BLOCK)
    return 3;

  launch_tty (&disk);

  munmap (fs, buf.st_size);


  return 0;
}
