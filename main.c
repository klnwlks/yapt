// replace 0s with proper values

#include <ncurses.h>
#include <stdlib.h> 
#include <unistd.h>
#include <signal.h>
#include <locale.h>
#include <string.h>
#include <time.h>

typedef enum { WORK, REST } status;
const char* header[] = {}; // move to a separate config.h
const char* work[] = {}; // move to a separate config.h
const char* rest[] = {}; // move to a separate config.h

int W = 30, R = 5, C = 4;

void resizeHandler(int sig){
    // do nothing lol
}

// after a (very painful) hour of attempting to figure out how to grab the size of a c string array,
// i give up and resort to using a simpler solution
void printHeader(WINDOW* win, const char** h, size_t s){
    int y = 1, x = LINES - strlen(h[0]) / 2;

    for (int i = 0; i < s; i++){
	mvwaddstr(win, y, x, h[i]);
	y++;
    }
    wrefresh(win);
}


void run(char* com, char** coml) {
    if (fork()) return;
    setsid();
    execvp(com, coml);
}

void recordStats(int m){
    int w;
}

void alert() {
    run("notify-send", (char*[]){ "notify-send", "-u", "critical", "\"Pomodoro Alert\"", NULL });
    run("mpg123", (char*[]){ "mpg123", "-q", "alarm.mp3", NULL });
}

// setup actual timer itself, will also print
// current status, header and time left.
void timer(WINDOW* win, int t, int c, status s){
    while (c >= 0) {
	// wclear includes the box so i have to call this every clear
	box(win, 0, 0);

	mvwprintw(win, 0, 0, "Cycle %d of %d", c, C);
	switch (s) {
	    case WORK:
		printHeader(win, work, 0);
		mvwprintw(win, 0, 0, "Work.");
		break;
	    case REST:
		printHeader(win, rest, 0);
		mvwprintw(win, 0, 0, "Rest.");
		break;
	}

	mvwprintw(win, 
	    // set at middle
	    0, 0,
	    "%d:%d",
	    t / 60, (t % 3600) % 60);


	sleep(1);
	wclear(win);
	c--;
    }
}

// setup window and loop for the cycles
void startPomo(WINDOW* win){
    WINDOW* pomo_win = newwin(0, 0, 0, 0);
    alert();
    for (int i; i < C; i++){
	alert();
	timer(win, W, i, WORK);
	alert();
	timer(win, C, i, WORK);
    }
    alert();
    delwin(pomo_win);
}

void printStatus(WINDOW* win) {
    WINDOW* status = newwin(8, 20, 
	    (LINES - 8) / 2, 
	    (COLS - 20) / 2);
    box(status, 0, 0);
    wrefresh(status);
}

void printMenu(WINDOW* win){
    const char* menu[] = {}; // move to a separate config.h
    int y = (sizeof(header)/sizeof(header[0]) + 3),
	x = LINES - strlen(menu[0]) / 2;

    for (int i = 0; i < sizeof(menu)/sizeof(menu[0]); i++){
	mvwaddstr(win, 0, x, menu[i]);
	y++;
    }
    wrefresh(win);
}

void grabInput(WINDOW* win){
    int ch;
    while (1) {
	ch = wgetch(win);
	switch(ch) {
	    case 'j':
	    case 'J':
		printStatus(win);
		break;

	    case 'k':
	    case 'K':
		startPomo(win);
		break;

	    case 'q':
	    case 'Q':
		return;
		break;
	}
    }
}

void pomoDashboard(){
    // handler for resize and etc.
    signal(SIGWINCH, resizeHandler);
    WINDOW* dashboard = newwin(0, 0, COLS, LINES);
    box(dashboard, 0, 0);

    printHeader(dashboard, header, 0);
    printMenu(dashboard);
    wrefresh(dashboard);

    grabInput(dashboard);
    // once function above ends, cleanup begins
    delwin(dashboard);
}

void help() {}

int main(int argc, char* argv[]){
    if (argc > 3 && argc < 5) {
	W = atoi(argv[1]);
	R = atoi(argv[2]);
	C = atoi(argv[3]);
    } else { 
	help(); 
    }

    setlocale(LC_ALL, "");

    initscr();
    noecho();
    raw();
    pomoDashboard();

    endwin();
    return 0;
}
