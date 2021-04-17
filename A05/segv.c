#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

/* This function installs a SIGSEGV handler using an alternate stack.
   This is required to handle the SIGSEGV signal when the max nested
   signals is reached. 
   DO NOT MODIFY THIS FILE! */
void install_segv_handler(void (*h)(int)) {
    stack_t segv_stack;
    struct sigaction action;

    /* allocate space for an alternate signal stack */
    segv_stack.ss_sp = malloc(SIGSTKSZ);
    if (segv_stack.ss_sp == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    /* install the alternate signal stack */
    segv_stack.ss_size = SIGSTKSZ;
    segv_stack.ss_flags = 0;
    if (sigaltstack(&segv_stack, NULL) == -1) {
         perror("sigaltstack");
         exit(EXIT_FAILURE);
    }

    /* set the handler */
    action.sa_handler = h;
    /* tell it to use the alternate stack */
    action.sa_flags = SA_ONSTACK;
    sigemptyset(&action.sa_mask);
    /* install for segv */
    sigaction(SIGSEGV, &action, NULL);
}