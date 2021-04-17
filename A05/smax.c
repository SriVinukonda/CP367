#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
// #include "segv.c"

/* a function in segv.c that installs a SIGSEGV handler */
void install_segv_handler(void (*)(int));
void segv_handler(int);
void ioHandler();

int count = 0;

int main(int argc, char *argv[]) {
    /* use the included segv.c code to handle SEGV */
    char *promptStr = "lol";
    install_segv_handler(segv_handler);
    struct sigaction sigAct;
    sigAct.sa_flags = SA_NODEFER; 
    sigAct.sa_handler = ioHandler;
    

    if(sigaction(SIGSYS, &sigAct, NULL) == -1) {
        printf("1");
        exit(0);
    }
     
    kill(getpid(),SIGSYS);

    return 0;
}

void segv_handler(int s) {
    printf("Max nested signals: %d\n", count);
    exit(0);
}

void ioHandler() {
    count += 1;
    kill(getpid(),SIGSYS);
}