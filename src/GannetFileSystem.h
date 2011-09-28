//#include <map>
#include "FileSystem.h"
#include "Stack.h"
//using std::cout;
//using std::endl;

class GannetFileSystem {
   private:
      //instantiate
      block block_array[TOTAL_BLOCKS];
      StackFS<int, MAX_DATA_BLOCKS> block_stack;
      StackFS<char, TOTAL_INODES> inode_stack;
      StackFS<char, FD_MAX> fd_stack;
      frecord fdtable[FD_MAX];
      
      //function
      void init_blocks();
      block *block_alloc(void);
      void free_ablock( block *b );
      int mount( );

      //inode operation
      int get_block_node( inode  *node,int pos,char **buffer,size_t *size,bool alloc);
      void free_blocks( block *blocks[],int nblocks );
      int free_block_node (inode *node);
      inode *alloc_node (GannetFS::mode_t mode);
      void free_node( inode *node );
      //void print();
      int get_block_direct( int pos,block **blocks, int nblocks,char **buffer, size_t *size,bool alloc);
      int get_block_indirect1( int pos,block ***blocks,int nblocks,char **buffer,size_t *size,bool alloc);
      int get_block_indirect2( int pos,block ****blocks,int nblocks,char **buffer,size_t *size,bool alloc);
      int dec_nlink( inode *node );
      

      //directory entry operation
      int add_entry( inode *dir,const char *name,int namelen,inode *node);
      dirent *get_entry( inode *dir, const char *name, int namelen );
      void set_translator( translator *tr,inode *dir, const char *name);
      int find_entry( translator *tr );
      int search( translator *d );
      
      int del_entry( inode *dir, const char *name, int namelen );

   public:
      GannetFileSystem ();
      ~GannetFileSystem ();

      //directory operation
      int mkdir (const char *name, GannetFS::mode_t mode );
      int rmdir    (  const char *name );

      //file operation
      int open (const char *name, int oflag, GannetFS::mode_t mode);
      int creat (const char *name, GannetFS::mode_t mode);
      int read (int fd, char *buf, int size);
      GannetFS::ssize_t write (int fd, char *buf, int size);
      int cp (const char* arg1, const char* arg2);
      int mv (const char* arg1, const char* arg2);
      int close (int fd);
      int unlink   ( const char *name );
      int remove(const char* name);
      GannetFS::off_t seek (int fd, int offset, int whence);

      //dir operation
      DIR *opendir(const char *dirname);
      dirent *readdir(DIR *dirp);
      void rewinddir(DIR *dirp);
      int closedir(DIR *dirp);
      

      //misc
      void mode_bits_to_str(int mode, char *str);
      int df();
      int stat(const char* name , filestat *statbuf);
      int tell(int fd);
      int fstat(int fd , filestat *statbuf);
      void listdir(const char *name);

};// end of class GannetFileSystem
