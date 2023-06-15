#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ncurses.h>
#include "rdp.h"

#define ARR_LEN(x) sizeof(x) / sizeof(x[0])

void print_screen();
char* report_choice(int, int);

typedef struct _pair {
    int x, y;
    char picked[2];
} OptionPair;

OptionPair choices[] = {
    {1, 1, "e"},
    {2, 1, "e"},
    {3, 1, "e"},
    {4, 1, "e"},
    {5, 1, "e"},
    {6, 1, "e"},
    {7, 1, "e"},
    {1, 3, "1"},
    {3, 3, "2"},
    {5, 3, "3"},
    {7, 3, "x"},
    {1, 4, "4"},
    {3, 4, "5"},
    {5, 4, "6"},
    {7, 4, "/"},
    {1, 5, "7"},
    {3, 5, "8"},
    {5, 5, "9"},
    {7, 5, "+"},
    {1, 6, "0"},
    {3, 6, "("},
    {5, 6, ")"},
    {7, 6, "-"},
    {1, 8, "c"},
    {2, 8, "c"},
    {3, 8, "c"},
    {4, 8, "c"},
    {5, 8, "c"},
    {6, 8, "c"},
    {7, 8, "c"},
    {1, 10, "p"},
    {2, 10, "p"},
    {3, 10, "p"},
    {4, 10, "p"},
    {5, 10, "p"},
    {6, 10, "p"},
    {7, 10, "p"},
};

char* lines[] = {
    "",
    "+-------+",
    "|Equals |",
    "+-------+",
    "|1|2|3|x|",
    "|4|5|6|/|",
    "|7|8|9|+|",
    "|0|(|)|-|",
    "+-------+",
    "| Clear |",
    "+-------+",
    "|Partial|",
    "+-------+",
    "|Output |",
    "+-------+",
    "",
};

int main(int argc, char* argv[]) {
    MEVENT event;
    int ch, total, running = 1;
    char* choice;
    char working[100];

    initscr();
    clear();
    noecho();
    cbreak();

    keypad(stdscr, TRUE);

    print_screen();
    mousemask(ALL_MOUSE_EVENTS, NULL);

    strcpy(working, "");

    while(running) {
        ch = getch();

        switch(ch) {
            case KEY_MOUSE:
                if(getmouse(&event) == OK) {
                    // Left click
                    if(event.bstate & BUTTON1_CLICKED) {
                        choice = report_choice(event.x, event.y);

                        if(choice != "n") {
                            switch(choice[0]) {
                                case 'e':
                                    mvprintw(17, 1, "Solving...");
                                    expressionToParse = (char*)malloc(sizeof(char) * 100);
                                    strcpy(expressionToParse, working);
                                    total = expression();
                                    mvprintw(18, 1, "Solved: %d", total);
                                    break;
                                case 'c':
                                    strcpy(working, "");
                                    total = 0;
                                    print_screen();
                                    break;
                                case 'p':
                                    sprintf(working, "%d", total);
                                    print_screen();
                                    mvprintw(16, 1, "Choice made is: %s", working);
                                    break;
                                default:
                                    strcat(working, choice);
                                    mvprintw(16, 1, "Choice made is: %s", working);
                                    break;
                            }
                        }

                        refresh();
                    }
                }
                break;
            case 27:
                running = 0;
                break;
        }   
    }

    nodelay(stdscr, FALSE);

    getch();

    endwin();
    return 0;
}

void print_screen() {
    int x, i;

    clear();

    for(i = 1; i < ARR_LEN(lines); i++) {
        mvprintw(i, 1, lines[i]);
    }

    refresh();
}

char* report_choice(int mouse_x, int mouse_y) {
    int choice, i;

    for(i = 0; i < ARR_LEN(choices); i++) {
        if(choices[i].x + 1 == mouse_x && choices[i].y + 1 == mouse_y) {
            return choices[i].picked;
        }
    }

    return "n";
}