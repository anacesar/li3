#ifndef interface_h
#define interface_h

#define ARRAYSIZE 4
#define FILIAIS 3
#define MES 12
#define BUFFER 300

#include "clientes.h"
#include "produtos.h"
#include "faturacao.h"
#include "filial.h"

#include "parser.h"
#include "resultados.h"

#include "view.h"


#include <glib.h>


typedef struct sgv* SGV;

GPtrArray* printInfoFiles(SGV sgv);

/*QUERIES*/
SGV initSGV();
void destroySGV(SGV sgv);
SGV loadSGVFromFiles(SGV sgv, char* clientsFilePath, char* productsFilePath, char* salesFilePath);
GSList* getProductsStartedByLetter(SGV sgv, char letter);
GPtrArray* getProductsSalesAndProfit(SGV sgv, char* productId, int month, int flag);
GPtrArray* getProductsNeverBougth(SGV sgv, int branchID);
GPtrArray* getClientsOfAllBranches(SGV sgv);
int* getClientsAndProductsNeverBougth(SGV sgv);
int** getProductsBougthByClient(SGV sgv, char* clientID);
Nr_total getSalesAndProfit(SGV sgv, int minMonth, int maxMonth);
ArrayNumero getProductsBuyers(SGV sgv, char* productID, int branchID);
GPtrArray* getClientFavoriteProducts(SGV sgv, char* clientID, int month);
GPtrArray* getTopSelledProducts(SGV sgv, int limit);
GPtrArray* getClientTopProfitProducts(SGV sgv, char* clientID, int limit);

#endif 