If you want to install gcc for sh4, look [here](https://github.com/TheRainbowPhoenix/sh4-devenv-gitpod/blob/master/Dockerfile)!

---

# Patching the fx-CP400 firmware
**You do not need to do this by hand if you just want to install programs on ypur calculator! Just use the program Snail2021.exe which is available in the releases.**

Simple instructions if you just want to install it: https://github.com/SnailMath/hollyhock-2#installation-simple 

If you don't want to youse the precompiled program and you want to compile it by hand, 
you need both a Windows and Linux machine to complete this process (as there is no updater for Linux, and the extraction tool extracts the firmware from the Windows DLLs). I'd recommend a virtual machine (or Vagrant box for simplicity) rather than two physical machines.

You'll also need a resource editor program - [Resource Hacker](http://www.angusj.com/resourcehacker/) has been tested to work ~~(Visual Studio can also be used to replace the RCDATA resource - just be careful to give the replaced resource an ID of 3070!).~~ (Snail2021 relies on 'Resource Hacker')

## 0. Clone this repository
Clone this repository onto your machine. If you're using a virtual machine, this is really great place to use the shared folders feature your VM host probably provides.

## 1. Build an SH4 cross-compiler
(This is also needed if you just want to develop programs for the Classpad II.)

Hop onto your Linux machine.

First, you'll have to define a few environment variables to ensure the process goes smoothly. Feel free to change `PREFIX` (the installation location), but don't change `TARGET`.

```sh
# folder to install our cross-compiler into
export PREFIX="$HOME/opt/cross"

# the architecture and executable format we're targeting
export TARGET=sh4-elf

# add the cross-compiler to our PATH
# you'll probably want to put this somewhere like .bash-profile or .bashrc
export PATH="$PREFIX/bin:$PATH"
```

Now, download the most recent stable version of the Binutils source code from the [Binutils website](https://gnu.org/software/binutils/). Extract the compressed file containing the source code into a directory, and `cd` into the newly created directory. Then, run these commands inside that directory. (SnailMath is using version 2.35)

```sh
mkdir build
cd build
../configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make
sudo make install
```

You'll now have tools such as `sh4aeb-elf-objcopy` available.

Now, download the most recent stable version of the GCC source code from the [GCC website](https://gnu.org/software/gcc/). Extract the compressed file containing the source code into a directory, and `cd` into the newly created directory. Then, run these commands inside that directory. Be warned - the `make` commands may take a long time, depending on your system, and the `contrib/download_prerequisites` script will download about 30 MB of archives. (SnailMath is using version 10.2.0)

```sh
contrib/download_prerequisites
mkdir build
cd build
../configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers --with-multilib-list=m4-nofpu
make all-gcc
make all-target-libgcc
sudo make install-gcc
sudo make install-target-libgcc
```
(Note: The `--with-multilib-list=m4-nofpu` flag is needed because the cpu doesn't have an fpu)

You’ll now have `sh4aeb-elf-g++` available for your usage.

_I would recommend adding `export PATH="$PATH:$HOME/opt/cross/bin"` to the file .bashrc in the home directory to add the path automatically after every login._
_I would recommend adding `export SDK_DIR="PATH-TO/hollyhock-2/sdk"` to the file .bashrc in the home directory to export the sdk dir automatically after every login._

## 2. Build the patches and the patcher
Keep working on your Linux machine.

`cd` into the `patcher/` directory. Run `make`.
(This will run `make` in `patches/` and it will merge the file mod.txt with the program Snail2021 into a file called Snail2021mod.c)

## 3. Building Snail2021
Snail2021 is the program, that will extract the firmware updater from the casio updater. To build this, go to your Windows machine.

-Install the MinGW-Installer
-Select `mingw32-base-bin` (from All Packages -> MinGW -> MinGW Base System) -> right click -> 'Mark for installation'
-Select `mingw32-libz-dev` (from All Packages -> MinGW -> MinGW Libaries -> MinGW Standard Libaries) -> right click -> 'Mark for installation'
-Click on  'Installation' -> 'Apply Changes'

-Double click on `patcher/make.bat`. You should not see any errors. This will create the file Snail2021.exe in the folder Snail2021 on the desktop

(If your name is not IEUser, you have to change the path in make.bat)

## 4. Run Snail2021 and modify the firmware
Go to the desktop into the folder Snail2021. 

Doule click on Snai2021.exe

Follow the instructions on screen:
- Download and install Resource Hacker http://angusj.com/resourcehacker/ (scroll down and click on exe install)
- Download the calculator updater version 02.01.2 https://tiplanet.org/forum/archives_voir.php?id=1044960 (Click on the green download
- Run the updater, click ok on every question, but __do not__ connect the calculator, go back to the Snail2021.exe window when you are asked to connect the calculator.
- Click Enter in the Snail2021.exe until it asks you to connect the calculator. 
- At this point, you can close the Official Updater again.
- When you click enter in Snail2021.exe for the last time, the modified updater opens.

## 5. Update the firmware
At this point the modified updater should be open. (it opens at the end of Snail2021.exe, but you can start it manually by clicking on the file `click_to_modify_the_calculator.bat`)

When the updater opens, hold the `[EXP]`, `[^]` and `Clear` keys on your fx-CP400. With those keys depressed, momentarily press the `RESET` button on the back of the calculator (hint: use the stylus!). Keep the three front buttons depressed until the LCD displays the following:

```
*****************
*    OS ERROR   *
* Please update *
*       OS      *
*****************
```

Release the buttons and connect the calculator to your PC using a USB cable. Proceed through the OS updater, and disconnect your calculator **only** once the update has finished.

## 6. Check the patch
Turn on your calculator and confirm the patch was applied correctly by opening the version dialog (either from the settings menu in the top left corner of Main > Version, or in System) and confirming the string "hollyhock" is displayed.

## 7. Build the launcher
`cd` into the `launcher/` directory. Run `make`, then copy the file `run.bin` into the root directory of your GC's flash.

## 8. Celebrate!
You've successfully patched your fx-CP400's firmware, and installed the Hollyhock Launcher. You're ready to go!

You can now start [using](using.md) or [developing](developing.md) software for your GC.
