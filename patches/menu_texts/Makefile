BIN_FILE:=imaginary_unit
ADDRengIM:=808B5CD8# Imaginary Unit
ADDRengEX:=808BB211# 808BB221 -User name editing (19 chars including 0x00)
ADDRfraIM:=808C5460# Imaginaire i ou j
ADDRfraEX:=808CB1E4# -Édition Nom d'Utilisateur (28 inc 0x00)
ADDRdeuIM:=808D65C0# Imaginäre Einheit
ADDRdeuEX:=808DC202# -Änderung des Benutzernamens (30 inc 0x00)
ADDRespIM:=808E7054# Unidad imaginaria
ADDRespEX:=808ECC6B# -Edición del nombre de usuario (33 inc 0x00 0x00)
ADDRsuoIM:=808F5F90# Imaginaariyks.
ADDRsuoEX:=808FB57A# -Käyttäjätunnuksen muokkaaminen (38 inc 4x 0x00)

mod.part.txt: Makefile ${BIN_FILE}.bin README.md
	#replaces the text Imaginary Unit in the settings with the text from 
	#imaginary_unit.bin and replaces the last line from the exam mode
	#start windows with the info that hollyhock is disabled during exam.
	#in english the original line "-User name editing" is too short, so we
	#have to shorten the line before that to get space...
	echo -n "'" > mod.part.txt
	head -2 README.md| tail -1 >> mod.part.txt
	#
	echo "${ADDRengIM}: 'eng" >> mod.part.txt
	xxd -ps ${BIN_FILE}.bin >> mod.part.txt
	echo "*" >> mod.part.txt
	echo "${ADDRengEX}:" >> mod.part.txt
	#also teplace "with 3pin cable" with "(3pin cable)" (2 less characters)
	echo -n "(3pin cable)\r-Hollyhock Launcher\000" | xxd -ps -c 64 >> mod.part.txt
	echo "*" >> mod.part.txt
	#
	echo "${ADDRfraIM}: 'fra" >> mod.part.txt
	xxd -ps ${BIN_FILE}.bin >> mod.part.txt
	echo "*" >> mod.part.txt
	echo "${ADDRfraEX}:" >> mod.part.txt
	echo -n "-Hollyhock Launcher\000" | xxd -ps -c 64 >> mod.part.txt #28
	echo "*" >> mod.part.txt
	#
	echo "${ADDRdeuIM}: 'deu" >> mod.part.txt
	xxd -ps ${BIN_FILE}.bin >> mod.part.txt
	echo "*" >> mod.part.txt
	echo "${ADDRdeuEX}:" >> mod.part.txt
	echo -n "-Hollyhock Launcher\000" | xxd -ps -c 64 >> mod.part.txt #30
	echo "*" >> mod.part.txt
	#
	echo "${ADDRespIM}: 'esp" >> mod.part.txt
	xxd -ps ${BIN_FILE}.bin >> mod.part.txt
	echo "*" >> mod.part.txt
	echo "${ADDRespEX}:" >> mod.part.txt
	echo -n "-Hollyhock Launcher\000" | xxd -ps -c 64 >> mod.part.txt #33
	echo "*" >> mod.part.txt
	#
	echo "${ADDRsuoIM}: 'suo" >> mod.part.txt
	xxd -ps ${BIN_FILE}.bin >> mod.part.txt
	echo "*" >> mod.part.txt
	echo "${ADDRsuoEX}:" >> mod.part.txt
	echo -n "-Hollyhock Launcher\000" | xxd -ps -c 64 >> mod.part.txt #38
	echo "" >> mod.part.txt
clean:
	rm mod.part.txt
