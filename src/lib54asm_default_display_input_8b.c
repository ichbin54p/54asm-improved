#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

void lib54asm_default_display_input_8b(FILE* ofd, int n1, int n2, int n3, int n4){
    fprintf(ofd, "%c%c%c%c", n1, n2, n3, n4);
    usleep(5000);
}