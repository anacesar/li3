#ifndef catalogo_h
#define catalogo_h


#include <glib.h>

typedef struct catalogo* Catalogo;

gint compareCode(gconstpointer code1, gconstpointer code2);

Catalogo initCatalogo();
void freeCatalogo(Catalogo cat);

void add(Catalogo cat, char* codigo, gpointer value);
int removeCode(Catalogo cat, char* codigo);
int existCodigo(Catalogo cat, char* codigo);

gpointer getValue(Catalogo cat, char* codigo);
void replaceValue(Catalogo cat, char* codigo, gpointer value);

int nnodes(Catalogo cat);

GSList* startsWithLetter(Catalogo cat, char l);

GPtrArray* printCatalogo(Catalogo cat);

void forEach(Catalogo catalogo, GTraverseFunc function, gpointer data);

Catalogo cloneCatalogo(Catalogo catalogo, int profundidade);

#endif