all:
	cd mod_marker;  make; cd ..
	cd file_loader; make; cd ..
	#
	# You have to make sure to cat the files in the right order by hand.
	# The patcher works its way from the start of the file, so the mod.txt
	# file needs the modifications in the correct order.
	cat mod_marker/mod.part.txt   > mod.txt #this is first
	echo "*\r\n\r" >> mod.txt
	cat file_loader/mod.part.txt >> mod.txt #this is second


clean:
	cd mod_marker;  make clean; cd ..
	cd file_loader; make clean; cd ..
	rm mod.txt

.PHONY: all clean
