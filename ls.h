#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <dirent.h>
#include <sys/dir.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifndef MAX_CHARS

#define _XOPEN_SOURCE 700
#define MAX_CHARS 255

char* path;

//flags

//print info about permissions,group, user, modification, and size 
int n_flag;
int l_flag;

//recursively print out sub-directories' contents
int R_flag;

//print based on size (ascending)
int S_flag;

//print based on time (ascending)
int t_flag;

//show inode number
int i_flag;

//reverse the order of printing
int r_flag;

//show hidden files
int a_flag;

#endif
 
