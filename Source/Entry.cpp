#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>

// Clear screen
#define CSCR "\e[2J"

// Move cursor
#define MCUR(loc) fputs("\e[" loc "H", stdout)

// Move cursor towards a direction
#define DCUR(stp, dir) printf("\e[%s%c", stp, dir)
#define MCRU DCUR("", 'A')
#define MCRD DCUR("", 'B')
#define MCRR DCUR("", 'C')
#define MCRL DCUR("", 'D')

// #define INS if (insert) break
// #define NINS if (!insert) break

// Change cursor style
#define CURS(sty) fputs("\e[" sty " q", stdout)
#define DCURS "0" // Default
#define SBCUR "1" // Solid block
#define BBCUR "2" // Blinking block
#define BUCUR "3" // Blinking underscode
#define BLCUR "4" // Blinking line

#define CLR(c) fputs("\e[" c "m", stdout);
#define DEFC "0"
#define FBLACK "30"
#define FRED "31"
#define FGREEN "32"
#define FYELLOW "33"
#define FBLUE "34"
#define FMAGENTA "35"
#define FCYAN "36"
#define FWHITE "37"
#define BBLACK "40"
#define BRED "41"
#define BGREEN "42"
#define BYELLOW "43"
#define BBLUE "44"
#define BMAGENTA "45"
#define BCYAN "46"
#define BWHITE "47"

enum {
    UP = 'A',
    DOWN,
    RIGHT,
    LEFT,
};

#define CMD(c)                                                                 \
    c;                                                                         \
    break

struct termios og_termios;
struct termios termios;

int chr = '#';
int input;
bool run = true;

// bool insert = false;

void enter_raw_mode() {
    termios.c_iflag &=
        ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    termios.c_oflag &= ~OPOST;
    termios.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    termios.c_cflag &= ~(CSIZE | PARENB);
    termios.c_cflag |= CS8;
    tcsetattr(STDOUT_FILENO, TCSAFLUSH, &termios);
}

void initiate() {
    tcgetattr(STDOUT_FILENO, &og_termios);
    termios = og_termios;
    enter_raw_mode();
    fputs(CSCR, stdout);
    MCUR("0;0");
}

void terminate() {
    // Enter cooked mode
    tcsetattr(STDOUT_FILENO, TCSAFLUSH, &og_termios);
}

void switch_char() {
    if (isspace(input = getchar()) || input == '\e')
        input = '#';
}

void switch_fg() {
    switch (getchar()) {
    case '0':
        CLR(FBLACK);
        break;
    case '1':
        CLR(FRED);
        break;
    case '2':
        CLR(FGREEN);
        break;
    case '3':
        CLR(FYELLOW);
        break;
    case '4':
        CLR(FBLUE);
        break;
    case '5':
        CLR(FMAGENTA);
        break;
    case '6':
        CLR(FCYAN);
        break;
    case '7':
        CLR(FWHITE);
        break;
    default:
        CLR(DEFC);
    }
}

void switch_bg() {
    switch (getchar()) {
    case '0':
        CLR(BBLACK);
        break;
    case '1':
        CLR(BRED);
        break;
    case '2':
        CLR(BGREEN);
        break;
    case '3':
        CLR(BYELLOW);
        break;
    case '4':
        CLR(BBLUE);
        break;
    case '5':
        CLR(BMAGENTA);
        break;
    case '6':
        CLR(BCYAN);
        break;
    case '7':
        CLR(BWHITE);
        break;
    default:
        CLR(DEFC);
    }
}

void loop() {
    while (run) {
        switch ((input = getchar())) {
        case CTRL('q'):
            run = false;
            break;
        case 'j':
            CMD(MCRD);
        case 'k':
            CMD(MCRU);
        case 'l':
            CMD(MCRR);
        case 'h':
            CMD(MCRL);
        case 'c':
            switch_char();
            break;
        case 'f':
            switch_fg();
            break;
        case 'b':
            switch_bg();
            break;
        case ' ':
            putchar(chr);
            MCRL;
            break;
        case 'd':
            putchar(' ');
            MCRL;
            break;
        default:
            break;
        }
    }
    terminate();
}

int main() {
    initiate();
    enter_raw_mode();
    loop();
    return 0;
}
