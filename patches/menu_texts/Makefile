BIN_FILE:=imaginary_unit
ADDRengIM:=808B5CD8  # Imaginary Unit
ADDRengEX:=808BABBA  # -User name editing (20 chars including 0x0D 0x0D)
ADDRfraIM:=808C5460  # Imaginaire i ou j
ADDRfraEX:=808CA8A1  # -Édition Nom d'utilisateur (29 inc 0x0D 0x0D)
ADDRdeuIM:=808D65C0  # Imaginäre Einheit
ADDRdeuEX:=808DB8A3  # -Änderung des Benutzernamens (31 inc 0x0D 0x0D)
ADDRespIM:=808E7054  # Unidad imaginaria
ADDRespEX:=808EC48F  # -Edición del nombre de usuario (33 inc 0x0D 0x0D)
ADDRsuoIM:=808F5F90  # Imaginaariyks.
ADDRsuoEX:=808FAE0A  # -Käyttäjätunnuksen muokkaaminen (36 inc 0x0D 0x0D)

mod.part.txt: Makefile ${BIN_FILE}.bin README.md
	echo -n "'" > mod.part.txt
	head -2 README.md| tail -1 >> mod.part.txt
	#
	echo "${ADDRengIM}: ' eng" >> mod.part.txt
	xxd -ps ${BIN_FILE}.bin >> mod.part.txt
	echo "*" >> mod.part.txt
	echo "${ADDRengEX}:" >> mod.part.txt
	echo -n -e "-Hollyhock        \015\015" | xxd -ps -c 64 >> mod.part.txt #20
	echo "*" >> mod.part.txt
	#
	echo "${ADDRfraIM}: ' fra" >> mod.part.txt
	xxd -ps ${BIN_FILE}.bin >> mod.part.txt
	echo "*" >> mod.part.txt
	echo "${ADDRfraEX}:" >> mod.part.txt
	echo -n -e "-Hollyhock Loader          \015\015" | xxd -ps -c 64 >> mod.part.txt #29
	echo "*" >> mod.part.txt
	#
	echo "${ADDRdeuIM}: ' deu" >> mod.part.txt
	xxd -ps ${BIN_FILE}.bin >> mod.part.txt
	echo "*" >> mod.part.txt
	echo "${ADDRdeuEX}:" >> mod.part.txt
	echo -n -e "-Hollyhock Loader            \015\015" | xxd -ps -c 64 >> mod.part.txt #31
	echo "*" >> mod.part.txt
	#
	echo "${ADDRespIM}: ' esp" >> mod.part.txt
	xxd -ps ${BIN_FILE}.bin >> mod.part.txt
	echo "*" >> mod.part.txt
	echo "${ADDRespEX}:" >> mod.part.txt
	echo -n -e "-Hollyhock Loader              \015\015" | xxd -ps -c 64 >> mod.part.txt #33
	echo "*" >> mod.part.txt
	#
	echo "${ADDRsuoIM}: ' suo" >> mod.part.txt
	xxd -ps ${BIN_FILE}.bin >> mod.part.txt
	echo "*" >> mod.part.txt
	echo "${ADDRsuoEX}:" >> mod.part.txt
	echo -n -e "-Hollyhock Loader                 \015\015" | xxd -ps -c 64 >> mod.part.txt #36
	echo "" >> mod.part.txt
clean:
	rm mod.part.txt
