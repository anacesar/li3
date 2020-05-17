#ifndef view_h
#define view_h


#include <glib.h>

void showWelcomeMenu();
void showMainMenu();
void showLoadingMenu();
void showLoadingError();
void showLoadMenu();
void showGoodbyeMenu();
void showModes();

void showMessage(char* message);
void systemclear();

void printGSList(GSList* list);
void printGPtrArray(GPtrArray* array);
void printArray(char** answer);

void showListLetter(char letter, GSList* products, int number);
void showSalesProfits(char* productId, char month, GPtrArray* result);
void showProductsNeverBougth(GPtrArray* result);
void showClientsAllBranches(GPtrArray* clients);
void showNumbersNeverBougth(int* numbers);
void printTable(int **result , int N, int M);
void showSalesAndProfit(int nr_vendas, double total, int minMonth, int maxMonth);
void showProductsBuyers(char* productID, int branchID, GPtrArray* codesN, int nr_n, GPtrArray* codesP, int nr_p);
void showFavouriteProducts(char* clientID, int month, GPtrArray* array);
void showTopSelledProducts(int limit, GPtrArray* result);
void showTopProfitProducts(char* client, int limit, GPtrArray* result);


void showMonthInvalid();
void showLetterInvalid();
void showModeInvalid();
void showBranchInvalid();
void showProductInvalid();
void showClientInvalid();

#endif