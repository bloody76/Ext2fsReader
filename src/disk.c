#include "ext2.h"

void init_disk (s_disk *disk,
                void   *fs)
{
  disk->fs = fs;
  disk->sblk = (void*)((char*)fs + OFFSET_SUPER_BLOCK);
  disk->block_size = BLOCK_SIZE (disk->sblk->log_block_size);
}

void  *get_block (const s_disk *disk,
                  const uint   pointer)
{
  return (void*)((char*)disk->fs + pointer * disk->block_size);
}
