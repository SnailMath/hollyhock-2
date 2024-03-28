# run `make all` to compile the .hhk and .bin file, use `make` to compile only the .bin file.
# The .hhk file is the original format, the bin file is a newer format.
APP_NAME:=app_template

ifndef SDK_DIR
$(error You need to define the SDK_DIR environment variable, and point it to the sdk/ folder)
endif

AS:=sh4aeb-elf-gcc
AS_FLAGS:=

CC:=sh4aeb-elf-gcc
CC_FLAGS:=-ffreestanding -flto -ffunction-sections -fdata-sections -fshort-wchar -Wall -Wextra -Os -I $(SDK_DIR)/include/

CXX:=sh4aeb-elf-g++
CXX_FLAGS:=-ffreestanding -flto -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fshort-wchar -Wall -Wextra -Os -I $(SDK_DIR)/include/ -m4a-nofpu

LD:=sh4aeb-elf-g++
LD_FLAGS:=-nostartfiles --specs=nosys.specs -Wno-undef -Wl,--gc-sections -L$(SDK_DIR)

READELF:=sh4aeb-elf-readelf
OBJCOPY:=sh4aeb-elf-objcopy

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

$(APP_ELF): $(OBJECTS) $(SDK_DIR)/libsdk.a linker_hhk.ld
	$(LD) -T linker_hhk.ld -o $@ $(LD_FLAGS) $(OBJECTS) -lsdk
	$(OBJCOPY) --set-section-flags .hollyhock_name=contents,strings,readonly $(APP_ELF) $(APP_ELF)
	$(OBJCOPY) --set-section-flags .hollyhock_description=contents,strings,readonly $(APP_ELF) $(APP_ELF)
	$(OBJCOPY) --set-section-flags .hollyhock_author=contents,strings,readonly $(APP_ELF) $(APP_ELF)
	$(OBJCOPY) --set-section-flags .hollyhock_version=contents,strings,readonly $(APP_ELF) $(APP_ELF)

$(APP_BIN): $(OBJECTS) $(SDK_DIR)/libsdk.a linker_bin.ld
	$(LD) -T linker_bin.ld -o $@ $(LD_FLAGS) $(OBJECTS) -lsdk

# We're not actually building libsdk.a, just telling the user they need to do it
# themselves. Just using the target to trigger an error when the file is
# required but does not exist.
$(SDK_DIR)/libsdk.a:
	$(error You need to build the SDK before using it. Run make in the SDK directory, and check the README.md in the SDK directory for more information)

%.o: %.s
	$(AS) -c $< -o $@ $(AS_FLAGS)

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
