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
        char* path_name = malloc(strlen(path) + 3 + strlen(file_name));        
 
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
        char* str = ctime(&(entry->st_mtime));
        char* time = malloc(strlen(str)- 1);
        strncpy(time,str,strlen(str) - 1);

        printf("%s ", time);
        free(time);
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
                struct group* group = getgrgid(entry->st_gid);
                struct passwd* user = getpwuid(entry->st_uid); 
                user != NULL ? printf("%4s ", user->pw_name) : perror("Error retrieving user name");
                group != NULL ? printf("%7s ", group->gr_name) : perror("Error retrieving group name");
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
        char* original_path = malloc(strlen(path) + 1);
        strcpy(original_path, path);

        //for each sub directory
        for(int i = 0; i < num_sub_dirs; i++)
        {
                path = realloc(path, strlen(sub_dirs[i]) + 1);
                if(path != NULL) 
                {
                        strcpy(path, sub_dirs[i]);

                        printf("\n%s\n", path);
                        process_dir(sub_dirs[i]);
                        free(sub_dirs[i]);
                }
        }

        path = realloc(path, strlen(original_path) + 1);
        if(path != NULL) strcpy(path, original_path);
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
        char** sub_dirs = R_flag ? malloc(0) : NULL;
        int num_sub_dirs = 0;
        int size = 0;

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
                        strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) 
                {
                        sub_dirs = realloc(sub_dirs, size + strlen(path_name));
                        
                        if(sub_dirs != NULL) sub_dirs[num_sub_dirs++] = path_name;
                        size += strlen(path_name);
                }
                else free(path_name);                                        
        }
        if(R_flag && size > 0)
        { 
                print_sub_dirs(sub_dirs, num_sub_dirs);
                free(sub_dirs);
        }
}
