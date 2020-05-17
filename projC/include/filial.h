#ifndef filial_h
#define filial_h

#include "catalogo.h"
#include "clientes.h"
#include "venda.h"
#include "resultados.h"

#include <glib.h>

typedef struct info* Info;
typedef struct filial* Filial;

typedef struct catalogos_array* CatalogosArray;
typedef struct tree_array* TreeArray;
typedef struct nr_quant* NrQuantidade;

Filial initFilial();
void freeFilial(Filial filial);

Filial cloneFilial(Filial filial);
Filial* cloneFiliais(Filial* filiais);

void addVendaFilial(Venda venda, Filial filial);

/*queries*/

CatalogosArray get_clients_branches(Clientes clientes, Filial* filiais, int flag);
GPtrArray* getGPtrArray2(CatalogosArray cats_array);
int getNumero2(CatalogosArray cats_array);

int** client_table(Filial* filiais, char* client);
void freeTable(int** table);

void product_buyers(Filial filial,ArrayNumero array_nr);

GPtrArray* products_by_cliente(Filial* filiais, char* clienteID, int month);

GPtrArray* top_products(Filial filial, int limit);

GPtrArray* top_profit_products(Filial* filiais, char* clientId, int limit);

#endif

