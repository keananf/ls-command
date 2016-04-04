#include "ls.h"

void process_dir(char* path);
void free_directory(struct dirent** dir, int num_files);
void check_args(int argc, char** argv);
char** print_dir(struct dirent** dir, int files);

extern int filter(struct dirent* entry);
extern int compare(struct dirent** entry1, struct dirent** entry2);

extern int num_sub_dirs;

int main(int argc, char** argv)
{
        path = ".";

        //read directory which is specified
        if(argc > 1)
        {
                check_args(argc, argv);
                process_dir(path);
        }

        //read current directory
        else if(argc == 1) 
        {
                process_dir(path);
        }

        else
        {
                printf("Incorrect number of arguments\n");
                return 0;
        }

        return 1;
}

/**
 *Prints out the contents for each sub-directory
 *
 *@param sub_dirs the char** of sub directories to print the
 *contents of.
 */
void print_sub_dirs(char** sub_dirs)
{
        int num = num_sub_dirs;
        num_sub_dirs = 0;
        //for each sub directory
        for(int i = 0; i < num; i++)
        {
                path = sub_dirs[i];
                printf("\n%s\n", path);
                process_dir(sub_dirs[i]);
        }
        free(sub_dirs);
}

/**
 *Calls scandir to list the contents of the directory
 *
 *This function calls scandir to load in the contents of the
 *directory into directory, and subsequently lists the names of
 *its contents if it succeeded.
 *@param path the path to the directory
 */
void process_dir(char* path)
{
        struct dirent** directory;
        int num_files = scandir(path, &directory, filter, compare);
                
        //scandir failed
        if(num_files < 0)
        {
                perror("Scandir failed...");
        }
        else if (num_files > 0)
        {
                char** sub_dirs = print_dir(directory, num_files);
                print_sub_dirs(sub_dirs);
        }
        //empty directory
        else
        {
                printf("Empty Directory...\n");        
        }
        free_directory(directory, num_files);       
}

