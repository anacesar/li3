#include "../include/resultados.h"


/*Estrutura Nr_total:
* é constituída por um i
*/
struct nr_total{
    int numero;
    double total;
    int mes;
    int Mes;
};

/*Estrutura ArrayNumero: estrutura auxiliar criada com o intuito de dar resposta à querie 9
* é constituída por dois arrays de apontadores, por dois inteiros e ainda uma string 
*/
struct array_numero{
    GPtrArray* array_n;
    GPtrArray* array_p;
    int numero_n;
    int numero_p;
    char* product;
};


/* Funções relativas à estrutura Nr_total*/

/*Aloca a memória necessária à estrutura Nr_total, inicializando as variáveis a zero*/
Nr_total initNrTotal(int minMonth, int maxMonth){
    Nr_total nrTotal = malloc(sizeof(struct nr_total));

    nrTotal->numero = 0;
    nrTotal->total = 0;
    nrTotal->mes = minMonth -1;
    nrTotal->Mes = maxMonth;

    return nrTotal;
}

/* Retorna o campo numero de uma estrutura Nr_total */
int get_numero(Nr_total nrTotal){
    return nrTotal->numero;
}

/* Adiciona um número passado como argumento ao número de uma estrutura Nr_total */
void set_numero(Nr_total nrTotal, int nr){
    nrTotal->numero += nr;
}

/* Retorna o campo total de uma estrutura Nr_total */
double get_total(Nr_total nrTotal){
    return nrTotal->total;
}

/* Adiciona um double passado como argumento ao total de uma estrutura Nr_total */
void set_total(Nr_total nrTotal, double tot){
    nrTotal->total += tot;
}

/* Retorna o campo mes de uma estrutura Nr_total */
int get_mes(Nr_total nrTotal){
    return nrTotal->mes;
}

/* Coloca um inteiro no campo mes de uma estrutura Nr_total */
void set_mes(Nr_total nrTotal, int mes){
    nrTotal->mes = mes;
}

/* Retorna o campo Mes de uma estrutura Nr_total */
int get_Mes(Nr_total nrTotal){
    return nrTotal->Mes;
}

/* Coloca um inteiro no campo Mes de uma estrutura Nr_total */
void set_Mes(Nr_total nrTotal, int Mes){
    nrTotal->Mes = Mes;
}


/*Funções relativas à estrutura ArrayNumero*/

/*Aloca a memória necessária à estrutura ArrayNumero, inicializando as variáveis inteiras a zero*/
ArrayNumero initArrayNumero(char* productID){
    ArrayNumero array_nr = malloc(sizeof(struct array_numero));
    array_nr->array_n = g_ptr_array_new();
    array_nr->array_p = g_ptr_array_new();
    array_nr->numero_n = 0;
    array_nr->numero_p = 0;
    array_nr->product = strdup(productID);

    return array_nr;
}

/*Liberta a memória alocada à estrutura ArrayNumero */
void freeArrayNumero(ArrayNumero array_nr){
    g_ptr_array_free(array_nr->array_n, TRUE);
    g_ptr_array_free(array_nr->array_p, TRUE);
    free(array_nr->product);
    free(array_nr);
}

/* Retorna o array de apontadores do campo array_n de uma estrutura ArrayNumero */
GPtrArray* getPtrArrayN(ArrayNumero array_nr){
    return array_nr->array_n;
}

/* Retorna o array de apontadores do campo array_p de uma estrutura ArrayNumero */
GPtrArray* getPtrArrayP(ArrayNumero array_nr){
    return array_nr->array_p;
}

/* Retorna o campo numero_n de uma estrutura ArrayNumero */
int getNumeroN(ArrayNumero array_nr){
    return array_nr->numero_n;
}

/* Função que incrementa o campo numero_n de uma estrutura ArrayNumero */
void incNumeroN(ArrayNumero array_nr){
    array_nr->numero_n++;
}

/* Retorna o campo numero_p de uma estrutura ArrayNumero */
int getNumeroP(ArrayNumero array_nr){
    return array_nr->numero_p;
}

/* Função que incrementa o campo numero_p de uma estrutura ArrayNumero */
void incNumeroP(ArrayNumero array_nr){
    array_nr->numero_p++;
}

/* Retorna o campo product de uma estrutura ArrayNumero */
char* getProductA(ArrayNumero array_nr){
    return array_nr->product;
}