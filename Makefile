CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -pedantic

SRC := $(wildcard src/*.c)
OBJ := $(patsubst src/%.c,build/%.o,$(SRC))
TOOLS := $(wildcard tools/*.c)
BINS := $(patsubst tools/%.c,/usr/local/bin/54asm-%,$(TOOLS))

default: install clean

build/%.o: src/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

.PHONY: install clean uninstall lib bin

install: lib bin

clean:
	rm -f build/*.o build/lib54asm.a

uninstall:
	rm -f /usr/local/lib/lib54asm.a
	rm -f /usr/local/bin/54asm-*
	rm -rf /usr/local/include/lib54asm

lib: $(OBJ)
	ar rcs -v build/lib54asm.a $(OBJ)
	cp build/lib54asm.a /usr/local/lib/

	mkdir -p /usr/local/include/lib54asm
	cp include/* /usr/local/include/lib54asm/

bin: $(BINS)

# Rule to build each tool
/usr/local/bin/54asm-%: tools/%.c
	$(CC) $(CFLAGS) $< build/lib54asm.a -o $@