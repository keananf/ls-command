#include "ls.h"

void process_dir(char* path);
void free_directory(struct dirent** dir, int num_files);
int check_args(int argc, char** argv);
void print_dir(struct dirent** dir, int files);

extern int filter(const struct dirent* entry);
extern int compare(const struct dirent** entry1, const struct dirent** entry2);

int main(int argc, char** argv)
{
        path = malloc(2);
        strcpy(path, ".");

        //read directory which is specified
        if(argc > 1)
        {
                int num_paths = check_args(argc, argv);
                //print out each directory passed into the program
                for(int i = 0; i < num_paths; i++)
                {
                        path = realloc(path,strlen(paths[i]));
                        strcpy(path, paths[i]);
                        printf(RED "\n%s:\n" RESET, path);
                        process_dir(path);
                }
                free(paths);
        }

        //read current directory
        else if(argc == 1) 
        {
                process_dir(path);
        }

        else
        {
                printf("Incorrect number of arguments\n");
        }

        free(path);
        return 1;
}

/**Calls scandir to list the contents of the directory
 *
 *This function calls scandir to load in the contents of the
 *directory into the dirent** struct, and subsequently lists the names of
 *its contents if it succeeded.
 *@param[in] path the path to the directory
 */
void process_dir(char* path)
{
        struct dirent** directory;
        int num_files = scandir(path, &directory, filter, compare);
                
        //scandir failed
        if(num_files < 0)
        {
                perror("Scandir failed...");
                return;
        }
        else if (num_files > 0)
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

