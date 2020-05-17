#ifndef resultados_h
#define resultados_h

#include <glib.h>


typedef struct nr_total* Nr_total;
typedef struct array_numero* ArrayNumero;


Nr_total initNrTotal(int minMonth, int maxMonth);

int get_numero(Nr_total nrTotal);
void set_numero(Nr_total nrTotal, int nr);

double get_total(Nr_total nrTotal);
void set_total(Nr_total nrTotal, double tot);

int get_mes(Nr_total nrTotal);
void set_mes(Nr_total nrTotal, int mes);

int get_Mes(Nr_total nrTotal);
void set_Mes(Nr_total nrTotal, int Mes);


ArrayNumero initArrayNumero(char* productID);
void freeArrayNumero(ArrayNumero array_nr);

GPtrArray* getPtrArrayN(ArrayNumero array_nr);

GPtrArray* getPtrArrayP(ArrayNumero array_nr);

int getNumeroN(ArrayNumero array_nr);
void incNumeroN(ArrayNumero array_nr);

int getNumeroP(ArrayNumero array_nr);
void incNumeroP(ArrayNumero array_nr);

char* getProductA(ArrayNumero array_nr);


#endif