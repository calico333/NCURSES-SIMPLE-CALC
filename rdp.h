#include <ncurses.h>
#include <math.h>
#include <string.h>

int expression();
int term();
int factor();
int number();
char peek();
char get();

char* expressionToParse;

int expression() {
    int result = term();

    while(peek() == '+' || peek() == '-') {
        if(get() == '+') {
            result += term();
        } else {
            result -= term();
        }
    }

    return result;
}

int term() {
    int result = factor();

    while(peek() == 'x' || peek() == '/') {
        if (get() == 'x') {
            result *= factor();
        } else {
            result /= factor();
        }
    }

    return result;
}

int factor() {
    if(peek() >= '0' && peek() <= '9') {
        return number();
    } else if (peek() == '(') {
        get();
        int result = expression();
        get();
        return result;
    } else if(peek() == '-') {
        get();
        return -factor();
    }

    return 0;
}

int number() {
    int result = get() - '0';

    while(peek() >= '0' && peek() <= '9') {
        result = 10 * result + get() - '0';
    }

    return result;
}

char peek() {
    return *expressionToParse;
}

char get() {
    return *expressionToParse++;
}