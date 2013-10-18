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

//funzione che verifica l'esistenza di un elemento (cava) all'interno di una lista (terriotorio) ....NON FUNZIONA
cava* cerca_cava(int cava_x, int cava_y, cava *testa){
	cava *p,*ret;
	ret=NULL;
	
	for (p=testa; p != NULL; p=p->next) {
		if (p->x == cava_x & p->y==cava_y) {
			ret=p;
		}	
			
			
	}
	return ret;
		
}

//inserimento in testa del blocco
blocco* insert_blocco(int dim_x, int dim_y, int dim_z, int pos_x0, int pos_y0, blocco *primo_blocco) {
	//ret=insert_blocco(dim_x, dim_y, dim_z, pos_x0, pos_y0, c->block);
	/*dim_x, dim_y, dim_z sono le dimensioni del blocco */
	/* pos_x0, pos_y0 indicano le coordinate della posizione del blocco*/
	/* *primo_blocco è il puntatore al primo blocco*/
	/*ritorna la nuova testa della lista di blocchi (quindi il puntatore al primo blocco)*/
	blocco *nuovo_el;
	//printf ("Il campo blocco della cava punta a %p\n", primo_blocco->next);
	nuovo_el=malloc(sizeof(blocco));	
	
	nuovo_el -> x0 = pos_x0;
	nuovo_el -> y0 = pos_y0;
	nuovo_el -> x = dim_x;
	nuovo_el -> y = dim_y;
	nuovo_el -> z = dim_z;
	nuovo_el -> next=primo_blocco;
	//printf("inserisco il blocco\n");
	//printf ("il puntatore al blocco è %p\n", nuovo_el->next);
	//printf ("Il nuovo blocco è in posizione: \n nuovo_el->x0:%d - nuovo_el->y0: %d\n", nuovo_el->x0, nuovo_el->y0);
	//printf ("Le dimensioni sono: \n nuovo_el->x:%d - nuovo_el->y: %d - nuovo_el->z: %d\n", nuovo_el->x, nuovo_el->y, nuovo_el->z);
	//printf ("Punta a : nuovo_el: %p \n", nuovo_el);
	//primo_blocco->next = nuovo_el;
	return nuovo_el;
}

//cerca il blocco in una cava
int cerca_blocco(int pos_x0, int pos_y0, blocco *primo_blocco){
	/* pos_x0, pos_y0 indicano le coordinate della posizione del blocco*/
	/* *primo_blocco è il puntatore al primo blocco*/
	/* ritorna la nuova testa della lista di blocchi */
	blocco *p;
	int trovato=0;
	
	//scorro i blocchi
	for (p=primo_blocco; p != NULL; p=p->next){
		//se lo trovo
		if (p->x0 == pos_x0 & p->y0 == pos_y0){
			//se trovo il blocco
			trovato=1;
		}
	}
	return trovato;
}

//inserimento del blocco nella prima cava (poi dovrò farlo in un sito!!!!!!!!!!!!!!!)
blocco* nuovo_blocco(int cava_x, int cava_y, int dim_x, int dim_y, int dim_z, int pos_x0, int pos_y0, cava *testa) {
	/* cava_x, int cava_y sono le coordinate della cava nel quale va inserito il blocco */
	/* int dim_x, int dim_y, int dim_z sono le dimensioni del blocco */
	/* int pos_x0, int pos_y0 sono le coordinate della posizione in cui inserire il blocco */
	/* *testa è il puntatore alla lista di cave */
	/* restituisco la testa della lista di blocchi o NULL se non faccio nulla */
	cava *c;
	blocco *ret;
	ret=NULL;
	int found=0;
	
	
	//scorro le cave
	for (c=testa; c != NULL; c=c->next) {
		//se trovo la cava passata in argomento
		if (c->x == cava_x & c->y == cava_y) {
			//e se è un sito...ricordarsi poi di mettere l'if a 1!!
			if (c->sito==0) {
			
				//cerco il blocco per vedere se esiste già
				found=cerca_blocco(pos_x0, pos_y0, c->block);
				//se il blocco non c'è
				if (found==0){
					//printf ("Le coordinate della cava sono: \n c->x:%d - c->y: %d\n", c->x, c->y);
					//printf ("Il campo blocco della cava punta a %p\n", c->block);
					//inserisco il blocco in testa
					//printf ("Il campo blocco, prima della insert, della cava punta a %p\n", c->block);
					c->block=insert_blocco(dim_x, dim_y, dim_z, pos_x0, pos_y0, c->block);
					//printf ("Il campo blocco della cava punta a %p\n", c->block);
					ret=c->block;
					//printf ("Il valore di ritorno  punta a %p\n", ret);
				}	
			}
		}					
	}
	//printf ("Il nuovo blocco è in posizione: \n ret->x0:%d - ret->y0: %d\n", ret->x0, ret->y0);
	//printf ("Le dimensioni sono: \n ret->x:%d - ret->y: %d - ret->z: %d\n", ret->x, ret->y, ret->z);
	//printf ("Punta a : ret->next %p \n", ret->next);
	return ret;	
	
}



//inserimento in testa della cava
cava* insert_cava(int cava_x, int cava_y, cava *testa) {
	/*cava_x e cava_b sono le coordinate x e y della cava; */
	/* testa è il territorio (puntatore alla testa della lista)*/
	/*ritorna la nuova testa della lista*/
	cava *nuovo_el;
	nuovo_el=malloc(sizeof(cava));
	nuovo_el->sito=0;
	nuovo_el ->x=cava_x;
	nuovo_el ->y=cava_y;
	nuovo_el ->block=NULL;
	nuovo_el ->next=testa;
	return nuovo_el;
}

//stampa la lista dei blocchi della cava che passo 
void printList_B(cava *testa, int a, int b){
	cava *c;
	blocco *p;
	for (c=testa; c!=NULL; c = c->next) {
		if (c->x == a & c->y==b){
			printf("I blocchi della cava con coordinate %d %d:\n", c->x, c->y );
			for (p=c->block; p != NULL; p = p->next){
				printf ("ciao");
				printf(" x0:%d y0:%d\n ", p->x0, p->y0 );
			}
			
		}
	}
	
	printf("\n");
}

//stampa la lista partendo dalla cava (in realtà dal sito) che passo
void printList_C(cava *testa){
	cava *p;
	p=testa;
	printf("Il territorio è composto dalle cave seguenti:\n");
	for (p=testa; p != NULL; p = p->next){
		printf(" %d %d\n", p->x, p->y );
	}
	
}

//creo una nuova cava nella lista passandogli la testa della lista e le coordinate
cava* nuova_cava(int cava_x, int cava_y, cava *t){
	cava *p;
	int esiste=0;
	//scorro la lista per vedere se esiste già una cava con coordinata x=a con quelle coordinate
	for (p=t; p != NULL; p=p->next) {
		printf("%d", p->x);
		//se esiste la sostituisco 
		if ((p->x) == cava_x) {
			(p->y) = cava_y;
			esiste=1;
		}	
	}
	//se non esiste la inserisco
	if (!esiste){
		//printList_C(t);
		t=insert_cava(cava_x, cava_y, t);	
	}	
	return t;	
}

//elimina dal territorio la cava indicata
cava* cava_esaurita(int a, int b, cava *t){
	/* int a, int b sono le coordinate della cava da eliminare*/
	/* *t è il territorio*/
	/* restituisce il territorio*/
	cava* precedente;
	cava* attuale;
	precedente=NULL;
	attuale=t;
	/*scorro il territorio con 2 puntatori perchè se trovo la cava, una volta eliminata, devo collegare la cava precedente
	a quella successiva a quella eliminata*/
	for (attuale=t; attuale != NULL; attuale=attuale->next){
		//se trovo la cava
		if (((attuale->x) == a) & ((attuale->y) == b)) {
			//se è la prima
			if (precedente==NULL) {
				//punto la testa del territorio al successivo
				t=attuale->next;
			} else {
				//altrimenti punto la cava precedente a quella successiva
				precedente->next=attuale->next;
				//e dealloco
				//uscire dal for
			}
		}
		//sposto in avanti il puntatore "precedente"
		precedente=attuale;	
	}
	return t;
}


void percorrenza_minima (cava *territorio) {
	cava *t;
	int y_min, y_max, i;
	i=0;
	if (territorio != NULL) {
		y_max=territorio->y;
		y_min=territorio->y;
	}
	
	//scorro il territorio per trovare la y minima e la y massima
	for (t=territorio; t != NULL; t=t->next){
		if (t->y < y_min)
			y_min=t->y;
		if (t->y >y_max)
			y_max=t->y;
	}
	printf("min:%d max:%d\n", y_min, y_max);
	
	
}

int main( void ) { 
	int i, length, a, b;
	int *coordinate;
	int *coordinate_b;
	cava *territorio;
	cava *trovato;
	cava *temp;
	trovato=NULL;
	territorio=NULL;
	

	temp=malloc(sizeof(cava));
	trovato=malloc(sizeof(cava));
	i=0;
	printf("Inserisci le coordinate x e y delle cave terminate con lo 0\n");
	
	coordinate=malloc(sizeof(int));
	//printf("%d", coordinate);
	
	while (scanf("%d", &coordinate[i])) {
		if (coordinate[i]==0)
			break;
		i++;	
		coordinate=realloc(coordinate, (i+1)*sizeof(int));
	}
	
	length=i;
	//printf("numero coordinate lette: %d\n", length);
	
	/*for (i=0; i<length; i++){
		printf("le coordinate nell'array sono:%d\n", coordinate[i]);
	}*/
	

	
	//i+=2 perchè mi sposto di due alla volta (coppia di coordinate)
	for(i=0; i<length; i+=2){
		territorio=nuova_cava(coordinate[i], coordinate[i+1], territorio);
	}
	
	printf("\n");
	printList_C(territorio);
	
	
	percorrenza_minima(territorio);
	
	territorio=cava_esaurita(4,5,territorio);
	printf("Eliminata la cava 4 - 5\n");
	printList_C(territorio);
	
	printf("Inserisci un blocco in una cava. Specifica posizione e dimensione del blocco, e le coordinate della cava terminate da uno zero\n");
	
	i=0;
	coordinate_b=malloc(sizeof(int));
	while (scanf("%d", &coordinate_b[i])) {
		if (coordinate_b[i]==0)
			break;
		i++;	
		coordinate_b=realloc(coordinate_b, (i+1)*sizeof(int));
	}
	length=0;
	length=i;
	printf("numero coordinate lette: %d\n", length);
	for (i=0; i<length; i+=7){
		territorio->block=nuovo_blocco(coordinate_b[i], coordinate_b[i+1], coordinate_b[i+2], coordinate_b[i+3], coordinate_b[i+4], coordinate_b[i+5], coordinate_b[i+6], territorio);
	}

	//territorio->block=nuovo_blocco(coordinate_b[0], coordinate_b[1], coordinate_b[2], coordinate_b[3], coordinate_b[4], coordinate_b[5], coordinate_b[6], territorio);
	printList_B(territorio, 8,9);
	//printList_B(territorio, 7, 6);
	printList_C(territorio);
	a=0;
	b=0;
	printf("Inserisci una cava da cercare\n");
	scanf("%d %d", &a, &b);
	trovato=cerca_cava(a, b, territorio);
	
	if (trovato != NULL)
		printf("Cava trovata\n");
	else
		printf("Cava non trovata\n");
	
	printList_C(trovato);
	
	
	

	
}