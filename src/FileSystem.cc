#ifdef VERBOSE
#include <iostream>
#endif
// for printf, TODO: replace with std::cout
#include <cstdio>

#include "FileSystem.h"
#include "Stack.h"

#ifdef VERBOSE
using std::cout;
using std::endl;
#endif

static block block_array[TOTAL_BLOCKS];
static StackFS<int,MAX_DATA_BLOCKS> block_stack;
static StackFS<char, TOTAL_INODES> inode_stack;
static StackFS<char, FD_MAX> fd_stack;
static frecord fdtable[FD_MAX];

/*
* init_blocks()
* Initialize the block allocator by putting 
* by putting all free block to stack
*/
void GannetFS::init_blocks()
{
    bool initialized = false;
    int tot_ino =TOTAL_INODES ;
    int ino_per_block = INODE_PER_BLOCK;
    int con = tot_ino/ino_per_block;


    if( !initialized )
    {
        int i; 
	for (i=TOTAL_BLOCKS-1; i>= 0;i--)
	{
		if (i>= (con))		
		  block_stack.push(i);
		  
	  		
		if (i< TOTAL_INODES)
		  inode_stack.push(i); 

		if (i< FD_MAX )
		  fd_stack.push(i);
		  
		  
	}
        initialized = true;
       
    }

}

/*
* block_alloc()
* Allocate a block for data storage
* by obtaining free block from stack.
*/
 block* GannetFS::block_alloc(void)
{
    
    block *b;
    if(block_stack.size()>0){
	b= block_array+block_stack.pop();
    }
    else
       b = NULL;
    // set values on block 
    if( b != NULL )
        memset( b, 0, BLOCK_SIZE );
    return b;

}

/*
* free_ablock()
* free an unused block
* put it back to stack 
*/
void GannetFS::free_ablock( block *b )
{ 
   block* bl = block_array;
   int i = b-bl;
   block_stack.push(i);
}

/*
* get_block_direct();
* Indexes into an array of block pointers and 
* extracts a pointer to the data at offset pos, 
* allocating new blocks if alloc is set.
*
* parameter:
* pos		: position offset
* **blocks	: pointer to the direct block
* nblocks	: number of direct block
* **buffer	: pointer to the target
* *size		: size of used block
* alloc		: boolean that indicates whether allocation new block is required
*
* return:
* OK 		: operation succsess
* ENOSPC 	: cannot obtain free block from stak. operation terminated
*
*/
 int GannetFS::get_block_direct( int pos,block **blocks, int nblocks,char **buffer, ::size_t *size,bool alloc)
{
    int block_index = pos / BLOCK_SIZE;
    int block_pos = pos % BLOCK_SIZE;
    block *b;

    *buffer = NULL;
    *size = 0;
    
    if (alloc){
    if( block_index >= nblocks )
        return OK;
    } 
    b = blocks[block_index];

    if( b == NULL )
    {
        // There is no block there. If _alloc_ is true we can fill the
        // slot in with a new block. If it is false, we indicate end of
        // data with a zero size result.
        if( alloc )
        {
            b = GannetFS::block_alloc();
            if( b == NULL )
	    {
#ifdef VERBOSE
                printf("block_alloc: Out of available block.\n");
#endif
                return ENOSPC;
            }
            blocks[block_index] = b;
        }
        else 
	    return OK;
    }


    //*buffer = reinterpret_cast<char*>(b);
    //*buffer = *buffer + block_pos;
    *buffer = &( reinterpret_cast<char*>(b) [block_pos]);
    *size = BLOCK_SIZE - block_pos;

    return OK;
}

/*
* get_block_indirect1();
* Indexes into an array of pointers to blocks 
* containing pointers to blocks and extracts 
* a pointer to the data at offset pos,
* allocating new blocks if alloc is set.
*
* parameter:
* pos		: position offset
* ***blocks	: pointer to the indirect1 block
* nblocks	: number of direct block
* **buffer	: pointer to the target
* *size		: size of used block
* alloc		: boolean that indicates whether allocation new block is required
*
* return:
* calling function get_block_direct();
*
*/
 int GannetFS::get_block_indirect1( int pos,block ***blocks,int nblocks,char **buffer, ::size_t *size,bool alloc)
{

    int block_index = pos / RAMFS_INDIRECT1_BLOCK;
    int block_pos = pos % RAMFS_INDIRECT1_BLOCK;
    int err;
    char *b;
    size_t sz;
    
    // Use get_block_direct() to index and allocate the first level indirect block
    err = GannetFS::get_block_direct( block_index*BLOCK_SIZE,(block **)blocks,nblocks,&b,&sz,alloc);

    if( err != OK )
        return err;

    if( sz == 0 )
    {
        *size = 0;
        return OK;
    }

    // Use get_block_direct() on the first level indirect block to allocate and return the data pointer.
    return GannetFS::get_block_direct( block_pos, blocks[block_index], RAMFS_INDIRECT_PER_BLOCK,buffer,size, alloc);
}


/*
* get_block_indirect2();
* Indexes into an array of pointers to blocks 
* containing pointers to blocks containing 
* pointers to blocks and extracts a pointer to blocks and extracts 
* a pointer to the data at offset pos,
* allocating new blocks if alloc is set.
*
* parameter:
* pos		: position offset
* ****blocks	: pointer to the indirect2 block
* nblocks	: number of direct block
* **buffer	: pointer to the target
* *size		: size of used block
* alloc		: boolean that indicates whether allocation new block is required
*
* return:
* calling function get_block_indirect1();
*
*/
 int GannetFS::get_block_indirect2 ( int pos,block ****blocks,int nblocks, char **buffer, size_t *size,bool alloc)
{
    int block_index = pos / RAMFS_INDIRECT2_BLOCK;
    int block_pos = pos % RAMFS_INDIRECT2_BLOCK;
    int err;
    char *b;
    size_t sz;

    // Use get_block_direct() to index and allocate the first level indirect block.
    err = GannetFS::get_block_direct( block_index*BLOCK_SIZE,(block **)blocks,nblocks,&b,&sz,alloc);

    if( err != OK )
        return err;

    if( sz == OK )
    {
        *size = 0;
        return OK;
    }

    // Use get_block_indirect1() on the first level indirect block to
    // index and allocate the next level indirect block and the data block.
    
    return GannetFS::get_block_indirect1( block_pos,blocks[block_index], RAMFS_INDIRECT_PER_BLOCK,buffer,size, alloc);
}

/*
* get_block_node()
* main function for calling the another
* function to get the pointer.
*
* parameter:
* *node		: pointer to the corresponding inode
* pos		: position offset
* **buffer	: pointer to the target
* *size		: size of used block
* alloc		: boolean that indicates whether allocation new block is required
*
* return:
* calling function get_block_direct() if offset position less than RAMFS_DIRECT_MAX;
* calling function get_block_indirect1() if offset position less than RAMFS_INDIRECT1_MAX;
* calling function get_block_indirect2() if offset position less than RAMFS_INDIRECT2_MAX;
* ENOSPC	: cannot obtain free block from stak. operation terminated 
*/
int GannetFS::get_block_node( inode  *node,int pos,char **buffer,::size_t *size,bool alloc)
{
    if( pos < RAMFS_DIRECT_MAX )
        return GannetFS::get_block_direct( pos,node->direct,BLOCKS_DIRECT,buffer,size,alloc);

    if( pos < RAMFS_INDIRECT1_MAX )
        return GannetFS::get_block_indirect1( pos - RAMFS_DIRECT_MAX, node->indirect1, 1,buffer,size,alloc);
    
    if( pos < RAMFS_INDIRECT2_MAX )
        return GannetFS::get_block_indirect2( pos - RAMFS_INDIRECT1_MAX,node->indirect2, 1,buffer,size, alloc);

    return ENOSPC;
}

/*
* free_blocks()
* free a list of data blocks
*
* parameter:
* *blocks[]	: pointer to list of data blocks
* nblocks	: number of data blocks
*/
void GannetFS::free_blocks( block *blocks[],int nblocks )
{
    int i;
    for( i = 0; i < nblocks ; i++ )
    {
        if( blocks[i] != NULL )
        {
            GannetFS::free_ablock( blocks[i] );
            blocks[i] = NULL;
        }
    }
}

/*
* free_block_node()
* Free all the data blocks in the node 
* and clear the pointers.
*
* parameter: 
* *node		: pointer to the corresponding inode
*
* return:
* OK		: if the operation success
*/
int GannetFS::free_block_node (inode *node)
{	// free a list of data block in direct block
	GannetFS::free_blocks (node->direct, BLOCKS_DIRECT);

        // free a list of data block in indirect1 block
        if( node->indirect1[0] != NULL )
        {
           GannetFS::free_blocks( (block **)node->indirect1[0], RAMFS_INDIRECT_PER_BLOCK );
           GannetFS::free_ablock( (block *)node->indirect1[0] );
           node->indirect1[0] = NULL;
        }
       // free a list of data block in indirect2 block
       if( node->indirect2[0] != NULL )
       {
           block ***b = node->indirect2[0];
           int j;
           for( j = 0; j < RAMFS_INDIRECT_PER_BLOCK ; j++ )
           {
              if( b[j] != NULL )
                 {
                     GannetFS::free_blocks( (block **)b[j], RAMFS_INDIRECT_PER_BLOCK );
                     GannetFS:: free_ablock( (block *)b[j] );
                     b[j] = NULL;
                 }
           }
          GannetFS::free_ablock( (block *)node->indirect2[0] );
          node->indirect2[0] = NULL;
      }
      return OK;


}

/*
* alloc_node()
* allocate a block and use it to store numbers of inode.
*
* parameter:
* mode		: type of file
*
* return:
* *inode	: a pointer to the new allocated inode
* NULL		: if there is no free inode in stack of inodes
*/
inode* GannetFS::alloc_node (GannetFS::mode_t mode)
{
    inode *node;
    int i = inode_stack.pop();
    node = reinterpret_cast<inode*> (block_array)+i;

    if( node == NULL )
    {
#ifdef VERBOSE
        printf("alloc_node: Out of available inode.");        
#endif
	return NULL;
    }
    memset( node, 0, sizeof(inode) );

    node->mode          = mode;
    node->nlink		= 0;
    node->size          = 0;
    return node;
}


/*
* free_node();
* free a node  back into the stack of inodes
* 
* parameter:
* *node		: pointer to the corresponding inode
*/
void GannetFS::free_node( inode *node )
{
   inode *in = reinterpret_cast<inode*> (block_array);
   int i = node-in;
   inode_stack.push(i);
}



/*
* add_entry()
* add a direntry to directory
*
* parameter:
* *dir		: pointer to parent's inode
* *name		: entry name that holtr until 20 chars
* namelen	: length of the name
* *node		: pointer to corresponding inode

* return :
* OK		: if the operation success
* ENOSPC	: either there is no available block or inode
*/
int GannetFS::add_entry( inode *dir,const char *name,int namelen,inode *node)
{
    int pos = 0;
    dirent *d = NULL, *dp = NULL;
    bool isfirst = true;
    int dirlen = sizeof(dirent);

    // Loop inserting fragments of the name into the directory 
    
    if (namelen >= MAX_NAME_LENGTH)
       return EPERM;

    while( namelen > 0 )
    {

	for(;;)
        {
            char *buf;
            size_t size;
            int err = GannetFS::get_block_node( dir, pos, &buf, &size, true );
            if( err != OK ) return err;

            d = (dirent *)buf;

            if( d->inuse )
            {
                pos += dirlen;
		continue;
            }

            break;
        }
        // d points to a free dirent structure
        d->node         = node;
        d->inuse        = 1;
        d->first        = isfirst;
        d->namelen      = namelen;
        if( dp ) dp->next = pos;
	
	
	memset (d->name,0, MAX_NAME_LENGTH);
	memcpy( d->name, name, namelen );

	// update the stat of direntry
        namelen         -= namelen;
        pos             += dirlen;
        dp              = d;
        isfirst         = false;

    }
    d->last = 1;        // Mark last fragment

    // Extend dir size according to the offset position
    if( pos > dir->size )
        dir->size = pos;

    // increment link to node
    node->nlink++;

    return OK;
}


/*
* get_entry()
* Find a directory entry for the name 
*
* Parameter:
* *dir		: pointer to the corresponding inode
* *name		: name of the direntry target
* namelen	: name length
*
* return:
* dirent	: pointer to the direntry target
* NULL		: if the target not found
*/
 dirent * GannetFS::get_entry( inode *dir, const char *name, int namelen )
{
    dirent *first = NULL;
    int pos = 0;
    int err;
    int dirlen = sizeof(dirent);

    // Loop over all the entries until a match is found 
    while( pos < dir->size )
    {
        const char *entname = name;
        dirent *d;
        char *buf;
        size_t size;
        
        // look for a first name fragment
        for(;;)
        {
            err = GannetFS::get_block_node( dir, pos, &buf, &size, false );
            if( err != OK || size == 0)
                return NULL;

            d = (dirent *)buf;

            if(  !d->inuse || !d->first )
            {
                pos += dirlen;
                continue;
            }

            break;
        }

	//compare the name length
        if( d->namelen == namelen )
        {
            // potential candidate 
            first = d;      

            // compare strings
            for(;;)
            {
                if( memcmp( entname, d->name, namelen ) != 0 )
                    break;

                if( d->last )
                        return first;

                // the candidate does not match, continue searching then
                err = GannetFS::get_block_node( dir, d->next, &buf, &size, false );
                if( err != OK )
                    return NULL;

                d = (dirent *)buf;

            }
        }

        pos += dirlen;        
    }

    return NULL;
}


/*
* set_translator()
* initialize a translator object to start a search
*
* Parameter:
* *tr		: pointer to corresponding translator object
* *dir		: root directory
* *name		: a path to a file or directory
*/
void GannetFS::set_translator( translator *tr,inode *dir, const char *name)
{
    tr->dir      = dir;
    tr->path     = name;
    tr->node     = dir;
    tr->name     = name;
    tr->namelen  = 0;
    tr->last     = false;    
}


/*
* find_entry()
* Search a directory for the next name in a path 
* and update the translator object
* 
* Parameter:
* *tr		: translator object
*
* return:
* OK		: the operation success
* ENOTDIR	: path is not a directory
* ENOENT	: the target directory is not found
*/
 int GannetFS::find_entry( translator *tr )
{
    inode *dir = tr->dir;
    const char *name = tr->path;
    const char *n = name;
    char namelen = 0;
    dirent *d;
    
     
     if( !S_ISDIR(dir->mode) )
        return ENOTDIR;

    // extract the next element of the path name 
    // by ignoring '/' and '\0'
    while( *n != '\0' && *n != '/' )
        n++, namelen++;

    // terminated on a char '\0' and set last flag
    if( *n == '\0' ) 
        tr->last = true;

    // update name in translator object
    tr->name = name;
    tr->namelen = namelen;
    
    // Search the directory for a matching entry
    d = GannetFS::get_entry( dir, name, namelen );

    if( d == NULL )
    {
	 return ENOENT;
    }
    // passing back the node 
    tr->node = d->node;

    return OK;

}

/*
* search()
* Main interface for directory search to locate 
* the object named by the pathname.
*
* parameter:
* *d 		: pointer to translator object
*
* return:
* OK		: the operation success
*/
 int GannetFS::search( translator *tr )
{
    int err;

    // path to root directory
    if( *(tr->path) == '\0' )
        return OK;

    // iterate down until found the object
    for(;;)
    {
        err = GannetFS::find_entry( tr );

        if( err != OK )
            return err;

        if( tr->last )
            return OK;

        // Update translator object to search next directory.
        tr->dir = tr->node;
        tr->path += tr->namelen;
	// skip dirname separators
        if( *(tr->path) == '/' ) tr->path++; 
    }
}


/*
* mode_bits_to_str();
* convert file mode to strings for displaying on console
* 
* parameter:
* mode		: type of file
* *str		: string to be displayed
*/
 void GannetFS::mode_bits_to_str(int mode, char *str)
{
	int i ;
	strcpy(str, "----------");

	if (S_ISDIR(mode))
		str[0]='d';

	for (i=7; i>0;i-=3,mode>>=3)
	{
		if (mode & S_IROTH) str[i]='r';
		if (mode & S_IWOTH) str[i+1]='w';
		if (mode & S_IXOTH) str[i+2]='x';
	}
}


/*
* listdir()
* displaying all direntries on a directory
* 
* parameter:
* *name		: path to the target directory
*/
void GannetFS::listdir (const char *name)
{
   DIR *dirp;
   char mode_str[11];
   dirent *direnp;
   dirp = GannetFS::opendir(name);
   printf("Directory listing for : %s \n", name);
   printf("mode          size    entries \n");
   while ((direnp = GannetFS::readdir(dirp))!=NULL)
   {
     mode_bits_to_str(direnp->node->mode, mode_str);
     printf ("%-13s %-7d %-16s \n", mode_str,direnp->node->size, direnp->name);
   }
   int err = GannetFS::closedir(dirp);
}

/*
* unmount()
* unmount the file system by deleting root directory
*
* return:
* OK		: Operation is success
*/
int GannetFS::unmount    ( )
{
    inode *root = reinterpret_cast<inode*> (block_array);
    free_node (root);
    return OK;
}

/*
* mount()
* mount the file system by creating root directory
*
* return:
* OK		: Operation is success
* ENOSPC	: No space available to create root directory
*/
int GannetFS::mount    ( )
{
    inode *root, *start;
    int err;
    //start = reinterpret_cast<inode*> (block_array);
    // Allocate a node to be the root of this filesystem and initialize it.
    root = GannetFS::alloc_node(S_IFDIR|S_IRWXU|S_IRWXG|S_IRWXO);

    if( root == NULL )
        return ENOSPC;

    // Add . and .. entries to the root directory
    err = GannetFS::add_entry( root, ".", 1, root );
    if( err == OK)
        err = GannetFS::add_entry( root, "..", 2, root );
    
    // delete root directory if there is any problem
    if( err != OK )
    {
        GannetFS::free_node( root );
        return err;
    } 
    
    return OK;
}
/*
* del_entry()
* Delete a directory entry by unsetting inuse flag
* and decrease nlink field on corresponding inode 
*
* Parameter:
* *dir		: pointer to the corresponding inode
* *name		: name of the direntry target
* namelen	: name length
*
* return:
* OK		: if the operation success
* ENOENT	: if the directory entry target not found
*/
 int GannetFS::del_entry( inode *dir, const char *name, int namelen )
{
    dirent *d = GannetFS::get_entry( dir, name, namelen );
    
    if( d == NULL )
        return ENOENT;

    for(;;)
    {
        int err;
        char *buf;
        size_t size;

        d->inuse = 0;
        if( d->last ) break;

        err = GannetFS::get_block_node( dir, d->next, &buf, &size, false );
        if( err != OK)
            return ENOENT;

        d = (dirent *)buf;
    }
    GannetFS::dec_nlink( d->node );
 return OK;
}


/*
* mkdir()
* create a new directory
*
* parameter:
* *name		: directory name
* mode		: directory file type
*
* result :
* OK		: the operation is success, a new directory is created
* EEXIST	: the name does exist, operation terminated
*/
 int GannetFS::mkdir (const char *name, GannetFS::mode_t mode ){
    translator tr;
    inode *node = NULL;
 
  
    inode *dir = reinterpret_cast<inode*> (block_array);
    int err;

   
    GannetFS::set_translator( &tr, dir, name );
    
    err = GannetFS::search( &tr );

    if( err == ENOENT )
    {
        if( tr.last )
        {
             int doterr, dotdoterr, direrr;
        
            node = GannetFS::alloc_node(mode);
	    if( node == NULL )
                return ENOSPC;

            // Add "." and ".." entries.
            doterr = GannetFS::add_entry( node, ".", 1, node );
            dotdoterr = GannetFS::add_entry( node, "..", 2, tr.dir );

            // add this entry to parent directory.
            direrr = GannetFS::add_entry( tr.dir, tr.name, tr.namelen, node );

            // check if there is any error
            if( doterr+dotdoterr+direrr != OK )
            {
                // undo the creation of direntry
                if( doterr == OK )
                    GannetFS::del_entry( node, ".", 1 );
                else err = doterr;

                if( dotdoterr == OK )
                    GannetFS::del_entry( node, "..", 2 );
                else err = dotdoterr;

                if( direrr == OK )
                    GannetFS::del_entry( tr.dir, tr.name, tr.namelen );
                else err = direrr;

                // Free the data and the node 
                GannetFS::free_block_node( node );
                GannetFS::free_node( node );
            }
            else err = OK;
        }

    }
    else
    {
        
        if( err == OK ){
            err = EEXIST;
#ifdef VERBOSE
	    printf(" cannot create directory %s : File exists\n", name);
#endif	
	}
    }
    
    return err;
}


/*
* rmdir ()
* remove a directory
*
* parameter:
* *name		: directory name
*
* result :
* OK		: the operation is success, a new directory is created
* ENOTDIR	: not a directory, operation terminated
* ENOTEMPTY 	: the directory is not empty, operation terminated
*/
 int GannetFS::rmdir    (  const char *name )
{
    translator tr;
    int err;
    inode *dir = reinterpret_cast<inode*> (block_array);
    GannetFS::set_translator( &tr,dir, name );
    
    err = GannetFS::search( &tr );

    if( err != OK ) return err;

    // Check whether it is a directory.
    if( !S_ISDIR(tr.node->mode) )
    {
        printf("rmdir: cannot remove %s : is not directory\n", name);
        return ENOTDIR;
    }

    // Check whether directory is empty
    if (tr.node->nlink > 2) //two, because of dot and dotdot 
    {
        printf("rmdir: cannot remove %s : directory is not empty\n", name);
        return ENOTEMPTY;
    }

    // Delete the entry. 
    
    err = GannetFS::del_entry( tr.dir, tr.name, tr.namelen );
    
    return err;
}


/*
* open()
* open a file if the file exist, or
* if the file does not exist, create a new file.
*
* parameter :
* *name		: path to the file
* oflag		: file access mode for write OR read
* mode		: file type directory
*
* return :
* file descriptor	: an unique number for accessing the file
*/

 int GannetFS::open (const char *name, int oflag, GannetFS::mode_t mode)
{
    //check file descriptor stack
    if (fd_stack.size() > FD_MAX || fd_stack.size()< 1)
    {
#ifdef VERBOSE
	cout<<"open: File descriptor table is full, operation on "<< name << " terminated. "<<endl;
#endif
        return -1;
    }
   //check mode
   if( S_ISDIR(mode) )
    {
#ifdef VERBOSE
	cout<< "open: Not regular file, operation on "<< name << " terminated. "<<endl;  
#endif      
	return -1;
    }

    

    translator tr;
    inode *node = NULL;
    inode *dir = reinterpret_cast<inode*> (block_array);
    int err;
    //check the name!
    GannetFS::set_translator( &tr, dir, name );
    err = GannetFS::search( &tr );
    
    //file not found, create new file;
    if( err == ENOENT )
    {
        if( tr.last )
        {
            // The entry does not exist
            // create new file 
            int doterr, dotdoterr, direrr;
        
            node = GannetFS::alloc_node (mode);
            if( node == NULL )
                return ENOSPC;

            // Add to parent directory
            direrr = GannetFS::add_entry( tr.dir, tr.name, tr.namelen, node );

            // check if there is any error
            if( direrr != OK )
            {   //undo the creation of direntry
                if( direrr == OK ){
                    GannetFS::del_entry( tr.dir, tr.name, tr.namelen );
                    err = direrr;
		    GannetFS::free_block_node( node );
                    GannetFS::free_node( node );
                }
               
            }
            else err = OK;
        }
	else return -1; //return error
    }
    else  // get the node
    {
    node = tr.node;
    }
    
    // check node's mode
    if( S_ISDIR(node->mode) )
    {
#ifdef VERBOSE	
	cout<< "open: Not regular file, operation on "<< name << " terminated. "<<endl;  
#endif      
	return -1;
    }

    frecord fr;
    fr.f_flag = oflag;
    fr.f_offset = 0;
    fr.f_dir = node;
    
    int fd = fd_stack.pop();
    fdtable[fd]=fr;
    return fd;       
}


/*
* read()
* read from a file
*
* parameter :
* fd		: file descriptor for a corresponding file
* *buf		: buffer to store the value that has been read
* size		: size of bytes that shall attempt to read
*
* return :
* int	: number of bytes that has been read
*/
 int GannetFS::read (int fd, char *buf, int size)
{
    
    if (fd> FD_MAX || fd< 0)
    {
#ifdef VERBOSE
	cout<<"read: File descriptor not valid, operation terminated. "<<endl;
#endif
        return 0;
    }

    frecord *fr = NULL; 
    fr = &fdtable[fd];
    
    // Check whether fd has a file record
    if (fr==NULL)
      return 0;

    inode *node;
    node = fr->f_dir;
    off_t pos = fr->f_offset;

    if (fr->f_flag != O_RDONLY)
    {
#ifdef VERBOSE
       cout<<"read: access denied, read only access."<<endl;
#endif
       return 0;
    }
   
       
    off_t length =size;

    // Looping until all data has been read
    while( length > 0 && pos < node->size )
    {
            char *fbuf;
            size_t bsize;
            off_t len = length;
            int err;

            // Get pointer
            err = get_block_node( node, pos, &fbuf, &bsize, false );

            if( err != OK )
                break;

            // set len to not exceed the file
            if( len > node->size-pos )
                len = node->size-pos;
            
            // chop buffer to fit in this block
            if( len > bsize )
                len = bsize;
            
            if (fbuf) {
              // read data out
              memcpy( buf, fbuf, len );
            } else { 
              // avoid junk value
              memset( buf, 0, len );
            }
 
            // Update variables
            length -= len;
            buf += len;
            pos += len;
            
        }
    //set file offset
    fr->f_offset = pos;
    return pos;
    

}


/*
* write()
* write on a file
*
* parameter :
* fd		: file descriptor for a corresponding file
* *buf		: buffer that contains values to be written to a file
* size		: size of bytes that shall attempt to write
*
* return :
* int	: number of bytes that has been written to file
*/
GannetFS::ssize_t GannetFS::write (int fd, char *buf, int size)
{

    //check file descriptor validity
    if (fd> FD_MAX || fd< 0)
    {
#ifdef VERBOSE
	cout<<"write: File descriptor not valid, operation terminated. "<<endl;
#endif
        return 0;
    }

    frecord *fr = NULL; 
    fr = &fdtable[fd];
    
    // Check whether fd has a file record
    if (fr==NULL)
      return 0;

    inode *node;
    node = fr->f_dir;
    off_t pos = fr->f_offset;
    
    // Check file flag
    if (fr->f_flag != O_WRONLY)
    {
#ifdef VERBOSE
       cout<<"write: access denied, write only access."<<endl;
#endif
       return 0;
    }
    int err;
    off_t length = size;

    while( length > 0 )
    {
            char *fbuf;
            size_t block_size;
            off_t  len = length;
            
            err = get_block_node( node, pos, &fbuf, &block_size, true );

            // Stop if there error occurs
            if( err != OK )
                break;
            
            // chop buffer to fit in this block
            if( len > block_size )
                len = block_size;

            // write 
            memcpy( fbuf, buf, len );


            // Update variables
            length -= len;
            buf += len;
            pos += len;
            
     }
     //set size on node
     if( pos > node->size )
        node->size = pos;    
     
     //set file offset
     fr->f_offset = pos;
     return pos;
}





/*
* close()
* close a file descriptor
*
* parameter:
* fd		: file descriptor
*
* return:
* OK		: on success operation
*/
int GannetFS::close (int fd)
{
    //fdtable.erase(fd);
    if (fd < FD_MAX && fd >= 0){
      fd_stack.push(fd);
      return OK;
    }
    else {
      return -1;
    }
}


/*
* df()
* displaying file system block space usage
*
*/
int GannetFS::df()
{
	int max = NUMBER_BLOCKS;
        int used = NUMBER_BLOCKS-block_stack.size() ;
	int avail = max - used;
	double doub = (double)used/max*100;
	printf ("Filesystem	(%dbytes) Blocks	Used	Available	Use% \n", sizeof (block));
	printf ("Ram FS          	     %-7d   	  %-7d      %-6d  %2.2f \n" ,max, used, avail, doub);
        return avail;
	
}





/*
* block_count()
* returning number of block usage on a file
*
*/
int block_count(int i)
{
    int div, mod, count;
    div = i / BLOCK_SIZE;
    mod = i % BLOCK_SIZE;
    
    if (mod == 0)
       return div;
    else
       return div+1;
    

}

/*
* stat()
* get file status by file name
*
* parameter:
* name		: file name
* *statbuf	: pointer to file status object
*
*/
int GannetFS::stat(const char* name , filestat *statbuf)
{
    translator tr;
    inode *node = NULL;
 
  
    inode *dir = reinterpret_cast<inode*> (block_array);
    int err;

   
    GannetFS::set_translator( &tr, dir, name );
    
    err = GannetFS::search( &tr );
    if (err == ENOENT)
	printf("file not found");
    else
    {
        node = tr.node;
	statbuf->st_ino = 1;
        statbuf->st_mode = tr.node->mode;
        statbuf->st_size = tr.node->size;
        statbuf->st_blksize = BLOCK_SIZE;
        statbuf->st_blkcnt = block_count(tr.node->size);

    }
    return err;
}

/*
* fstat()
* get file status by file descriptor
*
* parameter:
* fd		: file descriptor
* *statbuf	: pointer to file status object
*
*/
int GannetFS::fstat(int fd , filestat *statbuf)
{
    inode *node = NULL;
    frecord *fr;
    fr = &fdtable[fd];
    node = fr->f_dir;
  
    int err;

    if (node == NULL){
	printf("file not found");
        err = ENOENT;
    }
    else
    {
        statbuf->st_ino = 1;
        statbuf->st_mode = node->mode;
        statbuf->st_size = node->size;
        statbuf->st_blksize = BLOCK_SIZE;
        statbuf->st_blkcnt = block_count(node->size);
	err = OK;

    }
    return err;
}



/*
* seek()
* change the access position for an open channel
*
* parameter:
* fd		: file descriptor
* offset	: position to be set
* whence	: set the origin access of file
*/
GannetFS::off_t GannetFS::seek (int fd, int offset, int whence)
{
    frecord *fr = &fdtable[fd];
    inode *node = fr->f_dir;
    int pos = offset;
    
    if (S_ISDIR(node->mode) )
	return EPERM;
    
    else 
    {
       switch( whence )
       {
       case SEEK_SET:
           // Pos is at the beginning of file
           break;

       case SEEK_CUR:
           // Add pos to current offset.
           pos += fr->f_offset;
           break;

       case SEEK_END:
           // Add pos to file size.
           pos += node->size;
           break;

       default:
           return EINVAL;
       }
    }
    fr->f_offset = pos;
    return pos;

   
}


/*
* unlink ()
* remove a file
*
* parameter:
* *name		: file name name
*
* result :
* OK		: the operation is success, a new directory is created
* ENPERM	: not a reguler file, operation terminated
* ENOENTRY	: the file target not found, operation terminated
*/
int GannetFS::unlink   ( const char *name )
{
    translator tr;
    inode *node = NULL;
 
  
    inode *dir = reinterpret_cast<inode*> (block_array);
    int err;

   
    GannetFS::set_translator( &tr, dir, name );
        
    err = GannetFS::search( &tr );

    if( err != OK ) return err;

    // Cannot unlink directories
    if( S_ISDIR(tr.node->mode) )
    {
	printf("unlink: cannot delete file %s : is a directory \n", name);        
	return EPERM;
    }

    // Delete from parent directory
    err = GannetFS::del_entry( tr.dir, tr.name, tr.namelen );
    
    return err;
}
/*dec_nlink()
* Decrement a node's link count
*
* parameter: 
* *node		: pointer to corresponding inode
* 
* result:
* OK		: operation is success
*
*/
int GannetFS::dec_nlink( inode *node )
{
    node->nlink--;
    int err ;
    // check if there is any link to inode
    // if not, delete the direntry and free the node
    if( (S_ISREG(node->mode) && node->nlink == 0 ) || (S_ISDIR(node->mode) && node->nlink == 1) )
        
    {
        
        if( S_ISDIR(node->mode) )
        {
            GannetFS::del_entry( node, ".", 1 );
            GannetFS::del_entry( node, "..", 2 );
        }
        
        err = GannetFS::free_block_node( node );

        if( err == OK )
            GannetFS::free_node( node );
    }

    return err;
}
/*
* remove()
* remove a file. if it is a directory, 
* rmdir function shall be called. if it is a file,
* unlink function is called.
*/
int GannetFS::remove (const char* name)
{
    translator tr;
    int err;
    inode *dir = reinterpret_cast<inode*> (block_array);
    GannetFS::set_translator( &tr,dir, name );
    
    err = GannetFS::search( &tr );

    if( err != OK ) return err;

    // Check whether it is a directory
    if( S_ISDIR(tr.node->mode) )
    {
        err = GannetFS::rmdir    (  name );
        return err;
    }
 
    // Check whether it is regular file
    if( S_ISREG(tr.node->mode) )
    {
        err = GannetFS::unlink    (  name );
        return err;
    }
}


/*
* tell()
* get the current position of file descriptor's file
*
* parameter:
* fd		: file descriptor
*
* return:
* offset	: position of current file's pointer
*
*/
int GannetFS::tell(int fd)
{
    frecord *fr;
    fr = &fdtable[fd];
    return fr->f_offset;
}

/*
* cp()
* copy source file to destination file
*
*
*/
int GannetFS::cp (const char* arg1, const char* arg2)
{
    int fdin, fdout, length;
    int err;
    translator tr;
    inode *node = NULL;
 
  
    inode *dir = reinterpret_cast<inode*> (block_array);
    GannetFS::set_translator( &tr, dir, arg1 );

    err = GannetFS::search( &tr );
    
    if (err != OK){
       printf("cp: cannot stat %s : No such file \n", arg1);
       return err;
    } 

    fdin = GannetFS::open(arg1,O_RDONLY, S_IFREG| S_IRWXU|S_IRWXG|S_IRWXO);
    GannetFS::seek(fdin,0,SEEK_END);
    length = GannetFS::tell (fdin); 
    GannetFS::seek(fdin,0,SEEK_SET);
    char buffer[length];
    int read = GannetFS::read(fdin, buffer, length);
    if (read != length)
    {
#ifdef VERBOSE
      cout<< " Error on copying file :  "<< arg1 << endl;
#endif
      err = EIO;
    }
    GannetFS::close(fdin);
    fdout = GannetFS::open (arg2, O_WRONLY,S_IFREG| S_IRWXU|S_IRWXG|S_IRWXO);
    int written = GannetFS::write(fdout, buffer, length);
   if (written != length)
    {
#ifdef VERBOSE
      cout<<"Error on copying file :  "<< arg1 << endl;
#endif
      err = GannetFS::unlink (arg2);
      err = EIO;
    }
    
    GannetFS::close(fdout);
    return err;
}

/*
* mv()
* move source file to another file
*/
int GannetFS::mv (const char* arg1, const char* arg2)
{
   
  int err = GannetFS::cp (arg1, arg2) ;
    if (err != OK)
       return err;
    else
    {
       err = GannetFS::unlink(arg1);
       return err;
    }
}

/*
* opendir()
* open a directory. a file record object is created 
* and put on file descriptor table
*
* return:
* DIR	: pointer to a DIR object stream
*/
DIR* GannetFS::opendir(const char *dirname)
{
    translator tr;
    inode *node = NULL;
    inode *dir = reinterpret_cast<inode*> (block_array);
    int err;
    //check the name!
    GannetFS::set_translator( &tr, dir, dirname );
    err = GannetFS::search( &tr );
  
    node = tr.node;
    
    if( !S_ISDIR(node->mode) )
        return NULL;
  
    int fd = fd_stack.pop();
    DIR dp, *dptr;
    
    dp.f_flag = fd;
    dp.f_offset = 0;
    dp.f_dir = node;
    fdtable[fd]=dp;
    dptr = &fdtable[fd];
    
    return dptr;
    
}

/*
* readdir()
* read a directory
*
*/
dirent* GannetFS::readdir(DIR *dirp)
{
    DIR *dp = &fdtable[dirp->f_flag];
    int dirlen = sizeof(dirent);
    int pos = dp->f_offset;
    inode *node = dp->f_dir;
    dirent *d;
    size_t size;
    int err;
    char *buf;
    
    while (pos < dp->f_dir->size)
    {
        err = GannetFS::get_block_node( node, pos, &buf, &size, false );
        d = reinterpret_cast<dirent*>(buf);
	if (!d->inuse)
           pos+=dirlen;
        else
        {
	   dp->f_offset = pos+dirlen;
	   return d;
           break;
        }
    }
    return NULL;

}
/*
* rewinddir()
* set current DIR position to the beginning of directory
*
*/
void GannetFS::rewinddir(DIR *dirp)
{
    DIR *dp = &fdtable[dirp->f_flag];
    dp->f_offset = 0;
}


/*
* closedir()
* close a file descriptor
*
*/
int GannetFS::closedir(DIR *dirp)
{
   if (dirp==NULL)
      return EBADF;

   int fd = dirp->f_flag;
   fd_stack.push(fd);
   return OK;
}



