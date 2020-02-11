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
#define CONTENITORE "inside"



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




stanza_t** generastanze(int, char [],char[]); /*genera le stanze e inserisce gli oggetti*/
void stampastanze(stanza_t**,int);
void objprint(obj_t*h);/*stampa una lista di oggetti*/
void stampaoggetti(obj_t *);
obj_t * riempicontenitore(obj_t *, FILE *);
void delnewline(char []);
obj_t*objfind(obj_t*,char[]);/*trova un oggetto in una lista e restituisce l'indirizzo*/
int save(stanza_t**,int, char[], char[]);
obj_t * appendobj(obj_t *, char [], obj_t *);






int main (int argc, char*argv[]){
	stanza_t**stanze;
	int i;
	char carica,salva;
	char filestanze[50],fileobj[50], fstsave[50],fobjsave[50];
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

	for(i=1;i<=NSTANZE;i++)
		stampastanze(stanze,i);
	strcpy(fstsave,SAVEDIR);
	strcpy(fobjsave,SAVEDIR);
	strcat(fstsave,FSTANZE);
	strcat(fobjsave,FOBJSTANZE);


	if(save(stanze, NSTANZE, fstsave, fobjsave))
		printf("\nsalvataggio avvenuto con successo \n");
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
		printf("NORD %d, %d\n",stanza->nord->id, stanza->nordopen);
	if(stanza->sud)
		printf("SUD %d, %d\n",stanza->sud->id, stanza->sudopen);
	if(stanza->est)
		printf("EST %d, %d\n",stanza->est->id, stanza->estopen);
	if(stanza->ovest)
		printf("OVEST %d, %d\n",stanza->ovest->id, stanza->ovestopen);
	if(stanza->su)
		printf("SU %d, %d\n",stanza->su->id, stanza->suopen);
	if(stanza->giu)
		printf("GIU %d, %d\n",stanza->giu->id, stanza->giuopen);
	if(stanza->oggetti){
		stampaoggetti(stanza->oggetti);
		printf("\n");
	}
	printf("-------------\n\n");
}

void stampaoggetti(obj_t * h)
{
	obj_t * p;
	for(p=h; p; p=p->next){
		printf("%s ", p->nome);
		if(p->in){
			printf("(");
			stampaoggetti(p->in);
			printf(") ");
		}
	}

}


stanza_t** generastanze(int numstanze, char fst[],char fobj[]){
	FILE * fp;
	stanza_t * *stanze, *n, *stanza; /*giusto che siano 2 asterischi, perchè è una malloc di indirizzi*/
	int tmp, i, j;
	char nomeobj[LENOBJ];
	obj_t * obj;

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
				fscanf(fp, "%d", &stanza->nordopen);
				fscanf(fp, "%d", &stanza->sudopen);
				fscanf(fp, "%d", &stanza->estopen);
				fscanf(fp, "%d", &stanza->ovestopen);
				fscanf(fp, "%d", &stanza->suopen);
				fscanf(fp, "%d", &stanza->giuopen);

			}
			fclose(fp);
		}

		/*AGGIUNGO GLI OGGETTI*/
		if(fp=fopen(fobj, "r")){
			for(i=0; i<numstanze; i++){
				stanza=*(stanze+i);
				fscanf(fp, "%d", &tmp);
				for(j=0; j<tmp; j++){
					fgets(nomeobj, LENOBJ, fp);
					delnewline(nomeobj);		/* la fgets prende anche il carattere newline, che va eliminato */

					if(!strcmp(nomeobj, CONTENITORE))		/* la stringa CONTENITORE indica che gli oggetti */
						obj=riempicontenitore(obj, fp);					/* che seguono sono contenuti nel precedente */

					else
						stanza->oggetti=appendobj(stanza->oggetti, nomeobj, obj);
				}
			}
			fclose(fp);
		}


	}
	return stanze;
}

int save(stanza_t** stanze,int nstanze, char fst[], char fobj[]){
	FILE *fpst,*fpobj;
	stanza_t* stanza;
	obj_t*p,*q;
	int i, objcounter;

	if(fpst=fopen(fst,"w")){
		if(fpobj=fopen(fobj,"w")){
			for(i=0;i<nstanze;i++){
				stanza=*(stanze+i);
				//fprintf(fpst,"%d\t%d\t%d\t%d\t%d\t%d\n",stanza->nord->id,stanza->sud->id,stanza->est->id,stanza->ovest->id,stanza->su->id,stanza->giu->id);
				/*DA SISTEMARE: ci sono stanze senza oggetti e senza porte su alcune pareti (servono if, core dump)*/
				objcounter=0;
				for(p=stanza->oggetti;p;p=p->next)
					objcounter++;
				fprintf(fpobj,"%d ",objcounter);
				for(p=stanza->oggetti;p;p=p->next)
					fprintf(fpobj,"%s ",p->nome);
				fprintf(fpobj,"\n\n");
			}
			for(i=0;i<nstanze;i++){
				stanza=*(stanze+i);
				for(p=stanza->oggetti;p;p=p->next)
					if(p->in){
						objcounter=0;
						fprintf(fpobj,"%d %s ",i,p->nome);
						for(q=p->in;q;q=q->next)
							objcounter++;
						fprintf(fpobj,"%d ", objcounter);
						for(q=p->in;q;q=q->next)
							fprintf(fpobj, "%s ", q->nome);
						fprintf(fpobj, "\n");
					}
			}

			fclose(fpobj);
		}else return 0;
		fclose(fpst);
	}else return 0;
	return 1;
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

obj_t * objfind(obj_t*h,char obj[]){
	obj_t*p;
	for(p=h;h;p=p->next)
		if(!strcmp(obj,p->nome))
			return p;
	return NULL;
}



obj_t * appendobj(obj_t * h, char nomeobj[], obj_t * obj)
{
	obj_t * n, * p;

	if(n=malloc(sizeof(obj_t))){
		strcpy(n->nome, nomeobj);
		n->next=NULL;
		n->in=NULL;
		n->open=1;
		obj=n;
		if(h){
			for(p=h; p->next; p=p->next)
				;
			p->next=n;
		}else
			h=n;
	}else
		printf("appendobj: errore allocazione\n");

	return h;
}

obj_t * riempicontenitore(obj_t * h, FILE * fp)
{
	int n, i;
	char nomeobj[LENOBJ];
	obj_t * obj=NULL;

	fscanf(fp, "%d", &n);
	fgets(nomeobj, LENOBJ, fp);
	delnewline(nomeobj);
	h->in=appendobj(h->in, nomeobj, obj);

	if(n>1)
		for(i=0; i<n; i++){
			fgets(nomeobj, LENOBJ, fp);
			delnewline(nomeobj);
			h->in=appendobj(h->in, nomeobj, obj);
		}

	return h;

}

void delnewline(char s[]){
	int i;

	for(i=0; s[i]!='\n' && s[i]!='\0'; i++)
		;
	s[i]='\0';

}
