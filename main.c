#include "ls.h"

void process_dir(char* path);
void free_directory();
void check_args(int argc, char** argv);
void print_dir();

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

        free_directory();       
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
        num_files = scandir(path, &directory, filter, compare);
                
        //scandir failed
        if(num_files < 0)
        {
                perror("Scandir failed...");
                exit(0);
        }
        else if (num_files > 0)
        {
                print_dir(); 
        }
        //empty directory
        else
        {
                printf("Empty Directory...\n");        
        }
}


