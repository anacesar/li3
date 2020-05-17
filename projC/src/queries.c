#include "../include/interface.h"

#include <stdio.h>
#include <stdlib.h>



/**
 * Estrutura SGV: estrutura principal do programa que contêm toda a informação necessária a criação, manutenção e consulta do sistema de vendas
 * É constítuido por: 
 * - uma estrutura Clientes que guarda todos os códigos dos clientes válidos e registados no sistema de vendas;
 * - uma estrutura Produtos que guarda, por sua vez, todos os códigos dos produtos válidos e que podem ser vendidos no sistema de vendas;
 * - uma segunda estrutura Produtos que contêm todos os códigos dos produtos que não foram vendidos em nenhuma das filiais;
 * - três estruturas InfoFile que guardam a informação relativa aos 3 fichieros que são lidos para o carregamento dos dados;
 * - uma estrutura Faturacao que relaciona o número de registos de vendas e o total faturado de cada produto em cada mês;
 * - um array com tamanho igual ao número de filiais do sistema. Cada posição contêm uma estrutura Filial, que relaciona para cada cliente, os produtos que comprou a quantidade comprada em cada mes, o total gasto nesse produto e se ou comprou em modo Normal e/ou Promoção.
 */
struct sgv{
    Clientes clientes;
    Produtos produtos;
    Produtos prodsNaoComprados;
    InfoFile cliinfo;
    InfoFile prodinfo;
    InfoFile vendasinfo;
    Faturacao faturacao;
    Filial filiais[FILIAIS];
};


/*Função auxiliar responsável por inicializar a estrutura de dados SGV, contendo toda a informação necessária ao funcionamento do programa*/
SGV initSGV(){
    int i;
    SGV sgv= (SGV)malloc(sizeof(struct sgv));
    sgv->clientes = initClientes();
    sgv->produtos = initProdutos();
    sgv->prodsNaoComprados = initProdutos();
    sgv->cliinfo = initInfoFile();
    sgv->prodinfo = initInfoFile();
    sgv->vendasinfo = initInfoFile();
    sgv->faturacao = initFaturacao();
    for(i=0; i<FILIAIS; i++) sgv->filiais[i] = initFilial();
    return sgv;
}

/*Função auxiliar responsável pela libertação da memória alocada para a estrutura de dados SGV*/
void destroySGV(SGV sgv){
    freeClientes(sgv->clientes);
    freeProdutos(sgv->produtos);
    freeProdutos(sgv->prodsNaoComprados);
    freeFaturacao(sgv->faturacao);
    freeInfoFile(sgv->cliinfo);
    freeInfoFile(sgv->prodinfo);
    freeInfoFile(sgv->vendasinfo);
    free(sgv);
}

/* Função que devolve uma cópia do catálogo de clientes*/
Clientes getClientes(SGV sgv){
    return cloneClientes(sgv->clientes);
}

/* Função que devolve uma cópia do catálogo de produto*/
Produtos getProdutos(SGV sgv){
    return cloneProdutos(sgv->produtos);
}

/* Função que devolve uma cópia do catálogo de produto*/
Produtos getProdutosNaoComprados(SGV sgv){
    return cloneProdutos(sgv->prodsNaoComprados);
}

/* Função que devolve uma cópia da estrutura faturação*/
Faturacao getFaturacao(SGV sgv){
    return cloneFaturacao(sgv->faturacao);
}

/* Função que devolve uma cópia de uma determinada filial*/
Filial getFilial(SGV sgv, int i){
    return cloneFilial(sgv->filiais[i]);
}

/* Função que devolve uma cópia das filiais */
Filial* getFiliais(SGV sgv){
    return cloneFiliais(sgv->filiais);
}


/*Função auxiliar que guarda num array de apontadores a informação relativa à leitura dos ficheiros*/
GPtrArray* printInfoFiles(SGV sgv){
    GPtrArray* res = printInfoFile(sgv->cliinfo);
    g_ptr_array_extend_and_steal (res, printInfoFile(sgv->prodinfo));
    g_ptr_array_extend_and_steal (res, printInfoFile(sgv->vendasinfo));

    return res;
}


/** Query1:
 * Responsável por carregar para todas as estruturas que compoem o SGV, a informação relevante 
 */
SGV loadSGVFromFiles(SGV sgv, char* clientsFilePath, char* productsFilePath, char* salesFilePath){
    /*clock_t tic = clock();*/

    if(readClientes(clientsFilePath, sgv->clientes, sgv->filiais, sgv->cliinfo) ||
       readProdutos(productsFilePath, sgv->produtos, sgv->prodsNaoComprados, sgv->prodinfo) || 
       readVendas(salesFilePath, sgv->clientes, sgv->produtos, sgv->prodsNaoComprados, sgv->faturacao, sgv->filiais, sgv->vendasinfo)) sgv = NULL;

    /*clock_t toc = clock(); */
    /*printf("Query1: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC); */
    return sgv;
}

/**Query2:
 * Retorna uma estrutua auxiliar que contém uma lista ligada de todos os códigos de produtos começados pela letra letter(passada como parâmetro), e o número de códigos dessa lista
 */
GSList* getProductsStartedByLetter(SGV sgv, char letter){
    /* clock_t tic = clock(); */
    Produtos produtos_clone = getProdutos(sgv);
    
    GSList* list = productsStartsWithLetter(produtos_clone, letter);

    freeProdutos(produtos_clone);
    
    /*clock_t toc = clock();*/
    /*printf("Query2: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);*/

    return list;
}

/** Query3:
 * Devolve um array de apontadores com o número de vendas e o total faturado de um produto, num determinado mês.
 */
GPtrArray* getProductsSalesAndProfit(SGV sgv, char* productId, int month, int flag){
    /*clock_t tic = clock();*/
    GPtrArray* res = NULL;
    
    if(existCodigoProduto(sgv->produtos, productId)){
        res = g_ptr_array_new();

        int i;
        char info[BUFFER];

        Valores valores[ARRAYSIZE];
        Faturacao fat_clone = getFaturacao(sgv);

        valoresProdutoMes(fat_clone, productId, month-1, valores);

        if(flag==0){
            sprintf(info, "VALORES PARA AS %d FILIAIS" , FILIAIS);
            g_ptr_array_add(res, strdup(info));
            g_ptr_array_extend_and_steal(res,printValores(valores[FILIAIS]));
        }else{
            for(i=0; i<FILIAIS; i++){
                sprintf(info, "FILIAL %d", i+1);
                g_ptr_array_add(res, strdup(info));
                g_ptr_array_extend_and_steal(res,printValores(valores[i]));
            }
        }

        for(i=0; i<ARRAYSIZE; i++) free(valores[i]);
    }

    /*clock_t toc = clock(); */
    /*printf("Query3: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC); */

    return res;
}

/** Query4:
 * Devolve um array de apontadores com os códigos de produtos nunca comprados, resultado global ou filial a filial 
 */
GPtrArray* getProductsNeverBougth(SGV sgv, int branchID){
    /*clock_t tic = clock();*/
    int i;
    GPtrArray* res = g_ptr_array_new();

    char filial[BUFFER];
    char size[BUFFER];

    Produtos produtos_clone;

    if(branchID==0){
        produtos_clone = getProdutosNaoComprados(sgv);

        g_ptr_array_add(res, strdup("Nas 3 filiais: "));
        g_ptr_array_extend_and_steal(res, printProdutos(produtos_clone));

        sprintf(size, "Número de produtos não comprados--> %d", nnodos(produtos_clone));
        g_ptr_array_add(res, strdup(size));
    }else{
        Faturacao fat_clone = getFaturacao(sgv);

        for(i=0; i<FILIAIS; i++){
            produtos_clone = getProdutos(sgv);

            sprintf(filial, "Filial %d", i+1);
            g_ptr_array_add(res, strdup(filial));

            CatalogoArray cat_array = productsNeverBougth(produtos_clone, fat_clone, i);
            g_ptr_array_extend_and_steal(res, getGPtrArray(cat_array));

            sprintf(size, "Número de produtos não comprados--> %d", getNumero(cat_array));
            g_ptr_array_add(res, strdup(size));

            free(cat_array);
        }

        free(fat_clone);
    }
    freeProdutos(produtos_clone);

    /*clock_t toc = clock(); */
    /*printf("Query4: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC); */
    return res;
}

/** Query5:
 * Função que devolve um array de apontadores com os códigos dos clientes que compraram em todas as filiais.
 */
GPtrArray* getClientsOfAllBranches(SGV sgv){
    /*clock_t tic = clock();*/

    Clientes clientes_clone = getClientes(sgv);

    CatalogosArray cats_array = get_clients_branches(clientes_clone, getFiliais(sgv), 0);
    GPtrArray* res = getGPtrArray2(cats_array); 

    free(cats_array);
    free(clientes_clone);

    /*clock_t toc = clock(); */
    /*printf("Query5: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC); */
    
    return res;
}

/** Query6:
 * Função que devolve um array com dois inteiros: um que corresponde ao número de clientes que nunca efetuaram uma compra e o outro ao número de produtos nunca comprados
 */
int* getClientsAndProductsNeverBougth(SGV sgv){
    /*clock_t tic = clock();*/
    int* numbers = malloc(2 * sizeof(int));

    Clientes clientes_clone = getClientes(sgv);
    Produtos produtos_clone = getProdutosNaoComprados(sgv);

    numbers[0] = getNumero2(get_clients_branches(clientes_clone, getFiliais(sgv), 1));
    numbers[1] = nnodos(produtos_clone);

    freeClientes(clientes_clone);

    /*clock_t toc = clock(); */
    /*printf("Query6: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC); */

    return numbers;
}


/** Query7:
 * Função que devolve uma matriz que contem o número de produtos comprados por um cliente.
 * Cada linha corresponde a uma filial e cada coluna a um mês.
 */
int** getProductsBougthByClient(SGV sgv, char* clientID){
    /*clock_t tic = clock();*/
    int** result = NULL;
    
    if(existCodigoCliente(sgv->clientes, clientID)) result = client_table(getFiliais(sgv), clientID);

    /*clock_t toc = clock(); */
    /*printf("Query7: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC); */

    return result;
}

/** Query8:
 * Devolve uma estrutura com informação relativa ao número de registos de venda e total faturado num intervalo de meses.
 */
Nr_total getSalesAndProfit(SGV sgv, int minMonth, int maxMonth){
    /*clock_t tic = clock();*/
    Nr_total nrTotal = initNrTotal(minMonth, maxMonth);
    
    nr_vendas_and_total(getFaturacao(sgv), nrTotal);

    /*clock_t toc = clock(); */
    /*printf("Query8: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC); */

    return nrTotal;
}

/** Query9:
 * Função que devolve uma estrutura com informação relativamente aos códigos (e número de códigos) de um produto numa determinada filial, distiguindo entre o modo Normal e Promoção
 */
ArrayNumero getProductsBuyers(SGV sgv, char* productID, int branchID){
    /*clock_t tic = clock();*/
    ArrayNumero array_nr = NULL;

    if(existCodigoProduto(sgv->produtos, productID)){
        array_nr = initArrayNumero(productID);
        
        product_buyers(getFilial(sgv, branchID-1), array_nr);
    }

    /*clock_t toc = clock(); */
    /*printf("Query9: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC); */

    return array_nr;
}

/** Query10:
 * Dado um código de cliente e um mês, determina a lista de códigos de produtos que mais comprou por quantidade e não por facturação), por ordem descendente
 */
GPtrArray* getClientFavoriteProducts(SGV sgv, char* clientID, int month){
    /*clock_t tic = clock();*/
    GPtrArray* array = NULL;
    
    if(existCodigoCliente(sgv->clientes, clientID)){
        array = products_by_cliente(getFiliais(sgv), clientID, month-1);
    }

    /*clock_t toc = clock(); */
    /*printf("Query10: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC); */
 
    return array;
}

/** Query11:
 *  Cria uma lista dos limit produtos mais vendidos em todo o ano, indicando o número total de clientes e o número de unidades vendidas, filial a filial;
 */
GPtrArray* getTopSelledProducts(SGV sgv, int limit){
    /*clock_t tic = clock();*/
    int i;
    char info[BUFFER];
    GPtrArray* array = g_ptr_array_new();

    for(i=0; i<FILIAIS; i++){
        sprintf(info, "\nNa filial %d\nCódigo do Produto\tUnidades Vendidas\tNúmero de Clientes\n" , i+1);
        g_ptr_array_add(array, strdup(info));

        g_ptr_array_extend_and_steal(array, top_products(getFilial(sgv, i), limit));
    }

    /*clock_t toc = clock(); */
    /*printf("Query11: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC); */
 
    return array;
}


/** Query12:
 *  Dado um código de cliente determinar quais os códigos dos N produtos em que mais gastou dinheiro durante o ano;
 */
GPtrArray* getClientTopProfitProducts(SGV sgv, char* clientID, int limit){
    /*clock_t tic = clock();*/
    GPtrArray* array = NULL;

    if(existCodigoCliente(sgv->clientes, clientID))  array = top_profit_products(getFiliais(sgv), clientID, limit);

    /*clock_t toc = clock(); */
    /*printf("Query12: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC); */
 
    return array;
}