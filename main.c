#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define VIVO 1
#define MORTO 0
#define QUIT 0

#define FSTANZE "stanze.txt"
#define DEFAULTDIR "./default/"
#define SAVEDIR "./save/"
#define FOBJSTANZE "stanzeobj.txt"
#define NSTANZE 5
#define LENOBJ 15

typedef struct _obj{
	char nome[LENOBJ];
	struct _obj *next;
	struct _obj *in;
} obj_t;

typedef struct _stanza{
	int id;
	struct _stanza * nord;
	struct _stanza * sud;
	struct _stanza * est;
	struct _stanza * ovest;
	struct _stanza * su;
	struct _stanza * giu;
	obj_t * oggetti;
}stanza_t;

stanza_t** generastanze(int, char [],char[]); /*genera le stanze e inserisce gli oggetti*/
void stampastanze(stanza_t**,int);
obj_t * objappend(obj_t*,char[]); /*append per gli oggetti*/
void objprint(obj_t*h);/*stampa una lista di oggetti*/




int main (int argc, char*argv[]){
	stanza_t**stanze;
	int i;
	char carica;
	char filestanze[50],fileobj[50];
	printf("vuoi caricare il salvataggio? (y/n)  ");
	scanf("%c",&carica);
	printf("\n");
	if(carica=='n'){
		strcpy(filestanze,DEFAULTDIR);
		strcpy(fileobj,DEFAULTDIR);
	} else{
		strcpy(filestanze,SAVEDIR);
		strcpy(fileobj,SAVEDIR);	
	}
	strcat(filestanze,FSTANZE);
	strcat(fileobj,FOBJSTANZE);

	stanze=generastanze(NSTANZE,filestanze,fileobj);

	for(i=0;i<NSTANZE;i++)
		stampastanze(stanze,i);
	return 0;
}

/*
int main(int argc, char*argv[]){
	int isvivo, win, esc;
	esc=QUIT;
	riempistanze(NSTANZE, FSTANZE);
	while(!esc){
		vita=VIVO;
		win=NOTWIN
		inizio();
		while(vita&&!win&&!esc){
			esegui();
		}
		if(win)
			vinto();
		else if(!vita)
			morto();
	}
	return 0;
}*/

void stampastanze(stanza_t** add,int n){
	stanza_t * stanza;
	stanza = *(add+n);
	printf("-------------\n");
	printf("STANZA %d\n",stanza->id);
	if(stanza->nord)
		printf("NORD %d\n",stanza->nord->id);
	if(stanza->sud)
		printf("SUD %d\n",stanza->sud->id);
	if(stanza->est)
		printf("EST %d\n",stanza->est->id);
	if(stanza->ovest)
		printf("OVEST %d\n",stanza->ovest->id);
	if(stanza->su)
		printf("SU %d\n",stanza->su->id);
	if(stanza->giu)
		printf("GIU %d\n",stanza->giu->id);
	printf("OGGETTI:\n");
	objprint(stanza->oggetti);
	printf("-------------\n\n");
}


stanza_t** generastanze(int numstanze, char fst[],char fobj[]){
	FILE * fp;
	stanza_t * *stanze, *n, *stanza; /*giusto che siano 2 asterischi, perchè è una malloc di indirizzi*/
	int tmp, i;
	obj_t * listaobjstanza=NULL;	/*variabili per raccolta oggetti*/
	int numobj,j;
	char tmpstr[LENOBJ];

	if(stanze=malloc(numstanze*sizeof(stanza_t*))){
		for(i=0;i<numstanze;i++)
			if(n=malloc(sizeof(stanza_t)))
				*(stanze+i)=n;	/*popola la lista delle stanze*/

		/*LEGGO FILE STANZE*/
		if(fp=fopen(fst, "r")){
			for(i=0;i<numstanze;i++){		/*assegna ad ogni stanza gli indirizzi delle stanze adiacenti*/
				stanza=*(stanze+i);
				stanza->id=i;
				/*NORD*/
				fscanf(fp,"%d",&tmp);
				if(tmp==-1)
					stanza->nord=NULL;
				else
					stanza->nord=*(stanze+tmp);

				/*SUD*/
				fscanf(fp,"%d",&tmp);
				if(tmp==-1)
					stanza->sud=NULL;
				else
					stanza->sud=*(stanze+tmp);

				/*EST*/
				fscanf(fp,"%d",&tmp);
				if(tmp==-1)
					stanza->est=NULL;
				else
					stanza->est=*(stanze+tmp);

				/*OVEST*/
				fscanf(fp,"%d",&tmp);
				if(tmp==-1)
					stanza->ovest=NULL;
				else
					stanza->ovest=*(stanze+tmp);

				/*SU*/
				fscanf(fp,"%d",&tmp);
				if(tmp==-1)
					stanza->su=NULL;
				else
					stanza->su=*(stanze+tmp);

				/*GIU*/
				fscanf(fp,"%d",&tmp);
				if(tmp==-1)
					stanza->giu=NULL;
				else
					stanza->giu=*(stanze+tmp);

			}
			fclose(fp);
		}
		/*AGGIUNGO GLI OGGETTI*/
		/*nel file di oggetti, ogni riga corrisponde a una stanza. il primo dato è il numero di oggetti nella stanza, seguono gli oggetti stessi.*/
		if(fp=fopen(fobj,"r")){
			for(i=0;i<numstanze;i++){
				fscanf(fp,"%d ",&numobj);
				for(j=0;j<numobj;j++){
					fscanf(fp,"%s ",tmpstr);
					listaobjstanza=objappend(listaobjstanza,tmpstr);
				}
				stanza=*(stanze+i);
				stanza->oggetti=listaobjstanza;
				listaobjstanza=NULL;
			}

			/*BAULI*/
		}else
		 printf("not in");
	}
	return stanze;
}

obj_t * objappend(obj_t*h,char obj[]){
	obj_t*p,*n;
	if(n=malloc(sizeof(obj_t))){
		if(h){
			for(p=h;p->next;p=p->next)
				;
			p->next=n;
		} else
			h=n;
		strcpy(n->nome,obj);
		n->next=NULL;
		n->in=NULL;

	}
	return h;

}
void objprint(obj_t*h){
	obj_t* p;
	for(p=h;p;p=p->next){
		printf("%s, ",p->nome);
	}
	printf("\n");
	return;
}
