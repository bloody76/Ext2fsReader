#include "ext2.h"

s_group_descriptor *get_group_descriptor(s_disk         *disk,
                                         uint           inode)
{
  return (void*)((char*)disk->fs
      + OFFSET_SUPER_BLOCK
      + sizeof (s_super_block)
      + (inode - 1) / disk->sblk->inodes_per_group);
}

s_inode *get_inode (s_disk        *disk,
                    uint          inode)
{
  s_group_descriptor  *gd = 0;
  int                 index = 0;
  int                 inodes_size = 128;

  gd = get_group_descriptor (disk, inode);
  index = (inode - 1) % disk->sblk->inodes_per_group;

  return (void*)((char*)get_block (disk, gd->inode_table)
      + index * inodes_size);
}

s_directory *get_dir (s_disk         *disk,
                      uint           inode,
                      uint           block)
{
  s_inode *i = get_inode (disk, inode);

  return (s_directory*)get_block (disk, i->block[block]);
}
