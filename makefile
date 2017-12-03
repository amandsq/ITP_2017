
Catarata: filtros.o processamento.o main2.o
	gcc filtros.o main2.o processamento.o -o Catarata
filtros.o:
	gcc -o filtros.o -c filtros.c 
processamento.o:
	gcc -o processamento.o -c processamento.c 
main2.o:
	gcc -o main2.o -c main2.c 
clear:
	rm -rf *.o
 

#aprender a usar o sistema makefile