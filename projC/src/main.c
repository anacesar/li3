#include "../include/interface.h"


#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define NRFILES 3
#define BUFFER 300
#define FILIAIS 3
#define MONTH 12



/* Espera que o utilizador carregue no ENTER para continuar*/
static void wait_for_enter(){
    showMessage("Pʀᴇssɪᴏɴᴇ ENTER ᴘᴀʀᴀ ᴄᴏɴᴛɪɴᴜᴀʀ\n");
    while(getchar() != '\n');
}

/* Flush do stdin */
void flush_stdin(){
    fseek(stdin, 0, SEEK_END);
}

/* Função que copia para um array de strings os caminhos inseridos pelo utilizador para carregamento de dados*/
static void get_files_path(char** filesPath){
    char path[BUFFER];
    char* path_token;
    int i;

    for(i=0; i<NRFILES; i++){
        flush_stdin();
        showLoadMenu(filesPath[i]);
        fgets(path, sizeof(path), stdin);
        path_token = strtok(path, "\n");
        if(strcmp(path_token, "0") ==0) filesPath[i] = NULL;
        else filesPath[i] = strdup(path_token);
    }
}

/* Função responsável por encaminhar os caminhos dos ficheiros dados pelo utilizador para o carregamento dos dados do SGV*/
SGV load_data(SGV sgv){
    int i;
    sgv = initSGV();

    char* filesPath[NRFILES] = {"Clientes", "Produtos", "Vendas"};
    get_files_path(filesPath);

    showLoadingMenu();
    sgv = loadSGVFromFiles(sgv, filesPath[0], filesPath[1], filesPath[2]);

    if(! (sgv == NULL)){    
        GPtrArray* info = printInfoFiles(sgv);
        printGPtrArray(info);
    }else showMessage("Ocorreu um erro na leitura dos ficheiros! \n");

    for(i=0; i<NRFILES; i++) free(filesPath[i]);

    return sgv;
}

/* Destroí o sgv atual, libertando toda a memória alocada e volta a carregar o sgv com novos fichieros de dados*/
SGV restartSGV(SGV sgv){
    destroySGV(sgv);

    SGV sgv2 = NULL;

    while((sgv2 = load_data(sgv2)) == NULL);

    return sgv2;
}

/*Função responsável por recolher toda a informação necessária à querie 2, validando os inputs do utilizador. 
* Quando validados recebe a lista de produtos incializados por uma determinada letra e manda imprimir a resposta à query
*/
void list_letter(SGV sgv){
    char letter;

    showMessage("Introduza a letra pretendida: ");
    scanf(" %c", &letter);

    while(!(isalpha(letter))){
        flush_stdin();
        showLetterInvalid();
        scanf(" %c", &letter);
    }

    GSList* products = getProductsStartedByLetter(sgv, toupper(letter));
    showListLetter(toupper(letter), products, g_slist_length(products));

    g_slist_free(products);
}

/*Função responsável por recolher toda a informação necessária à querie 3, validando os inputs do utilizador. 
* Quando validados recebe um array de apontadores com a informação relativa a um produto num determinado mes e manda imprimir a resposta à query
*/
void totals_product_month(SGV sgv){
    char product[BUFFER];
    int month, mode;
    
    showMessage("Código do Produto: "); 
    scanf("%s", product);

    showMessage("Mês pretendido: ");
    scanf("%d", &month);
    while((month<1 || month>12)){
        flush_stdin();
        showMonthInvalid();
        scanf("%d", &month);
    }

    showModes();
    scanf("%d", &mode);
    while((mode!=0 && mode!=1)){
        flush_stdin();
        showModeInvalid();
        scanf("%d", &mode);
    }

    GPtrArray* result = getProductsSalesAndProfit(sgv, product, month, mode);

    if(result == NULL) showProductInvalid();
    else{
        showSalesProfits(product, month, result);
        g_ptr_array_free(result, TRUE);
    }
}

/*Função que recolhe a informação necessária à querie 4, dada pelo utilizador. 
* Recebe um array de apontadores com os códigos dos produtos nunca comprados, filial a filial ou global
*/ 
void products_never_bought(SGV sgv){
    int mode;

    showModes();
    scanf("%d", &mode);

    while((mode!=0 && mode!=1)){
        flush_stdin();
        showModeInvalid();
        scanf("%d", &mode);
    }

    GPtrArray* products = getProductsNeverBougth(sgv, mode); /*0- branch a branch , 1-global*/
    showProductsNeverBougth(products);

    g_ptr_array_free(products, TRUE);
}

/*Função que retorna a lista ordenada de códigos de clientes que realizaram compras em todas as filiais (Querie 5)
*/
void clients_all_branches(SGV sgv){
    GPtrArray* clients = getClientsOfAllBranches(sgv); 
    showClientsAllBranches(clients);

    g_ptr_array_free(clients, TRUE);
}

/*Função que retorna o número de clientes registados que não realizaram compras e o número de produtos que ninguém comprou (Querie 6)
*/
void clients_products_never_bougth(SGV sgv){
    int* numbers = getClientsAndProductsNeverBougth(sgv); 
    showNumbersNeverBougth(numbers);

    free(numbers);
}

/*Função que recolhe a informação necessária à querie 7, dada pelo utilizador. 
* Recebe uma tabela com o número total de produtos comprados, mês a mês, organizados por filiais
*/
void products_by_client(SGV sgv){
    char cliente[BUFFER];

    showMessage("Código do cliente: "); 
    scanf("%s", cliente);

    int** table= getProductsBougthByClient(sgv,cliente); 
    if(table == NULL) showClientInvalid();
    else {
        printTable(table, FILIAIS, MONTH);
        freeTable(table);
    }
}

/*Função que recolhe a informação necessária à querie 8, dada pelo utilizador. 
* Recebe o total de vendas registadas num determinado intervalo de meses e o total faturado
*/
void sales_and_profit(SGV sgv){
    int monthI, monthF;

    showMessage("Mês inicial: ");
    scanf("%d", &monthI);
    while((monthI<1 || monthI>12)){
        flush_stdin();
        showMonthInvalid();
        scanf("%d", &monthI);
    }

    showMessage("Mês final: ");
    scanf("%d", &monthF);
    while((monthF<1 || monthF>12)){
        flush_stdin();
        showMonthInvalid();
        scanf("%d", &monthF);
    }

    Nr_total nrTotal  = getSalesAndProfit(sgv, monthI, monthF); 
    showSalesAndProfit(get_numero(nrTotal), get_total(nrTotal), monthI, monthF);

    free(nrTotal);
}

/*Função que recolhe a informação necessária à querie 9, dada pelo utilizador. 
* Recebe os códigos dos clientes que o compraram, distinguindo o modo de compra N e P
*/
void products_buyers(SGV sgv){
    int branch;
    char product[BUFFER];

    showMessage("Código do produto: "); 
    scanf("%s", product);
    
    showMessage("Filial pretendida: "); 
    scanf("%d", &branch);
    while((branch<1 || branch>3)){
        flush_stdin();
        showBranchInvalid();
        scanf("%d", &branch);
    }

    ArrayNumero array_nr = getProductsBuyers(sgv, product, branch); 

    if(array_nr == NULL) showProductInvalid();
    else{
        showProductsBuyers(product, branch, getPtrArrayN(array_nr), getNumeroN(array_nr), getPtrArrayP(array_nr), getNumeroP(array_nr));
        freeArrayNumero(array_nr);
    }
}

/*Função que recolhe a informação necessária à querie 10, dada pelo utilizador. 
* Recebe a lista de códigos de produtos que o cliente mais comprou por quantidade, por ordem descendente
*/
void client_favourite_products(SGV sgv){
    char client[BUFFER];
    int month;

    showMessage("Código do cliente: "); 
    scanf("%s", client);

    showMessage("Mês pretendido: ");
    scanf("%d", &month);
    while((month<1 || month>12)){
        flush_stdin();
        showMonthInvalid();
        scanf("%d", &month);
    }

    GPtrArray* result = getClientFavoriteProducts(sgv, client, month); 

    if(result == NULL) showClientInvalid();
    else{
        showFavouriteProducts(client, month, result);
        g_ptr_array_free(result, TRUE);
    }

}

/*Função que recolhe a informação necessária à querie 11, dada pelo utilizador. 
* Recebe a lista dos N produtos mais vendidos em todo o ano, indicando o número total de clientes e o número de unidades vendidas, filial a filial
*/
void top_selled_products(SGV sgv){
    int limit;

    showMessage("Indique o número de produtos a apresentar: ");
    scanf("%d", &limit);

    GPtrArray* array = getTopSelledProducts(sgv, limit); 
    showTopSelledProducts(limit, array);

    g_ptr_array_free(array, TRUE);
}

/*Função que recolhe a informação necessária à querie 12, dada pelo utilizador. 
* Recebe quais os códigos dos N produtos em que o cliente escolhido mais gastou dinheiro durante o ano
*/
void client_top_profit_products(SGV sgv){
    char client[BUFFER];
    int limit;

    showMessage("Código do cliente: "); 
    scanf("%s", client);

    showMessage("Indique o número de produtos a apresentar: ");
    scanf("%d", &limit);

    GPtrArray*  result = getClientTopProfitProducts(sgv, client, limit); 
    if(result == NULL) showClientInvalid();
    else{
        showTopProfitProducts(client, limit, result);
        g_ptr_array_free(result, TRUE);
    }
}

/*Função responsável por manter a responsável interação com o utilizador
*/
int runController(){
    SGV sgv = NULL;

    showWelcomeMenu();
    
    int exit = 0;
    int command;
    
    while((sgv = load_data(sgv)) == NULL);

    /* Enquanto o utilizador não quiser sair */
    while (!exit){
        flush_stdin(); 

        wait_for_enter();

        systemclear();

        showMainMenu();
        scanf(" %d", &command);
         
        system("clear || cls");
    
        switch (command){
            case 1:
                sgv = restartSGV(sgv);
                break;
            case 2:
                list_letter(sgv);
                break;
            case 3: 
                totals_product_month(sgv);
                break;
            case 4:  
                products_never_bought(sgv);
                break;
            case 5:  
                clients_all_branches(sgv);
                break;
            case 6:  
                clients_products_never_bougth(sgv);
                break;
            case 7:  
                products_by_client(sgv);
                break;
            case 8:  
                sales_and_profit(sgv);
                break;
            case 9:  
                products_buyers(sgv);
                break;
            case 10:  
                client_favourite_products(sgv);
                break;
            case 11:  
                top_selled_products(sgv);
                break;
            case 12:  
                client_top_profit_products(sgv);
                break;
            case 13:
                exit=1;
                break;
            default:
                showMessage("A opção não é válida! \n");
                break;
        }
    }

    system("clear||cls");
    showGoodbyeMenu();

    destroySGV(sgv);
    return 0;
}

/*Função main, que corre enquanto o utilizador não sair do menu*/
int main(int argc, char* argv[]){ 
    int exitcode  = runController();

    return exitcode;
}
