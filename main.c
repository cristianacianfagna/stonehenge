#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//struttura di tipo blocco
typedef struct blocco {
	int x0, y0; /* valore dell’elemento: coordinate del blocco */
	int x,y,z; /*dimensioni del blocco*/
	struct blocco *next; /*puntatore al blocco successivo */
};

typedef struct blocco blocco;

//struttura di tipo cava determinata da due interi, un puntatore ai blocchi contenuti e un puntatore alla cava successiva
typedef struct cava {
	int x, y; /* valore dell’elemento: coordinate della cava */
	int sito; /* 1 se la cava è un sito, altrimenti 0*/
	struct blocco *block; /* indirizzo del primo blocco nella cava */
	struct cava *next; /* puntatore al prossimo elemento cava */
};

typedef struct cava cava;
