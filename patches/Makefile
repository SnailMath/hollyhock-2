all:
	cd mod_marker;  make; cd ..
	cd file_loader; make; cd ..
	#
	# You have to make sure to cat the files in the right order by hand.
	# The patcher works its way from the start of the file, so the mod.txt
	# file needs the modifications in the correct order.
	echo "'ver02.01.2000" > mod.txt #add the version of the os update we
	#want to modify. The user gets notified by Snail2021.exe if they try
	#to use the wrong version of the firmware.
	cat file_loader/mod.part.txt >> mod.txt #this is first
	echo "*\n" >> mod.txt
	cat mod_marker/mod.part.txt  >> mod.txt #this is second


clean:
	cd mod_marker;  make clean; cd ..
	cd file_loader; make clean; cd ..
	rm mod.txt

.PHONY: all clean
