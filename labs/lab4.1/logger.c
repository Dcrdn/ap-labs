  
#include <stdio.h> 
#include <stdarg.h>

#define RESET		0
#define BRIGHT 		1
#define DIM		    2
#define UNDERLINE 	3
#define BLINK		4
#define REVERSE		7
#define HIDDEN		8
/*-----------------------------------
RED for panic
BLUE for Info
YELLOW for warning
GREEN for error
-------------------------------------*/
#define BLACK 		0
#define RED		    1 
#define GREEN		2 
#define YELLOW		3  
#define BLUE		4
#define MAGENTA		5
#define CYAN		6 
#define	WHITE		7  

void textcolor(int attr, int fg, int bg){
	char command[13];
	sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
	printf("%s", command);
}

int infof(const char *format, ...){
    int counter;
    textcolor(BRIGHT, BLUE, BLACK);
    va_list arg;
    va_start(arg, format);
    counter = vprintf(format, arg);
    va_end(arg);
    textcolor(RESET, WHITE, BLACK);
    return counter;
}
int warnf(const char *format, ...){
    int counter;
    textcolor(BRIGHT, YELLOW, BLACK);
    va_list arg;
    va_start(arg, format);
    counter = vprintf(format, arg);
    va_end(arg);
    textcolor(RESET, WHITE, BLACK);
    return counter;
}
int errorf(const char *format, ...){
    int counter;
    textcolor(BRIGHT, GREEN, BLACK);
    va_list arg;
    va_start(arg, format);
    counter = vprintf(format, arg);
    va_end(arg);
    textcolor(RESET, WHITE, BLACK);
    return counter;
}
int panicf(const char *format, ...){
    int counter;
    textcolor(BRIGHT, RED, BLACK);
    va_list arg;
    va_start(arg, format);
    counter = vprintf(format, arg);
    va_end(arg);
    textcolor(RESET, WHITE, BLACK);
    return counter;
}