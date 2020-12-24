#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <zlib.h>
//#include <set>
//#include <csignal>
//#include <iostream>
#include<signal.h>
#include <string.h>
#include <unistd.h>

//using namespace std;
int callDepth = 0;
int escaped = false;

bool g_init = false;
gzFile trace_file;

char currCaller[256];

char currCallerFile[256];

//int currCallerLine;
char currCallerBB[256];

int Logging = 1;
//set<string> checkSet;
char* checkPos;


void fin_logger(){
    //gzclose(trace_file);
    gzflush(trace_file, Z_FINISH);
}

void setCaller(char* funcName, char* fileName, char* callerBB, int startLogging){
    strcpy(currCaller, funcName);
    strcpy(currCallerFile, fileName);
    strcpy(currCallerBB, callerBB);
    //if(startLogging == 0){
    //    string tmp(funcName, funcName+strlen(funcName)); 
    //    checkSet.insert(tmp);
    //}
    if(startLogging == 0 && checkPos == 0){
        char* tmp = malloc(strlen(funcName));
        strcpy(tmp, funcName);
        checkPos = tmp;
    }
    //Logging = startLogging;
}

void startLog(char* funcName){
    //Logging = 1;
    //string tmp(funcName, funcName+strlen(funcName));
    //checkSet.erase(tmp);
    if(strcmp(funcName, checkPos) == 0){
        checkPos = 0;
    }
}

void signal_handler(int code){
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
    signal(SIGFPE, signal_handler);
    g_init = true;
}



void logger_line_level(char* funcName, int lineNumber){
    if(!g_init){
        init_logger();
    }
    if(checkPos == 0){
    //if(Logging){
    //if(!checkSet.size()){
        gzprintf(trace_file, "%s:%d\n", funcName, lineNumber);
        fin_logger();
    }
}

void logger_line_level_func_begin(char* funcName){
    if(!g_init){
        init_logger();
    }
    if(checkPos == 0){
    //if(Logging){
    //if(!checkSet.size()){
        gzprintf(trace_file, "[B]%s\n", funcName);
        fin_logger();
    }
}

void logger_line_level_func_end(char* funcName){
    if(!g_init){
        init_logger();
    }
    if(checkPos == 0){
    //if(Logging){
    //if(!checkSet.size()){
        gzprintf(trace_file, "[E]%s\n", funcName);
        fin_logger();
    }
}


void logger_func_level(char* funcName){
    if(!g_init){
        init_logger();
    }
    if(checkPos == 0){
    //if(Logging){
    //if(!checkSet.size()){
        gzprintf(trace_file, "%s:%s:%s:%s\n", funcName, currCaller, currCallerFile, currCallerBB);
        fin_logger();
    }
    escaped = false;
    memset(currCaller, 0, sizeof(char)*256);
    memset(currCallerFile, 0, sizeof(char)*256);
    memset(currCallerBB, 0, sizeof(char)*256);

}

void logger_bb_level(char* funcName, char* bbID){
    if(!g_init){
        init_logger();
    }
    if(checkPos == 0){
    //if(Logging){
    //if(!checkSet.size()){
        gzprintf(trace_file, "%s:%s\n", funcName, bbID);
        fin_logger();
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
