#ifndef venda_h
#define venda_h

typedef struct venda* Venda;

Venda initVenda();

char* getCodeProduct(Venda venda);
void setCodeProduct(Venda venda, char* produto);

float getPrice(Venda venda);
void setPrice(Venda venda, float preco);

int getQuantity(Venda venda);
void setQuantity(Venda venda, int quantidade);

char getType(Venda venda);
void setType(Venda venda, char tipo);

char* getCodeClient(Venda venda);
void setCodeClient(Venda venda, char* cliente);

int getMonthSale(Venda venda);
void setMonthSale(Venda venda, int mes);

int getBranchSale(Venda venda);
void setBranchSale(Venda venda, int filial);

#endif 
