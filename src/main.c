#include "ext2.h"
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
  s_super_block       *super_block = 0;
  s_inode             *root_inode = 0;


  if (argc < 2)
    return 1; // No args.

  if ((fd = open (argv[1], O_RDONLY)) == -1)
    return 2; // Bad file.

  fstat (fd, &buf);
  if ((fs = mmap (0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == (void*)(-1))
    return 2;

  super_block = (void*)((char*)fs + OFFSET_SUPER_BLOCK);
  // We check that the magic number is correct.
  if (super_block->magic != MAGIC_SUPER_BLOCK)
    return 3;

  root_inode = get_inode (fs, super_block, EXT2_ROOT_INO);
  s_directory *dir = get_dirs (fs, super_block, EXT2_ROOT_INO);

  munmap (fs, buf.st_size);

  return 0;
}
