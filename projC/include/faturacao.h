#ifndef faturacao_h
#define faturacao_h

#include "venda.h"
#include "catalogo.h"
#include "produtos.h"
#include "resultados.h"

#include <glib.h>


typedef struct valores* Valores;

typedef struct fat* Faturacao;

typedef struct catalogo_array* CatalogoArray;


Faturacao initFaturacao();
void freeFaturacao(Faturacao fat);

GPtrArray* printValores(Valores valores);

Faturacao cloneFaturacao(Faturacao faturacao);

void addVendaFat(Venda venda, Faturacao fat);

void valoresProdutoMes(Faturacao fat, char* produto, int mes, Valores valores[]);

CatalogoArray productsNeverBougth(Produtos cat_prod, Faturacao fat, int i);
GPtrArray* getGPtrArray(CatalogoArray cat_array);
int getNumero(CatalogoArray cat_array);

void nr_vendas_and_total(Faturacao fat, Nr_total nrTotal);

#endif 