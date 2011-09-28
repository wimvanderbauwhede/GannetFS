#ifndef _MYFS_H
#define _MYFS_H 
#include <cstring>
#include <errno.h>
#include "head.h"




/*
  Size of inode shall power of two.
  Changes on inode's size only affect number of direct 
  data block.In any case you want to change configuration of 
  direct, indirect, and indirect2 data blocks, 
  you should change those fieltr manually
*/ //size of inode
#define INODE_SIZE 32

//size of bytes contained on a single block
#define BLOCK_SIZE 64

//number max file descriptor
#define FD_MAX 16

//number of block on a file system
#define TOTAL_BLOCKS 128

//number of inode on a file system
#define TOTAL_INODES 32

//number inode on a block
#define INODE_PER_BLOCK (BLOCK_SIZE)/sizeof(inode)

//number max data block
#define MAX_DATA_BLOCKS (TOTAL_BLOCKS - (TOTAL_INODES/INODE_PER_BLOCK))



//start number of data block
#define BLOCK_START (TOTAL_INODES/INODE_PER_BLOCK)

//number of data block on a file system
#define NUMBER_BLOCKS (TOTAL_BLOCKS - (TOTAL_INODES / ((BLOCK_SIZE)/sizeof(inode))))


//number of direct array
#define BLOCKS_DIRECT 3

// The maximum data offset for data directly accessed from the node
#define RAMFS_DIRECT_MAX (BLOCKS_DIRECT*BLOCK_SIZE)

// The number of indirect pointers that can be stored in a single data block
#define RAMFS_INDIRECT_PER_BLOCK (BLOCK_SIZE/sizeof(block *))

// Number of bytes contained in a one level indirect block
#define RAMFS_INDIRECT1_BLOCK (RAMFS_INDIRECT_PER_BLOCK* BLOCK_SIZE)

// number of bytes contained in a two level indirect block
#define RAMFS_INDIRECT2_BLOCK (RAMFS_INDIRECT_PER_BLOCK*RAMFS_INDIRECT_PER_BLOCK*BLOCK_SIZE)

// The maximum data offset for data accessed from the single level indirect blocks
#define RAMFS_INDIRECT1_MAX (RAMFS_DIRECT_MAX+RAMFS_INDIRECT1_BLOCK)

// The maximum data offset for data accessed from the two level indirect blocks
#define RAMFS_INDIRECT2_MAX (RAMFS_INDIRECT1_MAX+RAMFS_INDIRECT2_BLOCK)

// Then maximum file or directory name length 
#define MAX_NAME_LENGTH 20


typedef unsigned char uint8;

struct block
{
	char array[BLOCK_SIZE];
};


struct inode
{
    GannetFS::mode_t            mode;           // node type
    //GannetFS::uid_t		uid;		// user id
    //GannetFS::guid_t		guid;		// group id
    size_t              	size;           // size of file in bytes
    GannetFS::nlink_t		nlink;
    block         		*direct[BLOCKS_DIRECT];
    block         		**indirect1[1];
    block         		***indirect2[1];

};

struct dirent
{
    inode          	*node;          // pointer to node
    unsigned short      inuse:1,        // entry in use?
                        first:1,        // first directory entry fragment?
                        last:1,         // last directory entry fragment?
                        namelen:8;      // bytes in whole name
    int		next;
    char                name[MAX_NAME_LENGTH];
};

struct translator
{
    inode         	*dir;           // directory to search
    const char          *path;          // path to follow
    inode          	*node;          // Node found
    const char          *name;          // last name fragment used
    int                 namelen;        // name fragment length
    bool            	last;           // last name in path?
};


struct filestat
{
    GannetFS::ino_t		st_ino;		// inode number
    GannetFS::mode_t		st_mode;	// protection
    GannetFS::nlink_t		st_nlink;	// number of hard links
    int			st_size;	// total size, inbytes
    GannetFS::blksize_t		st_blksize;	// blocksize for filesystem I/O
    GannetFS::blkcnt_t		st_blkcnt;	// number of blocks allocated
 };

struct frecord
{
    int			f_flag;		// file state
    int		f_offset;	// current offset
    inode*              f_dir;		// inode
};

typedef frecord DIR;


namespace GannetFS
{
// basic operation
void init_blocks();
block *block_alloc(void);
void free_ablock( block *b );
int mount( );
int unmount    ( );

//inode operation
int get_block_node( inode  *node,int pos,char **buffer,::size_t *size,bool alloc);
void free_blocks( block *blocks[],int nblocks );
int free_block_node (inode *node);
inode *alloc_node (GannetFS::mode_t mode);
void free_node( inode *node );
//void print();
 int get_block_direct( int pos,block **blocks, int nblocks,char **buffer, ::size_t *size,bool alloc);
 int get_block_indirect1( int pos,block ***blocks,int nblocks,char **buffer,::size_t *size,bool alloc);
 int get_block_indirect2( int pos,block ****blocks,int nblocks,char **buffer,::size_t *size,bool alloc);


//directory entry operation
int add_entry( inode *dir,const char *name,int namelen,inode *node);
 dirent *get_entry( inode *dir, const char *name, int namelen );
void set_translator( translator *tr,inode *dir, const char *name);
 int find_entry( translator *tr );
 int search( translator *d );
void listdir(const char *name);
 int del_entry( inode *dir, const char *name, int namelen );

//directory operation
int mkdir (const char *name, GannetFS::mode_t mode );
int rmdir    (  const char *name );


//file operation
 int open (const char *name, int oflag, GannetFS::mode_t mode);
 int read (int fd, char *buf, int size);
 GannetFS::ssize_t write (int fd, char *buf, int size);
 int close (int fd);

 GannetFS::off_t seek (int fd, int offset, int whence);

void mode_bits_to_str(int mode, char *str);
int df();

int stat(const char* name , filestat *statbuf);
int dec_nlink( inode *node );
int unlink   ( const char *name );
int remove (const char *name);

int tell(int fd);
int fstat(int fd , filestat *statbuf);

int cp (const char* arg1, const char* arg2);
int mv (const char* arg1, const char* arg2);

DIR *opendir(const char *dirname);
dirent *readdir(DIR *dirp);
void rewinddir(DIR *dirp);
int closedir(DIR *dirp);
}


#endif



