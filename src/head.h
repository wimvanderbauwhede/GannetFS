
#ifndef _HEAD_H
#define _HEAD_H

namespace GannetFS {
typedef int off_t;
typedef int ssize_t;
typedef unsigned int size_t;
typedef int ino_t;
typedef int mode_t;
typedef int nlink_t;
typedef int blksize_t;
typedef int blkcnt_t;
typedef int nlink_t;

} 


#define OK 0


//inode mode
#define     S_IFMT        00000170000 /* type of file */
#define     S_IFLNK       00000120000 /* symbolic link */
#define     S_IFREG       00000100000 /* regular */
#define     S_IFBLK       00000060000 /* block special */
#define     S_IFDIR       00000040000 /* directory */
#define     S_IFCHR       00000020000 /* character special */
#define     S_IFIFO       00000010000 /* fifo */

#define     O_CREAT	  00000000100 /* create if file does not yet exist */
#define     O_EXCL	  00000000200 /* open exclusively */
#define     O_NOCTTY	  00000000300 /* no TTY control */
#define     O_TRUNC	  00000001000 /* truncate file to length 0 */
#define     O_APPEND	  00000002000 /* append to file */
#define     O_NONBLOCK	  00000004000 /* open in non-blocking mode */
#define     O_SYNC	  00000010000 /* write to disc at once */
#define     O_DIRECTORY	  00000040000 /* file must be directory */


#define     S_ISREG(m)    (((m) & S_IFMT) == S_IFREG)
#define     S_ISLNK(m)    (((m) & S_IFMT) == S_IFLNK)
#define     S_ISBLK(m)    (((m) & S_IFMT) == S_IFBLK)
#define     S_ISDIR(m)    (((m) & S_IFMT) == S_IFDIR)
#define     S_ISCHR(m)    (((m) & S_IFMT) == S_IFCHR)
#define     S_ISFIFO(m)   (((m) & S_IFMT) == S_IFIFO)

#define S_IUMSK 07777     /* user settable bits */

#define S_ISUID 04000     /* set user id on execution */
#define S_ISGID 02000     /* set group id on execution */

#define S_ISVTX 01000     /* save swapped text even after use */

#define S_IRWXU 00700     /* read, write, execute: owner */
#define S_IRUSR 00400     /* read permission: owner */
#define S_IWUSR 00200     /* write permission: owner */
#define S_IXUSR 00100     /* execute permission: owner */
#define S_IRWXG 00070     /* read, write, execute: group */
#define S_IRGRP 00040     /* read permission: group */
#define S_IWGRP 00020     /* write permission: group */
#define S_IXGRP 00010     /* execute permission: group */
#define S_IRWXO 00007     /* read, write, execute: other */
#define S_IROTH 00004     /* read permission: other */
#define S_IWOTH 00002     /* write permission: other */
#define S_IXOTH 00001     /* execute permission: other */

//O_FLAG
#define O_RDONLY 0	  /* read only flag */
#define O_WRONLY 1	  /* write only flag */
//#define O_RDWR   2	  /* read and write flag */

// TYPE OF SEEK
#define SEEK_SET 0	  /* offset is set to offset bytes */
#define SEEK_CUR 1	  /* offset is set to its current location plus offset bytes */
#define SEEK_END 2	  /* offset is set to the size of the file plus offset bytes */

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif


#endif


