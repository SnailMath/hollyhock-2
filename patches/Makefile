all:
	cd mod_marker;  make; cd ..
	#don't build the backslash files, because I made that mod.part.txt by hand...
	cd menu_texts;  make; cd ..
	cd file_loader; make; cd ..
	#
	# You have to make sure to cat the files in the right order by hand.
	# The patcher works its way from the start of the file, so the mod.txt
	# file needs the modifications in the correct order.
	echo "'ver02.01.2000\n" > mod.txt #add the version of the os update we
	#want to modify. The user gets notified by Snail2021.exe if they try
	#to use the wrong version of the firmware.
	cat file_loader/mod.part.txt >> mod.txt #this is first		80128018
	echo "*\n" >> mod.txt
	cat backslash/mod.part.txt  >> mod.txt #this is second	806321C8
	echo "*\n" >> mod.txt
	cat menu_texts/mod.part.txt  >> mod.txt #this is second	808B5CD8
	echo "*\n" >> mod.txt
	cat mod_marker/mod.part.txt  >> mod.txt #this is second		808FCC10


clean:
	cd mod_marker;  make clean; cd ..
	#don't remove the backslash files, because I made that mod.part.txt by hand...
	cd menu_texts;  make clean; cd ..
	cd file_loader; make clean; cd ..
	rm mod.txt

.PHONY: all clean
