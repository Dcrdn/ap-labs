#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>

int main(){
    //setlogmask (LOG_UPTO (LOG_NOTICE));

        va_list arg;
        va_start (arg, 1);
		openlog ("Logger-WARN", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
		vsyslog(LOG_WARNING, "cosa", arg);
		closelog();

        va_end (arg);
}

