DOXYGEN=doxygen

AS:=sh4aeb-elf-gcc
AS_FLAGS:=

CC:=sh4aeb-elf-g++

CC_FLAGS:=-ffreestanding -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -m4a-nofpu -Wall -Wextra -Os -I include/

AR:=sh4aeb-elf-ar
AR_FLAGS:=src

AS_SOURCES:=$(wildcard *.s) $(wildcard **/*.s) $(wildcard **/**/*.s)
CC_SOURCES:=$(wildcard *.cpp) $(wildcard **/*.cpp) $(wildcard **/**/*.cpp)
OBJECTS:=$(AS_SOURCES:.s=.o) $(CC_SOURCES:.cpp=.o)

all: libsdk.a sdk.o Makefile

docs:
	$(DOXYGEN)

clean:
	rm -f $(OBJECTS) libsdk.a sdk.o

clean_docs:
	rm -rf doc/

libsdk.a: $(OBJECTS)

lib%.a:
	$(AR) $(AR_FLAGS) $@ $^

%.o: lib%.a
	ln -s $< $@

%.o: %.s
	$(AS) -c $< -o $@ $(AS_FLAGS)

%.o: %.cpp
	$(CC) -c $< -o $@ $(CC_FLAGS)

.PHONY: all docs clean clean_docs
