#ifndef ASM54_TOOLS_8B
#define ASM54_TOOLS_8B
#include <stdio.h>
#include <stdint.h>

int lib54asm_compile_8b_1op(FILE* ifd, FILE* ofd);
int lib54asm_execute_8b_1op(FILE* log, FILE* ifd, int speed);

#endif