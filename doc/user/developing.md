# Developing for the fx-CP400
## I (SnailMath) have made a video, where I explained this. Just click [here: youtube.com/playlist...](https://www.youtube.com/playlist?list=PLrNL2wv7MwKqpz-BhOiwH_1DbS690ZDs4)

You'll need your SH4 cross compiler to develop applications for the fx-CP400. If you patched your firmware, you'll already have done this. If not, check the instructions in [this](patching.md) document to build and install one.

## 1. Build the SDK
This only needs to happen once to generate the required object files which will be linked with your application when it's compiled. It isn't necessary to rebuild the SDK itself whenever you change your application code.

`cd` into the `sdk/` directory, and run the `make` command. Ensure the file `sdk.o` is generated - this is the boilerplate code required for your applications to work when compiled.

If you'd like a local copy of the SDK documentation, run the `make docs` command. Open `sdk/doc/index.html` to view them.

_I would recommend adding `export PATH="$PATH:$HOME/opt/cross/bin"` to the file .bashrc in the home directory to add the path automatically after every login._
_I would recommend adding `export SDK_DIR="PATH-TO/hollyhock-2/sdk"` to the file .bashrc in the home directory to export the sdk dir automatically after every login._

## 2. Start your project
Copy the contents of the `app_template/` directory to an empty folder. This will become your project's root directory.

Open `main.cpp` in your favourite text editor, and edit the strings in the `APP_NAME`, `APP_DESCRIPTION`, `APP_AUTHOR` and `APP_VERSION` macro calls to match your application. These are the strings that will be displayed in the launcher when your application is selected. If you don't wish to provide one or more of these fields, simply delete the line (they're all optional).

Similarly, open the `Makefile` and edit the first line (`APP_NAME:=app_template` by default), changing `app_template` to the filename you'd like your generated `.hhk` file to have.

Edit `main.cpp` to your hearts content, and add other `.cpp`, `.c`, and `.s` files as you please. The `Makefile` will automatically detect new source files at compile time.

## 3. Get ready to build
You'll need to point the Makefile to the SDK using the `SDK_DIR` environment variable. For example, if you cloned this repository to `~/hollyhock-2/`, set `SDK_DIR` to `~/hollyhock-2/sdk/`. You can do this either by exporting the variable before running any `make` commands, or setting the variable in the same command as you run make. It's recommended you make this an absolute path and not a relative one - otherwise you may run into issues if you attempt to call `make` while not in the same directory as the one you defined `SDK_DIR` relative to.

```sh
# Either do this once, at the start of every session (recommended):
export SDK_DIR=~/hollyhock-2/sdk/

# Or, run make like this:
SDK_DIR=~/hollyhock-2/sdk/ make
```

## 4. Build your app!
Run `make` (remembering to set `SDK_DIR` inline if you didn't `export` it).

A `.hhk` file with the name you specified in the `Makefile` will be generated, which you can then copy onto the root directory of the calculator's flash.

Open the launcher and select your application to launch it. Have fun!

## Newlib
If you want to use C standard libraries such as math.h, string.h and others when developing for the fx-CP400, you must have a standard library implementation such as Newlib installed. Newlib also includes division and other arithmetic subroutines for our SuperH CPU, so that you do not have to always add them to your project manualy when you need them.

First, you will have to define a few environment variables. The `$PREFIX` variable can be changed to your prefered installation directory but `$TARGET` should be left untouched.

```sh
# Directory to install newlib into
# You most likely already have this defined if you have built a cross platform gcc
# I recommend you to change it to the SDK's directory like this:
export PREFIX="$SDK_DIR/newlib" 

# Our target architecture
# Again, you might already have $TARGET defined as "sh4-elf"
# Make sure to change it to "sh-elf"
export TARGET="sh-elf"

# The prefix of our cross compiler's binaries such as gcc, as, ld, etc ...
export TARGET_BINS="sh4-elf"
```

After that, grab the latest stable version of the Newlib source code from [their website](https://sourceware.org/newlib/) and extract it. Then run the following commands. 

_Make sure to change `VERSION` to your newlib version._

```sh
mkdir build-newlib
cd build-newlib
../newlib-VERSION/configure --target=$TARGET --prefix=$PREFIX CC_FOR_TARGET=${TARGET_BINS}-gcc AS_FOR_TARGET=${TARGET_BINS}-as LD_FOR_TARGET=${TARGET_BINS}-ld AR_FOR_TARGET=${TARGET_BINS}-ar RANLIB_FOR_TARGET=${TARGET_BINS}-ranlib
make all
make install
```

You will now have Newlib installed on your machine. To use it in your projects, add the following to your Makefile's `CC_FLAGS`, `CXX_FLAGS` and `LD_FLAGS`.
If you have used a different installation directory you need to change them here.

```make
CC_FLAGS:=-I $(SDK_DIR)/newlib/sh-elf/include

CXX_FLAS:=-I $(SDK_DIR)/newlib/sh-elf/include

LD_FLAGS:=-L$(SDK_DIR)/newlib/sh-elf/lib
```

Congratulations! You can now utilize standard C functions in your fx-CP400 project.

If you also want newlib to create the arithmetic subroutines, you will need to modify a few more things in your Makefile.

```make
LD:=sh4aeb-elf-g++ # Change the linker to gcc instead of ld
LD_FLAGS:=-nostartfiles ---specs=nosys.specs -m4a-nofpu -Wno-undef -L$(SDK_DIR)/newlib/sh-elf/lib # Change your LD_FLAGS to this (remove --no-undefined)

# For the APP_BIN target rule, change the line
$(LD) --oformat=binary -T linker.ld -o $@ $(LD_FLAGS) $(OBJECTS) $(SDK_DIR)/sdk.o

# To
$(LD) -Wl,--oformat=binary -T linker.ld -o $@ $(LD_FLAGS) $(OBJECTS) $(SDK_DIR)/sdk.o
```

Thats it. You can now build your app and enjoy Newlib!