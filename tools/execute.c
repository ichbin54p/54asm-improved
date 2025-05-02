#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lib54asm_execute_8b.h"

void help(char* a){
    printf("Usage: %s [STR | PATH] [arguements]\n\n-log [STR | PATH]\n-speed [INT]\n-size [INT]\n-debug [1 | 0]\n-debug-file [STR | PATH]\n", a);
}

int main(int argc, char** argv){
    if (argc < 2){
        help(argv[0]);

        return 1;
    }

    int debug = 0;
    int speed = 1000;
    size_t program = 64;
    
    FILE* debug_fd = stdout;
    FILE* ofd = stdout;
    FILE* ifd = fopen(argv[1], "r");

    for (int i = 2; i < argc; i+=2){
        if (strcmp("-log", argv[i]) == 0){
            ofd = fopen(argv[i+1], "w");
        } else if (strcmp("-debug-file", argv[i]) == 0){
            debug_fd = fopen(argv[i+1], "w");
        } else if (strcmp("-debug", argv[i]) == 0){
            debug = atoi(argv[i+1]);
        } else if (strcmp("-speed", argv[i]) == 0){
            speed = atoi(argv[i+1]);
        } else if (strcmp("-size", argv[i]) == 0){
            program = (unsigned long) atol(argv[i+1]);
        } else {
            printf("error: unkown arguement %s\n", argv[i]);

            return 1;
        }
    }

    if (ifd == NULL){
        printf("error: cannot open input file\n");

        return -1;
    } if (ofd == NULL){
        printf("error: cannot open output file\n");

        return -1;
    }

    if (lib54asm_execute_8b_1op(ofd, ifd, speed, program, debug, debug_fd) != 0){
        printf("error executing\n");

        return 1;
    }

    printf("successfully finished executing\n");

    return 0;
}