#include <errno.h>
#include <curses.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include "prime.h"
#include "dinoui.h"

const char * menu[] = {
                           "Tyrannosaurus", 
                           "Velociraptor ", 
                           "Allosaurus   ", 
                           "Diplodocus   ", 
                           "Brachiosaurus"
                      };
#define MENU_LENGTH (sizeof(menu)/sizeof(menu[0]))

/* This is a macro to simplify checking for errors from curses.
 * Almost every curses function returns ERR on an error, so to check
 * all of them consistently, it is easier to put the code in a 
 * macro. */
#define CHECK_CURSES(f, e) ((f == ERR) ? ui_down(), fprintf(stderr, "%s\n", e), exit(CURSES_ERROR) : 0)

/* curses WINDOWs for the menu and information display*/
WINDOW *menu_window, *info_window;

/* holds the current highlighted position in the menu */
int menu_position = 0;
/* holds the remaining times the highlighted position should blink */
int menu_blink = 0;

void handleInput();
void enable_kbd_signals();

void ui_up(void) {

    /* init the curses library */
    /* on error, initscr write a message and exits */
    initscr();
    
    /* set the cursor to invisible */
    CHECK_CURSES(curs_set(0), "Error setting cursor to invisible");
    
    /* turn off echo */
    CHECK_CURSES(noecho(), "Error disabling echo");
    
    /* line buffering disabled */
    CHECK_CURSES(cbreak(), "Error disabling line buffering");
    
    /* create a new window for the menu */
    menu_window = newwin(MENU_LENGTH * MENU_OFFSET + MENU_PAD, 
                         strlen(menu[0]) + (MENU_PAD * 2), 
                         MENU_TOP, 
                         MENU_LEFT);
    if (menu_window == NULL) {
        ui_down();
        fprintf(stderr, "Error creating new menu window\n");
        exit(CURSES_ERROR);
    }

    /* enable keypad for the menu window */
    CHECK_CURSES(keypad(menu_window, TRUE), "Error enabling keypad");
    
    /* create a new window for the info */
    info_window = newwin(INFO_HEIGHT + INFO_PAD, INFO_WIDTH + (INFO_PAD * 2), INFO_TOP, INFO_LEFT);
    if (info_window == NULL) {
        ui_down();
        fprintf(stderr, "Error creating new info window\n");
        exit(CURSES_ERROR);
    }
    
    /* clear the screen */
    CHECK_CURSES(clear(), "Error clearing screen");
    
    /* set initial menu position */
    menu_position = 0;
}

void draw_menu(void) {
    int i;

    /* draw a box around the menu */
    box(menu_window, 0, 0);
    
    /* iterate over the menu items */
    for (i = 0; i < MENU_LENGTH; i++) {
        /* if this is the menu position, highlight it */
        if (i == menu_position) {
            CHECK_CURSES(wstandout(menu_window), "Error standing out");
        }
    
        /* move the cursor */
        CHECK_CURSES(wmove(menu_window, MENU_OFFSET * i + (MENU_PAD / 2), MENU_PAD), "Error moving cursor");
        
        /* add the menu item */
        CHECK_CURSES(waddstr(menu_window, menu[i]), "Error adding string");
        
        /* if this is the menu position, turn off highlighting */
        if (i == menu_position) {
            CHECK_CURSES(wstandend(menu_window), "Error standing in");
        }
    }
    
    /* show the screen we just generated */
    CHECK_CURSES(wrefresh(menu_window), "Error refreshing menu_windowdow");
}

void draw_info(const char *target) {
    /* delete whatever was in the info window */
    CHECK_CURSES(werase(info_window), "Error erasing window");

    /* draw a box */
    box(info_window, 0, 0);
    
    /* move the cursor to the top of the box */
    CHECK_CURSES(wmove(info_window, INFO_PAD / 2, INFO_PAD), "Error moving cursor");
    /* write the string */
    CHECK_CURSES(waddstr(info_window, target), "Error adding string");
    /* refresh */
    CHECK_CURSES(wrefresh(info_window), "Error refreshing info window");
}

void ui_down(void) {
    /* turn off curses and reset the tty */
    /* don't use the error macro, since it will call this function */
    if (endwin() == ERR) {
        fprintf(stderr, "Error disabling curses\n");
        exit(CURSES_ERROR);
    }
}

/**
 * If selected, blink the menu cursor over the highlighted item.
 * Called by the timer signal.
 */
void blink_cursor(int signal) {
    static int blink_status = 0;

    if (menu_blink > 0) {
        if (blink_status) {
            CHECK_CURSES(wstandout(menu_window), "Error standing out");
            blink_status = 0;
        } else {
            CHECK_CURSES(wstandend(menu_window), "Error standing in");
            blink_status = 1;
        }
        /* move the cursor */
        CHECK_CURSES(wmove(menu_window, MENU_OFFSET * menu_position + (MENU_PAD / 2), MENU_PAD), "Error moving cursor");
        /* add the menu item */
        CHECK_CURSES(waddstr(menu_window, menu[menu_position]), "Error adding string");

        /* show the screen we just generated */
        CHECK_CURSES(wrefresh(menu_window), "Error refreshing menu_windowdow");
        /* count down the blinking */
        menu_blink--;
        /* if we ended, make sure the status is reset */
        if (menu_blink == 0) {
            blink_status = 0;
        }
        /* make sure we ened with standout mode off */
        CHECK_CURSES(wstandend(menu_window), "Error standing in");
    }
}

/**
 * Set up or disable the timer used for blinking the cursor.
 * pass zero to disable the timer.
 */
void set_timer(int ms) {
    struct itimerval timeset;
    long nsec, nusec;
    int saved_errno;
    
    /* first register the signal handler */
    if (ms > 0) {
        signal(SIGVTALRM, blink_cursor);
    } else {
        /* if disabling, set to ignore */
        signal(SIGVTALRM, SIG_IGN);
    }

    /* figure out the second and usecond components */
    nsec = ms / 1000;
    nusec = (ms % 1000) * 1000L;
    
    /* set both the interval and initial timers */
    timeset.it_interval.tv_sec = nsec;
    timeset.it_interval.tv_usec = nusec;
    timeset.it_value.tv_sec = nsec;
    timeset.it_value.tv_usec = nusec;
    
    if (setitimer(ITIMER_VIRTUAL, &timeset, NULL) == -1) {
        saved_errno = errno;
        ui_down();
        errno = saved_errno;
        perror("setitimer");
        exit(TIMER_ERROR);
    }
}

void handleInput() {


    int key;

    // qiflush();
    // cbreak();

    flushinp();

    key = wgetch(menu_window);
    switch (key) {
        case KEY_DOWN:
            /* change what is highlighted in the menu */
            menu_position = (menu_position + 1) % MENU_LENGTH;
            break;
        case KEY_UP:
            /* change what is highlighted in the menu */
            if (menu_position == 0) {
                menu_position = MENU_LENGTH - 1;
            } else {
                menu_position = menu_position - 1;
            }
            break;
        case '\n':
            /* add the info to the screen */
            draw_info(menu[menu_position]);
            /* set up the cursor blinking */
            menu_blink = 4;
            /* set the timer to go off every 100 ms */
            set_timer(100);
            /* wait */
            while (menu_blink > 0);
            /* disable the timer */
            set_timer(0);
            break;
        case ERR:
            /* an error occurred in wgetch */
            ui_down();
            fprintf(stderr, "Error getting character from terminal\n");
            exit(CURSES_ERROR);
            break;
    }
    draw_menu();

    

}

int main (int argc, char * argv[]) {
    signal(SIGIO,handleInput);
    enable_kbd_signals();


    int prime = 100000;
    char *primeStr[8192];
    int primeFile;
    primeFile = open("primes.out", O_CREAT|O_APPEND|O_WRONLY);

    ui_up();
    draw_menu();


    while(true) {
        if((INT_MAX > prime)) {
            if(is_prime(prime) == 1) {
                sprintf(primeStr, "%d\n", prime);
                write(primeFile,&primeStr,strlen(primeStr));
            }
            prime += 1;
        }   
    }

   
    ui_down();
    return 0;
}

void enable_kbd_signals() {
    int fd_flags;

    fcntl(0, F_SETOWN, getpid());
    fd_flags = fcntl(0, F_GETFL);
    fcntl(0, F_SETFL, (fd_flags|O_ASYNC));
}