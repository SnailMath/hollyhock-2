DOXYGEN=doxygen

AS:=sh4-elf-as
AS_FLAGS:=

CC:=sh4-elf-g++
CC_FLAGS:=-ffreestanding -fno-exceptions -fno-rtti -m4-nofpu -Wall -Wextra -O2 -I include/

# -r flag so the sdk.o file can be linked with the user's application object
# files: generates a relocatable object file
LD:=sh4-elf-ld
LD_FLAGS:=-nostdlib -r

AS_SOURCES:=$(wildcard *.s) $(wildcard **/*.s) $(wildcard **/**/*.s)
CC_SOURCES:=$(wildcard *.cpp) $(wildcard **/*.cpp) $(wildcard **/**/*.cpp)
OBJECTS:=$(AS_SOURCES:.s=.o) $(CC_SOURCES:.cpp=.o)

all: sdk.o Makefile

docs:
	$(DOXYGEN)

clean:
	rm -f $(OBJECTS) sdk.o

clean_docs:
	rm -rf doc/

sdk.o: $(OBJECTS)
	$(LD) -o $@ $(LD_FLAGS) $(OBJECTS)

%.o: %.s
	$(AS) $< -o $@ $(AS_FLAGS)

%.o: %.cpp
	$(CC) -c $< -o $@ $(CC_FLAGS)

.PHONY: all docs clean clean_docs
