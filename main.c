#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

enum {
	NORD = 0,
	SUD = 1,
	EST = 2,
	OVEST = 3} direzioni;

//struttura di tipo blocco
typedef struct blocco {
	int x0, y0; /* valore dell'elemento: coordinate del blocco */
	int x,y,z; /*dimensioni del blocco*/
	struct blocco *next; /*puntatore al blocco successivo */
};

typedef struct blocco blocco;

typedef struct strade {
	struct nodo *finale;
	struct strade *next;
};
typedef struct strade strade;

//struttura di tipo cava determinata da due interi, un puntatore ai blocchi contenuti e un puntatore alla cava successiva
typedef struct cava {
	int x, y; /* valore dell'elemento: coordinate della cava */
	int sito; /* 1 se la cava e' un sito, altrimenti 0*/
	struct blocco *block; /* indirizzo del primo blocco nella cava */
	int *mappa; /* puntatore alla mappa*/
	struct strade *vie; /*puntatore alla prima soluzione*/
	int x_min; /* estremi della mappa */
	int x_max; /* estremi della mappa */
	int y_min; /* estremi della mappa */
	int y_max; /* estremi della mappa */
	struct cava *next; /* puntatore al prossimo elemento cava */
};

typedef struct cava cava;

typedef struct sites {
	cava *site1;
	cava *site2;
};
typedef struct sites sites;

typedef struct nodo {
	int x;
	int y;
	struct nodo *nord;
	struct nodo *sud;
	struct nodo *est;
	struct nodo *ovest;
	struct nodo *padre;
};
typedef struct nodo nodo;



//funzione che verifica l'esistenza di un elemento (cava) all'interno di una lista (terriotorio) ....NON FUNZIONA
cava* cerca_cava(int cava_x, int cava_y, cava *testa){
	cava *p,*ret;
	ret=NULL;
	
	for (p=testa; p != NULL; p=p->next) {
		if ((p->x == cava_x) & (p->y==cava_y)) {
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
	/* *primo_blocco �� il puntatore al primo blocco*/
	/*ritorna la nuova testa della lista di blocchi (quindi il puntatore al primo blocco)*/
	blocco *nuovo_el;
	nuovo_el=malloc(sizeof(blocco));
	nuovo_el -> x0 = pos_x0;
	nuovo_el -> y0 = pos_y0;
	nuovo_el -> x = dim_x;
	nuovo_el -> y = dim_y;
	nuovo_el -> z = dim_z;
	nuovo_el -> next=primo_blocco;
	return nuovo_el;
}

//cerca il blocco in una cava
int cerca_blocco(int pos_x0, int pos_y0, blocco *primo_blocco){
	/* pos_x0, pos_y0 indicano le coordinate della posizione del blocco*/
	/* *primo_blocco �� il puntatore al primo blocco*/
	/* ritorna la nuova testa della lista di blocchi */
	blocco *p;
	int trovato=0;
	
	//scorro i blocchi
	for (p=primo_blocco; p != NULL; p=p->next){
		//se lo trovo
		if ((p->x0 == pos_x0) & (p->y0 == pos_y0)){
			//se trovo il blocco
			trovato=1;
		}
	}
	return trovato;
}

//inserimento del blocco nella prima cava
blocco* nuovo_blocco(int cava_x, int cava_y, int dim_x, int dim_y, int dim_z, int pos_x0, int pos_y0, cava *testa) {
	/* cava_x, int cava_y sono le coordinate della cava nel quale va inserito il blocco */
	/* int dim_x, int dim_y, int dim_z sono le dimensioni del blocco */
	/* int pos_x0, int pos_y0 sono le coordinate della posizione in cui inserire il blocco */
	/* *testa �� il puntatore alla lista di cave */
	/* restituisco la testa della lista di blocchi o NULL se non faccio nulla */
	cava *c;
	blocco *ret;
	ret=NULL;
	int found=0;
	
	
	//scorro le cave
	for (c=testa; c != NULL; c=c->next) {
		//se trovo la cava passata in argomento
		if ((c->x == cava_x) & (c->y == cava_y)) {
			//e se �� un sito
			if (c->sito==1) {
			
				//cerco il blocco per vedere se esiste gi��
				found=cerca_blocco(pos_x0, pos_y0, c->block);
				//se il blocco non c'��
				if (found==0){
					//printf ("Le coordinate della cava sono: \n c->x:%d - c->y: %d\n", c->x, c->y);
					//printf ("Il campo blocco della cava punta a %p\n", c->block);
					//inserisco il blocco in testa
					//printf ("Il campo blocco, prima della insert, della cava punta a %p\n", c->block);
					c->block=insert_blocco(dim_x, dim_y, dim_z, pos_x0, pos_y0, c->block);
					//printf ("Il campo blocco della cava punta a %p\n", c->block);
					ret=c->block;
				}	
			}
		}					
	}
	return ret;	
}

//inserimento in testa della cava
cava* insert_cava(int cava_x, int cava_y, cava *testa) {
	/*cava_x e cava_b sono le coordinate x e y della cava; */
	/* testa �� il territorio (puntatore alla testa della lista)*/
	/*ritorna la nuova testa della lista*/
	cava *nuovo_el;
	nuovo_el=malloc(sizeof(cava));
	nuovo_el->sito=0;
	nuovo_el ->x=cava_x;
	nuovo_el ->y=cava_y;
	nuovo_el ->block=NULL;
	nuovo_el ->next=testa;
	nuovo_el ->x_min=0;
	nuovo_el ->x_max=0;
	nuovo_el ->y_min=0;
	nuovo_el ->y_max=0;
	nuovo_el ->mappa=NULL;
	nuovo_el ->vie=NULL;
	return nuovo_el;
}

//stampa la lista dei blocchi della cava che passo 
void printList_B(cava *testa){
	cava *c;
	blocco *p;
	for (c=testa; c!=NULL; c = c->next) {
		if (c->sito == 1 ){
			printf("I blocchi della cava con coordinate %d %d:\n", c->x, c->y );
			for (p=c->block; p != NULL; p = p->next){
				//printf ("ciao");
				printf(" x0:%d y0:%d\n ", p->x0, p->y0 );
			}
			
		}
	}
	
	printf("\n");
}

//stampa la lista partendo dalla cava (in realt�� dal sito) che passo
void printList_C(cava *testa){
	cava *p;
	p=testa;
	printf("Il territorio è composto dalle cave seguenti:\n");
	for (p=testa; p != NULL; p = p->next){
		printf(" %d %d\n", p->x, p->y );
	}
	printf("\n");
}

//creo una nuova cava nella lista passandogli la testa della lista e le coordinate
cava* nuova_cava(int cava_x, int cava_y, cava *t){
	cava *p;
	int esiste=0;
	//scorro la lista per vedere se esiste gia' una cava con coordinata x=a con quelle coordinate
	for (p=t; p != NULL; p=p->next) {
		//se esiste la sostituisco 
		if ((p->x) == cava_x) {
			(p->y) = cava_y;
			esiste=1;
		}	
	}
	//se non esiste la inserisco
	if (!esiste){
		t=insert_cava(cava_x, cava_y, t);	
	}	
	return t;	
}

//elimina dal territorio la cava indicata
cava* cava_esaurita(int a, int b, cava *t){
	/* int a, int b sono le coordinate della cava da eliminare*/
	/* *t �� il territorio*/
	/* restituisce il territorio*/
	cava* precedente;
	cava* attuale;
	precedente=NULL;
	attuale=t;
	/*scorro il territorio con 2 puntatori perch�� se trovo la cava, una volta eliminata, devo collegare la cava precedente
	a quella successiva a quella eliminata*/
	for (attuale=t; attuale != NULL; attuale=attuale->next){
		//se trovo la cava
		if (((attuale->x) == a) & ((attuale->y) == b)) {
			//se �� la prima
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

//per ora non restituisco nulla
void percorrenza_minima (cava *territorio) {
	cava *t;
	int y_min, y_max, i, somma, p_totale, j, y_minima;
	i=0;
	somma=0;
	if (territorio != NULL) {
		y_max=territorio->y;
		y_min=territorio->y;
	}
	//scorro il territorio per trovare la y minima e la y massima...RIFARLO MEMORIZZANDOLA IN FASE DI INSERIMENTO DELLE CAVE
	for (t=territorio; t != NULL; t=t->next){
		if (t->y < y_min)
			y_min=t->y;
		if (t->y >y_max)
			y_max=t->y;
	}
	//printf("min:%d max:%d\n", y_min, y_max);
	//printf("p_totale:%d\n", p_totale);
	
	for (j=y_min; j<=y_max; j++){

		somma=0;
		//scorro il territorio per sottrarre alle yi le y trovate
		for (t=territorio; t != NULL; t=t->next){
			somma=somma+abs((t->y - j));
			//printf("somma:%d\n", somma);
		}
		//al primo giro imposto un valore di percorrenza totale per fare poi i confronti
		if (j==y_min)
			p_totale=somma;
		//printf("sommatoria vale %d con j:%d\n", somma, j);
		if (somma<p_totale) {
			p_totale=somma;
			y_minima=j;
		}			
	}
	printf("La percorrenza totale minina e' %d\n", p_totale);
	printf("La y che corrisponde alla percorrenza minina e' %d\n", y_minima);
	printf("\n");
}


//per ora non restituisco nulla
sites *siti_cerimoniali (cava *territorio) {
	/* *territorio è il territorio che passo alla funzione*/
	/* devo restituire la posizione dei siti*/
	cava *attuale, *successivo, *t;
	cava *sito1, *sito2;
	sites *siti;
	//coordinate dei siti che troveremo
	int x_a, y_a, x_b, y_b, x_a_def, x_b_def, y_a_def, y_b_def;
	int min, uno, due, lunghezza, lunghezza_minima;
	uno=0;
	due=0;
	min=0;
	x_a_def=0;
	x_b_def=0;
	y_a_def=0;
	y_b_def=0;
	siti=malloc(sizeof(sites));

	//devo settare la lunghezza minina a un valore	
	lunghezza_minima=INT_MAX;

	//scorro le cave
	for (attuale=territorio, successivo=territorio->next; attuale != NULL; successivo=successivo->next) {
		//e inizio con ipotizzare che il sito sia la prima cava
		x_a=attuale->x;
		y_a=attuale->y;
		x_b=successivo->x;
		y_b=successivo->y;
		lunghezza=0;
		//scorro il territorio per fare la differenza
		for (t=territorio; t != NULL; t=t->next){

				//calcolo il minimo valore assoluto
				uno=abs(t->x - x_a);
				due=abs(t->x - x_b);
				//memorizzo il valore minimo
				min=(uno<due)?uno:due;

				lunghezza=lunghezza+min;

		}
		

		//se ne trovo uno inferiore	
		if (lunghezza<lunghezza_minima) {
			lunghezza_minima=lunghezza;
			x_a_def=x_a;
			x_b_def=x_b;
			y_a_def=y_a;
			y_b_def=y_b;
			sito1=attuale;
			sito2=successivo;

		}

		//se ho confrontato la prima cava con tutte le altre, e quindi sono alla fine	
		if (successivo->next == NULL) {
			//passo al confronto della seconda cava (attuale=attuale->next) con tutte le altre da lì in avanti (risetto successivo=attuale)
			successivo=attuale;
			attuale=attuale->next;
		}
	}
	sito1->sito=1;
	sito2->sito=1;
	siti->site1=sito1;
	siti->site2=sito2;


	//printf("La minima somma totale delle lunghezze è %d\n", lunghezza_minima);
	printf("Le coordinate dei due siti sono: x_a:%d, y_a:%d,  x_b:%d, y_b:%d, \n", x_a_def, y_a_def, x_b_def, y_b_def);
	//printf("Il campo sito della cava %d è %d\n", sito1->x, sito1->sito);
	return siti;
}

void stampa_mappa(cava *territorio){

	cava *t;
	int i=0;
	int j=0;
	int pos=0;

	//scorro il territorio alla ricerca del sito
	for (t=territorio; t!=NULL; t=t->next) {
		if (t->sito==1){
			printf("sito: %d %d\n", t->x, t->y);
			for (j=t->y_max-t->y_min; j>=0; j--){
				for (i=0; i<=(t->x_max)-(t->x_min); i++){
					pos=(((t->x_max)-(t->x_min)+1)*j)+i;
					printf("%d - ", t->mappa[pos]);

				}
				printf("\n");
			}
			printf("\n");
		}
	}


}

//funzione che verifica l'occupazione delle celle comprese tra x_max e x_min, e tra y_min e y_max e
//restituisce 1 se occupata o 0 se libera
int verifica_copertura(cava *sito, int x_min, int x_max, int y_min, int y_max){
	int stato=0;
	int *m;
	int i=0;
	int j=0;
	int pos=0;
	int CELLE_X;
	blocco *b;
	b=NULL;
	CELLE_X=(sito->x_max)-(sito->x_min)+1;
	m=sito->mappa;
	printf("coordinate: %d %d %d %d\n", x_min, x_max, y_min, y_max );
	//verificare se le posizioni nella mappa sono a occupate o libere
	for (j=y_min; j<=y_max; j++){
		if ((j<=sito->y_max) & (j>=sito->y_min)){
			for (i=x_min; i<=x_max; i++){
				if ((i<=sito->x_max) & (i>=sito->x_min)){
					pos=(j-(sito->y_min))*(CELLE_X)+(i-(sito->x_min));
					printf("pos:%d\n",pos);
					if (m[pos]!=0)
						stato=1;
				}
			}
		}
	}

	return stato;
}

//funziona che crea una mappa (vettore) che evidenzia le posizioni occupate
void crea_mappa(cava *territorio) {
	cava *t;
	blocco *b;
	blocco *b1;
	int *mappa=NULL;
	int i, j, pos;
	int X_MIN;
	int X_MAX;
	int Y_MIN;
	int Y_MAX;
	int CELLE_X;
	int CELLE_Y;

	//scorro il territorio alla ricerca del sito
	for (t=territorio; t!=NULL; t=t->next) {
		if (t->sito==1){
			//se esistono blocchi nella cava
			if (t->block != NULL){
				b1=t->block;
				X_MIN=b1->x0;
				X_MAX=b1->x0+b1->x-1;
				Y_MIN=b1->y0;
				Y_MAX=b1->y0+b1->y-1;

				//scorro i blocchi per trovare le dimensioni minime che serviranno per allocare la mappa
				for (b=b1->next; b!=NULL; b=b->next){
					if (b->x0<X_MIN) {
						X_MIN=b->x0;
					}
					if (b->x0+b->x-1>X_MAX) {
						X_MAX=b->x0+b->x-1;
					}
					if (b->y0<Y_MIN) {
						Y_MIN=b->y0;
					}
					if (b->y0+b->y-1>Y_MAX) {
						Y_MAX=b->y0+b->y-1;
					}

				}

			}
			printf("X_MIN:%d, X_MAX:%d, Y_MIN:%d, Y_MAX:%d\n", X_MIN, X_MAX, Y_MIN, Y_MAX);
			if (t->mappa!=NULL){
				free(t->mappa);
			}
			CELLE_X=X_MAX-X_MIN+1;
			CELLE_Y=Y_MAX-Y_MIN+1;
			mappa=malloc(sizeof(int)*(CELLE_X)*(CELLE_Y));

			//azzero l'array
			for (i=0; i<((CELLE_X)*(CELLE_Y)); i++){
				mappa[i]=0;
			}
			t->mappa=mappa;
			t->x_min=X_MIN;
			t->x_max=X_MAX;
			t->y_min=Y_MIN;
			t->y_max=Y_MAX;


			//riscorro i blocchi per leggere le coordinate e popolare la mappa
			for (b=t->block; b!=NULL; b=b->next){
				for (j=0;j<b->y;j++){
					for (i=0;i<b->x;i++){
						pos=(((b->y0)+j)-Y_MIN)*(CELLE_X)+(((b->x0)+i)-X_MIN);
						mappa[pos]=b->z;
						printf("pos:%d - %d\n ", pos,mappa[pos]);
					}
				}
			}


			/*for (j=Y_MAX-Y_MIN-1; j>=0; j--){
				for (i=0; i<=X_MAX-X_MIN-1; i++){
					pos=((X_MAX-X_MIN)*j)+i;
					printf("%d - ", mappa[pos]);

				}
				printf("\n");
			}*/
			printf("\n");
			//printf("%d - %d - %d - %d", t->x_min, t->x_max, t->y_min, t->y_max);
			//printf("\n");
		}

	}

}

int rotazione(int pos_x0, int pos_y0, cava *territorio, int sito_x, int sito_y, int rotazione) {
	/*sito_x e sito_y sono le coordinate del sito*/
	/*pos_x0 e pos_y0 sono le coordinate del blocco*/
	/*rotazione indica la direzione della rotazione*/

	cava *t;
	blocco *attuale;
	int ok=0, dim_x=0, dim_y=0, dim_z=0;


	//scorro il territorio per verificare che il sito passato sia presente sul territorio
	for (t=territorio; t!=NULL; t=t->next){
		//se la cava esiste
		if ((t->x==sito_x) & (t->y==sito_y) ){
			//e se è un sito
			if (t->sito==1) {
				//scorro i blocchi per trovare quello corretto
				for (attuale=t->block; attuale != NULL; attuale=attuale->next){
					//se trovo il blocco passato alla funzione
					if ((attuale->x0==pos_x0) & (attuale->y0==pos_y0)) {
						dim_y=attuale->y;
						dim_z=attuale->z;
						dim_x=attuale->x;

						switch (rotazione){
						case NORD:
							printf("nord\n");
							printf("x0:%d, y0:%d, x:%d, y:%d, z:%d\n", attuale->x0, attuale->y0, attuale->x, attuale->y, attuale->z);
							ok=verifica_copertura(t, attuale->x0, attuale->x0+attuale->x-1, pos_y0+dim_y, pos_y0+dim_y+dim_z-1);
							printf("ok: %d\n", ok);
							if (ok==1)
								printf("rotazione non possibile\n");
							else {
								attuale->y=dim_z;
								attuale->z=dim_y;
								attuale->y0=pos_y0+dim_y;
								printf("x0:%d, y0:%d, x:%d, y:%d, z:%d\n", attuale->x0, attuale->y0, attuale->x, attuale->y, attuale->z );
							}
							break;
						case EST:
							printf("est\n");
							printf("x0:%d, y0:%d, x:%d, y:%d, z:%d\n", attuale->x0, attuale->y0, attuale->x, attuale->y, attuale->z);
							ok=verifica_copertura(t,attuale->x0+attuale->x, attuale->x0+attuale->x+attuale->z-1, attuale->y0, attuale->y0+attuale->y-1 );
							printf("ok: %d\n", ok);
							if (ok==1)
								printf("rotazione non possibile\n");
							else {
							attuale->x=dim_z;
							attuale->z=dim_x;
							attuale->x0=pos_x0+dim_x;
							printf("x0:%d, y0:%d, x:%d, y:%d, z:%d\n", attuale->x0, attuale->y0, attuale->x, attuale->y, attuale->z );
							}
							break;
						case SUD:
							printf("sud\n");
							printf("x0:%d, y0:%d, x:%d, y:%d, z:%d\n", attuale->x0, attuale->y0, attuale->x, attuale->y, attuale->z);
							ok=verifica_copertura(t, attuale->x0-t->x_min, attuale->x0+attuale->x-t->x_max, attuale->y0-attuale->z-t->y_min, attuale->y0-t->y_max);
							printf("ok: %d\n", ok);
							if (ok==1)
								printf("rotazione non possibile\n");
							else {
								attuale->y=dim_z;
								attuale->z=dim_y;
								attuale->y0=pos_y0-dim_z;
								printf("x0:%d, y0:%d, x:%d, y:%d, z:%d\n", attuale->x0, attuale->y0, attuale->x, attuale->y, attuale->z );
							}
							break;
						case OVEST:
							printf("ovest\n");
							printf("x0:%d, y0:%d, x:%d, y:%d, z:%d\n", attuale->x0, attuale->y0, attuale->x, attuale->y, attuale->z);
							ok=verifica_copertura(t, attuale->x0-attuale->z, attuale->x0-1, attuale->y0, attuale->y0+attuale->y-1);
							printf("ok: %d\n", ok);
							if (ok==1)
								printf("rotazione non possibile\n");
							else {
								attuale->x=dim_z;
								attuale->z=dim_x;
								attuale->x0=pos_x0-dim_z;
								printf("x0:%d, y0:%d, x:%d, y:%d, z:%d\n", attuale->x0, attuale->y0, attuale->x, attuale->y, attuale->z );
							}
							break;
						}
						crea_mappa(territorio);
						//non rientro più nel for
						//attuale=NULL;
					}
				}
			}
		}
	}
	return ok;
}






cava *leggi_cave(char *nomefile){
	int *coordinate;
	int i=0;
	int length=0;
	int result=0;
	cava *territorio=NULL;
	FILE *cave;



	//alloco spazio per array che conterrà le coordinate
	coordinate=malloc(sizeof(int));

	cave=fopen(nomefile,"r");
	//se non riesco ad aprire il file
	if (cave == NULL){
		result=1;
	}
	else {
		//leggo le coordinate e le memorizzo in un array
		while (fscanf(cave, "%d", &coordinate[i])) {
			if (coordinate[i]==0)
				break;
			i++;
			coordinate=realloc(coordinate, (i+1)*sizeof(int));
		}
		fclose(cave);
		length=i;
		//printf("length:%d\n", length);
		//creo le cave
		//i+=2 perche' mi sposto di due alla volta (coppia di coordinate)
		for(i=0; i<length; i+=2){
			territorio=nuova_cava(coordinate[i], coordinate[i+1], territorio);
		}
	}
	return territorio;
}

int leggi_blocchi(char *nomefile, cava *territorio ){
	int *coordinate_b;
	int i=0;
	int length=0;
	FILE *blocchi;
	int result=0;

	coordinate_b=malloc(sizeof(int));

	printf("Leggo le coordinate dei blocchi dal file blocchi.txt\n");
	//printf("Inserisci un blocco in una cava. Specifica le coordinate della cava, la posizione e dimensione del blocco, terminate da uno zero\n");
	blocchi=fopen(nomefile,"r");
	if (blocchi == NULL){
		printf("Non riesco a leggere il file con i blocchi\n");
		result=1;
	}
	else {
		i=0;
		coordinate_b=malloc(sizeof(int));
		while (fscanf(blocchi, "%d", &coordinate_b[i])) {
			if (coordinate_b[i]==9999)
				break;
			i++;
			coordinate_b=realloc(coordinate_b, (i+1)*sizeof(int));
		}
		fclose(blocchi);
		//length=0;
		length=i;
		printf("numero coordinate lette: %d\n", length);
		//creo i blocchi
		for (i=0; i<length; i+=7){
			territorio->block=nuovo_blocco(coordinate_b[i], coordinate_b[i+1], coordinate_b[i+2], coordinate_b[i+3], coordinate_b[i+4], coordinate_b[i+5], coordinate_b[i+6], territorio);
		}
	}
	return result;
}

void stampa_siti(cava *territorio) {
	cava *t;

	for (t=territorio;t!=NULL; t=t->next){
		if (t->sito ==1) {
			printf("Le coordinate del sito sono: x=%d, y=%d\n", t->x, t->y);
		}
	}

}
//
blocco *crea_stele(int x0, int y0){
	blocco *stele;
	stele=malloc(sizeof(blocco));
	stele->x0=x0;
	stele->y0=y0;
	stele->x=1;
	stele->y=1;
	stele->z=2;
	stele->next=NULL;
	return stele;
	//verificare se posizione libera
}




nodo *creo_percorso(int x0, int y0, int xf, int yf, int xa, int ya, int x, int y, int z, cava *sito) {
	//x0 e y0 sono le coordinate di partenza della stele
	//xf e yf sono le coordinate di arrivo della stele
	//xa e ya sono le coordinate della posizione attuale
	//x, y e z sono le dimesioni della stele
	//ritorna il puntatore al nodo della posizione attuale
	nodo *nodo_attuale;
	strade *via;

	//verifico di non aver raggiunto la destinazione
	if ((x0!=xf) & (y0!=yf)) {

		//verifico che le coordinate
		if ((xa>=x0) & (xa<=xf) & (ya<=y0) & (ya>=yf) ) {
			nodo_attuale=malloc(sizeof(nodo));
			nodo_attuale->x=xa;
			nodo_attuale->y=ya;
			nodo_attuale->sud=NULL;
			nodo_attuale->nord=NULL;
			nodo_attuale->est=NULL;
			nodo_attuale->ovest=NULL;
			nodo_attuale->padre=NULL;
			//sud
			if (((xa+x-1)<=xf) & ((ya-z)>=y0) & ((ya-1)>=yf)){
				if (verifica_copertura(sito, xa, xa+x-1, ya-z, ya-1) ==0) {
					printf("\nsud:");
					nodo_attuale->sud=creo_percorso(x0, y0, xf, yf, xa, ya-z, x, z, y, sito);
				}
			}
			//est
			if ((( xa+x)>=x0) & ((xa+x+z-1)<=xf) & ((ya)>=y0) & ((ya+y-1)>=yf)){
				if (verifica_copertura(sito, xa+x, xa+x+z-1, ya, ya+y-1)==0) {
					//printf("\nest:");
					nodo_attuale->est=creo_percorso(x0, y0, xf, yf, xa+x, ya, z, y, x, sito);
				}
			}

			//nord
			if (((xa+x-1)<=xf) & ((ya+y-1)>=y0) & ((ya+y+z-1)>=yf)){
				if (verifica_copertura(sito, xa, xa+x-1, ya+y-1, ya+y+z-1) ==0)
					nodo_attuale->nord=creo_percorso(x0, y0, xf, yf, xa, ya+y-1, x, z, y, sito);
			}
			//ovest
			if ((( xa-z)>=x0) & ((xa-1)<=xf) & ((ya)>=y0) & ((ya+y-1)>=yf)){
				if (verifica_copertura(sito, xa-z, xa-1, ya, ya+y-1 ) ==0)
					nodo_attuale->ovest=creo_percorso(x0, y0, xf, yf, xa-z, ya, z, y, x, sito);
			}
		}
	} else {
		if (z==2)
			via=malloc(sizeof(strade));
			via->finale=nodo_attuale;
			via->next=sito->vie;
			sito->vie=via;
	}
	printf("nodo attuale:%d\n", nodo_attuale->x);
	printf("xa:%d, ya:%d\n", xa, ya);
	return nodo_attuale;


}






//fare funzione per passare il file rotazione.txt
int main( int argc, char *argv[]) {
	int file_c=0;
	int file_b=0;
	sites *siti;
	cava *territorio;
	cava *trovato;
	blocco *ruotato;
	blocco *stele;
	nodo *partenza;


	partenza=NULL;
	trovato=NULL;
	territorio=NULL;
	trovato=malloc(sizeof(cava));



	//se argc<2 esci
	if (argc<2){
		printf("Non hai passato tutti i file richiesti che devono essere almeno due: uno per le cave e uno per i blocchi\n");
	}
	else {
		printf("Leggo le coordinate delle cave dal file %s e creo il territorio\n", argv[1]);
		//creo le cave leggendo il file passato al main
		territorio=leggi_cave(argv[1]);

		//chiamo la funzione
		if (file_c==0){
			printf("Ho creato correttamente le cave\n");
		}

		//stampo le cave che compongno il territorio
		printList_C(territorio);

		//determino i siti
		siti=siti_cerimoniali(territorio);

		//stampo i siti
		stampa_siti(territorio);

		printf("Leggo le coordinate dei blocchi  dal file %s e creo il territorio\n", argv[2]);
		//creo i blocchi
		file_b=leggi_blocchi(argv[2], territorio);

		//creo le mappe con l'occupazione e le stampo
		crea_mappa(territorio);
		stampa_mappa(territorio);

		//stampo i blocchi
		printList_B(territorio);

		//ruoto il blocco in posizione 6,7 che si trova nel sito -4,3 in direzione ...
		ruotato=rotazione(-1,-4,territorio,-4,3, EST);
		//stampa_mappa(territorio);
		ruotato=rotazione(-3,-5,territorio,-4,3, SUD);
		//stampa_mappa(territorio);
		ruotato=rotazione(4,0,territorio,-4,3, OVEST);
		//stampa_mappa(territorio);
		ruotato=rotazione(2,2,territorio,-4,3, NORD);
		stampa_mappa(territorio);

		partenza=creo_percorso(0,0, 7, -5, 0, 0, 1, 1, 2, siti->site1);


		//creo la mappa con l'occupazione
		//crea_mappa(territorio);
		/*a=0;
		b=0;
		printf("Inserisci una cava da cercare\n");
		scanf("%d %d", &a, &b);
		trovato=cerca_cava(a, b, territorio);

		if (trovato != NULL)
			printf("Cava trovata\n");
		else
			printf("Cava non trovata\n");*/

	}
	
}
