
all: Snail2021mod.c
	#
	# Use make.bat under windows to compile the windows executable.
	#

Snail2021mod.c: Snail202x/Snail2021.c ../patches/mod.txt Makefile
	#this will output the file Snail2021mod.c which can be compiled under
	#Windows using MinGW.
	echo -n "#define MOD_TXT \"" > Snail2021mod.c
	cat ../patches/mod.txt | tr '\\\n' '/\\' | sed "s/\\\/\\\r\\\n/g" | sed "s/\"/\\\\\"/g" >> Snail2021mod.c 
	echo "\"" >> Snail2021mod.c
	cat Snail202x/Snail2021.c >> Snail2021mod.c

../patches/mod.txt:
	cd ../patches; make; cd ../patcher

clean:
	rm Snail2021mod.c Snail2021.exe
	cd ../patches; make clean; cd ../patcher
