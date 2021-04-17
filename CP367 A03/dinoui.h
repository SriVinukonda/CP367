#define CURSES_ERROR 1
#define TIMER_ERROR 2

#define MENU_LEFT 5
#define MENU_TOP 5
#define MENU_OFFSET 2
#define MENU_PAD 4

#define INFO_LEFT 40
#define INFO_TOP 5
#define INFO_WIDTH 20
#define INFO_HEIGHT 20
#define INFO_PAD 4

void ui_up(void);
void ui_down(void);
void draw_menu(void);
void draw_info(const char *);
void set_timer(int);
void blink_cursor(int);