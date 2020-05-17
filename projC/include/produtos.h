#ifndef produtos_h
#define produtos_h

#include "catalogo.h"
#include "resultados.h"


typedef struct cat_produtos* Produtos;


Produtos initProdutos();
void freeProdutos(Produtos cat_prod);

Catalogo getCatalogoProd(Produtos cat_prod);

void addProduto(Produtos cat_prod, char* produto);
void removeProduto(Produtos cat_prod, char* produto);

int existCodigoProduto(Produtos cat_prod, char* produto);

GSList* productsStartsWithLetter(Produtos cat_prod, char letter);

int nnodos(Produtos cat_prod);

GPtrArray* printProdutos(Produtos cat_prod);

Produtos cloneProdutos(Produtos cat_prod);

#endif