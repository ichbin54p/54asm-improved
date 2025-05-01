#include <stdio.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "lib54asm_default_gpu_8b.h"

void help(char* a){
    printf("Usage: %s [ARGUEMENTS]\n\n--display-draw-speed [INT]\n--port-path [STR | PATH]\n--display-port(0-2) [INT | PORT]\n-o [STR | PATH]\n", a);
}

volatile int run = 1;

void exitHandle(int signal){
    run = 0;
}

int main(int argc, char** argv){
    signal(SIGINT, exitHandle);

    char* opath = NULL;
    char* ipath = NULL;

    uint32_t dds = 100000;
    uint8_t dp[5] = {
        0,
        1,
        2,
        3,
        4
    };

    for (int i = 0; i < argc; i++){
        if (strcmp(argv[i], "--display-draw-speed") == 0){
            dds = atoi(argv[i+1]);
        } else if (strcmp(argv[i], "--port-path") == 0){
            ipath = argv[i+1];
        } else if (strcmp(argv[i], "--gpu-port-0") == 0){
            dp[0] = atoi(argv[i+1]);
        } else if (strcmp(argv[i], "--gpu-port-1") == 0){
            dp[1] = atoi(argv[i+1]);
        } else if (strcmp(argv[i], "--gpu-port-2") == 0){
            dp[2] = atoi(argv[i+1]);
        } else if (strcmp(argv[i], "--gpu-port-3") == 0){
            dp[3] = atoi(argv[i+1]);
        } else if (strcmp(argv[i], "--gpu-port-4") == 0){
            dp[4] = atoi(argv[i+1]);
        } else if (strcmp(argv[i], "-o") == 0){
            opath = argv[i+1];
        }
    }

    if (ipath == NULL){
        ipath = "cpu.ports";
    }

    if (opath == NULL){
        opath = "display.in";
    }

    if (dp[0] == dp[1] || dp[0] == dp[2] || dp[1] == dp[0] || dp[1] == dp[2] || dp[2] == dp[0] || dp[2] == dp[1] || dp[0] > 7 || dp[1] > 7 || dp[2] > 7){
        printf("invalid port setup\n");

        return 1;
    }

    while (run){
        if (lib54asm_default_gpu_handle(ipath, opath, dp) != 0){
            break;
        }

        usleep(dds);
    }

    printf("exit\n");

    return 0;
}