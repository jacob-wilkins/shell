#include "utils.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>

#define BUFFER_SIZE 1024

void print(const char* format, ...) {
    char buffer[BUFFER_SIZE];   // Buffer to store formatted output
    va_list args;   // Holds all the arguments after 'format'
    
    va_start(args, format);     // Initialize the va_list to start processing the arguments after 'format'

    /* How to use vsnprintf(s, n, format, arg):
        - s: buffer to store formatted string
        - n: maximum number of bytes in the buffer
        - format: String that contains a format string
        - args: va_list that contains all the arguments after the formatted string
          can only be used after va_start is called, and until va_end is called
    */
    vsnprintf(buffer, BUFFER_SIZE, format, args);

    va_end(args);   // Clean up the va_list when done processing the arguments

    write(STDOUT_FILENO, buffer, strlen(buffer));
    fflush(stdout);  // Ensure output is written immediately
}