#include "../include/produtos.h"


#define PROD_PROFUNDIADE 2

/*Estrutura Produtos:
* consiste num Catalogo responsável por guardar todos os codigos dos produtos vendidos 
*/
struct cat_produtos{
    Catalogo produtos;
};

/*Função responsável por alocar a memória necessária à criação da estruta Produtos.*/
Produtos initProdutos(){
    Produtos cat_prod = malloc(sizeof(struct cat_produtos));
    cat_prod->produtos = initCatalogo(PROD_PROFUNDIADE);
    return cat_prod;
}

/*Função responsável por libertar toda a memória alocada necessária à estrutua Produtos*/
void freeProdutos(Produtos cat_prod){
    freeCatalogo(cat_prod->produtos);
    free(cat_prod);
}

/*Função que devolve o Catalogo de um catalogo de produtos*/
Catalogo getCatalogoProd(Produtos cat_prod){
    return cat_prod->produtos;
}

/*Função responsável por adicionar um produto a um catalogo de produtos*/
void addProduto(Produtos cat_prod, char* produto){
    add(cat_prod->produtos, produto, NULL);
}

/* Função que remove um código de um produto de uma estrutura Produtos*/
void removeProduto(Produtos prodsNaoComprados, char* produto){
    removeCode(prodsNaoComprados->produtos, produto);
}

/*Função que verifica se um determinado produto existe num catalogo de produtos*/
int existCodigoProduto(Produtos cat_prod, char* produto){
    return existCodigo(cat_prod->produtos, produto);
}

/*Função que retorna uma lista de produtos, ordenada alfabeticamente, começados por uma letra*/
GSList* productsStartsWithLetter(Produtos cat_prod, char letter){
    return startsWithLetter(cat_prod->produtos, letter);
}

/*Função que calcula o número de códigos presentes num catalogo de produtos*/
int nnodos(Produtos cat_prod){
    return nnodes(cat_prod->produtos);
}

/*Função que coloca num array de apontadores todos os códigos de um catálogo de produtos*/
GPtrArray* printProdutos(Produtos cat_prod){
    return printCatalogo(cat_prod->produtos);
}

/*Função responsável por clonar uma estrutura Produtos*/
Produtos cloneProdutos(Produtos cat_prod){
    Produtos cat_prod_clone = malloc(sizeof(struct cat_produtos));
    cat_prod_clone->produtos = cloneCatalogo(cat_prod->produtos, PROD_PROFUNDIADE);

    return cat_prod_clone;
}
