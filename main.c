#include "ls.h"

void process_dir(char* path);
void free_directory(struct dirent** dir, int num_files);
void check_args(int argc, char** argv);
void print_dir(struct dirent** dir, int files);

extern int filter(struct dirent* entry);
extern int compare(struct dirent** entry1, struct dirent** entry2);

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
        struct stat dir;
        int res = lstat(path, &dir);
        int num_files = scandir(path, &directory, filter, compare);
                
        //scandir failed
        if(num_files < 0)
        {
                perror("Scandir failed...");
                return;
        }
        else if (num_files > 0 && res == 0 && S_ISDIR(dir.st_mode))
        {
                print_dir(directory, num_files);
               
        }
        //empty directory
        else
        {
                printf("Empty Directory...\n");        
        }
        free_directory(directory, num_files);       
}

