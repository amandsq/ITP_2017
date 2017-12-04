
Catarata: filtros.o processamento.o main2.o
	gcc filtros.o main2.o processamento.o -o Catarata -lm
filtros.o:
	gcc -o filtros.o -c filtros.c -lm
processamento.o:
	gcc -o processamento.o -c processamento.c -lm
main2.o:
	gcc -o main2.o -c main2.c -lm
clear:
	rm -rf *.o
 

#aprender a usar o sistema makefile
