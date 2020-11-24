ADDR:=80128018

AS:=sh4-elf-as
AS_FLAGS:=

OBJCOPY:=sh4-elf-objcopy
OBJCOPY_FLAGS:=-O binary

all: mod.part.txt Makefile

clean:
	rm -f file_loader.o file_loader.bin mod.part.txt

mod.part.txt: file_loader.bin Makefile
	echo -n "${ADDR}: '" > mod.part.txt
	head -2 README.md| tail -1 >> mod.part.txt
	echo "" >> mod.part.txt
	xxd -ps file_loader.bin >> mod.part.txt
	echo "" >> mod.part.txt

file_loader.bin: file_loader.o
	$(OBJCOPY) $(OBJCOPY_FLAGS) file_loader.o file_loader.bin

file_loader.o: file_loader.s
	$(AS) $< -o $@ $(AS_FLAGS)

.PHONY: all clean
