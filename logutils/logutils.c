#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <zlib.h>
#include <signal.h>
#include <unistd.h>

int callDepth = 0;
int escaped = false;

bool g_init = false;
gzFile trace_file;

void fin_logger(){
    //gzclose(trace_file);
    gzflush(trace_file, Z_FINISH);
}

void signal_handler(){
    fin_logger();
    exit(255);
}

void init_logger(){
    trace_file = gzopen("dynamic_trace.gz", "w");  
    if(trace_file == NULL){
        perror("Failed to open logfile \"dynamic_trace\"");
        exit(-1);
    }
    atexit(&fin_logger);
    signal(SIGABRT, signal_handler);
    signal(SIGSEGV, signal_handler);
    signal(SIGINT, signal_handler);
    g_init = true;
}



void logger_line_level(char* fileName, int lineNumber){
    if(!g_init){
        init_logger();
    }
    gzprintf(trace_file, "Line: %s:%d\n", fileName, lineNumber);
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
