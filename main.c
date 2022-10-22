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

void resizeHandler(int sig){
    // do nothing lol
}

void run(char* com, char** coml) {
    if (fork()) return;
    setsid();
    execvp(com, coml);
}

void alert() {
    run("notify-send", (char*[]){ "notify-send", "-u", "critical", "\"Pomodoro Alert\"", NULL });
    run("mpg123", (char*[]){ "mpg123", "-q", "alarm.mp3", NULL });
}

void timer(WINDOW* win, int c, status s){
}

void startPomo(WINDOW* win){
    int w, r, c;
    alert();
    for (int i; i < c; i++){
	alert();
	timer(win, c, WORK);
	alert();
	timer(win, c, WORK);
    }
    alert();
}

void recordStats(int m){
    int w;
}

void printStatus(WINDOW* win) {
    WINDOW* status = newwin(8, 20, 
	    (LINES - 8) / 2, 
	    (COLS - 20) / 2);
    box(status, 0, 0);
    wrefresh(status);
}

void printHeader(WINDOW* win){
    int y = 1, x = LINES - strlen(header[0]) / 2;

    for (int i = 0; i < sizeof(header)/sizeof(header[0]); i++){
	mvwaddstr(win, 0, x, header[i]);
	y++;
    }
    wrefresh(win);
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
    signal(SIGWINCH, resizeHandler);
    WINDOW* dashboard = newwin(0, 0, COLS, LINES);
    box(dashboard, 0, 0);

    printHeader(dashboard);
    printMenu(dashboard);
    wrefresh(dashboard);

    grabInput(dashboard);
}

int main(){
    setlocale(LC_ALL, "");

    initscr();
    noecho();
    raw();
    pomoDashboard();

    endwin();
    return 0;
}
