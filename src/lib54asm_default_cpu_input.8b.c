#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int lib54asm_default_cpu_port_input_8b(char* pfp, unsigned long port, size_t ports, uint8_t n){
    ports *= 2;
    port *= 2;

    if (port >= ports){
        return 1;
    }
    
    FILE* ifd = fopen(pfp, "rb");

    if (ifd == NULL){
        return 1;
    }

    uint8_t* old_ports = malloc(ports);

    if (old_ports == NULL){
        fclose(ifd);

        return 1;
    }

    fread(old_ports, sizeof(uint8_t), ports, ifd);

    old_ports[port] = n;

    fclose(ifd);

    FILE* ofd = fopen(pfp, "wb");

    fwrite(old_ports, sizeof(uint8_t), ports, ofd);
    fclose(ofd);
    free(old_ports);

    return 0;
};