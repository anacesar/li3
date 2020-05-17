#include "../include/catalogo.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

#define BASE 26 /*são usados arrays de 26 posições face às 26 letras do alfabeto português */
#define ARRAYSIZE 4 /*tamanho usado para criar um array de catalogos que contem os codigos que nunca foram vendidos em cada uma das filias ou nas tres */
#define BUFFER 200 


/*Estrututa Catalogo: 
 * HashTable estática de apontadores para uma árvore balanceada da biblioteca Glib, 
 * o tamanho desta hashtable é definido por um tamanho base (26 posições, que correspondem às 26 letras do alfabeto) exponenciado a um inteiro 
 * contido na estrutura (profundiade) que pode tomar o valor 1 OU 2.
 * cada uma destas árvores contêm os códigos(produto ou cliente), ordenados alfabeticamente desse catálogo.
*/ 
struct catalogo{
    int profundidade; /*Clientes = 1, Produtos = 2*/
    GTree** codigos;
};

/*——————————————————————————Funções auxiliares à criação, manutenção e libertação da estrutura Catalogo——————————————————————————*/

/*Função que calcula o índice do array em que um determinado código deve ser colocado;*/
int hashKey(char* codigo, int profundidade){
    int index = codigo[0] - 'A';
    if(profundidade == 2) index = (index * BASE) + (codigo[1] - 'A');
    return index;
}

/*Função auxiliar que garante a inserção por ordem alfabética dos códigos nas árvores balanceadas*/
gint compareCode(gconstpointer code1, gconstpointer code2){
     return (strcmp(code1, code2));
}

/*Função auxiliar responsável por armazenar e imprimir todas as keys de uma GTree numa lista ligada*/
gint TraverseTree(gpointer key, gpointer value, gpointer data){
    data = g_slist_append(data, key); 

    return FALSE;
}

/*Função auxiliar responsável por armazenar e imprimir todas as keys de uma GTree numa lista ligada*/
gint TraverseTree2(gpointer key, gpointer value, gpointer data){
    g_ptr_array_add(data, key); 

    return FALSE;
}

/*Função responsável por alocar a memória necessária à criação de um catálogo.*/
Catalogo initCatalogo(int profundidade){
    int i;
    Catalogo cat= (Catalogo)malloc(sizeof(struct catalogo));
    int SIZE = pow(BASE, profundidade);

    cat->profundidade = profundidade;
    cat->codigos = (GTree**) malloc(SIZE * sizeof(GTree *));

    for(i=0; i<SIZE; i++){
       cat->codigos[i] = g_tree_new(compareCode);
    }

    return cat;
}

/*Função responsável por libertar toda a memória alocada para a criação de um catálogo*/
void freeCatalogo(Catalogo cat){
    int i;
    int SIZE = BASE * cat->profundidade;
    for (i=0; i<SIZE; i++)
        g_tree_destroy (cat->codigos[i]);
    free(cat);
}


/*Função que garante a correta inserção de um código num catálogo:
* calcula o índice do array ao qual deve ser adicionado, por ordem alfabética, o código 
*/
void add(Catalogo cat, char* codigo, gpointer value){
    int index= hashKey(codigo, cat->profundidade);
    g_tree_insert(cat->codigos[index], codigo, value); 
}

/*Função que insere um determinado código de um produto numa lista de catálogos */
void addCatalogos(Catalogo* catalogos, char* codigo){
    int i;
    for(i=0; i<ARRAYSIZE; i++)
       add(catalogos[i], codigo, NULL); 
}
/*Função que remove um código de produto de um catálogo */
int removeCode(Catalogo cat, char* codigo){
    int index= hashKey(codigo, cat->profundidade);
    return g_tree_remove(cat->codigos[index], codigo);
}


/*Função responsável por verificar se um determinado código existe num catálogo */
int existCodigo(Catalogo cat, char* codigo){
    int index= hashKey(codigo, cat->profundidade);
    return g_tree_lookup_extended(cat->codigos[index], codigo, NULL, NULL);
}

/*Função que devolve o apontador para o value de um determinado código presente num catálogo*/
gpointer getValue(Catalogo cat, char* codigo){
    int index= hashKey(codigo, cat->profundidade);
    return g_tree_lookup(cat->codigos[index], codigo);
}

void replaceValue(Catalogo cat, char* codigo, gpointer value){
    int index= hashKey(codigo, cat->profundidade);
    g_tree_replace(cat->codigos[index], codigo, value);
}

/*Função que calcula o número de códigos de um catálogo */
int nnodes(Catalogo cat){
    int i, res=0;
    int SIZE = pow(BASE, cat->profundidade);

    for (i = 0; i < SIZE; i++)  res+= g_tree_nnodes(cat->codigos[i]);
    
    return res;
}

/*Função responsável por copiar todos os códigos de um catálogo começados por uma determinada letra para uma lista ligada*/
GSList* startsWithLetter(Catalogo cat, char l){
    GSList* list = g_slist_alloc();
    int i = (l - 'A') * BASE;

    if(cat->profundidade == 1){
        g_tree_foreach(cat->codigos[i], TraverseTree, list);
    }else{ 
        int top = i + BASE;
        for( ; i< top; i++)
            g_tree_foreach(cat->codigos[i], TraverseTree, list);
    }

    return list;
}


/*Função que imprime um catálogo, por ordem alfabética uma vez que os códigos são inseridos por essa norma */
GPtrArray* printCatalogo(Catalogo cat){
    int i;
    int SIZE = pow(BASE, cat->profundidade);
    GPtrArray* catalogo = g_ptr_array_new();

    for(i=0; i<SIZE ; i++)
        g_tree_foreach(cat->codigos[i], TraverseTree2, catalogo);

    return catalogo;
}

/*Função que aplica uma determinada função passada como parâmetro a todos os elementos de um catálogo*/
void forEach(Catalogo catalogo, GTraverseFunc function, gpointer data){
    int i;
    int arraySize = pow(BASE, catalogo->profundidade);

    for(i=0; i<arraySize; i++)
        g_tree_foreach(catalogo->codigos[i], function, data);
}

/*Função auxiliar que clona todos os códigos presentes num catálogo para um outro catálogo passado como parâmetro*/
gboolean cloneCode(gpointer key, gpointer value, gpointer data){
    char* code = strdup(key);
    gpointer value2 = value;

    add(data, code, value2);

    return FALSE;
}

/*Função responsável por clonar uma estrutura Catalogo*/
Catalogo cloneCatalogo(Catalogo catalogo, int profundidade){
    Catalogo cat_clone = initCatalogo(profundidade);
    forEach(catalogo, cloneCode, cat_clone);

    return cat_clone;
}