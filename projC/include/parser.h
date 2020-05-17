#ifndef parser_h
#define parser_h

#include "clientes.h"
#include "produtos.h"
#include "venda.h"
#include "faturacao.h"
#include "filial.h"

#include <glib.h>

typedef struct infoFile* InfoFile;

InfoFile initInfoFile();
void freeInfoFile(InfoFile info);
void setInfoFile(InfoFile info, char* filename, int llidas, int lvalidas);
GPtrArray* printInfoFile(InfoFile info);

int readClientes(char* path, Clientes clientes, Filial* filiais, InfoFile info);

int readProdutos(char* path, Produtos produtos, Produtos prodsNaoComprados, InfoFile info);

int readVendas(char* path, Clientes clientes, Produtos produtos, Produtos prodsNaoComprados, Faturacao fat, Filial* filial, InfoFile info);


#endif