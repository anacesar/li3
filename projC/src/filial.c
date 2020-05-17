#include "../include/filial.h"


#include <stdio.h>

#define MES 12
#define MODOS 2
#define FILIAIS 3
#define BUFFER 200


/*Estrutura auxiliar à estrutura Filial: 
* Para cada produto comprado por um determinado cliente é guardado :
* o total gasto por esse cliente nesse determinado produto, a quantidade comprada desse produto em cada mes do ano 
* e um array utilizado como flag para saber se esse produto foi comprado por esse cliente em modo Normal e/ou em modo Promoção 
*/
struct info{
    double totalGasto; 
    int quantidade[MES]; 
    int modo[MODOS]; /*modo[0] PARA MODO N, modo[1] para modo P :  usamos 1 se ja tiverem sido efetuadas compras desse produto por esse cliente nesse modo, 0 caso contrário */
};

/*Estrutura Filial: corresponde a um catálogo de clientes
* e que a cada código de cliente está associado uma árvore balanceada (GTree*) que contém os códigos de todos os produtos compados por ele mesmo 
* a cada produto está associada informação relevante na estrututa auxiliar Info
*/
struct filial{
    Catalogo clientes;
};



/*Estrutura CatalogosArray: estrutura auxiliar à resposta da query 5
* é constituída por um array de catálogos e por um apontador para um array de apontadores da biblioteca glib(GPtrArray)
* Contêm ainda um inteiro que nos indica o número de catálogos presentes na estrutura e um número que nos indica quantos elementos contém o array de apontadores.
*/
struct catalogos_array{
    Catalogo* catalogos;
    GPtrArray* array;
    int nr_catalogos;
    int numero;
};


/*Estrutura TreeArray: estrutura auxiliar à resposta das queries 10,11 e 12 */
struct tree_array{
    GTree* tree_source;
    GTree* tree_dest;
    GPtrArray* array;
    int numero;
    int nr_clients; 
    int printed;
    int quantidade;
    double total;
};

/*Estrutura NrQuantidade: estrutura auxiliar à query 11
* é usada para guardar informação relativamente a um produto numa GTree: neste caso, o número de clientes que o compraram e a quantidade comprada
*/
struct nr_quant{
    int nr_clientes;
    int quantidade;
};


/*Funcão responsável por alocar a memória necessária à estrutura auxiliar Info.
* É também responsável por inicializar corretamente a estrutura com a informação relativa ao primeiro registo de venda de um determinado 
* produto para um determinado cliente
*/
Info initInfo(int quantidade, int mes, double total, int tipo){
    int i;
    Info info = malloc(sizeof(struct info));
    for(i=0; i<MES; i++) info->quantidade[i] = 0;
    for(i=0; i<MODOS; i++) info->modo[i] = 0;
    info->totalGasto = total;
    info->quantidade[mes] = quantidade;
    info->modo[tipo]  = 1;
    return info;
}

/*Funcão responsável por libertar a memória alocada para a estrutura Info*/
gboolean freeInfo(gpointer key, gpointer value, gpointer data){
    Info info = value;
    free(info->quantidade);
    free(info->modo);

    return FALSE;
}

/*Funcão responsável por alocar a memória necessária à estrutura Filial*/
Filial initFilial(){
    Filial filial = malloc(sizeof(struct filial));
    filial->clientes = initCatalogo(1);
    return filial;
}


/*Função auxiliar à função responsável por libertar */
gboolean freeGTree(gpointer key, gpointer value, gpointer data){
    GTree* produtos = value;

    g_tree_foreach(produtos, freeInfo, NULL);

    g_tree_destroy(produtos);

    return FALSE;
}

/*Funcão responsável por libertar a memória alocada para a estrutura Filial*/
void freeFilial(Filial filial){
    forEach(filial->clientes, freeGTree, NULL);
    freeCatalogo(filial->clientes);
    free(filial);
}

/*Função responsável por atualizar os valores de um produto consoante uma nova venda dele mesmo para um determinado cliente*/
void updateInfo(Info info, int quantidade, int mes, double total, int tipo){
    info->totalGasto += total;
    info->quantidade[mes] += quantidade;
    info->modo[tipo] = 1;
}

/*Função que adiciona um código de cliente ao catalogo de clientes da estrutura Filial*/
void addClienteFilial(Filial filial, char* cliente){
    GTree* produtos = g_tree_new(compareCode);
    add(filial->clientes, cliente, produtos);
}

/*Função que adiciona um código de cliente ao catalogo de clientes da estrutura Filial*/
void addClienteFiliais(Filial* filial, char* cliente){
    int i;
    for(i=0; i<FILIAIS; i++)
        add(filial[i]->clientes, cliente, NULL);
}

/*Função que retorna um novo apontador de uma estrutura filial*/
Filial cloneFilial(Filial filial){
    Filial filial_clone = malloc(sizeof(filial));
    filial_clone = filial;

    return filial_clone;
}

/*Função que retorna um array de novos apontadores para as estruturas das filiais*/
Filial* cloneFiliais(Filial* filiais){
    int i;
    Filial* filiais_clone = malloc(FILIAIS * sizeof(Filial));

    for(i=0; i<FILIAIS; i++)
        filiais_clone[i] = cloneFilial(filiais[i]);

    return filiais_clone;
}

/*Função responsável por adicionar a informação relevante de uma venda à respetiva filial*/
void addVendaFilial(Venda venda, Filial filial){
    char* cliente = strdup(getCodeClient(venda));
    char* produto = strdup(getCodeProduct(venda));
    int mes = getMonthSale(venda) -1;
    float preco = getPrice(venda);
    int quant = getQuantity(venda);
    char type = getType(venda);
    double totalF = preco * quant;
    int tipo;

    if(type=='N') tipo = 0;
    else tipo = 1;

    GTree* produtos = getValue(filial->clientes, cliente);
    
    if(produtos == NULL){
        produtos = g_tree_new(compareCode);
        Info info = initInfo(quant, mes, totalF,  tipo);
        g_tree_insert(produtos, produto, info);
        add(filial->clientes, cliente, produtos);
    }else{
        Info info = g_tree_lookup(produtos, produto);

        if(info==NULL){
            info = initInfo(quant, mes, totalF,  tipo);
            g_tree_insert(produtos, produto, info);
        }else{ 
            updateInfo(info, quant, mes, totalF, tipo);
            g_tree_replace(produtos, produto, info);
        }
    }
   
}


/*  Funções auxiliares à resposta das queries   */


/*Função responsável por guardar todas as keys de uma gtree, num array de apontadores, que existem em todos os catálogos da estrutura CatalogosArray*/
gboolean lookCodeALL(gpointer key, gpointer value, gpointer data){
    char* codigo = key;
    CatalogosArray cats_array = data;
    int exist = 1;
    int i;

    for(i=0; i<cats_array->nr_catalogos && exist; i++) 
        exist = existCodigo(cats_array->catalogos[i], codigo);

    if(exist) g_ptr_array_add(cats_array->array, codigo);

    return FALSE;
}

/*Função responsável por contar os de codigos uma gtree que nao estejam presentes em nenhum dos nr_catalogos catalogos*/
gboolean lookCodeNON(gpointer key, gpointer value, gpointer data){
    CatalogosArray cats_array = data;
    int exist = 0;
    int i;

    for(i=0; i<cats_array->nr_catalogos && !exist; i++) 
        exist = existCodigo(cats_array->catalogos[i], key);

    if(! exist) cats_array->numero++;

    return FALSE;
}

/*
* Função responável por percorrer um catalogo e comparar todos os códigos com os "size" catalogos passados como parametro 
* A flag da função serve para saber que tipo de criterio de comparação temos que usar 
*/
CatalogosArray clients_branches(Catalogo base, Catalogo* procura, int size, int flag){
    CatalogosArray cats_array = malloc(sizeof(struct catalogos_array));
    cats_array->catalogos = procura;
    cats_array->array = g_ptr_array_new();
    cats_array->nr_catalogos = size;

    if(flag == 0) forEach(base, lookCodeALL, cats_array);
    else  forEach(base, lookCodeNON, cats_array);

    return cats_array;
} 

/**
 * Cria um array de Catalogos, com tamanho igual ao numero de filiais, que correspondem aos catálogos de cada uma das filiais do sistema 
 * Retorna uma estrutura CatalogosArray que contem 
 */
CatalogosArray get_clients_branches(Clientes clientes, Filial* filiais, int flag){
    int i;
    Catalogo catalogos_filiais[FILIAIS];
    for(i=0; i<FILIAIS; i++) catalogos_filiais[i] = filiais[i]->clientes;

    return clients_branches(getCatalogoCli(clientes), catalogos_filiais, FILIAIS, flag);
}

/* Função que devolve o array de apontadores(GPtrArray) de uma estrutura CatalogosArray */
GPtrArray* getGPtrArray2(CatalogosArray cats_array){
    return cats_array->array;
}

/* Função que devolve o número de uma estrutura CatalogosArray */
int getNumero2(CatalogosArray cats_array){
    return cats_array->numero;
}


/*          Funções auxiliares à query 7        */

/**
 * Função de travessia da árvore de produtos associada a um cliente, responsável por atualizar os valores das colunas de uma determinada linha da matriz alvo.
 * Estes valores são atualizados por os valores presentes na estrutura Info que cada produto tem associado como value.
 */
gboolean add_quantidades(gpointer key, gpointer value, gpointer data){
    Info info = value;
    int* array = data;
    int i;

    for(i=0; i<MES; i++) array[i] += info->quantidade[i];

    return FALSE;
}

/**
 * Função auxiliar à função client_table que percorre cada filial e para um determinado cliente, atualiza os valores da linha i(correspondente a essa filial) da matriz.
 * Para isso percorre todos os produtos associados a esse cliente nessa mesma filial.
 */
void get_quantidades(Filial filial, char* cliente, int** table, int i){
    int j;
    GTree* produtos = getValue(filial->clientes, cliente);

    for(j=0; j<MES; j++) table[i][j] = 0;

    if(!(produtos == NULL)) g_tree_foreach(produtos, add_quantidades, table[i]);
    
}

/* Função que devolve uma matriz de inteiros (FILIAIS x MES) que contem as quantidades compradas por um determinado cliente em cada filial num respetivo mes */
int** client_table(Filial* filiais, char* client){
    int i;

    int** table = (int**)malloc(FILIAIS * sizeof(int *));
    for(i=0; i<FILIAIS; i++) table[i] = (int*)malloc(MES * sizeof(int));

    for(i=0; i<FILIAIS; i++) get_quantidades(filiais[i], client, table, i);
    
    return table;
}

/* Função que liberta a memória alocada para criar uma matriz de inteiros*/
void freeTable(int** table){
    int i;
    for(i=0; i<FILIAIS; i++) free(table[i]);

    free(table);
}

/*          Funções auxiliares à query 9        */

/**
 * lookProductCode:
 * @key: código de cliente guardado na Gtree
 * @value: Gtree* associada a cada cliente que contem todos os códigos de produtos que comprou
 * @data: estrutura ArrayNumero que vai guardar a informação presente nas estruturas Info guardadas na Gtree de produtos(value)
 * 
 * Returns: um boolean FALSE que permite fazer a travessia completa da Gtree
 */
gboolean lookProductCode(gpointer key, gpointer value, gpointer data){
    char* client = key;
    GTree* produtos = value;
    ArrayNumero array_nr = data;

    Info info = g_tree_lookup(produtos, getProductA(array_nr));

    if(!(info == NULL)){
        if(info->modo[0] == 1){ /*siginifica que houveram compras no modo N*/
            g_ptr_array_add(getPtrArrayN(array_nr), strdup(client));
            incNumeroN(array_nr);
        }
        if(info->modo[1] == 1){  /*siginifica que houveram compras no modo P*/
            g_ptr_array_add(getPtrArrayP(array_nr), strdup(client));
            incNumeroP(array_nr);
        }
    }

    return FALSE;
}

/* Função que atualiza os valores de uma estrtura ArrayNumero para uma determinada filial */
void product_buyers(Filial filial, ArrayNumero array_nr){
    forEach(filial->clientes, lookProductCode, array_nr);
}



/*          Funções auxiliares à query 10        */


/*Função auxiliar que garante a inserção por ordem decrescente de inteiros nas árvores balanceadas*/
gint compareInt(gconstpointer int1, gconstpointer int2){
    return GPOINTER_TO_INT(int2) - GPOINTER_TO_INT(int1);
}

/**
 * Função de travessia de uma gtree, cujas keys são todos os códigos de produtos que foram vendidos numa determinada quantidade
 * Os códigos dos produtos e as respetivas quantidades são guardados num array de apontadores
 */
gboolean get_product(gpointer key, gpointer value, gpointer data){
    char info[100];
    char* produto = key;
    TreeArray tree_array = data;

    sprintf(info, "%s\t\t\t%d", produto, tree_array->quantidade);
    g_ptr_array_add(tree_array->array, strdup(info));

    return FALSE;
}

/**
 * Função de travessia de uma gtree cujas keys são quantidades(inteiros), cujos value são uma outra gtree que contem todos os códigos de produtos que foram vendidos nessa quantidade.
 * Esta função garante a travessia desta segunda tree com objetivo de saber extamente todos os códigos dos produtos vendidos a uma mesma quantidade
 */
gboolean print_to_array(gpointer key, gpointer value, gpointer data){
    TreeArray tree_array = data;
    tree_array->quantidade = GPOINTER_TO_INT(key);
    
    g_tree_foreach(value, get_product, tree_array);

    return FALSE;
}

/**
 * Função que coloca um determinado produto e a sua quantidade vendida num determinado mes, numa nova árvore balanceada esse produto como key e a quantidade como value
 */
gboolean addProduct(gpointer key, gpointer value, gpointer data){
    Info info = value;
    TreeArray t_array = data;


    int info_quantidade = info->quantidade[t_array->numero];

    /*se nao tiver sido vendido nesse mes nao faz sentido adicionar */
    if(info_quantidade!=0){ 
        gpointer quantidade = g_tree_lookup(t_array->tree_source, key);

        if(quantidade == NULL) quantidade = GINT_TO_POINTER(info_quantidade);
        else info_quantidade += GPOINTER_TO_INT(quantidade);

        g_tree_insert(t_array->tree_source, strdup(key), quantidade);
    }

    return FALSE;
}

/*Função que para todos os códigos de produtos vendidos para um determinado cliente, lhes associa a quantidade vendida*/
void set_quantidades_to_products(Filial filial, char* client, TreeArray t_array){
    GTree* produtos = getValue(filial->clientes, client); 

    g_tree_foreach(produtos, addProduct, t_array);
}

/** Função de travessia da gtree que contem os codigos de produtos como key e como value a quantidade vendida desse produto
 *  Esta funçao recebe como argumento uma nova gtree* (data) cuja key é uma a quantidade vendida e cujo value é uma nova gtree que vai conter todos os codigos de produtos 
 * que foram vendidos nessa mesma quantidade.
 * O critério de inserção desta árvore garante que as quantidades são inseridas por ordem decrescente.
 */
gboolean organize_by_quant(gpointer key, gpointer value, gpointer data){
    GTree* tree_dest = data;

    /* Verificar se ja existe uma key com a quantidade value*/
    GTree* quant_tree = g_tree_lookup(tree_dest, value); 

    /* Se nao existir criar gtree onde vamos colocar os produtos dessa quantidade*/
    if(quant_tree == NULL){ 
        quant_tree = g_tree_new(compareCode); 
        g_tree_insert(tree_dest, value, quant_tree);
    }
    
    /*Inserir código do produto na Gtree dessa quantidade*/
    g_tree_insert(quant_tree, key, NULL);

    return FALSE;
}


/** Função que devolve um array de apontadores com os códigos de produtos e as quantidades vendidas desse produto, ordenados por ordem decrescente de quantidade */
GPtrArray* products_by_cliente(Filial* filiais, char* clienteID, int month){
    int i;

    TreeArray t_array = malloc(sizeof(struct tree_array));
    t_array->tree_source = g_tree_new(compareCode);
    t_array->tree_dest = g_tree_new(compareInt);
    t_array->array = g_ptr_array_new();
    t_array->numero = month;
    t_array->quantidade = 0;

    for(i=0; i<FILIAIS; i++){
        set_quantidades_to_products(filiais[i], clienteID, t_array);
    }

    g_tree_foreach(t_array->tree_source, organize_by_quant, t_array->tree_dest);

    g_tree_foreach(t_array->tree_dest, print_to_array, t_array);

    return  t_array->array;
}

/*-------------------------QUERY 11--------------------------------------------------------------------------------------------*/

/**Função de travessia de uma gtrre responsável por associar a um codigo de produto,
 *  a quantidade vendida desse produto numa filial assim como o número total de clientes que o compraram
 */
gboolean addProductAndQuantity(gpointer key, gpointer value, gpointer data){
    int i, quant = 0;
    Info info = value;
    TreeArray t_array = data;

    for(i=0; i<MES;i++)
        quant+= info->quantidade[i];
    
    NrQuantidade nr_quant = g_tree_lookup(t_array->tree_source, key);

    if(nr_quant == NULL){
        nr_quant = malloc (sizeof(struct nr_quant));
        nr_quant->quantidade = quant;
        nr_quant->nr_clientes = 1;
    }else{
        nr_quant->quantidade += quant;
        nr_quant->nr_clientes++;
    }

    g_tree_insert(t_array->tree_source, key, nr_quant);
    return FALSE;
}

/* Função de travessia de uma gtree que percorre os codigos de produtos de cada um dos clientes presentes numa filial*/
gboolean top_products_Traverse(gpointer key, gpointer value, gpointer data){

    g_tree_foreach(value, addProductAndQuantity, data);

    return FALSE;
}


/** Função de travessia da gtree que contem os codigos de produtos como key e como value uma estrutura NrQuantidade que contem a quantidade vendida desse produto e o número de clientes que o compraram
 *  Esta funçao recebe como argumento uma nova gtree* (data) cuja key é a quantidade vendida e cujo value é uma nova gtree que vai conter todos os codigos de produtos 
 * que foram vendidos nessa mesma quantidade e o value desta nova gtree é o número de clientes que o compraram.
 * O critério de inserção desta árvore garante que as quantidades são inseridas por ordem decrescente.
 */
 gboolean organize_by_quant2(gpointer key, gpointer value, gpointer data){
    NrQuantidade nr_quant = value;
    GTree* tree_dest = data;

    gpointer quantidade = GINT_TO_POINTER(nr_quant->quantidade);
     /*verificar se ja existia uma key com a quantidade desse produto */
    GTree* quant_tree = g_tree_lookup(tree_dest, quantidade);

    /*se nao existir criar gtree onde vamos colocar os produtos dessa quantidade*/
    if(quant_tree == NULL){ 
        quant_tree = g_tree_new(compareCode); 
        g_tree_insert(tree_dest, quantidade, quant_tree);
    }
    
    g_tree_insert(quant_tree, key, GINT_TO_POINTER(nr_quant->nr_clientes));
    
    return FALSE;
}

/**
 *  Função de travessia de uma gtree cuja key é um código de produto e o value é o numero de clientes que o comprou
 *  Temos ainda uma estrutura auxiliar TreeArray passada como parametro que contem o array de apontadores cuja informação necessária à resposta da querie
 * (codigo de produto - quantidade - nr_clientes) é inserida. A quantidade comprada desse produto está ainda contida no campo quantidade dessa mesma estrutura
 */
gboolean get_product_and_nr(gpointer key, gpointer value, gpointer data){
    char info[500];
    TreeArray tree_array = data;

    gboolean leave = TRUE;

    if(tree_array->printed < tree_array->numero){
        sprintf(info, "%s\t\t\t%d\t\t\t%d", key, tree_array->quantidade, GPOINTER_TO_INT(value));
        g_ptr_array_add(tree_array->array, strdup(info));

        tree_array->printed++;

        leave = FALSE;
    }
    
    return leave;
}

/** Função responsável por guardar num array de apontadores os códigos de produtos, a quantidade vendida e o número de clientes que o compraram
 * Para isso percorre uma gtree ordenada por quantidades cujo value é uma outra gtree que contem todos os codigos vendidos nessa quantidade.
 * Nesta gtree de códigos a cada código está associado um value que nos indica o número de clientes que o comprou
 */
gboolean print_to_array2(gpointer key, gpointer value, gpointer data){
    TreeArray tree_array = data;
    tree_array->quantidade = GPOINTER_TO_INT(key);
    
    gboolean leave = TRUE;

    if(tree_array->printed < tree_array->numero){
        g_tree_foreach(value, get_product_and_nr, tree_array);

        leave = FALSE;
    }

    return leave;
}

/* Função que devolve um array de apontadores que contem um limit codigos ordenados por ordem decrescente da quantidade que foi vendido */
GPtrArray* top_products(Filial filial, int limit){
    TreeArray t_array = malloc(sizeof(struct tree_array));
    t_array->tree_source = g_tree_new(compareCode);
    t_array->tree_dest = g_tree_new(compareInt);
    t_array->array = g_ptr_array_new();
    t_array->numero = limit;
    t_array->printed = 0;    

    forEach(filial->clientes, top_products_Traverse , t_array);

    g_tree_foreach(t_array->tree_source, organize_by_quant2, t_array->tree_dest);

    g_tree_foreach(t_array->tree_dest, print_to_array2, t_array);

    return t_array->array;
}

/*-----------------------Query 12--------------------------------------------*/

/*Função auxiliar que garante a inserção por ordem decrescente de valores do tipo double nas árvores balanceadas*/
gint compareDouble(gconstpointer d1, gconstpointer d2){
    double a = *((double*) d1);
    double b = *((double*) d2);
    return (b-a);
}

/**
 * Coloca num array de apontadores todos os códigos de produto que foram vendidos a uma determinada quantidade, e essa mesma quantidade
 */
gboolean get_products(gpointer key, gpointer value, gpointer data){
    char* produto = key;
    char info[BUFFER]; 
    TreeArray tree_array = data;

    gboolean leave = TRUE;

    if(tree_array->printed < tree_array->numero){

        sprintf(info, "%s\t\t\t%.2f", produto, tree_array->total);
        g_ptr_array_add(tree_array->array, strdup(info));

        tree_array->printed++;

        leave = FALSE;
    }
    return leave;
}

/*  Converte a informação de uma árvore balanceada num array de apontadores, com um tamanho limite ambos contidos na estrura TreeArray passada como parâmetro */
gboolean print_totals(gpointer key, gpointer value, gpointer data){
    TreeArray tree_array = data;
    tree_array->total = *((double*) key);

    gboolean leave = TRUE;

    if(tree_array->printed < tree_array->numero){
        g_tree_foreach(value, get_products, tree_array);
        leave = FALSE;
    }
    return leave;
}

/**
 * Função de travessia de uma gtree que contem todos os produtos(key) comprados por um determinado cliente, e o total gasto associado(value).
 * Esta função organiza uma outra gtree* passada como parâmetro(data) de acordo com os totais de cada produto.
 */
gboolean organize_by_total(gpointer key, gpointer value, gpointer data){
    /* verificar se ja existia uma key com o total */
    GTree* total_tree = g_tree_lookup(data, value); 

    /* se nao existir criar gtree onde vamos colocar os produtos com esse total faturado */
    if(total_tree == NULL){ 
        total_tree = g_tree_new(compareCode); 
        g_tree_insert(data, value, total_tree);
    }
    
    g_tree_insert(total_tree, key, NULL);
    return FALSE;
}

/* Função que atribui a cada código de produto o total que o cliente gastou nesse mesmo, guardando o resultado numa gtree* */
gboolean set_totais_to_produtos(gpointer key, gpointer value, gpointer data){
    Info info = value;

    double* total = g_tree_lookup(data, key);

    if(total == NULL){
        total =  malloc(sizeof(double*));
        *total = info->totalGasto;
    } 
    else *total += info->totalGasto;

    g_tree_insert(data, key, total);

    return FALSE;
}

/**
 * Função que percorre todas as estruturas filiais, e para um determinado cliente, devolve um array de apontadores com o código dos limit produtos que mais gastou dinheiro em todo o ano.
 * Começa por percorrer cada filial do sistema de vendas e guarda numa árvore balanceada (gtree*) todos os códigos dos produtos que um determinado cliente comprou como chave, e o total gasto nesse produto como value
 * Posteriormente, cria uma nova gtree* cujo critério de ordenação garante a inserção por ordem decresente dos valores totais como key e todos os códigos de produtos com esse total faturado
 * Finalmente transfomra a informação desta última gtree num array de apontadores que é devolvido pela função
 * 
 * Todas estas estruturas que são necessárias à resposta da query, são guardadas numa estrutura auxiliar TreeArray.
 */
GPtrArray* top_profit_products(Filial* filiais, char* clientId, int limit){
    int i;
    TreeArray t_array = malloc(sizeof(struct tree_array));
    t_array->tree_source = g_tree_new(compareCode);
    t_array->tree_dest = g_tree_new(compareDouble);
    t_array->array = g_ptr_array_new();
    t_array->numero = limit;
    t_array->printed = 0; 
    t_array->total = 0;

    for(i=0; i<FILIAIS; i++){ 
        GTree* produtos = getValue(filiais[i]->clientes, clientId);
        g_tree_foreach(produtos, set_totais_to_produtos, t_array->tree_source);
    }
    
    g_tree_foreach(t_array->tree_source, organize_by_total ,t_array->tree_dest);

    g_tree_foreach(t_array->tree_dest, print_totals, t_array);

    /*dealocação de memória*/
    g_tree_destroy(t_array->tree_source);
    g_tree_destroy(t_array->tree_dest);
    free(t_array);

    return t_array->array;
}

