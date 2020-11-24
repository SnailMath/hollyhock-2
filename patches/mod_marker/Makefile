ADDR:=808FCC10

mod.part.txt: Makefile mod_marker.bin
	echo -n "${ADDR}: '" > mod.part.txt
	head -2 README.md| tail -1 >> mod.part.txt
	echo "" >> mod.part.txt
	xxd -ps mod_marker.bin >> mod.part.txt
clean:
	rm mod.part.txt
