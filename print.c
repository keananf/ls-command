#include "ls.h"
#include <time.h>


void process_dir(char* path);

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
 *function for both the -n and -l flags
 *
 *prints out the long version of the file passed into it.
 *This includes permissions, user and group names / ids, 
 *the modification date and the size in bytes.
 *@param entry the file to print the long information for.
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
 *Prints out the contents for each sub-directory
 *
 *@param sub_dirs the char** of sub directories to print the
 *contents of.
 */
void print_sub_dirs(char** sub_dirs, int num_sub_dirs)
{
        char* original_path = malloc(strlen(path));
        strcpy(original_path, path);

        //for each sub directory
        for(int i = 0; i < num_sub_dirs; i++)
        {
                path = sub_dirs[i];

                printf("\n%s\n", path);
                process_dir(sub_dirs[i]);
        }

        strcpy(path, original_path);
        free(original_path);
}

/**
 *prints out the directory's contents based on the respective flags
 *
 *@param directory the directory to print the contents of
 *@param files the number of files in the directory
 */
void print_dir(struct dirent** directory, int files)
{
        char** sub_dirs = R_flag ? malloc(sizeof(char*)) : NULL;
        int num_sub_dirs = 0;

        //list names of directory's contents based on flags, if applicable
        for(int i = 0; i < files; i++)
        {
                struct dirent* entry = directory[i]; 
                struct stat buffer;
                char* path_name = get_file_name(entry->d_name);
                        
                //no error
                if (lstat(path_name, &buffer) == 0) 
                {
                        if(i_flag) print_inode(&buffer); 
                        if(l_flag || n_flag) list_l(&buffer); 
                        
                        printf("%s\n", entry->d_name);
                }
                else perror("Error");
               
                //check if a sub directory 
                if(S_ISDIR(buffer.st_mode) && R_flag && 
                        strcmp(path_name, ".") != 0 && strcmp(path_name, "..") != 0) 
                {
                        char* path = malloc(strlen(path_name));
                        strcpy(path, path_name);
                        sub_dirs = realloc(sub_dirs, sizeof(sub_dirs) + sizeof(char*));
                        
                        if(sub_dirs != NULL) sub_dirs[num_sub_dirs++] = path;
                }                                        

                free(path_name);
        }
 
        print_sub_dirs(sub_dirs, num_sub_dirs);
        free(sub_dirs);
}
