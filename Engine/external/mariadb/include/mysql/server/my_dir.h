

#ifndef MY_DIR_H
#define MY_DIR_H

#include <sys/stat.h>

#ifdef	__cplusplus
extern "C" {
#endif

	

#define MY_S_IFMT	S_IFMT	
#define MY_S_IFDIR	S_IFDIR 
#define MY_S_IFCHR	S_IFCHR 
#define MY_S_IFBLK	S_IFBLK 
#define MY_S_IFREG	S_IFREG 
#define MY_S_IFIFO	S_IFIFO 
#define MY_S_ISUID	S_ISUID 
#define MY_S_ISGID	S_ISGID 
#define MY_S_ISVTX	S_ISVTX 

#ifndef S_IREAD
#define MY_S_IREAD      S_IRUSR 
#define MY_S_IWRITE     S_IWUSR 
#define MY_S_IEXEC      S_IXUSR 
#else
#define MY_S_IREAD      S_IREAD 
#define MY_S_IWRITE     S_IWRITE 
#define MY_S_IEXEC      S_IEXEC 
#endif

#define MY_S_ISDIR(m)	(((m) & MY_S_IFMT) == MY_S_IFDIR)
#define MY_S_ISCHR(m)	(((m) & MY_S_IFMT) == MY_S_IFCHR)
#define MY_S_ISBLK(m)	(((m) & MY_S_IFMT) == MY_S_IFBLK)
#define MY_S_ISREG(m)	(((m) & MY_S_IFMT) == MY_S_IFREG)
#define MY_S_ISFIFO(m)	(((m) & MY_S_IFMT) == MY_S_IFIFO)


#define MY_WANT_SORT     8192   
#define MY_WANT_STAT	16384	
#define MY_DONT_SORT        0

	

#ifdef USE_MY_STAT_STRUCT

typedef struct my_stat
{
  dev_t		st_dev;		
  ino_t		st_ino;		
  ushort	st_mode;	
  short		st_nlink;	
  ushort	st_uid;		
  ushort	st_gid;		
  dev_t		st_rdev;	
  off_t		st_size;	
  time_t	st_atime;	
  time_t	st_mtime;	
  time_t	st_ctime;	
} MY_STAT;

#else

#if defined(_MSC_VER)
#define MY_STAT struct _stati64 
#else
#define MY_STAT struct stat	
#endif

#endif 


typedef struct fileinfo
{
  char			*name;
  MY_STAT		*mystat;
} FILEINFO;

typedef struct st_my_dir	
{
  
  struct fileinfo	*dir_entry;
  uint			number_of_files;
} MY_DIR;

extern MY_DIR *my_dir(const char *path,myf MyFlags);
extern void my_dirend(MY_DIR *buffer);
extern MY_STAT *my_stat(const char *path, MY_STAT *stat_area, myf my_flags);
extern int my_fstat(int filenr, MY_STAT *stat_area, myf MyFlags);

#ifdef	__cplusplus
}
#endif

#endif 

