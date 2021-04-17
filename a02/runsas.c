#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <pwd.h>

int main(int argc, char** argv) {
    
    struct stat stats;
    
    if(argc != 2) {
        fprintf(stderr,"ERROR: Incorrect amount of arguments.\n");
        return -1;
    }
    int fileExists = stat(argv[1],&stats);

    if(fileExists == -1) {
        fprintf(stderr,"ERROR: Incorrect file does not exist.\n");
        return -1;
    }
    if( !(stats.st_mode & S_IXUSR)) {
        fprintf(stderr,"ERROR: File not executable.\n");
        return -1;
    } else {
        if(S_ISUID & stats.st_mode) {
            printf("The owner is: %s", getpwuid(stats.st_uid)->pw_name);
        } else {
            printf("The effective user is: %s", getpwuid(geteuid())->pw_name);
            // printf("ERROR: set-uid bit not set.\n");
        }
    }
    return 0;
}