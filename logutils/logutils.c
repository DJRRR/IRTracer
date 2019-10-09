#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <zlib.h>

int callDepth = 0;
int escaped = false;

void log_line_level(int lineNumber){
    printf("Line: %d\n", lineNumber);
}

void log_func_level(char* funcName){
    printf("Call: %s\tDepth:%d\n", funcName, callDepth);
    escaped = false;
}


void call_depth_inc() {
    if(!escaped){
        callDepth++;
        /* TODO: carefully re-design escape mechanism */
        escaped = true;
    }
}

void call_depth_dec(){
    callDepth--;
}
