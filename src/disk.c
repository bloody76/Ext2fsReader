#include "ext2.h"
#include <stdlib.h>
#include <stdio.h>

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

void print_file (s_disk *disk,
                 uint   inode)
{
  s_inode     *tmp = get_inode (disk, inode);
  uint        size = tmp->size;
  int         *p = NULL;
  int         *pp = NULL;
  int         *ppp = NULL;
  char        *block = NULL;

  for (int i = 0; i < 12 && tmp->block[i]; i++)
  {
    block = get_block (disk, tmp->block[i]);
    printf ("%.*s", size > disk->block_size ? disk->block_size : size, block);
    size -= size > disk->block_size ? disk->block_size : size;
  }

  if (tmp->block[12] != 0)
  {
    p = get_block (disk, tmp->block[12]);
    for (int i = 0; i < disk->block_size / 4 && p[i]; i++)
    {
      block = get_block (disk, p[i]);
      printf ("%.*s", size > disk->block_size ? disk->block_size : size, block);
      size -= size > disk->block_size ? disk->block_size : size;
    }
  }
  if (tmp->block[13] != 0)
  {
    p = get_block (disk, tmp->block[13]);
    for (int i = 0; i < disk->block_size / 4 && p[i]; i++)
    {
      pp = get_block (disk, p[i]);
      for (int j = 0; j < disk->block_size / 4 && pp[j]; j++)
      {
        block = get_block (disk, pp[j]);
        printf ("%.*s", size > disk->block_size ? disk->block_size : size, block);
        size -= size > disk->block_size ? disk->block_size : size;
      }
    }
  }

  if (tmp->block[14] != 0)
  {
    p = get_block (disk, tmp->block[14]);
    for (int i = 0; i < disk->block_size / 4 && p[i]; i++)
    {
      pp = get_block (disk, p[i]);
      for (int j = 0; j < disk->block_size / 4 && pp[j]; j++)
      {
        ppp = get_block (disk, pp[i]);
        for (int k = 0; k < disk->block_size / 4 && ppp[k]; k++)
        {
          block = get_block (disk, ppp[k]);
          printf ("%.*s", size > disk->block_size ? disk->block_size : size, block);
          size -= size > disk->block_size ? disk->block_size : size;
        }
      }
    }
  }
  printf ("\n");
}
