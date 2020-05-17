#include "../include/parser.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

#define MAXREADER 10
#define NFILIAIS 3 
#define CAMPOSVENDA 7
#define MAXLINHAVENDA 50
#define BUFFER 500


#define cliPath    "././Dados/Clientes.txt"
#define prodPath   "././Dados/Produtos.txt"
#define vendasPath "././Dados/Vendas_1M.txt"

/*Estrutura InoFile:
* responsável por guardar informação relativa à leitura dos ficheiros para carregamento de dados.
* Guarda o nome do ficheiro lido, assim como o número de linhas lidas no ficheiro e o número de linhas validadas.
*/
struct infoFile{
    char* name;
    int linhasLidas;
    int linhasValidas;
};

/*Função responsável por alocar memória necessária à estrutura InfoFile , e inicializar as suas variávies para realizar a leitura dos ficheiros*/
InfoFile initInfoFile(){
    InfoFile info = (InfoFile)malloc(sizeof(struct infoFile));
    info->name= NULL;
    info->linhasLidas = 0;
    info->linhasValidas = 0;
    return info;
}

/*Função responsável por libertar a memória alocada necessária à criação da estrutura InfoFile*/
void freeInfoFile(InfoFile info){
    free(info->name);
    free(info);
}

/*Função que coloca os valores respetivos à leitura de um determinado ficheiro na estrutura apropriada, InfoFile*/
void setInfoFile(InfoFile info, char* filename, int llidas, int lvalidas){
    info->name= strdup(filename);
    info->linhasLidas = llidas;
    info->linhasValidas = lvalidas;
}

/*Função responsável por transformar a informação contida na estrutura InfoFile numa string contida num array de apontadores*/
GPtrArray* printInfoFile(InfoFile info){
  GPtrArray* infoFile = g_ptr_array_new();
  char string[BUFFER];

  sprintf(string, "Leitura do ficheiro '%s' \nNúmero de linhas lidas --> %d\nNúmero de linhas válidas --> %d\n", info->name, info->linhasLidas, info->linhasValidas);
  g_ptr_array_add(infoFile, strdup(string));

  return infoFile;
}

/*Função auxiliar que verfica a validade de um código de produto consoante o determinado critério:
* cada código de produto é formado por duas letras maiúsculas e 4 dígitos (que representam um inteiro entre 1000 e 9999)
*/
int validaProduto(char* produto){
    int ok=1;
	if(! (isupper(produto[0]) && isupper(produto[1])) ) ok=0;
	if(!(atoi(produto+2)>=1000 && atoi(produto+2)<=9999)) ok=0;
	return ok;
}

/*Função auxiliar que verfica a validade de um código de cliente consoante o determinado critério:
* cada código de cliente formado por uma letra maiúscula e 4 dígitos que representam um inteiro entre 1000 e 5000.
*/
int validaCliente(char* reader){ 
    int ok =1;
	if(!isupper(reader[0])) ok=0;
	if(!(atoi(reader+1)>=1000 && atoi(reader+1)<=5000)) ok=0;
	return ok;
}

/*Função auxiliar que verfica a validade de um código de cliente consoante o determinado critério:
* será formada por um código de produto, um preço unitário decimal (entre 0.0 e 999.99), o número inteiro de unidades compradas (entre 1 e 200), 
* a letra N ou P conforme tenha sido uma compra Normal ou uma compra em Promoção, o código do cliente, o mês da compra (1 .. 12) e a filial (de 1 a 3)
* Caso a venda seja válida todos os seus campos anteriormente referidos são devidamente guardados num estrutura auxiliar Venda
*/
int loadValidPurchase(Venda venda, Produtos produtos, Clientes clientes, char* token, int field){
  int valido, quantidade, mes, filial;
  float preco;
  char tipo;
  switch(field){
    case 0:
      valido = existCodigoProduto(produtos, token);
      setCodeProduct(venda, token);
      break;
    case 1:
      preco = atof(token);
      valido = (preco >= 0.0 && preco <= 999.99);
      setPrice(venda, preco);
      break;
    case 2:
      quantidade = atoi(token);
      valido = (quantidade >= 1 && quantidade <= 200);
      setQuantity(venda, quantidade);
      break;
    case 3:
      tipo = token[0];
      valido = ((tipo == 'N' || tipo == 'P') && strlen(token) == 1);
      setType(venda, tipo);
      break;
    case 4:
      valido = existCodigoCliente(clientes, token);
      setCodeClient(venda, token);
      break;
    case 5:
      mes = atoi(token);
      valido = (mes >= 1 && mes <= 12);
      setMonthSale(venda, mes);
      break;
    case 6:
      filial = atoi(token);
      valido = (filial >= 1 && filial <= 3);
      setBranchSale(venda, filial);
      break;
  }
 
  return valido;
}

/*Função responsável pela leitura de um ficheiro que contêm a informação relativa aos produtos.
* Contabiliza o número de linhas lidas e validadas no ficheiro e guarda a informação na respetiva estrutra InfoFile,
* Caso o produto seja válido, é responsável também por inserir o código no catálogo de produtos (Produtos)
*/
int readProdutos(char* path, Produtos produtos, Produtos produtosNaoComprados, InfoFile info){

    char str[MAXREADER];
	  char* produto= NULL;

    int llidas=0;
    int lvalidas=0;
    
    if(path==NULL) path = strdup(prodPath);
    FILE* fp = fopen(path , "r");
	  if(fp==NULL) return 1;

    while(fgets(str, MAXREADER, fp)){
		  produto = strdup(strtok(str, "\r\n"));
      llidas++;
		  if(validaProduto(produto)){
            lvalidas++;
            addProduto(produtos, produto);
            addProduto(produtosNaoComprados, produto);
		  }
	  }
    fclose(fp);
    setInfoFile(info, path, llidas, lvalidas);
    return 0;
}

/*Função responsável pela leitura de um ficheiro que contêm a informação relativa aos clientes.
* Contabiliza o número de linhas lidas e validadas no ficheiro e guarda a informação na respetiva estrutra InfoFile,
* Caso o cliente seja válido, é responsável também por inserir o código no catálogo de clientes (Clientes)
*/
int readClientes(char* path, Clientes clientes, Filial* filiais, InfoFile info){

    char str[MAXREADER];
	  char* cliente= NULL;
    
    int llidas=0;
    int lvalidas=0;

    if(path==NULL) path= strdup(cliPath);
    FILE* fp = fopen(path , "r");
	  if(fp==NULL) return 1;
 
    while(fgets(str, MAXREADER, fp)){
		    cliente = strdup(strtok(str, "\r\n"));
        llidas++;
		    if(validaCliente(cliente)){    
            addCliente(clientes, cliente);
            lvalidas++;  
		    }
	  }
    
    fclose(fp);
    setInfoFile(info, path, llidas, lvalidas);
    return 0;
}

/*Função responsável pela leitura de um ficheiro que contêm a informação relativa às vendas.
* Contabiliza o número de linhas lidas e validadas no ficheiro e guarda a informação na respetiva estrutra InfoFile,
* Caso a venda seja válida, é responsável também por inserir as suas informações relevantes nas estruturas de Faturação e na Filial correspondente
*/
int readVendas(char* path, Clientes clientes, Produtos produtos, Produtos prodsNaoComprados, Faturacao fat, Filial* filial, InfoFile info){
    char str[MAXLINHAVENDA];

    int llidas=0;
    int lvalidas=0;
    int valido;
    int field;

    if(path==NULL) path = strdup(vendasPath);
    FILE* fp = fopen(path  , "r");
	  if(fp==NULL) return 1;

    while(fgets(str,MAXLINHAVENDA,fp)){
        Venda venda = initVenda();
        valido = 1;
        field = 0;
 
        char* token = strdup(strtok(str, " \r\n"));
        llidas++;

        for(; token != NULL && valido; token = strtok(NULL, " \r\n"), field++){
          valido = loadValidPurchase(venda, produtos, clientes, token, field);
        }
        if(valido){
          addVendaFat(venda, fat);
          addVendaFilial(venda, filial[getBranchSale(venda)-1]);
          removeProduto(prodsNaoComprados,getCodeProduct(venda));
          lvalidas++;
        }
        free(venda);
    }

    
    fclose(fp);
    setInfoFile(info, path, llidas, lvalidas);
    return 0;
}
