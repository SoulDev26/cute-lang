#ifndef CUTE_VM_LOGGING_H
#define CUTE_VM_LOGGING_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define CUTE_TEXT_RESET "\033[0m"

#define CUTE_TEXT_RED "\033[31m"
#define CUTE_TEXT_GREEN "\033[32m"
#define CUTE_TEXT_YELLOW "\033[33m"
#define CUTE_TEXT_BLUE "\033[34m"

#define CUTE_TEXT_BOLD "\033[1m"
#define CUTE_TEXT_UNDERLINED "\033[4m"


static struct 
{
    bool handle;
    bool paniced;
} cute_Logging;

#define CUTEVM_PANIC(...) \
{ \
    cute_Logging.paniced = true; \
    if (!cute_Logging.handle) \
    { \
        fprintf(stderr, "[" CUTE_TEXT_RED "PANIC" CUTE_TEXT_RESET "] %s: ", __FUNCTION__); \
        fprintf(stderr, __VA_ARGS__); \
        fprintf(stderr, "\n"); \
        exit(1); \
    } \
}

#define HANDLE_PANIC(expr) \
{ \
    cute_Logging.paniced = false; \
    cute_Logging.handle = true; \
    expr \
    cute_Logging.handle = false; \
}

#define IS_PANICED (cute_Logging.paniced)


#define CUTEVM_INFO(...) \
{ \
    fprintf(stderr, "[" CUTE_TEXT_GREEN "INFO" CUTE_TEXT_RESET "] %s: ", __FUNCTION__); \
    fprintf(stderr, __VA_ARGS__); \
    fprintf(stderr, "\n"); \
}

#define CUTEVM_WARN(...) \
{ \
    fprintf(stderr, "[" CUTE_TEXT_YELLOW "WARN" CUTE_TEXT_RESET "] %s: ", __FUNCTION__); \
    fprintf(stderr, __VA_ARGS__); \
    fprintf(stderr, "\n"); \
}

#ifdef CUTEVM_DEBUG
    #define CUTEVM_DBG(...) \
    { \
        fprintf(stderr, "[" CUTE_TEXT_BLUE "DEBUG" CUTE_TEXT_RESET "] %s: ", __FUNCTION__); \
        fprintf(stderr, __VA_ARGS__); \
        fprintf(stderr, "\n"); \
    }
#else
    #define CUTEVM_DBG(...)
#endif

#endif // CUTE_VM_LOGGING_H
