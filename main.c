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
	//ret=malloc(sizeof(cava *));
	for (p=testa; p != NULL; p=p->next) {
		if (p->x == cava_x & p->y==cava_y) {
			ret=p;
			p = NULL;	
		}			
	}
	return ret;	
}

//inserimento in testa del blocco
blocco* insert_blocco(int dim_x, int dim_y, int dim_z, int pos_x0, int pos_y0, blocco *primo_blocco) {
	/*dim_x, dim_y, dim_z sono le dimensioni del blocco */
	/* pos_x0, pos_y0 indicano le coordinate della posizione del blocco*/
	/* *primo_blocco è il puntatore al primo blocco*/
	/*ritorna la nuova testa della lista di blocchi (quindi il puntatore al primo blocco)*/
	blocco *nuovo_el;
	nuovo_el=malloc(sizeof(blocco));	
	nuovo_el->x0= pos_x0;
	nuovo_el ->y0 = pos_y0;
	nuovo_el ->x = dim_x;
	nuovo_el ->y = dim_y;
	nuovo_el ->z = dim_z;
	nuovo_el ->next=primo_blocco;
	return nuovo_el;
}

//cerca il blocco in una cava
blocco* cerca_blocco(int pos_x0, int pos_y0, blocco *primo_blocco){
	/* pos_x0, pos_y0 indicano le coordinate della posizione del blocco*/
	/* *primo_blocco è il puntatore al primo blocco*/
	/* ritorna la nuova testa della lista di blocchi */
	blocco *p, *ret;
	ret=NULL;
	
	//scorro i blocchi
	for (p=primo_blocco; p != NULL; p=p->next){
		//se lo trovo
		if (p->x0 == pos_x0 & p->y0 == pos_y0){
			//assegno al valore che torno il puntatore alla lista di blocchi
			ret=p;
			p=NULL;
		}
	}
	return ret;
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
	//c=malloc(sizeof(cava *));
	//cerco la cava nel territorio....NON FUNZIONA! FACCIO LA RICERCA DIRETTAMENTE QUI DENTRO
	//c=cerca_cava(testa, cava_x, cava_y);
	
	
	for (c=testa; c != NULL; c=c->next) {
		if (c->x ==cava_x & c->y==cava_y) {
			if (c->sito==0) {
			printf("ciao\n");
			//cerco il blocco per vedere se esiste già
			c->block=cerca_blocco(pos_x0, pos_y0, c->block);
			//se non c'è
				if (c->block == NULL){
					//inserisco il blocco in testa
					ret=insert_blocco(dim_x, dim_y, dim_z, pos_x0, pos_y0, c->block);
				}	
			}
		}					
	}
	
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
void printList_B(cava *testa){
	blocco *p;
	printf("lista posizione blocchi:");
	for (p=testa->block; p != NULL; p = p->next){
		printf(" x0:%d y0:%d; ", p->x0, p->y0 );
	}
	printf("\n");
}

//stampa la lista partendo dalla cava (in realtà dal sito) che passo
void printList_C(cava *testa){
	cava *p;
	printf("lista:");
	for (p=testa; p != NULL; p = p->next){
		printf(" %d %d;", p->x, p->y );
	}
	printf("\n");
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

int main( void ) { 
	int i, length, a, b;
	int *coordinate;
	int *coordinate_b;
	cava *territorio;
	territorio=NULL;

	
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
	printf("numero coordinate lette: %d\n", length);
	
	/*for (i=0; i<length; i++){
		printf("le coordinate nell'array sono:%d\n", coordinate[i]);
	}*/
	

	
	//i+=2 perchè mi sposto di due alla volta (coppia di coordinate)
	for(i=0; i<length; i+=2){
		territorio=nuova_cava(coordinate[i], coordinate[i+1], territorio);
	}
	
	printf("\n");
	printList_C(territorio);
	
	territorio=cava_esaurita(4,5,territorio);
	printList_C(territorio);
	
	printf("Inserisci un blocco in una cava. Specifica posizione e dimensione del blocco, e le coordinate della cava\n");
	
	i=0;
	coordinate_b=malloc(sizeof(int));
	while (i<7) {
		scanf("%d", &coordinate_b[i]);
		i++;	
		coordinate_b=realloc(coordinate_b, (i+1)*sizeof(int));
	}
	length=i;
	printf("numero coordinate lette: %d\n", length);
	for (i=0; i<length; i++){
		printf("le coordinate nell'array sono:%d\n", coordinate_b[i]);
	}

	territorio->block=nuovo_blocco(coordinate_b[0], coordinate_b[1], coordinate_b[2], coordinate_b[3], coordinate_b[4], coordinate_b[5], coordinate_b[6], territorio);
	printList_B(territorio);
	
	a=0;
	b=0;
	printf("Inserisci una cava da cercare\n");
	scanf("%d %d", &a, &b );
	territorio=cerca_cava(a, b, territorio);
	

	
}