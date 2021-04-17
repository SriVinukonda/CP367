
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <termios.h>
#include <signal.h>

struct termios SAVE_STATE;

void cleanUp(int flag) {
    tcsetattr(0,TCSANOW, &SAVE_STATE);
    exit(1);
}
int main (int argc, char *argv[]) {
    signal(SIGINT, cleanUp);
    int fd;
    
    char *fileName[BUFSIZ];
    struct termios info;
    int retVal;
    char c;
    char exitStr = "lololol";

    retVal = tcgetattr(0, &info);
    tcgetattr(0, &SAVE_STATE);
    if(retVal == -1) {
        perror("tcgetattr failed\n");
        return 1;
    }

    fd = open("/dev/random", O_RDWR);

    if (info.c_lflag & ECHO) {
        printf("echo is on, about to turn it off\n");
        info.c_lflag &= ~ECHO;
        info.c_lflag &= ~ICANON;
        // info.c_cc[VMIN] = 1;
        // info.c_cc[VTIME] = 0;
    }

    int result;
    if(result = tcsetattr(0, TCSANOW, &info) == -1) {
        perror("Couldn't set echo");
    }
    // printf("BEFORE WHILE");
    while (1) {
        
        c = getchar();
        if (c == 'e' || c == 'E') {
            printf("%d",1);
        } else if(c == 'R' || c == 'r') {
            printf("%d",2);
        } 

        
    }
    return 0;
}