#
#
#
# *Tarea1 - Lenguajes de Programacion - 2016-2
# *Repositorio en gitlab: git@gitlab.labcomp.cl:gsepulve/tarea1LP-2016-2-gsepulve.git
#
#
#

##################  VARIABLES  ###################

# Cambiar source_dir por el nombre de la carpeta en donde se guardara el juego
# en caso de cambiarlo.

OBJ=elepemon.o

###################  COMANDOS  ###################

INIH:
	make -C inih/extra

ATTACKS:
	make -C attacks

COMPILE: INIH ATTACKS
	gcc -c elepemon.c
	gcc -Linih/extra main.c $(OBJ) -o main  -linih -ldl

RUN:
	./main

CLEAN:
	make clean -C inih/extra
	make clean -C attacks
	rm -f $(OBJ)
	rm main