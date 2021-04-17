#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <stdlib.h>

#define PATHSIZE 512


int recurseDirectories(char *username, char *file);

int main(int argc, char** argv) {
    DIR *dir = NULL;
    struct dirent *dirPointer;
    struct group *gr;
    struct passwd *pw;
    gid_t *groups;

    char *userIn[PATHSIZE];
    char *currentDirectory[256]; 
    int ngroups;
    

    groups = malloc(ngroups * sizeof(gid_t));
    if(argc < 2) {
        scanf("%s",userIn);
        printf("The input from keyboard is %s:\n",userIn);
    } else {
        strcpy(userIn,argv[1]);
        printf("The input from commandline is %s\n",userIn);
    }

    if(getpwnam(userIn)  == NULL) {
        printf("ERROR: user doesn't exist\n");
        return -1;
    } 

    dir = opendir(".");
    if (dir == NULL) {
        printf("Directory doesn't exist: INSIDE THE FUNCTION\n");
        return -1;
    }

    getgrouplist(userIn,NULL, groups, &ngroups);
    printf("\nngroups: %d\n",ngroups);
    getgrouplist(userIn,getpwnam(userIn)->pw_gid, groups, &ngroups);
    printf("\nngroups: %d\n",ngroups);
    for (int j = 0; j < ngroups; j++) {
        printf("%d", groups[j]);
        gr = getgrgid(groups[j]);
        if (gr != NULL)
            printf("  (%s)", gr->gr_name);
        printf("\n");

    }

    strcpy(currentDirectory, ".");
    
    recurseDirectories(userIn, currentDirectory);
    return 0;
}   

int recurseDirectories(char *userName, char *file) {
    DIR *dir = NULL;
    struct dirent *dirPointer;
    struct stat stats;

    char temp[PATHSIZE];
    dir = opendir(file);
    if (dir == NULL) {
        printf("Directory doesn't exist: INSIDE THE FUNCTION\n");
        return 0;
    }


    while ((dirPointer = readdir(dir)) != NULL) {
        // strcpy(temp, file);
        if(dirPointer->d_type == DT_DIR) {
            if(strcmp(dirPointer->d_name,".") != 0 && strcmp(dirPointer->d_name,"..") != 0) {
                printf("%s\n",file);
                
                strcat(file,"/");
                strcat(file,dirPointer->d_name);

                if((opendir(file)) != NULL){
                    recurseDirectories(userName, file);
                } 

            }

        //ADDED PRINT STATEMENT
        } else if(dirPointer->d_type == DT_REG) {
            char filePath[PATHSIZE];
            stat(dirPointer->d_name,&stats);
            printf("------------\nINSIDE ELSE\n------------------\n");
            printf("%s == ", getpwuid(stats.st_uid)->pw_name);
            printf("%s\n",userName);
            if(strcmp(getpwuid(stats.st_uid)->pw_name, userName) == 0) {
                if(stats.st_mode & S_IRUSR) {
                    printf("Filename: %s\n",dirPointer->d_name);
                    realpath(dirPointer->d_name,filePath);
                    printf("Filepath: %s\n", filePath);
                }
            } 
        }
        // strcpy(file,temp);

    }

    closedir(dir);
}