# TIRAR ERROS EXTRAS ANTES DE ENTREGAR
parametrosCompilacao=-Wall -Wextra -O2 #-Wshadow
nomePrograma=trab

all: $(nomePrograma)

$(nomePrograma): main.o redblack.o
	gcc -o $(nomePrograma) main.o redblack.o $(parametrosCompilacao)

main.o: main.c
	gcc -c main.c $(parametrosCompilacao)

ordenacao.o: redblack.h redblack.c
	gcc -c redblack.c $(parametrosCompilacao)

clean:
	rm -f *.o *.gch $(nomePrograma)
