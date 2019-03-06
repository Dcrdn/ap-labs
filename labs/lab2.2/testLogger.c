#include <logger.h>

int main(){	
    infof("info: %s type %s \n", "Hi, this is ", "info");
    warnf("warning: %s type %s \n", "Hi, this is ", "warning");
    errorf("error: %s type %s \n", "Hi, this is ", "error");
    panicf("panic: %s type %s \n", "Hi, this is ", "panic");
    return 0;
}