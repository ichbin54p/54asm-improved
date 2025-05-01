# 54asm

54asm is a programming language designed to run on minecraft computers, spefically CPU's with 1 operand.

# Table of contents

* [Usage](#usage)
    * [Compiling](#compiling)
    * [Syntax](#syntax)
    * [ISA](#isa)
        * [Default ISA](#default-isa)
        * [Custom ISA's](#custom-flags)
    * [Flags](#flags)
        * [Default Flags](#default-flags)
        * [Custom Flags](#custom-flags)

# Usage

## Compiling

To compile 54asm from source, make sure you have `make` installed then run

```sh
sudo make
```

or

```sh
sudo make install
sudo make clean
```

But, if you want to compile 54asm code, make sure you have `54asm-compile` and `54asm-execute` in your PATH and then run

```sh
54asm-compile <INPUT> -o <OUTPUT>
```

To execute the compiled file, run

```sh
54asm-execute <INPUT> -log <LOG_FILE> -speed <EXECUTE-SPEED-IN-NANOSECONDS> -size <PROGRAM-SIZE>
```

## Syntax

54asm's syntax is very simple, for every line you have an opcode and then operands. The examples in this tutorial will all have 1 operand (*1 operand in the sense that it is compatable for 1 operand minecraft CPU's, for example: the LDI instruction has 2 operands but the first operand is unused and the second operand is technically another instruction*)

An example of the syntax would be

```py
RST 0
```

This program will store the value of ACC into register 0, depending on your ISA.

Another example here is loading immediate

```py
LDI 0 54
RST 0
```

This will load an immediate of the value 54 and store it in register 0. The first operand has no use currently.

## ISA

In this tutorial, we will use the default ISA. Custom ISA's can be defined and used, it will be covered later.

### Default ISA

#### LDI `LDI <op1> <op2>`

Loads an immediate of value `<op2>` into ACC

#### RST `RST <op1>`

Stores the value of ACC into reg `<op1>`

#### ADD `ADD <op1>`

Adds reg `<op1>` with ACC and replaces current ACC value with the sum

#### SUB `SUB <op1>`

Subtracts the value of ACC from reg `<op1>` and replaces current ACC value with the sum

#### LOD `LOD <op1> <op2>`

Stores value from ram address `<op2>` into ACC

#### STR `STR <op1> <op2>`

Loads ACC value into ram address `<op2>`

#### BRC `BRC <op1> <op2>`

Branches to instruction `<op2>` if flag `<op1>` is true. See placeholder_flags

#### PLD `PLD <op1>`

Loads value from port `<op1>` into ACC

#### PST `PST <op1>`

Stores value from port `<op1>` into ACC

#### XOR `XOR <op1>`

XOR's ACC and the value of reg `<op1>` and replaces current ACC value with the sum

#### XNOR `XOR <op1>`

XNOR's ACC and the value of reg `<op1>` and replaces current ACC value with the sum

#### INC `INC <op1>`

Increments ACC

#### DEC `DEC <op1>`

Decrements ACC

### Custom ISA's

To create custom ISA's, you will have to modify the source code of lib54asm, specifically `isa.h`.

## Flags

### Default flags

Here's a table of the default flags

```
index  name  description

  0    NULL  always on
  1    ZERO  only on if ZERO
  2    COUT  only on if COUT
```

The reason of the NULL flag is to replace the JMP instruction

### Custom flags

The feature of adding custom flags is not officially supported