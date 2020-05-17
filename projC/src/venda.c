#include "../include/venda.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAMPOSVENDA 7
#define MAXLINHAVENDA 35


/**Estrutua venda:
 * responsável por armazenar todas as informações relativas a uma venda
 */
struct venda {
       char* codProd;
       float precoUnit;
       int quantidade;
       char tipo;
       char* codCli;
       int mes;
       int filial;
};

/* Função responsável por alocar memória relativa à estrutura Venda */
Venda initVenda(){
    Venda venda = malloc(sizeof(struct venda));
    return venda;
}

/* Função responsável por retornar o código de produto de uma venda */
char* getCodeProduct(Venda venda){
    return venda->codProd;
}

/* Função responsável por definir o código de produto de uma venda */
void setCodeProduct(Venda venda, char* produto){
    venda->codProd = strdup(produto);
}

/* Função responsável por retornar o preço unitário do produto de uma venda */
float getPrice(Venda venda){
    return venda->precoUnit;
}

/* Função responsável por definir o preço unitário do produto de uma venda */
void setPrice(Venda venda, float preco){
    venda->precoUnit = preco;
}

/* Função responsável por retornar a quantidade vendida de um produto numa venda */
int getQuantity(Venda venda){
    return venda->quantidade;
}

/* Função responsável por definir a quantidade vendida de um produto numa venda */
void setQuantity(Venda venda, int quantidade){
    venda->quantidade = quantidade;
}

/* Função responsável por retornar o tipo de uma venda (Normal ou Promoção) */
char getType(Venda venda){
    return venda->tipo;
}

/* Função responsável por definir o tipo de uma venda (Normal ou Promoção) */
void setType(Venda venda, char tipo){
    venda->tipo = tipo;
}

/* Função responsável por retornar o código de cliente de uma venda */
char* getCodeClient(Venda venda){
    return venda->codCli;
}

/* Função responsável por definir o código de cliente de uma venda */
void setCodeClient(Venda venda, char* cliente){
    venda->codCli = strdup(cliente);
}

/* Função responsável por retornar o mês de uma venda */
int getMonthSale(Venda venda){
    return venda->mes;
}

/* Função responsável por definir o mês de uma venda */
void setMonthSale(Venda venda, int mes){
    venda->mes = mes;
}

/* Função responsável por retornar a filial de uma venda */
int getBranchSale(Venda venda){
    return venda->filial;
}

/* Função responsável por definir a filial de uma venda */
void setBranchSale(Venda venda, int filial){
    venda->filial = filial;
}

