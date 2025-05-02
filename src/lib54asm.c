#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "lib54asm.h"

volatile int lib54asm_run_cpu = 1;

void lib54asm_handle_cpu_execute_signal(int signal){
    lib54asm_run_cpu = 0;
}

void lib54asm_error_print(lib54asm_errors_t e, FILE* ofd){
    switch (e){
        case LIB54ASM_SUCCESS:
            fprintf(ofd, "LIB54ASM_ERROR_HANDLE: success (0)\n");

            break;
        case LIB54ASM_ERROR_INIT_MEM_NULL:
            fprintf(ofd, "LIB54ASM_ERROR_INIT_MEM_NULL: memory initialization failed, pointers are null\n");
            
            break;
        case LIB54ASM_ERROR_INIT_MEM_SIZE:
            fprintf(ofd, "LIB54ASM_ERROR_INIT_MEM_SIZE: memory initialization failed, memory size too small\n");

            break;
        case LIB54ASM_ERROR_INIT_FILE_NULL:
            fprintf(ofd, "LIB54ASM_ERROR_INIT_FILE_NULL: creating file failed, files are null\n");

            break;
        case LIB54ASM_ERROR_ADD_OUT_OF_BOUNDS:
            fprintf(ofd, "LIB54ASM_ERROR_ADD_OUT_OF_BOUNDS: adding item failed, index out of bounds\n");

            break;
        default:
            fprintf(ofd, "LIB54ASM_ERROR_HANDLE: unknown error (%d)\n", e);

            break;
    }
}

int lib54asm_init_isa(lib54asm_isa_t* isa, uint8_t size){
    isa->size = size;

    if (isa->size < 1){
        return LIB54ASM_ERROR_INIT_MEM_SIZE;
    }

    isa->iidx = 0;
    isa->instructions = malloc(isa->size * sizeof(lib54asm_instruction_ptr_t));

    if (isa->instructions == NULL){
        return LIB54ASM_ERROR_INIT_MEM_NULL;
    }

    return LIB54ASM_SUCCESS;
}

int lib54asm_init_mem8(lib54asm_mem8_t* memory, size_t ram, size_t ports){
    memory->ram = NULL;
    memory->port = NULL;

    if (ram < 1 || ports < 1){
        return LIB54ASM_ERROR_INIT_MEM_SIZE;
    }

    memory->ram = malloc(ram);
    memory->port = fopen("cpu.ports", "w+");
    memory->port_count = ports;

    memset(memory->ram, 0, ram);

    if (memory->ram == NULL){
        return LIB54ASM_ERROR_INIT_MEM_NULL;
    }

    if (memory->port == NULL){
        return LIB54ASM_ERROR_INIT_FILE_NULL;
    }

    for (int i = 0; i < memory->port_count; i++){
        fprintf(memory->port, "%c%c", 0, 0);
    }

    return LIB54ASM_SUCCESS;
}

int lib54asm_init_cpu8_1op(lib54asm_cpu8_1op_t* cpu, size_t regs, size_t program, int speed, FILE* logfd){
    cpu->program.program = NULL;
    cpu->program.size = program;
    cpu->operands = NULL;
    cpu->reg = NULL;

    if (regs < 1 || program < 1){
        return LIB54ASM_ERROR_INIT_MEM_SIZE;
    }

    cpu->program.program = malloc(cpu->program.size * sizeof(lib54asm_instruction_ptr_t));
    cpu->program.iidx = 0;
    cpu->operands = malloc(program * sizeof(uint16_t));
    cpu->flags[LIB54ASM_FLAG_NULL] = 1;
    cpu->flags[LIB54ASM_FLAG_ZERO] = 0;
    cpu->flags[LIB54ASM_FLAG_COUT] = 0;
    cpu->speed = speed;
    cpu->reg = malloc(regs * sizeof(uint8_t));
    cpu->ofd = logfd;
    cpu->pc = 0;

    if (cpu->ofd == NULL || cpu->program.program == NULL || cpu->reg == NULL || cpu->operands == NULL){
        return LIB54ASM_ERROR_INIT_MEM_NULL;
    }

    memset(cpu->reg, 0, regs);

    fprintf(cpu->ofd, "Successfully initialized 8 bit CPU:\n\n- ISA size: %d(%ldB)\n- program size: %ld\n- program size in bytes: %ldB(%ldB single)\n\n", cpu->isa.size, cpu->isa.size * sizeof(lib54asm_instruction_ptr_t), cpu->program.size, cpu->program.size * sizeof(lib54asm_instruction_ptr_t), sizeof(lib54asm_instruction_ptr_t));
    fflush(cpu->ofd);

    return LIB54ASM_SUCCESS;
}

void lib54asm_cleanup_mem8(lib54asm_mem8_t* memory){
    if (memory != NULL){
        if (memory->port != NULL){
            fclose(memory->port);
        } if (memory->ram != NULL){
            free(memory->ram);
        }
    }
}

void lib54asm_cleanup_isa(lib54asm_isa_t* isa){
    if (isa != NULL){
        if (isa->instructions != NULL){
            free(isa->instructions);
        }

        isa->iidx = 0;
        isa->size = 0;
    }
}

void lib54asm_cleanup_cpu8_1op(lib54asm_cpu8_1op_t* cpu){
    fprintf(cpu->ofd, "Cleaning up memory:\n");

    if (cpu != NULL){
        if (cpu->reg != NULL){
            fprintf(cpu->ofd, "- freeing cpu.reg\n");
            free(cpu->reg);

            cpu->reg = NULL;
        } if (cpu->program.program){
            fprintf(cpu->ofd, "- freeing cpu.program.program\n");
            free(cpu->program.program);

            cpu->program.program = NULL;
        } if (cpu->operands){
            fprintf(cpu->ofd, "- freeing cpu.operands\n");
            free(cpu->operands);

            cpu->operands = NULL;
        }
    }

    lib54asm_cleanup_isa(&cpu->isa);
    fprintf(cpu->ofd, "- freed cpu.isa\n");
    lib54asm_cleanup_mem8(&cpu->mem);
    fprintf(cpu->ofd, "- freed cpu.mem\n\n");
}

int lib54asm_isa_add_opcode(lib54asm_isa_t* isa, lib54asm_instruction_ptr_t f){
    if (isa->iidx >= isa->size){
        return LIB54ASM_ERROR_ADD_OUT_OF_BOUNDS;
    }

    if (f != NULL){
        isa->instructions[isa->iidx] = f;
    } else {
        return LIB54ASM_ERROR_INIT_MEM_NULL;
    }

    isa->iidx += 1;

    return isa->iidx;
}

int lib54asm_cpu8_1op_add_instruction(lib54asm_cpu8_1op_t* cpu, uint8_t isa_idx, uint8_t op1, uint8_t op2){
    if (cpu == NULL || cpu->program.program == NULL || cpu->operands == NULL || cpu->isa.instructions == NULL){
        return LIB54ASM_ERROR_INIT_MEM_NULL;
    }
    
    if (isa_idx > cpu->isa.size - 1 || cpu->program.iidx >= cpu->program.size){
        return LIB54ASM_ERROR_ADD_OUT_OF_BOUNDS;
    }

    cpu->program.program[cpu->program.iidx] = cpu->isa.instructions[isa_idx];
    cpu->operands[cpu->program.iidx] = ((uint16_t)op1 << 8) | op2;

    fprintf(cpu->ofd, "Added instruction at index %ld:\n- ID: %d\n- op: %d:%d\n\n", cpu->program.iidx, isa_idx, op1, op2);

    cpu->program.iidx += 1;
    return cpu->program.iidx;
}

int lib54asm_cpu8_1op_exec(lib54asm_cpu8_1op_t* cpu){
    unsigned long instruction_count = 0;
    unsigned long i = 0;
    char plural = ' ';

    if (cpu->program.iidx > 1){
        plural = 's';
    }

    fprintf(cpu->ofd, "Executing 8b1op CPU:\n- speed: %d\n- program size: %ld instruction%c\n- ISA size: %d\n\n", cpu->speed, cpu->program.iidx, plural, cpu->isa.iidx);

    lib54asm_instruction8_arg_t args;

    signal(SIGINT, lib54asm_handle_cpu_execute_signal);

    while (i < cpu->program.iidx && lib54asm_run_cpu){
        args.i = &i;
        args.cpu = cpu;
        args.op1 = (cpu->operands[i] >> 8) & 0xFF;
        args.op2 = cpu->operands[i] & 0xFF;

        cpu->program.program[i](&args);
        instruction_count += 1;

        fflush(cpu->ofd);
        usleep(cpu->speed);
    }

    if (lib54asm_run_cpu != 1){
        fprintf(cpu->ofd, "\nKeyboard interrupt was detected\n\n");
    }

    fprintf(cpu->ofd, "\nFinished executing:\n- executed instructions: %ld\n\n", instruction_count);

    return LIB54ASM_SUCCESS;
}