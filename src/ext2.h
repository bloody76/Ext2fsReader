#ifndef EXT2_H_
# define EXT2_H_

# define OFFSET_SUPER_BLOCK 1024
# define MAGIC_SUPER_BLOCK 0xEF53
# define BLOCK_SIZE(LogBlockSize) \
  (1024 << LogBlockSize)

typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;

typedef struct
{
  // Count of inodes in the filesystem.
  uint   inodes_count;
  // Count of blocks in the filesystem.
  uint   blocks_count;
  // Count of the number of reserved blocks.
  uint   r_blocks_count;
  // Count of the number of free blocks.
  uint   free_blocks_count;
  // Count of the number of free inodes.
  uint   free_nodes_count;
  // The first block which contains data.
  uint   first_data_block;
  // Indicator of the block size.
  uint   log_block_size;
  // Indicator of the size of the fragments.
  uint   log_frag_size;
  // Count the number of blocks per group.
  uint   blocks_per_group;
  // Count of the number of fragments in each block group.
  uint   frags_per_group;
  // Count of the number of inodes in each block group.
  uint   inodes_per_group;
  // The time that the filesystem was last mounted.
  uint   mtime;
  // The time that the filesystem was last written to.
  uint   wtime;
  // The number of times the file system has been mounted.
  ushort mnt_count;
  // The number of times the file system can be mounted.
  short max_mnt_count;
  // Magic number indicating ex2fs.
  ushort magic;
  // Flags indicating the current state of the filesystem.
  ushort state;
  // Flags indicating the procedures for error reporting.
  ushort errors;
  // Minor portion of version.
  ushort min_rev_level;
  // The time that the filesystem was last checked.
  uint   last_check;
  // The maximum time permissible between checks.
  uint   check_interval;
  // Indicator of which OS created the filesystem.
  uint   creator_os;
  // Major portion of version.
  uint   rev_level;
  // User id.
  ushort usr_id;
  // Group id.
  ushort grp_id;
  // Padding to 1024 bytes.
  uint   reserved[235];
}__attribute__((packed)) s_super_block;

typedef struct
{
  // The address of the block containing.
  uint   block_bitmap;
  // the block bitmap for this group.
  uint   inode_bitmap;
  // The address of the block containing the inode bitmap for this group.
  uint   inode_table;
  // The count of free blocks in this group.
  ushort free_blocks_count;
  // The count of free inodes in this group.
  ushort free_inodes_count;
  // The number inodes in this group which are directories.
  ushort used_dirs_count;
  // Padding.
  ushort pad;
  // Padding;
  uint   reserved[3];
}__attribute__((packed)) s_group_descriptor;

typedef struct
{
  // File mode.
  ushort mode;
  // Owner uid.
  ushort uid;
  // Size in bytes.
  uint   size;
  // Access time.
  uint   atime;
  // Creation time.
  uint   ctime;
  // Modification time.
  uint   mtime;
  // Deletion time.
  uint   dtime;
  // Group id.
  ushort gid;
  // Links count.
  ushort links_count;
  // Blocks count.
  uint   blocks;
  // File flags.
  uint   flags;
  // Os dependent.
  uint   reserved1;
  // Pointer to blocks.
  uint   block[15];
  // File version (for NFS).
  uint   version;
  // File ACL.
  uint   file_acl;
  // Directory ACL.
  uint   dir_acl;
  // Fragment address.
  uint   faddr;
  // Fragment number.
  uchar  frag;
  // Fragment size.
  uchar  fsize;
  // Padding.
  ushort pad1;
  // Padding.
  uint   reserved2[2];
}__attribute__ ((packed)) s_inode;

typedef struct
{
  // Address if inode.
  uint    inode;
  // Length of this record.
  ushort  rec_len;
  // Length of file name.
  ushort  name_len;
  // The file name.
  char    name[0];
}__attribute__ ((packed)) s_directory;

typedef struct
{
  void          *fs;
  s_super_block *sblk;
  uint          block_size;
}               s_disk;

enum e_file_mode
{
  S_IFMT    = 0xF000, // Format mask.
  S_IFSOCK  = 0xA000, // Socket.
  S_IFLNK   = 0xC000, // Symbol link.
  S_IFREG   = 0x8000, // Regular file.
  S_IFBLK   = 0x6000, // Block device.
  S_IFDIR   = 0x4000, // Directory.
  S_IFCHR   = 0x2000, // Character device.
  S_IFIFO   = 0x1000, // FIFO.

  S_ISUID   = 0x0800, // SUID.
  S_ISGID   = 0x0400, // SGID.
  S_ISVTX   = 0x0200, // Sticky bit.

  S_IRWXU   = 0x01C0, // User mask.
  S_IRUSR   = 0x0100, // Read.
  S_IWUSR   = 0x0080, // Write.
  S_IXUSR   = 0x0040, // Execute.

  S_IRWXG   = 0x0038, // Group mask.
  S_IRGRP   = 0x0020, // Read.
  S_IWGRP   = 0x0010, // Write.
  S_IXGRP   = 0x0008, // Execute.

  S_IRWXO   = 0x0007, // Other mask.
  S_IROTH   = 0x0004, // Read.
  S_IWOTH   = 0x0002, // Write.
  S_IXOTH   = 0x0001, // Execute.
};

enum e_special_inodes
{
  EXT2_BAD_INO          = 1, // Bad blocks inode.
  EXT2_ROOT_INO         = 2, // Root inodes.
  EXT2_ACL_IDX_INO      = 3, // ACL inode.
  EXT2_ACL_DATA_INO     = 4, // ACL inode.
  EXT2_BOOT_LOADER_INO  = 5, // Boot loader inode.
  EXT2_UNDEL_DIR_INO    = 6, // Undelete directory inode.
  EXT2_FIRST_INO        = 11,// First non reserved inode.
};


void init_disk (s_disk  *disk,
                void    *fs);

void  *get_block (const s_disk *disk,
                  const uint   pointer);

void print_file (s_disk *disk,
                 uint   inode);

s_group_descriptor *get_group_descriptor(s_disk         *disk,
                                         uint           inode);

s_inode *get_inode (s_disk        *disk,
                    uint          inode);

s_directory *get_dir (s_disk         *disk,
                      uint           inode,
                      uint           block);

#endif /* !EXT2_H_ */
