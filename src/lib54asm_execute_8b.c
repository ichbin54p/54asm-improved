#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib54asm.h"
#include "isa.h"

int lib54asm_execute_8b_1op(FILE* log, FILE* ifd, int speed, size_t program, int debug, FILE* debug_fd){
    int status = 0;
    lib54asm_cpu8_1op_t cpu;

    if (debug){
        fprintf(debug_fd, "mem init...\n");
        fflush(debug_fd);
    }

    status = lib54asm_init_mem8(&cpu.mem, 255, 8);

    if (status != 0){
        lib54asm_error_print(status, log);

        return 1;
    }

    if (debug){
        fprintf(debug_fd, "isa init...\n");
        fflush(debug_fd);
    }

    status = lib54asm_init_isa(&cpu.isa, ISA_SIZE);

    if (status != 0){
        lib54asm_error_print(status, log);
        lib54asm_cleanup_isa(&cpu.isa);
        lib54asm_cleanup_mem8(&cpu.mem);

        return 1;
    }

    if (debug){
        fprintf(debug_fd, "cpu init...\n");
        fflush(debug_fd);
    }

    status = lib54asm_init_cpu8_1op(&cpu, 4, program, speed, log);

    if (status != 0){
        lib54asm_error_print(status, log);
        lib54asm_cleanup_isa(&cpu.isa);
        lib54asm_cleanup_mem8(&cpu.mem);

        return 1;
    }

    if (debug){
        fprintf(debug_fd, "adding ISA's instructions...\n");
        fflush(debug_fd);
    }

    for (int i = 0; i < ISA_SIZE; i++){
        if (debug){
            fprintf(debug_fd, "- adding instruction %d...\n", i);
            fflush(debug_fd);
        }

        status = lib54asm_isa_add_opcode(&cpu.isa, ISA[i]);

        if (status < 0){
            lib54asm_error_print(status, stdout);
            fprintf(log, "error adding opcode %d\n", i);

            lib54asm_cleanup_cpu8_1op(&cpu);
    
            return 1;
        }
    }

    if (debug){
        fprintf(debug_fd, "loading program into CPU rom...\n");
        fflush(debug_fd);
    }

    char fbuffer;
    uint8_t opci = 0;
    uint8_t op1 = 0;
    uint8_t op2 = 0;
    int operands_expected = 0;
    int operands_loaded = 0;

    while (fread(&fbuffer, 1, 1, ifd)) {
        if (operands_expected == 0) {
            opci = fbuffer - ISA_OFFS;
            operands_expected = tokens[opci].oc;
            operands_loaded = 0;
        } else if (operands_loaded == 0) {
            op1 = fbuffer;
            operands_loaded = 1;

            if (operands_expected == 1) {
                if (debug){
                    fprintf(debug_fd, "- loading %s %d...\n", tokens[opci].name, op1);
                    fflush(debug_fd);
                }

                status = lib54asm_cpu8_1op_add_instruction(&cpu, opci, op1, 0);

                if (status < 0){
                    fprintf(log, "Error adding opcode %d operands: %d %d\n", opci, op1, op2);
                    lib54asm_error_print(status, log);
                    lib54asm_cleanup_cpu8_1op(&cpu);
            
                    return 1;
                }

                operands_expected = 0;
            }
        } else if (operands_loaded == 1) {
            op2 = fbuffer;

            if (debug){
                fprintf(debug_fd, "- loading %s %d %d...\n", tokens[opci].name, op1, op2);
                fflush(debug_fd);
            }

            status = lib54asm_cpu8_1op_add_instruction(&cpu, opci, op1, op2);
            
            if (status < 0){
                fprintf(log, "Error adding opcode %d operands: %d %d\n", opci, op1, op2);
                lib54asm_error_print(status, log);
                lib54asm_cleanup_cpu8_1op(&cpu);
        
                return 1;
            }

            operands_expected = 0;
        }
    }

    if (debug){
        fprintf(debug_fd, "executing CPU...\n");
        fflush(debug_fd);
    }

    status = lib54asm_cpu8_1op_exec(&cpu);

    if (status < 0){
        lib54asm_error_print(status, log);
        lib54asm_cleanup_cpu8_1op(&cpu);

        return 1;
    }

    if (debug){
        fprintf(debug_fd, "cleaning up CPU...\n");
        fflush(debug_fd);
    }

    lib54asm_cleanup_cpu8_1op(&cpu);

    return 0;
}