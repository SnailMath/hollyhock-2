ADDR:=808FCC10
BIN_FILE:=mod_marker

mod.part.txt: Makefile ${BIN_FILE}.bin README.md
	echo -n "${ADDR}: '" > mod.part.txt
	head -2 README.md| tail -1 >> mod.part.txt
	echo "" >> mod.part.txt
	xxd -ps ${BIN_FILE}.bin >> mod.part.txt
	echo "" >> mod.part.txt
clean:
	rm mod.part.txt

