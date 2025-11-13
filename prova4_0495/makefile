parametrosCompilacao=-Wall -Wextra -O2 -lm#-Wshadow
nomePrograma=trab

all: $(nomePrograma)

$(nomePrograma): main.o KD_Tree.o
	gcc -o $(nomePrograma) main.o KD_Tree.o $(parametrosCompilacao)

main.o: main.c
	gcc -c main.c $(parametrosCompilacao)

ordenacao.o: KD_Tree.h KD_Tree.c
	gcc -c KD_Tree.c $(parametrosCompilacao)

clean:
	rm -f *.o *.gch $(nomePrograma)
