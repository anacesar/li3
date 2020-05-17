 CC = gcc
FLAGS = -Wall -O2 -g -ansi -Qunused-arguments
LIBS = `pkg-config --cflags --libs glib-2.0`
INCLUDES = ./include/
SRC = ./src/

program: view.o main.o queries.o parser.o filial.o faturacao.o clientes.o produtos.o catalogo.o venda.o resultados.o
	$(CC) $(FLAGS) view.o main.o queries.o parser.o filial.o faturacao.o clientes.o produtos.o catalogo.o venda.o resultados.o -o program $(LIBS) 

main.o: $(SRC)main.c $(INCLUDES)interface.h 
	$(CC) $(FLAGS) -c $(SRC)main.c $(LIBS) 

queries.o: $(SRC)queries.c $(INCLUDES)interface.h 
	$(CC) $(FLAGS) -c $(SRC)queries.c $(LIBS)

parser.o: $(SRC)parser.c $(INCLUDES)parser.h 
	$(CC) $(FLAGS) -c $(SRC)parser.c $(LIBS)

filial.o: $(SRC)filial.c $(INCLUDES)filial.h 
	$(CC) $(FLAGS) -c $(SRC)filial.c $(LIBS)

faturacao.o: $(SRC)faturacao.c $(INCLUDES)faturacao.h 
	$(CC) $(FLAGS) -c $(SRC)faturacao.c $(LIBS)

produtos.o: $(SRC)produtos.c $(INCLUDES)produtos.h 
	$(CC) $(FLAGS) -c $(SRC)produtos.c $(LIBS)

clientes.o: $(SRC)clientes.c $(INCLUDES)clientes.h 
	$(CC) $(FLAGS) -c $(SRC)clientes.c $(LIBS)

catalogo.o: $(SRC)catalogo.c $(INCLUDES)catalogo.h 
	$(CC) $(FLAGS) -c $(SRC)catalogo.c $(LIBS)

venda.o: $(SRC)venda.c $(INCLUDES)venda.h 
	$(CC) $(FLAGS) -c $(SRC)venda.c $(LIBS)

resultados.o: $(SRC)resultados.c $(INCLUDES)resultados.h 
	$(CC) $(FLAGS) -c $(SRC)resultados.c $(LIBS)

view.o: $(SRC)view.c $(INCLUDES)view.h
	$(CC) $(FLAGS) -c $(SRC)view.c $(LIBS)


clean: 
	rm -rf *.o *.dSYM 
	rm program

run: 
	./program