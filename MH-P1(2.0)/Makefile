
TARGETS= pro
CFLAGS= -g -c -std=gnu99 -I$(INC)
CC = gcc
BIN = bin
SRC = src
OBJ = obj
INC = include

default : $(TARGETS)

$(TARGETS) : lectura.o greedy.o timer.o main.o busqueda_local.o random.o enfriamiento_simulado.o
	$(CC) $^ -lm -o $@

lectura.o: lectura.c lectura.h greedy.c greedy.h timer.c timer.h busqueda_local.c busqueda_local.h random.c random.h enfriamiento_simulado.c enfriamiento_simulado.h
	$(CC) $(CFLAGS) $< -lm -o $@

clean:
	rm *.o

mrproper: clean
	rm $(TARGETS)
