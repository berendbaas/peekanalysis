#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<inttypes.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdbool.h>
#include<dirent.h>
#include<errno.h>

/**
 * Checks wether two directories have the same filenames.
 * Does not close directories. 
 * NOTE: Can we assume directories would have files in same order?
 */
bool compare_directories(DIR* left, DIR* right) {
    struct dirent* dp_left;
    struct dirent* dp_right;

    // Reset directory pointers to the beginning
    rewinddir(left);
    rewinddir(right);
    while (left && right) {
        if (errno == 0) {
            // No errors, good to go
            // Advance pointers for left and right
            dp_left = readdir(left);
            dp_right = readdir(right);

            if (dp_left == NULL && dp_right == NULL) {
                // Reached the end with no differences in either pointer
                // We're good
                return true;
            } else if (dp_left == NULL) {
                return false;
            } else if (dp_right == NULL) {
                return false;
            } else {
                // Both values exist, check if values are equal.
                if (strcmp(dp_left->d_name, dp_right->d_name) != 0) {
                    // Names are unequal => directories are not the same
                    return false;
                }
            }
        } else {
            printf("Error while iterating over left & right pointers\n");
        }
    }

    if (left || right) {
        // WTF clause
        fprintf(stderr, "Problem in one of the directory streams");
        return false;
    }
    return true;
}


/**
 * Iterates over a directory and prints all links pointing to similar directories
 * NOTE: Only checks for filenames, not filesizes or file contents.
 */
void walk_links(char* parent_path) {
    DIR* parent = opendir(parent_path);
    DIR* parent_compare = opendir(parent_path);
    DIR* child;

    size_t parent_len = strlen(parent_path); // TODO: Should this be factored out?
    struct dirent* dp_parent;

    size_t filename_len = 0;
    char* file_path;   // The potential link file
    char* target_path; // The target file
    int stat_status = 0;
    struct stat file_stat;
    bool compare_result; // result from the operation

    while (parent) {
        if (errno == 0) {
            dp_parent = readdir(parent);
            if (dp_parent != NULL) {
                // Craft new filename
                filename_len = strlen(dp_parent->d_name);
                file_path = malloc(parent_len + filename_len + 2);
                strcpy(file_path, parent_path);
                file_path[parent_len] = '/';
                strcpy(file_path + parent_len + 1, dp_parent->d_name);

                // Checking wether file is a link
                stat_status = lstat(file_path, &file_stat);
                target_path = realpath(file_path, NULL); // TODO: This should work, but check
                if (stat_status == 0
                    && S_ISLNK(file_stat.st_mode)
                    && target_path != NULL) {
                    stat_status = lstat(target_path, &file_stat);
                    if (stat_status == 0 && S_ISDIR(file_stat.st_mode)) {
                        // Link points to a directory => we can compare parent & child
                        child = opendir(target_path);
                        compare_result = compare_directories(parent_compare, child);
                        if (compare_result) {
                            printf("%s -> %s\n", parent_path, target_path);
                        }
                        
                        // Cleanup
                        closedir(child);
                    }
                }
                
                // Cleanup
                free(target_path);
                free(file_path);
            } else {
                break;
            }
        } else {
            fprintf(stderr, "Error while iterating through directory\n");
        }
    }
    
    // Cleanup
    closedir(parent);
    closedir(parent_compare);
}

/**
 * iterates through a directory depth first, only recursing on subdirs (ignoring links)
 * Calls walk_links on every node it passes
 */
void depth_first_search(char* path) {
    // If node is not a leaf, recurse
    // AKA: Does this directory have child directories?
    DIR* parent = opendir(path);
    
    size_t parent_len = strlen(path);
    struct dirent* dp_parent;
    size_t filename_len = 0;
    char* file_path;

    int stat_status = 0;
    struct stat file_stat;
    
    while (parent) {
        if (errno == 0) {
            dp_parent = readdir(parent);
            if (dp_parent != NULL) {

                if (strcmp(dp_parent->d_name, ".") != 0
                    && strcmp(dp_parent->d_name, "..") != 0) {
                    // craft new filename
                    filename_len = strlen(dp_parent->d_name);
                    file_path = malloc(parent_len + filename_len + 2);
                    strcpy(file_path, path);
                    file_path[parent_len] = '/';
                    strcpy(file_path + parent_len + 1, dp_parent->d_name);

                    // Checking wether file is a dir
                    stat_status = lstat(file_path, &file_stat);
                    if (stat_status == 0 && S_ISDIR(file_stat.st_mode)) {
                        // printf("Recursing on %s\n", file_path);
                        depth_first_search(file_path);
                    }

                    // Cleanup
                    free(file_path);
                }
            } else {
                break;
            }
        } else {
            fprintf(stderr, "Error while iterating over pointers\n");
        }
    }
    // Cleanup
    closedir(parent);

    //    printf("Cannot go deeper than %s -> Checking for links now\n", path);
    
    // walk links
    walk_links(path);
}


// Go to children of parent and check if they are links or directories. If so -> compare against parent.
int main(int argc, char** argv) {
    char* parent_path = argv[1];
    // Go through the tree depth first search
    depth_first_search(parent_path);
}
