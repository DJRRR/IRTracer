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

bool g_init = false;
gzFile trace_file;

void fin_logger(){
    gzclose(trace_file);
}

void init_logger(){
    trace_file = gzopen("dynamic_trace.gz", "w");  
    if(trace_file == NULL){
        perror("Failed to open logfile \"dynamic_trace\"");
        exit(-1);
    }
    atexit(&fin_logger);
    g_init = true;
}



void logger_line_level(int lineNumber){
    if(!g_init){
        init_logger();
    }
    gzprintf(trace_file, "Line: %d\n", lineNumber);
}

void logger_func_level(char* funcName){
    if(!g_init){
        init_logger();
    }
    gzprintf(trace_file, "Call: %s\tDepth: %d\n", funcName, callDepth);
    escaped = false;
}

void logger_bb_level(char* bbHash){
    if(!g_init){
        init_logger();
    }
    gzprintf(trace_file, "BB: %s\n", bbHash);
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
