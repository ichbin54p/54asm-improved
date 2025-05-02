#ifndef ISA_H
#define ISA_H
#define ISA_SIZE 16
#define ISA_OFFS 64 // Don't change this value for compatability
#include "lib54asm.h"

typedef struct {
    int id;
    int oc;
    char name[4];
} ISA_token;

void update_flags(lib54asm_cpu8_1op_t* cpu, int16_t* n){
    cpu->flags[LIB54ASM_FLAG_NULL] = 1;
    cpu->flags[LIB54ASM_FLAG_ZERO] = 0;
    cpu->flags[LIB54ASM_FLAG_COUT] = 0;

    if (*n > 255){
        *n -= 256;
        cpu->flags[LIB54ASM_FLAG_COUT] = 1;
    } else if (*n < 0){
        *n += 256;
        cpu->flags[LIB54ASM_FLAG_COUT] = 1;
    } 
    
    if (*n == 0){
        cpu->flags[LIB54ASM_FLAG_ZERO] = 1;
    }

    fprintf(cpu->ofd, "\nflag: UPD: updating flags:\n- n: %d\n- f:\n-- null: %d\n-- zero: %d\n-- cout: %d\n\n", *n, cpu->flags[LIB54ASM_FLAG_NULL], cpu->flags[LIB54ASM_FLAG_ZERO], cpu->flags[LIB54ASM_FLAG_COUT]);
}

void LDI(lib54asm_instruction8_arg_t* args){
    fprintf(args->cpu->ofd, "exec: LDI: loading %d to reg0(%d)\n", args->op2, args->cpu->reg[0]);

    args->cpu->reg[0] = args->op2;
    *args->i += 1;
}

void RST(lib54asm_instruction8_arg_t* args){
    fprintf(args->cpu->ofd, "exec: RST: loading reg0(%d) to reg%d(%d)\n", args->cpu->reg[0], args->op1 + 1, args->cpu->reg[args->op1 + 1]);

    args->cpu->reg[args->op1 + 1] = args->cpu->reg[0];
    *args->i += 1;
}

void RLD(lib54asm_instruction8_arg_t* args){
    fprintf(args->cpu->ofd, "exec: RLD: loading reg%d(%d) to reg0(%d)\n", args->op1 + 1, args->cpu->reg[args->op1 + 1], args->cpu->reg[0]);

    args->cpu->reg[0] = args->cpu->reg[args->op1 + 1];
    *args->i += 1;
}

void ADD(lib54asm_instruction8_arg_t* args){
    fprintf(args->cpu->ofd, "exec: ADD: adding reg%d(%d) with reg0(%d) = %d\n", args->op1 + 1, args->cpu->reg[args->op1 + 1], args->cpu->reg[0], args->cpu->reg[args->op1 + 1] + args->cpu->reg[0]);

    int16_t n = args->cpu->reg[args->op1 + 1] + args->cpu->reg[0];

    update_flags(args->cpu, &n);

    args->cpu->reg[0] = n;
    *args->i += 1;
}

void SUB(lib54asm_instruction8_arg_t* args){
    fprintf(args->cpu->ofd, "exec: SUB: subtracting reg%d(%d) with reg0(%d) = %d\n", args->op1 + 1, args->cpu->reg[args->op1 + 1], args->cpu->reg[0], args->cpu->reg[args->op1 + 1] - args->cpu->reg[0]);

    int16_t n = args->cpu->reg[args->op1 + 1] - args->cpu->reg[0];

    update_flags(args->cpu, &n);

    args->cpu->reg[0] = n;
    *args->i += 1;
}

void NOOP(lib54asm_instruction8_arg_t* args){
    fprintf(args->cpu->ofd, "exec: NOOP: doing nothing\n");

    *args->i += 1;
}

void STR(lib54asm_instruction8_arg_t* args){
    fprintf(args->cpu->ofd, "exec: LOD: loading reg0(%d) to ram addr %d(%d)\n", args->cpu->reg[0], args->op2, args->cpu->mem.ram[args->op2 + 1]);

    args->cpu->mem.ram[args->op2 + 1] = args->cpu->reg[0];
    *args->i += 1;
}

void LOD(lib54asm_instruction8_arg_t* args){
    fprintf(args->cpu->ofd, "exec: STR: loading ram addr %d(%d) to reg0(%d)\n", args->op2, args->cpu->mem.ram[args->op2], args->cpu->reg[0]);

    args->cpu->reg[0] = args->cpu->mem.ram[args->op2];
    *args->i += 1;
}

void BRC(lib54asm_instruction8_arg_t* args){
    if (args->op1 > 2){
        fprintf(args->cpu->ofd, "WARNING: flag index %d is out of bounds\n", args->op1);

        return;
    }

    fprintf(args->cpu->ofd, "exec: BRC: attempting to branch to instruction %d, flag %d(%d):\n", args->op2, args->op1, args->cpu->flags[args->op1]);

    if (args->cpu->flags[args->op1]){
        fprintf(args->cpu->ofd, "- condition is true, branching to %d:\n\n", args->op2);
        
        *args->i = args->op2;
    } else {
        fprintf(args->cpu->ofd, "- condition is false, not branching to %d:\n\n", args->op2);

        *args->i += 1;
    }
}

void PLD(lib54asm_instruction8_arg_t* args){
    if (args->op1 * 2 >= args->cpu->mem.port_count * 2){
        fprintf(args->cpu->ofd, "WARNING: unable to acces port %d as it exceeds the max port count %d\n", args->op1, args->cpu->mem.port_count);

        *args->i += 1;

        return;
    }

    fseek(args->cpu->mem.port, args->op1 * 2, SEEK_SET);
    uint8_t n = fgetc(args->cpu->mem.port);

    fprintf(args->cpu->ofd, "exec: PLD: loading port%d(%d) to reg0(%d)\n", args->op1, n, args->cpu->reg[0]);

    args->cpu->reg[0] = n;
    *args->i += 1;
}

void PST(lib54asm_instruction8_arg_t* args){
    if ((args->op1 * 2) + 1 >= (args->cpu->mem.port_count * 2) + 1){
        fprintf(args->cpu->ofd, "WARNING: unable to acces port %d as it exceeds the max port count %d\n", args->op1, args->cpu->mem.port_count);

        *args->i += 1;

        return;
    }

    uint8_t n = fgetc(args->cpu->mem.port);

    fprintf(args->cpu->ofd, "exec: PST: loading reg0(%d) to port%d(%d)\n", args->cpu->reg[0], args->op1, n);

    fseek(args->cpu->mem.port, (args->op1 * 2) + 1, SEEK_SET);
    fputc(args->cpu->reg[0], args->cpu->mem.port);

    *args->i += 1;
}

void XOR(lib54asm_instruction8_arg_t* args){
    fprintf(args->cpu->ofd, "exec: XOR: xor'ing reg%d(%d) with reg0(%d) = %d\n", args->op1 + 1, args->cpu->reg[args->op1 + 1], args->cpu->reg[0], args->cpu->reg[args->op1 + 1] ^ args->cpu->reg[0]);

    int16_t n = args->cpu->reg[args->op1 + 1] ^ args->cpu->reg[0];

    update_flags(args->cpu, &n);

    args->cpu->reg[0] = n;
    *args->i += 1;
}

void XNOR(lib54asm_instruction8_arg_t* args){
    fprintf(args->cpu->ofd, "exec: XNOR: xnor'ing reg%d(%d) with reg0(%d) = %d\n", args->op1 + 1, args->cpu->reg[args->op1 + 1], args->cpu->reg[0], ~(args->cpu->reg[args->op1 + 1] ^ args->cpu->reg[0]));

    int8_t n = ~(args->cpu->reg[args->op1 + 1] ^ args->cpu->reg[0]);
    int16_t nf = n;

    update_flags(args->cpu, &nf);

    args->cpu->reg[0] = nf;
    *args->i += 1;
}

void INC(lib54asm_instruction8_arg_t* args){
    fprintf(args->cpu->ofd, "exec: INC: incrementing reg0(%d) = %d\n", args->cpu->reg[0], args->cpu->reg[0] + 1);

    int16_t n = args->cpu->reg[0] + 1;

    update_flags(args->cpu, &n);

    args->cpu->reg[0] = n;
    *args->i += 1;
}

void DEC(lib54asm_instruction8_arg_t* args){
    fprintf(args->cpu->ofd, "exec: DEC: decrementing reg0(%d) = %d\n", args->cpu->reg[0], args->cpu->reg[0] - 1);

    int16_t n = args->cpu->reg[0] - 1;

    update_flags(args->cpu, &n);

    args->cpu->reg[0] = n;
    *args->i += 1;
}

lib54asm_instruction_ptr_t ISA[ISA_SIZE] = {
    LDI,
    RST,
    RLD,
    ADD,
    SUB,
    PLD,
    PST,
    LOD,
    STR,
    BRC,
    XOR,
    XNOR,
    INC,
    DEC,
    NOOP,
    NOOP
};

ISA_token tokens[ISA_SIZE] = {
    {0, 2, "LDI"},
    {1, 1, "RST"},
    {2, 1, "RLD"},
    {3, 1, "ADD"},
    {4, 1, "SUB"},
    {5, 1, "PLD"},
    {6, 1, "PST"},
    {7, 2, "LOD"},
    {8, 2, "STR"},
    {9, 2, "BRC"},
    {10, 1, "XOR"},
    {11, 1, "XNOR"},
    {12, 1, "INC"},
    {13, 1, "DEC"}
};

#endif