#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lib54asm_default_cpu_input.8b.h"

int main(int argc, char** argv){
    char* pfp = NULL;
    unsigned long port = 0;
    size_t ports = 8;
    uint8_t value = 0;

    for (int i = 0; i < argc; i+=2){
        if (strcmp(argv[i], "-if") == 0){
            pfp = argv[i+1];
        } else if (strcmp(argv[i], "-port") == 0){
            port = atol(argv[i+1]);
        } else if (strcmp(argv[i], "-ports") == 0){
            ports = atol(argv[i+1]);
        } else if (strcmp(argv[i], "-value") == 0){
            value = atoi(argv[i+1]);
        } 
    }

    if (pfp == NULL){
        pfp = "cpu.ports";
    }

    int status = lib54asm_default_cpu_port_input_8b(pfp, port, ports, value);

    if (status != 0){
        printf("error: lib54asm_default_cpu_port_input_8b() returned 1\n");
    }

    return status;
}