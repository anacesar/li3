#ifndef clientes_h
#define clientes_h

#include "catalogo.h"

typedef struct cat_clientes* Clientes;


Clientes initClientes();
void freeClientes(Clientes cat_cli);

Catalogo getCatalogoCli(Clientes cat_cli);

void addCliente(Clientes cat_cli, char* cliente);

int existCodigoCliente(Clientes cat_cli, char* cliente);

Clientes cloneClientes(Clientes cat_cli);

#endif