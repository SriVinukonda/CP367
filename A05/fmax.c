#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

/* Modify this program to output the maximum number of file descriptors
   that a process may hold open at once on your system */
int main(int argc, char * argv[]) {
    int count = 3;

    
    while(1) {
        int fd = open("guineaPig.txt", O_CREAT|O_APPEND|O_WRONLY);
        if (fd >= 0) {
            count+= 1;
            // printf("%d\n",count);
        } else {
            break;
        }
    }
    printf("Max file descriptors is: %d\n", count);
  
    return 0;
}

// int openFd(int count) {

//     int fd;
//     if(fd = open("guineaPig.txt", O_CREAT|O_APPEND|O_WRONLY)) {
//         count + 1;
//         return openFd + count;
//     } else {
//         return 0;
//     }

    

    
// }
