#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#define VIVO 1
#define MORTO 0
#define QUIT 0

#define FSTANZE "stanze.txt"
#define DEFAULTDIR "./default/"
#define FOBJ "obj.txt"
#define NSTANZE 5
#define LENOBJ 15
#define CONTENITORE "in"



typedef struct _obj{
	char nome[LENOBJ];
	struct _obj *next;
	struct _obj *in;
	int open;
} obj_t;

typedef struct _stanza{
	int id;
	struct _stanza * nord;
	struct _stanza * sud;
	struct _stanza * est;
	struct _stanza * ovest;
	struct _stanza * su;
	struct _stanza * giu;
	int nordopen, sudopen, estopen, ovestopen, suopen, giuopen;
	obj_t * oggetti;
}stanza_t;




stanza_t** generastanze(int, char []);
void stampastanze(stanza_t**,int);
void delnewline(char []);






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
	}
	strcat(filestanze,FSTANZE);
	strcat(fileobj,FOBJ);

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
	printf("-------------\n\n");
}


stanza_t** generastanze(int numstanze, char fst[],char fobj[]){
	FILE * fp;
	stanza_t * *stanze, *n, *stanza; /*giusto che siano 2 asterischi, perchè è una malloc di indirizzi*/
	int tmp, i, j;
	char obj[LENOBJ]

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

				/*APERTURA/CHIUSURA PORTE*/
				fscanf(fp, "%d", stanza->nordopen);
				fscanf(fp, "%d", stanza->sudopen);
				fscanf(fp, "%d", stanza->estopen);
				fscanf(fp, "%d", stanza->ovestopen);
				fscanf(fp, "%d", stanza->suopen);
				fscanf(fp, "%d", stanza->giuopen);

			}
			fclose(fp);
		}

		/*AGGIUNGO GLI OGGETTI*/

		if(fp=fopen(fobj, "r")){
			for(i=0; i<numstanze; i++){
				fscanf(fp, "%d", &tmp);
				for(j=0; j<tmp; j++){
					fgets(obj, LENOBJ, fp);
					delnewline(obj);		/* la fgets prende anche il carattere newline, che va eliminato */

					if(!strcmp(obj, CONTENITORE))		/* la stringa CONTENITORE indica che gli oggetti */
						riempicontenitore();					/* che seguono sono contenuti nel precedente */

					else
						appendobj();
				}
			}
			fclose(fp);
		}


	}
	return stanze;

}

void riempicontenitore()

void delnewline(char s[]){
	int i;

	for(i=0; s[i]!='\n' && s[i]!='\0'; i++)
		;
	s[i]='\0';

}
