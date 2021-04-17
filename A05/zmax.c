#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


/* Modify this program to output the maximum number of zombie process
   that a process may have at once on your system */
int main(int argc, char * argv[]) {
    int count;
    
    pid_t currPid = fork();
    while(currPid != -1) {
            if(currPid == 0) {
                exit(0);
            }
            count+=1; 
            currPid = fork();
            
            
    }
    printf("Max zombie processes is: %d\n", count);

    return 0;
}
