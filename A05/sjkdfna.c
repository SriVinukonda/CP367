#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

/* a function in segv.c that installs a SIGSEGV handler /
void install_segv_handler(void ()(int));
void segv_handler(int);

int count;

/* Modify this program to output the maximum number of file descriptors
   that a process may hold open at once on your system */
int main(int argc, char argv[]) {
    /* use the included segv.c code to handle SEGV */
    install_segv_handler(segv_handler);

    struct sigaction temp;

    return 0;
}

void handler_tester() {
    count++;
}

void segv_handler(int s) {
    printf("Max nested signals: %d\n", count);
    exit(0);
}