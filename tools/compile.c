#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib54asm_compile_8b.h"

void help(char* a){
    printf("Usage: %s [input | path] [arguements]\n\n-o, --output [STR | PATH]\n", a);
}

int main(int argc, char** argv){
    if (argc < 2){
        help(argv[0]);

        return 1;
    }
    
    FILE* ofd = NULL;
    FILE* ifd = fopen(argv[1], "r");

    for (int i = 2; i < argc; i+=2){
        if (strcmp("-o", argv[i]) == 0 || strcmp("--output", argv[i]) == 0){
            ofd = fopen(argv[i+1], "w");
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

    if (lib54asm_compile_8b_1op(ifd, ofd) != 0){
        printf("error compiling\n");

        return 1;
    }

    printf("successfully finished compiling\n");

    return 0;
}