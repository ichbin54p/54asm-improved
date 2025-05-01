#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "isa.h"

int lib54asm_compile_8b_1op(FILE* ifd, FILE* ofd){
    char* line = malloc(64);
    char* cmdbuf = malloc(64);

    uint8_t f = 0;
    uint8_t oi = 0;
    int opc = 0;
    int opoffs = 0;
    int opiidx = 0;
    size_t ll = 0;

    while (fgets(line, 64, ifd)){
        if (line[0] != '#'){
            memset(cmdbuf, 0, 64);
            ll = strlen(line);

            for (unsigned long i = 0; i < ll; i++){
                if (line[i] == ' '){
                    opoffs = i;
                    cmdbuf[i] = '\0';

                    break;
                } else if (line[i] == '\n'){
                    opoffs = -1;

                    break;
                }

                cmdbuf[i] = line[i];
            }

            f = 0;

            for (int i = 0; i < ISA_SIZE; i++){
                if (strlen(cmdbuf) < 1){
                    f = 1;

                    break;
                }

                if (strcmp(tokens[i].name, cmdbuf) == 0){
                    f = 1;
                    oi = tokens[i].id + ISA_OFFS;

                    fwrite(&oi, 1, 1, ofd);

                    break;
                }
            }

            opc = 0;

            if (!f){
                printf("warning: instruction not found \"%s\"\n", cmdbuf);

                break;
            }

            if (opoffs > 0){
                opiidx = 0;

                memset(cmdbuf, 0, 64);

                for (unsigned long i = opoffs + 1; i < ll; i++){
                    if (line[i] == ' '){
                        if (strlen(cmdbuf) > 0){
                            oi = atoi(cmdbuf);
                            
                            if (opc < 1){
                                if (oi > 15){
                                    printf("warning: operand value %d is too large for operand index %d\n", oi, opc + 1);
        
                                    oi = 0;
                                }
                            }

                            opiidx = 0;
                            
                            fwrite(&oi, 1, 1, ofd);
                            memset(cmdbuf, 0, 64);
                        }
                    } else if (line[i] == '\n'){
                        break;
                    }

                    cmdbuf[opiidx] = line[i];
                    opiidx += 1;
                }

                opc += 1;
            }

            if (strlen(cmdbuf) > 0){
                oi = atoi(cmdbuf);

                fwrite(&oi, 1, 1, ofd);
                memset(cmdbuf, 0, 64);
            }
        }
    }

    free(line);
    free(cmdbuf);

    return 0;
}