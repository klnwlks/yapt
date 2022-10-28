#ifndef CONFIG_H
#define CONFIG_H

// strlen has trouble reading braille characters, so have an indice with all spaces 
// matching the length of your header
const char* header[] = { 
"                              ",
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⣰⣷⣦⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ",
"⠀⠀⠀⠀⠀⠀⠀⣀⣶⣿⣿⣿⣿⣿⣿⣷⣶⣶⣶⣦⣀⡀⠀⢀⣴⣇⠀⠀⠀ ",
"⠀⠀⠀⠀⠀⢠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀ ",
"⠀⠀⠀⠀⣰⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀ ",
"⠀⠀⠀⣴⣿⣿⣿⣿⠛⣿⣿⣿⣿⣿⣿⣿⣿⡿⣿⣿⣿⣿⣿⣿⣿⣿⣄⠀⠀ ",
"⠀⠀⣾⣿⣿⣿⣿⣿⣶⣿⣯⣭⣬⣉⣽⣿⣿⣄⣼⣿⣿⣿⣿⣿⣿⣿⣿⣷⡀ ",
"⠀⣸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡄",
"⢸⣿⣿⣿⣿⠟⠋⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠉⠁⣿⣿⣿⣿⡿⠛⠉⠉⠉⠉⠁",
"⠘⠛⠛⠛⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠛⠛⠛⠃⠀⠀⠀⠀  ⠀",
"         pomodoro.            ",
};
const int hsize = 11;

const char* menu[] = {
" View Stats                j J",
" Start Timer               k K",
" Exit                      q Q"
}; 
const int msize = 3;

const char* work[] = {
"      )  (      ",
"     (   ) )    ",
"      ) ( (     ",
"    _______)_   ",
" .-'---------|  ",
"( C|/\\/\\/\\/\\/|  ",
" '-./\\/\\/\\/\\/|  ",
"   '_________'  ",
"    '-------'   "
}; 
const int wsize = 8;

const char* rest[] = {
"      )  (      ",
"     (   ) )    ",
"      ) ( (     ",
"    _______)_   ",
" .-'---------|  ",
"( C|/\\/\\/\\/\\/|  ",
" '-./\\/\\/\\/\\/|  ",
"   '_________'  ",
"    '-------'   "
};
const int rsize = 8;


#endif
