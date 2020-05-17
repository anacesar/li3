#include "../include/faturacao.h"

#include <stdio.h>

#define MES 12
#define FILIAIS 3
#define SIZE 4
#define BUFFER 200
#define PROFUNDIDADE 2


/*Estrutura Valores: contêm informação relativa a um produto numa determinada filial
* Contêm informação relativa ao número de vendas desse produto nessa filial, assim como o total faturado
* Esta informação está organizada por meses e dividida em modos de venda (Normal ou Promoção)
*/
struct valores{
    int nVendasN[MES];
    double totalFatN[MES];
    int nVendasP[MES];
    double totalFatP[MES];
};

/*
* Estrutura Faturação: array de catálogos de produtos (tamanho do array é igual ao número de filiais). 
* Para cada filial corresponde um catalogo com os produtos vendidos nesse filial.
* A cada produto está associado a uma estrutura de Valores.
*/
struct fat{
    Catalogo filial[FILIAIS];
};

/*Estrutura CatalogoLArray:
* é constituída por um catálogo e por um apontador para um array de apontadores da biblioteca glib(GPtrArray) e um inteiro 
*/
struct catalogo_array{
    Catalogo catalogo;
    GPtrArray* array;
    int numero;
};



/*Função responsável por alocar memória necessária à estrutura Faturacao*/
Faturacao initFaturacao(){
    int i;
    Faturacao fat = (Faturacao)malloc(sizeof(struct fat));

    for(i=0; i<FILIAIS; i++)
            fat->filial[i] = initCatalogo(PROFUNDIDADE);
    return fat;
}


/*Função responsável por libertar a memória alocada para a estrutura Faturacao*/
void freeFaturacao(Faturacao fat){
    int i;
    for(i=0; i<FILIAIS; i++)
        freeCatalogo(fat->filial[i]);

    free(fat);
}

/*Função responsável por alocar memória necessária à estrutura Valores, inicializada a zero */
Valores initValores(){
    Valores val = (Valores)malloc(sizeof(struct valores));
    int i;
    for(i=0; i<MES; i++){
        val->nVendasN[i] = 0;
        val->totalFatN[i] = 0;
        val->nVendasP[i] = 0;
        val->totalFatP[i] = 0;
    }
    return val;
}

/*
* Função responsável por alocar memória necessária à estrutura Valores
* É também responsável por inicializar corretamente a estrutura com a informação relativa ao primeiro registo de venda de um determinado 
* produto num determinado mes e filial 
*/
Valores initValores2(double total, int mes, char tipo){
    Valores val = (Valores)malloc(sizeof(struct valores));
    int i;
    for(i=0; i<MES; i++){
        if(i==mes){
            if(tipo == 'N'){
                val->nVendasN[i] = 1;
                val->totalFatN[i] = total;
                val->nVendasP[i] = 0;
                val->totalFatP[i] = 0;
            }else{
                val->nVendasN[i] = 0;
                val->totalFatN[i] = 0;
                val->nVendasP[i] = 1;
                val->totalFatP[i] = total;
            }
        }else{
            val->nVendasN[i] = 0;
            val->totalFatN[i] = 0;
            val->nVendasP[i] = 0;
            val->totalFatP[i] = 0;
        }
        
    }

    return val;
}

/*Função responsável por atualizar os valores de um produto consoante uma nova venda desse mesmo produto nesse mes e nessa filial*/
Valores updateValores(Valores val, double total, int mes, char tipo){
    if(tipo == 'N'){
        val->nVendasN[mes]++;
        val->totalFatN[mes] += total;
    }else{
        val->nVendasP[mes]++;
        val->totalFatP[mes] += total;
    }
    return val;
}


/* Função que adiciona os valores de uma estrutura Valores a outra */
Valores updateValores2(Valores dest, Valores source, int mes){
    dest->nVendasN[mes] += source->nVendasN[mes];
    dest->totalFatN[mes] += source->totalFatN[mes];
    dest->nVendasP[mes] += source->nVendasP[mes];
    dest->totalFatP[mes] += source->totalFatP[mes];
    return source;
}



/* Função responsavel por colocar num array de apontadores a informação presente na estrutura Valores */
GPtrArray* printValores(Valores valores){
    GPtrArray* val = g_ptr_array_new();
    int i;
    int nVendasN = 0;
    int nVendasP = 0;
    double totalFatN = 0;
    double totalFatP = 0;

    char nvendas[BUFFER];
    char total[BUFFER];

    for(i=0; i<MES; i++){
        nVendasN += valores->nVendasN[i];
        nVendasP += valores->nVendasP[i];
        totalFatN += valores->totalFatN[i];
        totalFatP += valores->totalFatP[i];
    }

    sprintf(nvendas, "\nNº total de vendas em modo:\n*N--> %d\n*P--> %d", nVendasN, nVendasP); 
    sprintf(total, "Total faturado em modo:\n*N--> %.2f\n*P--> %.2f\n", totalFatN, totalFatP);

    g_ptr_array_add(val, strdup(nvendas));
    g_ptr_array_add(val, strdup(total));

    return val;
}

/* Função que retorna um novo apontador para a estrutura faturacao */
Faturacao cloneFaturacao(Faturacao fat){
    Faturacao fat_clone = malloc(sizeof(struct fat));
    fat_clone = fat;

    return fat_clone;
}

/* Função que adiciona uma venda à estrutura de faturação */
void addVendaFat(Venda venda, Faturacao fat){

    char* produto = strdup(getCodeProduct(venda));
    int mes = getMonthSale(venda) -1;
    int filial = getBranchSale(venda) -1; 
    float preco = getPrice(venda);
    int quant = getQuantity(venda);
    char tipo = getType(venda);
    double totalF = preco * quant;

    Valores valores = getValue(fat->filial[filial], produto);

    if(valores == NULL){
        valores = initValores2(totalF, mes, tipo);
        add(fat->filial[filial], produto, valores);
    }else{
        valores = updateValores(valores, totalF, mes, tipo);
        replaceValue(fat->filial[filial], produto, valores);
    }
}


/*Funcoes de resposta às queries*/

/*
* Função responsável por colocar num array de Valores os valores, relativos a um determinado código de produto e mes(passados como parâmetro)
* O tamanho do array é igual ao número de filiais mais um. Cada posição corresponde aos valores deste produto neste mês numa das filiais e a última posição corrsponde aos valores globais
*/
void valoresProdutoMes(Faturacao fat, char* produto, int mes, Valores valores[]){
    int i;

    for(i=0; i<SIZE; i++)
        valores[i] = initValores();

    for(i=0; i<FILIAIS; i++){
        Valores val = getValue(fat->filial[i], produto);
        if(!(val == NULL)){
            updateValores2(valores[i], val, mes);
            updateValores2(valores[SIZE-1], val, mes);
        }
    }
}

/*Função responsável por guardar todas as keys de uma gtree, num array de apontadores, que nao existam num determinado catálogo*/
gboolean lookCodeNO(gpointer key, gpointer value, gpointer data){
    char* codigo = key;
    CatalogoArray cat_array = data;
    
    if(! existCodigo(cat_array->catalogo, codigo)) {
        g_ptr_array_add(cat_array->array, codigo);
        cat_array->numero++;
    }
    return FALSE;
}

/*função responável por percorrer um catalogo e guardar todos os seus códigos que não constem num outro catalogo presente na estrutura CatalogoArray*/
CatalogoArray productsNeverBougth(Produtos produtos, Faturacao fat, int i){
    CatalogoArray cat_array = malloc(sizeof(struct catalogo_array));
    cat_array->catalogo = fat->filial[i];
    cat_array->array = g_ptr_array_new();
    cat_array->numero = 0;

    forEach(getCatalogoProd(produtos), lookCodeNO, cat_array);

    return cat_array;
} 

/*Função responsável por retornar o apontador para o GPtrArray presente na estrutura CatalogoArray*/
GPtrArray* getGPtrArray(CatalogoArray cat_array){
    return cat_array->array;
}

/*Função responsável por retornar o inteiro presente na estrutura CatalogoArray*/
int getNumero(CatalogoArray cat_array){
    return cat_array->numero;
}

/*Função auxiliar à travessia do catálogo da faturação: 
* Para cada produto presente no catálogo adiciona a uma estrutua auxiliar passada como parametro, os valores guardados na estrutura Valores desse produto 
*/
gboolean get_nr_total(gpointer key, gpointer value, gpointer data){
    Valores valores = value;
    Nr_total nrTotal = data;
    int i= get_mes(nrTotal);
    int maxM = get_Mes(nrTotal);

    for( ; i<maxM ; i++){
        int nVendas = valores->nVendasN[i] + valores->nVendasP[i];
        set_numero(nrTotal, nVendas);
        double tot = valores->totalFatN[i] + valores->totalFatP[i];
        set_total(nrTotal, tot);
    }

    return FALSE;
}

/*Função auxiliar à query 8:
* é responsável por calcular o número de vendas e o total faturado num intervalo de tempo [minMonth, maxMonth] 
* os valores alvo sâo passados como parâmetros à função e atualizados assim que calculados 
*/
void nr_vendas_and_total(Faturacao fat, Nr_total nrTotal){
    int i;
    for(i=0; i<FILIAIS; i++){
        forEach(fat->filial[i], get_nr_total, nrTotal);
    }

}


