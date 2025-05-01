#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <stdint.h>
#include <unistd.h>
#include "lib54asm_default_display_input_8b.h"

int latest_data[5] = {0, 0, 0, 0, 0};

int lib54asm_default_gpu_handle(char* ipath, char* opath, uint8_t dp[5]){
    FILE* pfd = fopen(ipath, "r");
    FILE* ofd = fopen(opath, "w");

    if (!pfd){
        printf("error opening port file\n");
        fclose(pfd);
        fclose(ofd);

        return 1;
    }

    fseek(pfd, (dp[0] * 2) + 1, SEEK_SET);
    uint8_t opcode = fgetc(pfd);

    fseek(pfd, (dp[1] * 2) + 1, SEEK_SET);
    uint8_t op1 = fgetc(pfd);

    fseek(pfd, (dp[2] * 2) + 1, SEEK_SET);
    uint8_t op2 = fgetc(pfd);

    fseek(pfd, (dp[3] * 2) + 1, SEEK_SET);
    uint8_t op3 = fgetc(pfd);

    fseek(pfd, (dp[4] * 2) + 1, SEEK_SET);
    uint8_t op4 = fgetc(pfd);

    printf("opcode: %d, operands: %d, %d, %d, %d\r", opcode, op1, op2, op3, op4);

    if (opcode != latest_data[0] || op1 != latest_data[1] || op2 != latest_data[2] || op3 != latest_data[3] || op4 != latest_data[4]){
        switch (opcode){
            case 0: // NOOP
                printf("noop\n");

                break;
            case 1: // PIXEL
                lib54asm_default_display_input_8b(ofd, op1, op2, op3, 1);
                printf("Adding pixel at %d, %d state %d with operand 1\n", op1, op2, op3);

                break;
            case 2: // CLEAR
                lib54asm_default_display_input_8b(ofd, 0, 0, op1, 2);
                printf("Adding pixel at %d, %d state %d with operand 1\n", op1, op2, op3);

                break;
            default:
                printf("Unknown opcode %d\n", opcode);

                break;
        }

        fflush(ofd);

        latest_data[0] = opcode;
        latest_data[1] = op1;
        latest_data[2] = op2;
        latest_data[3] = op3;
        latest_data[4] = op4;
    }

    fflush(stdout);
    fclose(pfd);
    fclose(ofd);

    return 0;
}