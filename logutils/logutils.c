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
#include <string.h>

int callDepth = 0;
int escaped = false;

bool g_init = false;
gzFile trace_file;

char currCaller[256];

char currCallerFile[256];

int currCallerLine;

int Logging = 1;

void fin_logger(){
    //gzclose(trace_file);
    gzflush(trace_file, Z_FINISH);
}

void setCaller(char* funcName, char* fileName, int lineNum, int startLogging){
    strcpy(currCaller, funcName);
    strcpy(currCallerFile, fileName);
    currCallerLine = lineNum;
    Logging = startLogging;
}

void startLog(){
    Logging = 1;
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



void logger_line_level(char* funcName, int lineNumber){
    if(!g_init){
        init_logger();
    }
    if(Logging){
        gzprintf(trace_file, "%s:%d\n", funcName, lineNumber);
    }
}

void logger_line_level_func_begin(char* funcName){
    if(!g_init){
        init_logger();
    }
    if(Logging){
        gzprintf(trace_file, "[B]%s\n", funcName);
    }
}

void logger_line_level_func_end(char* funcName){
    if(!g_init){
        init_logger();
    }
    if(Logging){
        gzprintf(trace_file, "[E]%s\n", funcName);
    }
}


void logger_func_level(char* funcName){
    if(!g_init){
        init_logger();
    }
    if(Logging){
        gzprintf(trace_file, "%s:%s:%s:%d\n", funcName, currCaller, currCallerFile, currCallerLine);
    }
    escaped = false;
    memset(currCaller, 0, sizeof(char)*256);
    memset(currCallerFile, 0, sizeof(char)*256);
    currCallerLine=0;

}

void logger_bb_level(char* bbHash){
    if(!g_init){
        init_logger();
    }
    if(Logging){
        gzprintf(trace_file, "%s\n", bbHash);
    }
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
