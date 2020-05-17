#include "../include/view.h"

#include <stdio.h>

/*Funções responsáveis pela impressão de menus e requisitação de informação necessária à execução das queries*/

/* Menu inicial do programa */
void showWelcomeMenu(){
    printf(" ———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————\n");
    printf("|                                             Sɪsᴛᴇᴍᴀ ᴅᴇ Gᴇsᴛᴀ̃ᴏ ᴅᴇ Vᴇɴᴅᴀs                                                   |\n");
    printf(" ———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————\n\n");
}

/* Menu principal, mostrado ao utilizador para que escolha a operação pretendida*/
void showMainMenu(){
    printf(" ———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————\n");
    printf("|                                                  MENU                                                                     |\n");
    printf(" ———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————\n");
    printf("1  - Reiniciar dados.\n");                                                                                                 
    printf("2  - Lista e total de produtos inicializados por uma determinada letra.\n");
    printf("3  - Total de vendas e de faturacção de um produto num determinado mês (𝖾𝗆 𝗆𝗈𝖽𝗈 𝖭 𝖾 𝗆𝗈𝖽𝗈 𝖯)\n");
    printf("4  - Lista ordenada e número de códigos dos produtos que ninguém comprou.\n");
    printf("5  - Lista ordenada de códigos de clientes que realizaram compras em todas as filias.\n");
    printf("6  - Número de clientes registados que não realizaram compras bem como o número de produtos que ninguém comprou.\n");
    printf("7  - Tabela com o número total de produtos comprados por um cliente, mês a mês.\n");
    printf("8  - Total de vendas registadas e total faturado num intervalo de meses.\n");
    printf("9  - Códigos dos clientes que compraram (𝖾𝗆 𝗆𝗈𝖽𝗈 𝖭 𝖾 𝗆𝗈𝖽𝗈 𝖯 ) um produto numa determinada filial.\n");
    printf("10 - Lista de códigos de produtos mais comprados por um cliente, num determinado mês.\n");
    printf("11 - Lista dos N produtos mais vendidos em todo o ano, número de clientes e de unidades vendidas, filial a filial.\n");
    printf("12 - Dado um código de cliente determinar quais os códigos dos N produtos em que mais gastou dinheiro durante o ano.\n");
    printf("13 - 𝖲𝖠𝖨𝖱\n\nInsira opção pretendida: ");
}


/*Função responsável por informar o utilizador que a leitua dos ficheiros está a ser efetuada*/
void showLoadingMenu(){
    printf("————————————————————————————————————————————— Leitura dos Ficheiros ————————————————————————————————————————————————————————\n");
}

/*Função responsável por pedir ao utilizador os caminhos para os diferentes ficheiros*/
void showLoadMenu(char* file){
    printf("Insira o caminho para o carregamento de %s (0 ᴘᴀʀᴀ ᴅᴇғᴀᴜʟᴛ)\n", file);
}

/*Menu de saída*/
void showGoodbyeMenu(){
    printf("\n                                   ＧＯＯＤＢＹＥ\n\n");
}


/*Informa o utilizador das diferentes formas de apresentação de resultados*/
void showModes(){
    printf("Selecione a opção para apresentação de resultados:\n");
    printf("0 - Global\n");
    printf("1 - Filial a Filial\n");

}

/*Função genérica que imprime uma string passada como argumento*/
void showMessage(char* message){
    printf("\n%s", message);
}

/* Função responsável por limpar o ecra do terminal */
void systemclear(){
    system("clear || cls");
}

/*  Funções responsáveis pela apresentação dos resultados    */

/*Função auxiliar que imprime para o terminal*/
void print(gpointer data, gpointer user_data){
    if (!(data == NULL)) printf("%s\n", data);
}

/*Função responsável por imprimir para o terminal os elementos presentes numa GList*/
void printGSList(GSList* list){
    g_slist_foreach(list, print, NULL);
}

/*Função responsável por imprimir para o terminal os elementos presentes num Array de Apontadores(GPtrArray) */
void printGPtrArray(GPtrArray* array){
    g_ptr_array_foreach(array, print, NULL);
}


/* Menu da query 2 - imprime todos os produtos inicializados de por uma letra*/
void showListLetter(char letter, GSList* products, int number){
    printf("——————————————————————————————————————Lista de produtos incializados pela letra %c——————————————————————————————————————\n", letter);

    printGSList(products);
    
    printf("Número de produtos incializados pela letra %c --> %d\n", letter, number);
}

/* Menu da query 3 - imprime o número de vendas e lucro de um produto num determinado mês */
void showSalesProfits(char* productId, char month, GPtrArray* result){

    printf("\n—————————————————————————————————————Vendas e Lucro do Produto %s no Mês %d————————————————————————————————————————\n\n" , productId, month);

    printGPtrArray(result);
}

/* Menu da query 4 - imprime a lista de produtos nunca comprados */
void showProductsNeverBougth(GPtrArray* result){
    printf("\n————————————————————————————————————————Lista de Produtos nunca comprados————————————————————————————————————————————————\n");

    printGPtrArray(result);
}

/* Menu da query 5- imprime a lista ordenada dos clientes que realizaram compras em todas as filiais */
void showClientsAllBranches(GPtrArray* clients){
    printf("\n—————————————————————————————————Lista de Clientes que compraram em todas as Filiais—————————————————————————————————————\n");
    printGPtrArray(clients);
}

/* Menu da query 6 - imprime o número de clientes que nunca comprarame o número de produtos nunca comprados */
void showNumbersNeverBougth(int* numbers){
    printf("\n—————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————\n");
    printf("Número de clientes que nunca compraram --> %d\n" , numbers[0]);
    printf("Número de produtos nunca comprarados   --> %d\n" , numbers[1]);
    printf("———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————\n");

}

/* Menu da query 7 - imprime uma tabela com as quantidades vendidas de um determinado produto, em cada mes e em cada uma das filiais */
void printTable(int** result , int N, int M){
    int i,j;
    printf("\n");

    printf(" ___________________________________________________________________________________________________________________________________\n");
    printf("| MES       |    1    |    2    |    3    |    4    |    5    |    6    |    7    |    8    |    9    |    10   |    11   |    12   | \n"); 
    printf(" ___________________________________________________________________________________________________________________________________\n");
    
    for(i=0; i<N; i++){
        printf("\n| FILIAL %d  |", i+1);
        for(j=0; j<M; j++)
            printf("  %3d    |", result[i][j]);
    }

    printf("\n ___________________________________________________________________________________________________________________________________\n");
}

/* Menu da query 8 - imprime o número total de vendas e total faturado num intervalo de meses */
void showSalesAndProfit(int nr_vendas, double total, int minMonth, int maxMonth){
    printf("——————————————————————————————————————————————Valores para o intervalo [%d, %d]——————————————————————————————————————————————————————\n" , minMonth, maxMonth);

    printf("Número de vendas --> %d\n", nr_vendas);
    printf("Total Faturado   --> %.2f\n", total);

}

/* Menu da query 9 - imprime a lista e o número de clienres que compraram um determinado produto, numa determinada filial, distinguindo em modo N e modo P */
void showProductsBuyers(char* productID, int branchID, GPtrArray* codesN, int nr_n, GPtrArray* codesP, int nr_p){
    printf("———————————————————————————————Lista de códigos + Número de clientes que compraram o produto %s na filial %d———————————————————————————————\n" , productID, branchID);

    printf("**Em modo N** \n");
    printGPtrArray(codesN);
    printf("Número de clientes que compraram o produto em modo N --> %d\n", nr_n);
    
    printf("\n**Em modo P** \n");
    printGPtrArray(codesP);
    printf("Número de clientes que compraram o produto em modo P --> %d\n", nr_p);

}

/* Menu da query 10 - imprime a lista dos produtos mais comprados por um determinado cliente num determinado mes */
void showFavouriteProducts(char* clientID, int month, GPtrArray* array){
    printf("———————————————————————————————Lista de produtos mais comprados pelo cliente %s no mês %d———————————————————————————————\n" , clientID, month);
    
    printf("Código do Produto\tQuantidade\n");

    printGPtrArray(array);
}

/* Menu da query 11 - imprime uma lista de limit codigos de produtos mais vendidos em todo o ano */
void showTopSelledProducts(int limit, GPtrArray* result){
    printf("———————————————————————————————Top %d de produtos mais vendidos———————————————————————————————" , limit);

    printGPtrArray(result);
}

/* Menu da query 12 - imprime uma lista de limit codigos de produtos que um determinado cliente gastou mais dinheiro */
void showTopProfitProducts(char* client, int limit, GPtrArray* result){
    printf("——————————————————————————Top %d de produtos que o cliente %s gastou mais dinheiro——————————————————————————\n" , limit, client);

    printf("\nCódigo do Produto\tTotal gasto\n\n");

    printGPtrArray(result);
}

/* ERROS */

void showMonthInvalid(){
    printf("\nO mês introduzido não é válido!\n");
    printf("Digite de novo: ");
}

void showLetterInvalid(){
    printf("\nLetra Inválida!\n");
    printf("Digite de novo: ");
}

void showModeInvalid(){
    printf("Modo Inválido!\n");
    printf("Digite de novo: ");
}

void showBranchInvalid(){
    printf("Filial Inválida!\n");
    printf("Digite de novo: ");
}

void showProductInvalid(){
    printf("O código de produto não existe!\n ");
}

void showClientInvalid(){
    printf("O código de cliente não existe!\n ");
}