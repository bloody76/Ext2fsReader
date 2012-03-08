#include "ext2.h"

s_group_descriptor *get_group_descriptor(void           *fs,
                                         s_super_block  *blk,
                                         uint           inode)
{
  return (void*)((char*)fs
      + OFFSET_SUPER_BLOCK
      + sizeof (s_super_block)
      + (inode - 1) / blk->inodes_per_group);
}

s_inode *get_inode (void          *fs,
                    s_super_block *blk,
                    uint          inode)
{
  s_group_descriptor  *gd = 0;
  int                 index = 0;
  int                 inodes_size = 128;

  gd = get_group_descriptor (fs, blk, inode);
  index = (inode - 1) % blk->inodes_per_group;

  return ((void*)((char*)fs
      + gd->inode_table * BLOCK_SIZE(blk->log_block_size)
      + index * inodes_size));
}
