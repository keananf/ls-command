#include "ls.h"
#include <regex.h>
#include <errno.h>
#include <time.h>

char* get_file_name(char* name);
/**
 *Checks that the names of a file doesnt start with '.'
 *
 *This function filters out contents that start with a '.'
 *so that scandir will not return them.
 *@param entry the content to be checked
 *@return 0 is the file starts with a '.', and otherwise 1
 */
int filter(const struct dirent* entry)
{
        //if first char is a '.'
        if(entry->d_name[0] == '.')
        {
                return a_flag ? 1 : 0;
        }
        return 1;       
}

/*
 *gets scandir to sort the contents by time
 *
 *If the t_flag is active, this function will
 *create two stat structs for the two dirent structs passed in, and will return the 
 *difference of their sizes in bytes. This sorts the directory's contents by size, insttead
 *of alphabetically.
 *@param entry1 the first entry to be compared
 *@param entry2 the second entry to be compared
 */
int t_compare(struct dirent** entry1, struct dirent** entry2)
{
        struct stat one, two;
               
        //get whole path relative to current directory
        char* name1 = get_file_name((*entry1)->d_name);
        char* name2 = get_file_name((*entry2)->d_name);
         
        //if can stat these files, compare their sizes
        if((lstat(name1, &one) == 0) && (lstat(name2, &two) == 0))
        {        
                //if r flag active as well, sort ascending based on file size
                //else descending
                int compare = !r_flag ? (int) difftime(two.st_mtime, one.st_mtime) 
                                      : (int) difftime(one.st_mtime, two.st_mtime); 
                return compare;
        }
        return 0;
}

/*
 *gets scandir to sort the contents by size in bytes
 *
 *If the s_flag is active, this function will
 *create two stat structs for the two dirent structs passed in, and will return the 
 *difference of their sizes in bytes. This sorts the directory's contents by size, insttead
 *of alphabetically.
 *@param entry1 the first entry to be compared
 *@param entry2 the second entry to be compared
 */
int S_compare(struct dirent** entry1, struct dirent** entry2)
{
        struct stat one, two;
               
        //get whole path relative to current directory
        char* name1 = get_file_name((*entry1)->d_name);
        char* name2 = get_file_name((*entry2)->d_name);
         
        //if can stat these files, compare their sizes
        if((lstat(name1, &one) == 0) && (lstat(name2, &two) == 0))
        {        
                //if r flag active as well, sort ascending based on file size
                //else descending
                int compare = !r_flag ? two.st_size - one.st_size 
                                      : one.st_size - two.st_size;

                return compare;
        }
        return 0;
}

/**
 *The comparison function used with scandir
 * 
 *This function checks which flags are active to deside how to compare.
 *It utilises the strcasecmp function
 *to check whether the first string is less than, equal to,
 *or greater than the second string, and will return -1, 0, or 1
 *repectively. 
 *@param entry1 the first entry to be compared
 *@param entry2 the second entry to be compared
 *@return an integer -1, 0, or 1 if the first string was less
 *than, equal to or greater than the second.
 */
int compare(struct dirent** entry1, struct dirent** entry2)
{
        //sort by time
        if(t_flag)
        {
                return t_compare(entry1, entry2);
        }
        //sort by size in bytes
        if(S_flag)
        {
                return S_compare(entry1, entry2);            
        }
        
        //otherwise sort alphabetically (ascending)
        if(!r_flag) return strcasecmp((*entry1)->d_name,(*entry2)->d_name);
        
        //descending
        return strcasecmp((*entry2)->d_name,(*entry1)->d_name);

}

/**
 *Sets the flags for this particular arg
 *
 *This method gets called whenever a '-' is seen,
 *indicating a flag. It also iterates through the flag
 *if there is more than one char coming after the '-',
 *to recognise args like '-Sn,' and subsequently set the corresponding
 *flags
 *@param arg the current flag being analysed
 */
void set_flags(char* arg)
{
        //allows for args like '-Sn' instead of just '-S -n'
        for(unsigned int j = 1; j < strlen(arg); j++)
        {
                switch(arg[j])
                {
                        case 'l':
                                l_flag = 1;
                                break;
                        case 'S':
                                S_flag = 1;
                                break;
                        case 'R':
                                R_flag = 1;
                                break;
                        case 't':
                                t_flag = 1;
                                break;
                        case 'i':
                                i_flag = 1;
                                break;
                        case 'n':
                                n_flag = 1;
                                break;
                        case 'r':
                                r_flag = 1;
                                break;
                        case 'a':
                                a_flag = 1;
                                break;
                        
                        //unrecognised flag
                        default:
                        {
                                errno = EINVAL;
                                perror("Invalid flag");
                                exit(0);
                        }
                }
       }
}

/**
 *Looks at the different args passed into the program
 *
 *This method looks at all other arguments passed into the program
 *besides the first one (program name) to ascertain
 *which flags are present. It checks for the '-' char to indicate
 *a flag, as well as for a directory name. 
 *@param argc the total number of args
 *@param argv the list of command line args passed to main
 */
void check_args(int argc, char** argv)
{
        //for each command line arg
        for(int i = 1; i < argc; i++)
        {
                char* arg = argv[i];
               
                //if flag
                if(arg[0] == '-')
                {
                       set_flags(arg);
                }
                //path hasn't been set yet
                else if(strcmp(path, ".") == 0)
                {
                        path = realloc(path, strlen(arg) + 1); 
                        if(path != NULL) strcpy(path,arg);
                }
                
                //path already set, indicating malformed arg list
                else
                {
                        errno = EINVAL;
                        perror("Try Again.");
                        exit(0);
                }
        }        
}

/**
 *Free all entries within the directory global variable
 *
 *This function calls free on each different struct dirent*
 *within the directory dirent**
 */
void free_directory(struct dirent** directory, int num_files)
{
        for(int i = 0; i < num_files; i++)
        {
                free(directory[i]);
        }
        free(directory);
}
