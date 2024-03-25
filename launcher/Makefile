ifndef SDK_DIR
$(error You need to define the SDK_DIR environment variable, and point it to the sdk/ folder)
endif

AS:=sh4-elf-as
AS_FLAGS:=

CC:=sh4-elf0-gcc
CC_FLAGS:=-ffreestanding -flto -fshort-wchar -Wall -Wextra -O2 -I $(SDK_DIR)/include/

CXX:=sh4-elf-g++
CXX_FLAGS:=-ffreestanding -flto -fno-exceptions -fno-rtti -fshort-wchar -Wall -Wextra -O2 -I $(SDK_DIR)/include/

LD:=sh4-elf-ld
LD_FLAGS:=-nostdlib --no-undefined

READELF:=sh4-elf-readelf
OBJCOPY:=sh4-elf-objcopy

AS_SOURCES:=$(wildcard *.s)
CC_SOURCES:=$(wildcard *.c)
CXX_SOURCES:=$(wildcard *.cpp)
OBJECTS:=$(AS_SOURCES:.s=.o) $(CC_SOURCES:.c=.o) $(CXX_SOURCES:.cpp=.o)

all: run.bin Makefile

clean:
	rm -f $(OBJECTS) run.bin

run.bin: $(OBJECTS) $(SDK_DIR)/sdk.o linker.ld
	$(LD) -T linker.ld -o $@ $(LD_FLAGS) $(OBJECTS) $(SDK_DIR)/sdk.o

# We're not actually building sdk.o, just telling the user they need to do it
# themselves. Just using the target to trigger an error when the file is
# required but does not exist.
$(SDK_DIR)/sdk.o:
	$(error You need to build the SDK before using it. Run make in the SDK directory, and check the README.md in the SDK directory for more information)

%.o: %.s
	$(AS) $< -o $@ $(AS_FLAGS)

%.o: %.c
	$(CC) $< -o $@ $(CC_FLAGS)

# Break the build if global constructors are present:
# Read the sections from the object file (with readelf -S) and look for any
# called .ctors - if they exist, give the user an error message, delete the
# object file (so that on subsequent runs of make the build will still fail)
# and exit with an error code to halt the build.
%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXX_FLAGS)
	@$(READELF) $@ -S | grep ".ctors" > /dev/null && echo "ERROR: Global constructors aren't supported." && rm $@ && exit 1 || exit 0

.PHONY: all clean
