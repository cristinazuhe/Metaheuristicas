
TARGETS= pro
CFLAGS= -g -c -std=gnu99 -I$(INC)
CC = gcc
BIN = bin
SRC = src
OBJ = obj
INC = include

default : $(TARGETS)

$(TARGETS) : lectura.o greedy.o timer.o main.o busqueda_local.o random.o enfriamiento_simulado.o busqueda_tabu.o busqueda_tabu_ext.o
	$(CC) $^ -lm -o $@

lectura.o: lectura.c lectura.h greedy.c greedy.h timer.c timer.h busqueda_local.c busqueda_local.h random.c random.h busqueda_tabu.c busqueda_tabu.h busqueda_tabu_ext.c busqueda_tabu_ext.h enfriamiento_simulado.c enfriamiento_simulado.h
	$(CC) $(CFLAGS) $< -lm -o $@

clean:
	rm *.o

mrproper: clean
	rm $(TARGETS)
