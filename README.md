# 54asm

54asm is a programming language designed to run on minecraft computers, spefically CPU's with 1 operand.

This is lib54asm's github page, the documentation here is intended for the usage of lib54asm, to learn how to use 54asm as a programming language, visit <placeholder>

# Compiling from source

To compile 54asm from source, make sure you have make installed and then run

```sh
sudo make install
```

If you ever need to uninstall 54asm, just run

```sh
sudo make uninstall
```

**NOTE** for updating, I would recommend uninstalling and then installing again otherwise, some files might not get updated

# Tools

54asm comes with tools which were developed for CLI usage. There are several tools included in 54asm but the most important ones are `54asm-compile` and `54asm-execute`

The other tools which aren't as important should also be installed, they include

- `54asm-gpu`
- `54asm-display-input`
- `54asm-cpu-port-input`

The usage of these tools can be found later in the documentation

## compile

To compile your 54asm files, you can use `54asm-compile`, if you just run the file without any arguements, it will print a usage help text

Usage help:

```
Usage: 54asm-compile [input | path] [arguements]

-o, --output [STR | PATH]
```

Arguements in more detail:

- `-o`, `--output` **required**, output file of the compiled 54asm code

Example usage:

```sh
54asm-execute main.build -log log.log -size 128 -speed 100000
```

## execute

When you've compiled a 54asm file you can execute it with the `54asm-execute` tool. The `54asm-execute` tool is basically like an emulator with virtual memory.

Usage help:

```
Usage: 54asm-execute [STR | PATH] [arguements]

-log [STR | PATH]
-speed [INT]
-size [INT]
-debug [1 | 0]
-debug-file [STR | PATH]
```

Arguements in more detail:

- `-log` log file path, will overwrite existing file. Default `stdout`
- `-speed` the CPU's instruction speed in nanoseconds
- `-size` the size of the program, the max amount of instructions for a *compiled* program
- `-debug` a bool (1 | 0), print debug information
- `-debug-file` debug file path, will overwrite existing file. Default `stdout`

Example usage:

```sh
54asm-execute main.build -log log.log -size 128 -speed 100000
```

## less important tools

Will document this later

# Using lib54asm

Lib54asm is the 54asm library, it can be imported into your C projects. The 54asm-tools are actually based off lib54asm so, lib54asm would be their dependency.

The full documentation of lib54asm will be written later... For now you can use the tools source code as reference.