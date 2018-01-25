#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>

//log
#define DEBUG(...) printf(__VA_ARGS__);printf("\n");
#define INFO(...) printf(__VA_ARGS__);printf("\n");
#define WARNING(...) printf(__VA_ARGS__);printf("\n");
#define ERROR(...) printf(__VA_ARGS__);printf("\n");
#define FATAL(...) printf(__VA_ARGS__);printf("\n");

//show
#define SHOW_WHITE(...)
#define SHOW_GREEN(...)
#define SHOW_YELLOW(...)
#define SHOW_RED(...)

//set log level
#define SET_LOG_LEVEL_DEBUG()
#define SET_LOG_LEVEL_INFO()
#define SET_LOG_LEVEL_WARNING()
#define SET_LOG_LEVEL_ERROR()
#define SET_LOG_LEVEL_FATAL()

#endif