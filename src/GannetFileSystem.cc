//#include <iostream>
//#include <cstdio>
//#include <fstream>

#include "GannetFileSystem.h"


GannetFileSystem::GannetFileSystem(){
    
    GannetFS::init_blocks();
    GannetFS::mount();
}

GannetFileSystem::~GannetFileSystem()
{
    GannetFS::unmount();
}

void GannetFileSystem::init_blocks()
{
    GannetFS::init_blocks();
}

block* GannetFileSystem::block_alloc(void)
{
    block *b = GannetFS::block_alloc();
    return b;
}

void GannetFileSystem::free_ablock( block *b )
{
   GannetFS::free_ablock(b);
}

int GannetFileSystem::mount( )
{
   int retval = GannetFS::mount();
   return retval;
}

int GannetFileSystem::get_block_node( inode  *node,int pos,char **buffer,size_t *size,bool alloc)
{
   int retval = GannetFS::get_block_node( node,pos,buffer,size,alloc);
   return retval;
}

void GannetFileSystem::free_blocks( block *blocks[],int nblocks )
{
   GannetFS::free_blocks( blocks,nblocks );
}

int GannetFileSystem::free_block_node (inode *node)
{
   int retval = GannetFS::free_block_node (node);
   return retval;
}

inode *GannetFileSystem::alloc_node (GannetFS::mode_t mode)
{
   inode *node = GannetFS::alloc_node( mode);
   return node;
}

void GannetFileSystem::free_node( inode *node )
{
   GannetFS::free_node(node);
}

/*void GannetFileSystem::print()
{
   GannetFS::print();
}*/

int GannetFileSystem::get_block_direct( int pos,block **blocks, int nblocks,char **buffer, size_t *size,bool alloc)
{
   int retval = GannetFS::get_block_direct(  pos,blocks,  nblocks,buffer, size, alloc);
   return retval;
}

int GannetFileSystem::get_block_indirect1( int pos,block ***blocks,int nblocks,char **buffer,size_t *size,bool alloc)
{
   int retval = GannetFS::get_block_indirect1( pos,blocks, nblocks,buffer,size, alloc);
   return retval;
}

int GannetFileSystem::get_block_indirect2( int pos,block ****blocks,int nblocks,char **buffer,size_t *size,bool alloc)
{
   int retval = GannetFS::get_block_indirect2( pos,blocks, nblocks,buffer,size, alloc);
   return retval;
}

int GannetFileSystem::dec_nlink( inode *node )
{
   int retval = GannetFS::dec_nlink(node);
   return retval;
}

int GannetFileSystem::add_entry( inode *dir,const char *name,int namelen,inode *node)
{
   int retval = GannetFS::add_entry( dir,name, namelen,node);
   return retval;
}

dirent* GannetFileSystem::get_entry( inode *dir, const char *name, int namelen )
{
   dirent *dirp = GannetFS::get_entry( dir, name, namelen );
   return dirp;
}

void GannetFileSystem::set_translator( translator *tr,inode *dir, const char *name)
{
   GannetFS::set_translator( tr,dir, name);
}

int GannetFileSystem::find_entry( translator *tr )
{
   int retval = GannetFS::find_entry ( tr);
   return retval;
}

int GannetFileSystem::search( translator *d )
{
   int retval = GannetFS::search (d);
   return retval;
}

void GannetFileSystem::listdir(const char *name)
{
   GannetFS::listdir (name);
}

int GannetFileSystem::del_entry( inode *dir, const char *name, int namelen )
{
   int retval = GannetFS::del_entry( dir, name,  namelen );
   return retval;
}

int GannetFileSystem::mkdir (const char *name, GannetFS::mode_t mode )
{
   int retval = GannetFS::mkdir (name, mode );
   return retval;
}

int GannetFileSystem::rmdir    (  const char *name )
{
   int retval = GannetFS::rmdir ( name);
   return retval;
}

int GannetFileSystem::open (const char *name, int oflag, GannetFS::mode_t mode)
{
   int retval = GannetFS::open (name,  oflag,  mode);
   return retval;
}

int GannetFileSystem::creat (const char *name, GannetFS::mode_t mode)
{
   int retval = GannetFS::open (name, O_WRONLY,  mode);
   return retval;
}


int GannetFileSystem::read (int fd, char *buf, int size)
{
  int retval = GannetFS::read ( fd, buf,  size);
  return retval;
}

GannetFS::ssize_t GannetFileSystem::write (int fd, char *buf, int size)
{
  int retval = GannetFS::write ( fd, buf,  size);
  return retval; 
}

int GannetFileSystem::cp (const char* arg1, const char* arg2)
{
   int retval = GannetFS::cp( arg1,  arg2);
   return retval;
}

int GannetFileSystem::mv (const char* arg1, const char* arg2)
{
   int retval = GannetFS::mv( arg1,  arg2);
   return retval;
}

int GannetFileSystem::close (int fd)
{
   int retval = GannetFS::close( fd);
   return retval;
}

int GannetFileSystem::unlink   ( const char *name )
{
   int retval = GannetFS::unlink ( name );
   return retval;
}


GannetFS::off_t GannetFileSystem::seek (int fd, int offset, int whence)
{
   int retval = GannetFS::seek ( fd, offset,  whence);
   return retval;
}

DIR * GannetFileSystem::opendir(const char *dirname)
{
   DIR *dirp = opendir (dirname);
   return dirp;
}

dirent * GannetFileSystem::readdir(DIR *dirp)
{
   dirent *retval = readdir(dirp);
   return retval;
}

void GannetFileSystem::rewinddir(DIR *dirp)
{
   GannetFS::rewinddir(dirp);
}

int GannetFileSystem::closedir(DIR *dirp)
{
    int retval = GannetFS::closedir(dirp);
    return retval;
}

void GannetFileSystem::mode_bits_to_str(int mode, char *str)
{
   GannetFS::mode_bits_to_str( mode, str);
}

int GannetFileSystem::df()
{
   int retval = GannetFS::df();
   return retval;
}

int GannetFileSystem::stat(const char* name , filestat *statbuf)
{
   int retval = GannetFS::stat(name , statbuf);
   return retval;
}

int GannetFileSystem::tell(int fd)
{
   int retval = GannetFS::tell( fd);
   return retval;
}

int GannetFileSystem::fstat(int fd , filestat *statbuf)
{
   int retval = GannetFS::fstat( fd , statbuf);
   return retval;
}

int GannetFileSystem::remove(const char *name)
{
   int retval = GannetFS::remove(name);
   return retval;
}

