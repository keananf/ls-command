#include "ls.h"
#include <time.h>

/**
 *This function gains the relative path for the file passed in
 *
 *Appends the file name to the path name stored in the global variable,
 *so that lstat will return the stat struct for the correct file.
 *@param file_name the name of the file
 *@return the relative path based on the current directory
 */
char* get_file_name(char* file_name)
{
        char* path_name = malloc(strlen(path) + strlen(file_name));        
 
        strcpy(path_name, path);
        
        strcat(path_name, "/");
        strcat(path_name, file_name);

        return path_name;
}

/**
 *prints the time last modified for the given stat struct
 *
 *Converts the time_t last modified field into a struct,
 *and then prints a formatted string.
 *@param entry the stat struct representing the current
 *entry in the directory
 */
void print_time(struct stat* entry)
{
        char* months[] = {"Jan", "Feb", "Mar", "Apr",
                         "May", "Jun", "Jul", "Aug",
                         "Sep", "Oct", "Nov", "Dec"};

        struct tm* time = gmtime(&(entry->st_mtime));
        printf("%3s  %2d %2d:%2d ", months[time->tm_mon], time->tm_mday
                                        ,time->tm_hour + 1, time->tm_min);        
}


/**
 *prints the permissions for the given stat struct
 *
 *performs bitwise operations to ascertain the different permissions,
 *and subsequently prints out the results of said operation
 *@param entry the stat struct representing the current
 *entry in the directory
 */
void print_permissions(struct stat* entry)
{
        printf( (S_ISDIR(entry->st_mode)) ? "d" : "-");

        //user
        printf( (entry->st_mode & S_IRUSR) ? "r" : "-");
        printf( (entry->st_mode & S_IWUSR) ? "w" : "-");
        printf( (entry->st_mode & S_IXUSR) ? "x" : "-");

        //group
        printf( (entry->st_mode & S_IRGRP) ? "r" : "-");
        printf( (entry->st_mode & S_IWGRP) ? "w" : "-");
        printf( (entry->st_mode & S_IXGRP) ? "x" : "-");
        
        //others
        printf( (entry->st_mode & S_IROTH) ? "r" : "-");
        printf( (entry->st_mode & S_IWOTH) ? "w" : "-");
        printf( (entry->st_mode & S_IXOTH) ? "x " : "- ");
}

/**
 *prints the inode number of the given stat struct
 *
 *@param entry the stat struct representing the current
 *entry in the directory
 */
void print_inode(struct stat* entry)
{
        printf("%-7lu", entry->st_ino);
}

/**
 *prints the size  of the given stat struct
 *
 *@param entry the stat struct representing the current
 *entry in the directory
 */
void print_size(struct stat* entry)
{
        printf("%-3lu", entry->st_blocks);
}

/**
 *function for both the -n and -l flags
 *
 *
 */
void list_l(struct stat* entry)
{
        print_permissions(entry);
        printf("%1lu ", entry->st_nlink);
        
        //print ids
        if(n_flag)
        {
                printf("%5u ", entry->st_uid);
                printf("%5u ", entry->st_gid);
        }
        //print names
        else
        {
                printf("%4s ", getpwuid(entry->st_uid)->pw_name);
                printf("%7s ", getgrgid(entry->st_gid)->gr_name);
        }
        printf("%6lu ", entry->st_size);
        print_time(entry);
}

/**
 *prints out the directory's contents based on the respective flags
 */
void print_dir()
{
   
        printf("------------------\n");
        //list names of directory's contents based on flags, if applicable
        for(int i = 0; i < num_files; i++)
        {
                struct dirent* entry = directory[i]; 
                struct stat buffer;
                char* path_name = get_file_name(entry->d_name);
                        
                //no error
                if (lstat(path_name, &buffer) == 0)
                {
                        if(i_flag) print_inode(&buffer); 
                        if(s_flag) print_size(&buffer); 
                        if(l_flag || n_flag) list_l(&buffer); 
                        
                        printf("%s\n", entry->d_name);
                }
                else perror("Error");
                
                free(path_name);
        }
        printf("------------------\n");
}
