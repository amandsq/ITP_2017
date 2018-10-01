# PATH'S
OBJDIR = ./obj
INCDIR = ./include
SRCDIR = ./src
BINDIR = ./bin

PROJ_NAME = catarata

# SHELL CONVENTIONS
CC = gcc
FLAGS = -Wall -g -I $(INCDIR)

all: bin
	$(CC) $(FLAGS) $(SRCDIR)/main.c $(SRCDIR)/filtros.c $(SRCDIR)/processamento.c -o $(BINDIR)/$(PROJ_NAME)
	@echo "Compilation process complete!"

bin:
	mkdir -p bin

clean:
	rm -rf $(BINDIR)
