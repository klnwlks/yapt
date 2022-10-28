#include <ncurses.h>
#include <stdlib.h> 
#include <unistd.h>
#include <signal.h>
#include <locale.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "config.h"

typedef enum { WORK, REST } status;
int W = 30, R = 5, C = 4;

void resizeHandler(){
    // do nothing lol
    wrefresh(stdscr);
}

// after a (very painful) hour of attempting to figure out how to grab the size of a c string array,
// i give up and resort to using a simpler solution
void printHeader(WINDOW* win, const char** h, size_t s, int winx){
    int y = 1, x = (winx - strlen(h[0])) / 2;

    for (int i = 0; i < (int) s; i++){
	mvwprintw(win, y, x, "%s", h[i]);
	y++;
    }
    wrefresh(win);
}


void run(char* com, char** comm) {
    if (fork()) return;
    setsid();
    execvp(com, comm);
}

void recordStats(int m){
    // hours, sessions, then the same but weekly
    float h = 0, wh = 0;
    int s = 0, ws = 0; 
    FILE* f = fopen("stats", "w+"); // open for read and write
    if (!f) return; // replace with exception

    char day[10];
    time_t r_time = time(0);
    struct tm* tinfo = localtime(&r_time);
    strftime(day, 10, "%a", tinfo);

    // read stats
    fscanf(f, "%f %d %f %d", &h, &s, &wh, &ws); 

    h += fmod((float)m, 60.0); 
    s++;

    // if a week has passed, refresh statistics
    if (ws > 0 && !strcmp(day, "Sun")) {
	wh = 0;
	ws = 0;
    } else {
	wh += fmod((float)m, 60.0); 
	ws++;
    }

    // write stats
    fprintf(f, "%f %d %f %d", h, s, wh, ws);
    fclose(f);
}

void alert() {
    // dunno if this is the best solution
    run("notify-send", (char*[]){ "notify-send", "-u", "critical", "\"Pomodoro Alert\"", NULL });
    run("mpg123", (char*[]){ "mpg123", "-q", "alarm.mp3", NULL });
}

// setup actual timer itself, will also print
// current status, header and time left.
void timer(WINDOW* win, int t, int c, status s){
    int y = 0, x = getmaxx(win);
    while (t >= 0) {
	// wclear includes the box so i have to call this every clear
	box(win, 0, 0);

	switch (s) {
	    case WORK:
		y = wsize + 2;
		printHeader(win, work, wsize, x);
		mvwprintw(win, y, (x - 7) / 2, " work.");
		y++;
		break;
	    case REST:
		y = rsize + 2;
		printHeader(win, rest, rsize, x);
		mvwprintw(win, y, (x - 7) / 2, " rest.");
		y++;
		break;
	}

	mvwprintw(win, 
	    // set at middle
	    y, (x - 5) / 2,
	    "%d:%d",
	    t / 60, (t % 3600) % 60);
	y++;
	mvwprintw(win, y, (x - 12) / 2, "Cycle %d of %d", c, C);


	wrefresh(win);
	sleep(1);
	wclear(win);
	t--;
    }
}

void printStats(WINDOW* win) {
    WINDOW* stats = newwin(15, 30, 
	    (LINES - 15) / 2, (COLS - 30) / 2);
    box(stats, 0, 0);

    float h = 0, wh = 0;
    int s = 0, ws = 0, y = 1, x = getmaxx(stats); 
    FILE* f = fopen("stats", "r"); // open for read
    if (!f) return; // replace with exception

    // read stats
    fscanf(f, "%f %d %f %d", &h, &s, &wh, &ws); 
    fclose(f);

    mvwprintw(stats, y, (x - 5) / 2, "STATS");
    y += 2;
    mvwprintw(stats, y, (x - 15) / 2, "Total Hours: %.1f", h);
    y++;
    mvwprintw(stats, y, (x - 18) / 2, "no. of sessions: %d", s);
    y += 2;
    mvwprintw(stats, y, (x - 12) / 2, "WEEKLY STATS");
    y += 2;
    mvwprintw(stats, y, (x - 15) / 2, "Total Hours: %.1f", wh);
    y++;
    mvwprintw(stats, y, (x - 18) / 2, "no. of sessions: %d", ws);
    mvwprintw(stats, getmaxy(stats) - 2, (x - 15) / 2, "any key to exit");

    wrefresh(stats);
    wgetch(stats);

    delwin(stats);
    wrefresh(win);
}


// setup window and loop for the cycles
void startPomo(WINDOW* win){
    WINDOW* pomo_win = newwin(15, 30,
	    (LINES - 15) / 2, 
	    (COLS - 30) / 2);
    box(pomo_win, 0, 0);
    wclear(win);

    alert();
    for (int i = 0; i < C; i++){
	timer(pomo_win, W, i, WORK);
	alert();
	timer(pomo_win, R, i, REST);
	alert();
    }

    delwin(pomo_win);
    wrefresh(win);

    recordStats(W * C);
    printStats(win);
}

void printMenu(WINDOW* win){
    int y = hsize + 3,
	x = (COLS - strlen(menu[0])) / 2;

    for (int i = 0; i < msize; i++){
	mvwprintw(win, y, x, "%s", menu[i]);
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
		printStats(win);
		printHeader(win, header, hsize, COLS);
		printMenu(win);
		box(win, 0, 0);
		break;

	    case 'k':
	    case 'K':
		startPomo(win);
		printHeader(win, header, hsize, COLS);
		printMenu(win);
		box(win, 0, 0);
		break;

	    case 'q':
	    case 'Q':
		return;
		break;

	    default:
		continue;
	}
    }
}

void pomoDashboard(){
    // handler for resize and etc.
    WINDOW* dashboard = newwin(LINES, COLS, 0, 0);
    box(dashboard, 0, 0);

    printHeader(dashboard, header, hsize, COLS);
    printMenu(dashboard);
    wrefresh(dashboard);

    grabInput(dashboard);
    // once function above ends, cleanup begins
    delwin(dashboard);
}

void help() {
    printf("usage: yapt [work] [rest] [cycles] \n");
}

int main(int argc, char* argv[]){
    if (argc > 3 && argc < 5) {
	W = atoi(argv[1]) * 60;
	R = atoi(argv[2]) * 60;
	C = atoi(argv[3]);
    } else { 
	help(); 
	return 1;
    }

    setlocale(LC_ALL, "");

    initscr();
    curs_set(0);
    noecho();
    pomoDashboard();

    endwin();
    return 0;
}
