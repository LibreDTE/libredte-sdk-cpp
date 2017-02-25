CC=g++
CPPFLAGS=-ggdb -Wall -std=c++11
all: ejemplos
clean: clean-ejemplos
.PHONY: ejemplos
ejemplos:
	cd ejemplos; $(CC) $(CPPFLAGS) 001-generar_dte.cpp -o 001-generar_dte -lcurl
	cd ejemplos; $(CC) $(CPPFLAGS) 002-actualizar_estado.cpp -o 002-actualizar_estado -lcurl
clean-ejemplos:
	rm -f ejemplos/001-generar_dte
	rm -f ejemplos/002-actualizar_estado
