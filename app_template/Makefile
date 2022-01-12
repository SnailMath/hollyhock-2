# run `make all` to compile the .hhk and .bin file, use `make` to compile only the .bin file.
# The .hhk file is the original format, the bin file is a newer format.
APP_NAME:=app_template

ifndef SDK_DIR
$(error You need to define the SDK_DIR environment variable, and point it to the sdk/ folder)
endif

AS:=sh4-elf-as
AS_FLAGS:=

CC:=sh4-elf-gcc
CC_FLAGS:=-ffreestanding -fshort-wchar -Wall -Wextra -O2 -I $(SDK_DIR)/include/

CXX:=sh4-elf-g++
CXX_FLAGS:=-ffreestanding -fno-exceptions -fno-rtti -fshort-wchar -Wall -Wextra -O2 -I $(SDK_DIR)/include/ -m4a-nofpu

LD:=sh4-elf-ld
LD_FLAGS:=-nostdlib --no-undefined

READELF:=sh4-elf-readelf
OBJCOPY:=sh4-elf-objcopy

AS_SOURCES:=$(wildcard *.s)
CC_SOURCES:=$(wildcard *.c)
CXX_SOURCES:=$(wildcard *.cpp)
OBJECTS:=$(AS_SOURCES:.s=.o) $(CC_SOURCES:.c=.o) $(CXX_SOURCES:.cpp=.o)

APP_ELF:=$(APP_NAME).hhk
APP_BIN:=$(APP_NAME).bin

bin: $(APP_BIN) Makefile

hhk: $(APP_ELF) Makefile

all: $(APP_ELF) $(APP_BIN) Makefile

clean:
	rm -f $(OBJECTS) $(APP_ELF) $(APP_BIN)

$(APP_ELF): $(OBJECTS) $(SDK_DIR)/sdk.o linker.ld
	$(LD) -T linker.ld -o $@ $(LD_FLAGS) $(OBJECTS) $(SDK_DIR)/sdk.o
	$(OBJCOPY) --set-section-flags .hollyhock_name=contents,strings,readonly $(APP_ELF) $(APP_ELF)
	$(OBJCOPY) --set-section-flags .hollyhock_description=contents,strings,readonly $(APP_ELF) $(APP_ELF)
	$(OBJCOPY) --set-section-flags .hollyhock_author=contents,strings,readonly $(APP_ELF) $(APP_ELF)
	$(OBJCOPY) --set-section-flags .hollyhock_version=contents,strings,readonly $(APP_ELF) $(APP_ELF)

$(APP_BIN): $(OBJECTS) $(SDK_DIR)/sdk.o linker.ld
	$(LD) --oformat binary -T linker.ld -o $@ $(LD_FLAGS) $(OBJECTS) $(SDK_DIR)/sdk.o

# We're not actually building sdk.o, just telling the user they need to do it
# themselves. Just using the target to trigger an error when the file is
# required but does not exist.
$(SDK_DIR)/sdk.o:
	$(error You need to build the SDK before using it. Run make in the SDK directory, and check the README.md in the SDK directory for more information)

%.o: %.s
	$(AS) $< -o $@ $(AS_FLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CC_FLAGS)

# Break the build if global constructors are present:
# Read the sections from the object file (with readelf -S) and look for any
# called .ctors - if they exist, give the user an error message, delete the
# object file (so that on subsequent runs of make the build will still fail)
# and exit with an error code to halt the build.
%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXX_FLAGS)
	@$(READELF) $@ -S | grep ".ctors" > /dev/null && echo "ERROR: Global constructors aren't supported." && rm $@ && exit 1 || exit 0

.PHONY: bin hhk all clean
