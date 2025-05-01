#include <stdio.h>
#include <stdlib.h>
#include "lib54asm_default_display_input_8b.h"

int main(int argc, char** argv){
    if (argc < 5){
        printf("error: too few arguements (%d/4)\n", argc - 1);

        return 1;
    }

    FILE* f = fopen("display.in", "w");
    
    lib54asm_default_display_input_8b(f, atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));

    return 0;
}