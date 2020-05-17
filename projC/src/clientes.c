#include "../include/clientes.h"


#define CLI_PROFUNDIADE 1

/*Estrutura Clientes:
* consiste num Catalogo responsável por guardar todos os codigos dos clientes registados
*/
struct cat_clientes{
    Catalogo clientes;
};

/*Função responsável por alocar a memória necessária à criação da estruta Clientes.*/
Clientes initClientes(){
    Clientes cat_cli = malloc(sizeof(struct cat_clientes));
    cat_cli->clientes = initCatalogo(CLI_PROFUNDIADE);
    return cat_cli;
}

/*Função responsável por libertar toda a memória alocada necessária à estrutua Clientes*/
void freeClientes(Clientes cat_cli){
    freeCatalogo(cat_cli->clientes);
    free(cat_cli);
}

/*Função responsável por devolver o catálogo da estruta Clientes*/
Catalogo getCatalogoCli(Clientes cat_cli){
    return cat_cli->clientes;
}

/*Função responsável por adicionar um código de cliente na estrutura Clientes*/
void addCliente(Clientes cat_cli, char* cliente){
    add(cat_cli->clientes, cliente, NULL);
}

/*Função que verifica se um determinado código existe na estrutura Clientes*/
int existCodigoCliente(Clientes cat_cli, char* cliente){
    return existCodigo(cat_cli->clientes, cliente);
}

/*Função responsável por clonar uma estrutura Clientes*/
Clientes cloneClientes(Clientes cat_cli){
    Clientes cat_cli_clone = malloc(sizeof(struct cat_clientes));
    cat_cli_clone->clientes = cloneCatalogo(cat_cli->clientes, CLI_PROFUNDIADE);

    return cat_cli_clone;
}