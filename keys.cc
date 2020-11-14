// Example for inputting a single keystroke in C++ on Linux
// by Adam Pierce <adam@doctort.org> on http://www.doctort.org/adam/nerd-notes/reading-single-keystroke-on-linux.html
// This code is freeware. You are free to copy and modify it any way you like.
// Modify by me Putra Kusaeri


#include <iostream>
#include <termios.h>
#define STDIN_FILENO 0
using namespace std;
int main()
{
// Black magic to prevent Linux from buffering keystrokes.
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~ICANON;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);

// Once the buffering is turned off, the rest is simple.
    char c;
    while (1) {
      cout << "Enter a character: " << endl;
      cin >> c;
      cout << endl << "Your input: '" << c << "'" << endl;
    }
// Using 3 char type, Cause up down right left consist with 3 character
    /*
    if ((c==27)&&(d=91)) {
        if (e==65) { cout << "UP";}
        if (e==66) { cout << "DOWN";}
        if (e==67) { cout << "RIGHT";}
        if (e==68) { cout << "LEFT";}
    }
    */
    return 0;
}

