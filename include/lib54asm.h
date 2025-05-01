#ifndef LIB54ASM_H
#define LIB54ASM_H
#include <stdio.h>
#include <stdint.h>
#include <signal.h>

typedef enum {
    LIB54ASM_FLAG_NULL = 0,
    LIB54ASM_FLAG_ZERO = 1,
    LIB54ASM_FLAG_COUT = 2
} lib54asm_flags_t;

typedef enum {
    LIB54ASM_SUCCESS = 0,
    LIB54ASM_ERROR_INIT_MEM_NULL = -1,
    LIB54ASM_ERROR_INIT_MEM_SIZE = -2,
    LIB54ASM_ERROR_INIT_FILE_NULL = -3,
    LIB54ASM_ERROR_ADD_OUT_OF_BOUNDS = -4,
} lib54asm_errors_t;

#endif
#ifndef LIB54ASM_8_H
#define LIB54ASM_8_H

typedef struct lib54asm_cpu8_1op lib54asm_cpu8_1op_t;

typedef struct lib54asm_mem8 {
    FILE* port;
    uint8_t* ram;
    uint8_t port_count;
} lib54asm_mem8_t;

typedef struct lib54asm_instruction8_arg {
    lib54asm_cpu8_1op_t* cpu;
    uint8_t op1;
    uint8_t op2;
    unsigned long* i;
} lib54asm_instruction8_arg_t;

typedef void (*lib54asm_instruction_ptr_t)(lib54asm_instruction8_arg_t*);

typedef struct lib54asm_isa {
    uint8_t size;
    uint8_t iidx;
    lib54asm_instruction_ptr_t* instructions;
} lib54asm_isa_t;

typedef struct lib54asm_program8 {
    lib54asm_instruction_ptr_t* program;
    size_t size;
    unsigned long iidx;
} lib54asm_program8_t;

typedef struct lib54asm_cpu8_1op {
    FILE* ofd;
    uint8_t* reg;
    uint16_t* operands;
    int speed;
    uint8_t flags[3];
    uint64_t pc;
    lib54asm_isa_t isa;
    lib54asm_mem8_t mem;
    lib54asm_program8_t program;
} lib54asm_cpu8_1op_t;

int lib54asm_cpu8_1op_add_instruction(lib54asm_cpu8_1op_t* cpu, uint8_t isa_idx, uint8_t op1, uint8_t op2);
int lib54asm_init_cpu8_1op(lib54asm_cpu8_1op_t* cpu, size_t regs, size_t program, int speed, FILE* logfd);
int lib54asm_isa_add_opcode(lib54asm_isa_t* isa, lib54asm_instruction_ptr_t f);
int lib54asm_init_mem8(lib54asm_mem8_t* memory, size_t ram, size_t ports);
int lib54asm_cpu8_1op_exec(lib54asm_cpu8_1op_t* cpu);
int lib54asm_init_isa(lib54asm_isa_t* isa, uint8_t size);

void lib54asm_error_print(lib54asm_errors_t e, FILE* ofd);
void lib54asm_cleanup_cpu8_1op(lib54asm_cpu8_1op_t* cpu);
void lib54asm_handle_cpu_execute_signal(int signal);
void lib54asm_cleanup_mem8(lib54asm_mem8_t* memory);
void lib54asm_cleanup_isa(lib54asm_isa_t* isa);

#endif
