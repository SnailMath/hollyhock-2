# `patcher aka Snail2021`
This folder contains the program to modify the firmware updater with the modifications from the folder patches. This program is called __Snail2021.exe__, it war first developed for an old project of mine (LoaderII and LoaderIIb). 

__Snail202x is a submodule!__ (https://github.com/SnailMath/Snail202x)

Because the firmware updater is only available for windows, this needs to be compiled and executed unter windows.

The executable is called Snail2021.exe because this is derived from my previous attempts to modify the firmware, where the program was called Snail2020.exe. I don't want to change this name, so the experience for users who just want to do this modification see the similarity of this programs when looking at just the file name.

If you want to compile Snail2021 from source, you need minGW under windows.
(You can always download the precompiled file Snail2021.zip which contains Snail2021.exe and the modifications precompiled, so you don't need Linux to compile the patches [in the folder /patches] )

#colpile#
I split the compilation in two parts.

- The first part is running `make all` under Linux. This will make all the patches, create the mod.txt for modifying the firmware and create Snail2021mod.c,
which is a copy of Snail2020.c which contains the mod.txt. 
I did this, so the user does not need to download mod.txt seperately. 
When they execute the executable (which will be compiled in step two) the file
mod.txt will be created, because this step includes it in the code.

- The second step is running `make.bat` (the one in this folder, not the one from Snail202x) under windows.
You need to install minGW (and zlib) beforehand. See the README file in the Snail202x repository for more info.
The batch file make.bat will use gcc to compile Snail2021mod.c (and the required libaries) in a single file called `Snail2021-hollyhock2.exe`.

`Snail2021-hollyhock2.exe` is the only file you need from all this to modify the calculator.
It includes the tools to extract the frirmware, it contains the mod.txt and it contains the programs to start the updater.
Copy this file to a real Computer (If you compiled it in a VM) and execute it.
It will tell the user to download the last two missing parts: the Updater itself and Resourcehacker.



##Note##
I am not affiliated with the program ResourceHacker and I am not publishing any part of it. 
