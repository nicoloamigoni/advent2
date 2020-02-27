#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#define VIVO 1
#define MORTO 0
#define QUIT 0
#define NOLINK -1

#define FSTANZE "stanze.txt"
#define DEFAULTDIR "./default/"
#define FOBJ "stanzeobj.txt"
#define SAVEDIR "./save/"
#define SAVESTANZE "savestanze.txt"
#define SAVEOBJ "saveobj.txt"
#define FACT "./default/act.txt"
#define NSTANZE 22
#define LENOBJ 30
#define LENCMD 100
#define LENACT 20
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

void inizio(stanza_t **);

stanza_t** generastanze(int, char [], char[]);
void salva(int, stanza_t **, char [], char []);
void salvaoggetti(obj_t *, FILE *);
void stampastanze(stanza_t**,int);
void stampaoggetti(obj_t *);
obj_t * appendobj(obj_t *, char [], obj_t **);
obj_t * extractobj(obj_t** h, char []);
obj_t * findobj(obj_t *, char []);
obj_t * riempicontenitore(obj_t *, FILE *);
void delcarret(char []);
void delnewline(char []);
void delspace(char []);

void interpretacomando();
int findact(char []);
int vai(char []);
int prendi(char[]);
int lascia(char[]);
int apri(char []);
int apriporta(char []);
int suona(char []);






stanza_t* player=NULL;
obj_t* inventario=NULL;
int time=0;

int main (int argc, char*argv[]){
	int i;
	stanza_t**stanze=NULL;
	inizio(stanze);

/*	printf("vuoi salvare? (y/n)   ");
	scanf("%c",&save);
	printf("\n");
	if(save=='y'){

	}
	strcpy(salvastanze,SAVEDIR);
	strcpy(salvaobj,SAVEDIR);
	strcat(salvastanze,SAVESTANZE);
	strcat(salvaobj,SAVEOBJ);

	salva(NSTANZE, stanze, salvastanze, salvaobj);*/
/*	for(i=0;i<NSTANZE;i++)
		stampastanze(stanze,i);*/
	printf("All'inizio, il player è in %d\n",player->id);
	printf("L'inventario è: ");
	stampaoggetti(inventario);
	printf("\n\n");
	printf("Cosa faccio?\n");
	scanf(" "); /*Risolve il problema "Comando non trovato"*/
	while(1){
		interpretacomando();
		printf("Il player è nella stanza %d\n",player->id);
		printf("L'inventario è: ");
		stampaoggetti(inventario);
		printf("\nNella stanza c'è: ");
		stampaoggetti(player->oggetti);
		time++;
		printf("\nTime = %d", time);
		printf("\n\n");
		printf("Cosa faccio?\n");
	}

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

void inizio(stanza_t **stanze){
	char carica;
	char filestanze[50],fileobj[50];

	printf("vuoi caricare il salvataggio? (y/n)  ");
	scanf("%c",&carica);
	printf("\n");
	if(carica=='y'){
		strcpy(filestanze,SAVEDIR);
		strcpy(fileobj,SAVEDIR);
		strcat(filestanze,SAVESTANZE);
		strcat(fileobj,SAVEOBJ);
	}else{
		strcpy(filestanze,DEFAULTDIR);
		strcpy(fileobj,DEFAULTDIR);
		strcat(filestanze,FSTANZE);
		strcat(fileobj,FOBJ);
	}


	stanze=generastanze(NSTANZE,filestanze,fileobj);

	player=*(stanze); /*partiamo dalla prima stanza*/
}

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
void stampaoggetti(obj_t * h){
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
	obj_t * obj=NULL;

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
				stanza->oggetti=NULL;
				fscanf(fp, "%d", &tmp);
				for(j=0; j<tmp; j++){
					fgets(nomeobj, LENOBJ, fp);
	/*				for(int k=0; nomeobj[k]!='\0'; k++)
						printf("%d ", nomeobj[k]);
					printf("\n");*/
					delcarret(nomeobj);						/* nel file viene inserito a fine riga il carattere carriage return	*/
					delnewline(nomeobj);					/* la fgets prende anche il carattere new line, che va eliminato */
					delspace(nomeobj);

					if(!strcmp(nomeobj, CONTENITORE))							/* la stringa CONTENITORE indica che gli oggetti  */
						obj=riempicontenitore(obj, fp);					/* che seguono sono contenuti nel precedente */

					else
						stanza->oggetti=appendobj(stanza->oggetti, nomeobj, &obj);

				}
			}
			fclose(fp);
		}else
			printf("errore accesso a file %s\n", fobj);


	}

	return stanze;

}

obj_t * appendobj(obj_t * h, char nomeobj[], obj_t ** obj){
	obj_t * n, * p;

	if(n=malloc(sizeof(obj_t))){
		strcpy(n->nome, nomeobj);
		n->next=NULL;
		n->in=NULL;
		n->open=1;
		*obj=n;
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
obj_t * riempicontenitore(obj_t * h, FILE * fp){
	int n, i;
	char nomeobj[LENOBJ];
	obj_t * obj=NULL;

	fscanf(fp, "%d", &n);
	fgets(nomeobj, LENOBJ, fp);
	delcarret(nomeobj);
	delnewline(nomeobj);
	delspace(nomeobj);
	h->in=appendobj(h->in, nomeobj, &obj);

	if(n>1)
		for(i=1; i<n; i++){
			fgets(nomeobj, LENOBJ, fp);
			delcarret(nomeobj);
			delnewline(nomeobj);
			delspace(nomeobj);
			h->in=appendobj(h->in, nomeobj, &obj);
		}

	return h;

}

void salva(int numstanze, stanza_t ** stanze, char savestanze[], char saveobj[]){
	int i;
	stanza_t * stanza;
	FILE * fp;

	/* SALVATAGGIO STANZE*/

	if(fp=fopen(savestanze, "w")){
		for(i=0; i<numstanze; i++){
			stanza=*(stanze+i);
			if(stanza->nord)
				fprintf(fp, "%d ", stanza->nord->id);
			else
				fprintf(fp, "%d ", NOLINK);

			if(stanza->sud)
				fprintf(fp, "%d ", stanza->sud->id);
			else
				fprintf(fp, "%d ", NOLINK);

			if(stanza->est)
				fprintf(fp, "%d ", stanza->est->id);
			else
				fprintf(fp, "%d ", NOLINK);

			if(stanza->ovest)
				fprintf(fp, "%d ", stanza->ovest->id);
			else
				fprintf(fp, "%d ", NOLINK);

			if(stanza->su)
				fprintf(fp, "%d ", stanza->su->id);
			else
				fprintf(fp, "%d ", NOLINK);

			if(stanza->giu)
				fprintf(fp, "%d ", stanza->giu->id);
			else
				fprintf(fp, "%d ", NOLINK);

			fprintf(fp, "  ");
			fprintf(fp, "%d ", stanza->nordopen);
			fprintf(fp, "%d ", stanza->sudopen);
			fprintf(fp, "%d ", stanza->estopen);
			fprintf(fp, "%d ", stanza->ovestopen);
			fprintf(fp, "%d ", stanza->suopen);
			fprintf(fp, "%d ", stanza->giuopen);
			fprintf(fp, "\n");

		}
		fclose(fp);
	}else
		printf("errore accesso al file %s\n", savestanze);

	/*SALVATAGGIO OGGETTI*/

	if(fp=fopen(saveobj, "w")){
		for(i=0; i<numstanze; i++){
			stanza=*(stanze+i);
			salvaoggetti(stanza->oggetti, fp);
		}
		fclose(fp);
	}else
		printf("errore accesso al file %s\n", saveobj);

}


void salvaoggetti(obj_t * h, FILE * fp)
{
	int nobj;
	obj_t * p;

	nobj=0;
	if(h){
		for(p=h; p; p=p->next){
			nobj++;
			if(p->in)
				nobj++;
		}
		fprintf(fp, "%d ", nobj);
		fprintf(fp, "%s\n", h->nome);
		if(h->in){
			fprintf(fp, "%s\n", CONTENITORE);
			salvaoggetti(h->in, fp);
		}
		if(nobj>1)
			for(p=h->next; p; p=p->next){
				fprintf(fp, "%s\n", p->nome);
				if(p->in){
					fprintf(fp, "%s\n", CONTENITORE);
					salvaoggetti(p->in, fp);
				}
			}

	}else
		fprintf(fp, "%d\n", nobj);
}


obj_t* extractobj(obj_t** h, char obj[]){
	obj_t*p, *q;
	p=*h;
	if(*h){
		if(p->in){
			q=extractobj(&(p->in),obj);
			if(q)
				return q;
		} else if(!strcmp(p->nome, obj)){
			*h=p->next;
			p->next=NULL;
			return p;
		}
		else for(;p->next;p=p->next){
			if(p->next->in){
				q=extractobj(&(p->next->in),obj);
				if(q)
					return q;
			}else if(!strcmp(p->next->nome,obj)){
				q=p->next;
				p->next=q->next;
				q->next=NULL;
				return q;
			}

		}
	}
	return NULL;
}

obj_t * findobj(obj_t * h, char nomeobj[]){
	obj_t * obj, * p;

	for(obj=h; obj; obj=obj->next){
		if(!strcmp(obj->nome, nomeobj))
			return obj;
		else if(obj->in)
			if(p=findobj(obj->in, nomeobj))
				return p;
	}

	return NULL;
}

void interpretacomando(){
	char comando[LENCMD], azione[LENACT];
	int i, act, check;
	//fgets(comando,LENCMD,stdin); /*Non va bene nel ciclo..*/

	check=1;
	/*printf("Cosa faccio?\n");*/
	fgets(comando,LENCMD,stdin);
	delnewline(comando);
	for(i=0; comando[i] != ' ' && i<LENACT && comando[i]!='\0'; i++)
		azione[i]=comando[i];
	azione[i]='\0';
	act = findact(azione);

	switch(act){
		case 0: check=vai(&comando[i]);
						break;
		case 1: check=prendi(&comando[i]);
						break;
		case 2: check=lascia(&comando[i]);
						break;
		case 4: check=apri(&comando[i]);
						break;
		case 5: check=suona(&comando[i]);
						break;
		default: check=0;
	}


	while(!check){
		check=1;
		printf("\nCosa faccio?\n");
		fgets(comando,LENCMD,stdin);
		delnewline(comando);
		for(i=0; comando[i] != ' ' && i<LENACT && comando[i]!='\0'; i++)
			azione[i]=comando[i];
		azione[i]='\0';
		act = findact(azione);


		switch(act){
			case 0: check=vai(&comando[i]);
							break;
			case 1: check=prendi(&comando[i]);
							break;
			case 2: check=lascia(&comando[i]);
							break;
			case 4: check=apri(&comando[i]);
							break;
			case 5: check=suona(&comando[i]);
							break;
			default: check=0;
		}

	}

}


int findact(char azione[]){
	FILE*fp;
	int act;
	char verbo[LENACT];

	if(fp=fopen(FACT, "r")){
		fscanf(fp,"%d ",&act);
		while(!feof(fp)){
			fscanf(fp,"%s",verbo);
			if(!strcmp(verbo, azione)){
				fclose(fp);
				return act;
			}
			fscanf(fp,"%d ",&act);
		}
		printf("Comando non trovato\n");
		fclose(fp);
	}else
		printf("findact: errore acesso al file %s\n", FACT);
	return -1;
}



int vai(char comando[]){
	int i;
	i=0;
	do
		i++;
	while(comando[i]==' ');
	if(!strcmp(&comando[i],"nord")){
		if(player->nord)
			if(player->nordopen)
				player=player->nord;
			else
				printf("La porta è chiusa\n");
		else{
			printf("Non c'è alcun varco a nord\n");
			return 0;
		}
	}else if(!strcmp(&comando[i],"sud")){
		if(player->sud)
			if(player->sudopen)
				player=player->sud;
			else
				printf("La porta è chiusa\n");
		else{
			printf("Non c'è alcun varco a sud\n");
			return 0;
		}
	}else if(!strcmp(&comando[i],"est")){
		if(player->est)
			if(player->estopen)
				player=player->est;
			else
				printf("La porta è chiusa\n");
		else{
			printf("Non c'è alcun varco a est\n");
			return 0;
		}
	}else if(!strcmp(&comando[i],"ovest")){
		if(player->ovest)
			if(player->ovestopen)
				player=player->ovest;
			else
				printf("La porta è chiusa\n");
		else{
			printf("Non c'è alcun varco a ovest\n");
			return 0;
		}
	}else if(!strcmp(&comando[i],"su")){
		if(player->su)
			if(player->suopen)
				player=player->su;
			else
				printf("La porta è chiusa\n");
		else{
			printf("Non c'è alcun varco su\n");
			return 0;
		}
	}else if(!strcmp(&comando[i],"giù")){
		if(player->giu)
			if(player->giuopen)
				player=player->giu;
			else
				printf("La porta è chiusa\n");
		else{
			printf("Non c'è alcun varco giù\n");
			return 0;
		}
	}else{
		printf("Direzione non trovata\n");
		return 0;
	}
	return 1;
}


int prendi(char comando[]){
	int i;
	obj_t*p,*q;
	i=0;
	do
		i++;
	while(comando[i]==' '&&comando[i]!='\0');
	if(comando[i]=='\0'){
		printf("Specifica l'oggetto da prendere\n");
		return 0;
	}

	if(p=extractobj(&(player->oggetti),&comando[i])){
		if(inventario){
			for(q=inventario;q->next;q=q->next)
				;
			q->next=p;
		}else
			inventario=p;
		printf("%s aggiunto al tuo inventario\n", p->nome);
		return 1;
	}
	printf("Oggetto non trovato: %s\n",&comando[i]);
	return 0;
}


int lascia(char comando[]){
	int i;
	obj_t*p,*q;
	i=0;
	do
		i++;
	while(comando[i]==' '&&comando[i]!='\0');
	if(comando[i]=='\0'){
		printf("Specifica l'oggetto da lasciare\n");
		return 0;
	}

	if(p=extractobj(&(inventario),&comando[i])){
		if(player->oggetti){
			for(q=player->oggetti;q->next;q=q->next)
				;
			q->next=p;
		}else
			player->oggetti=p;
		printf("%s lasciato nella stanza %d\n",p->nome,player->id);
		return 1;
	}
	printf("Oggetto non trovato: %s\n",&comando[i]);
	return 0;
}


int apri(char comando[]){
	int i, j;
	char s[LENOBJ];
	int check;
	obj_t * obj;

	i=0;
	do
		i++;
	while(comando[i]==' ' && comando[i]!='\0');
	if(comando[i]=='\0'){
		printf("Specifica cosa aprire\n");
		return 0;
	}

	for(j=0; comando[i] != ' ' && j<LENOBJ && comando[i]!='\0'; i++, j++)
		s[j]=comando[i];
	s[j]='\0';

	if(!strcmp(s, "porta"))
		check=apriporta(&comando[i]);
	else if(!strcmp(s, "baule")){
		if(obj=findobj(player->oggetti, "baule")){
			if(obj->open){
				printf("Il baule è già aperto\n");
				return 0;
			}else{
				do
					i++;
				while(comando[i]==' ' && comando[i]!='\0');
				if(comando[i]=='\0'){
					printf("Specifica con cosa aprire il baule\n");
					return 0;
				}

				for(j=0; comando[i] != ' ' && j<LENOBJ && comando[i]!='\0'; i++, j++)
					s[j]=comando[i];
				s[j]='\0';
				if(strcmp(s, "con")){
					printf("Specifica con cosa aprire il baule\n");
					return 0;
				}

				do
					i++;
				while(comando[i]==' ' && comando[i]!='\0');
				if(comando[i]=='\0'){
					printf("Specifica con cosa aprire il baule\n");
					return 0;
				}

				if(!strcmp(&comando[i], "piccola chiave")){
					if(findobj(inventario, "piccola chiave")){
						obj->open=1;
						printf("Il baule si apre\n");
						check=1;
					}else{
						printf("Non hai questo oggetto\n");
						return 0;
					}
				}else{
					printf("Non è l'oggetto giusto\n");
					check=1;
				}
			}
		}else{
			printf("Oggetto non trovato\n");
			return 0;
		}
	}else if(!strcmp(s, "armadio")){
		printf("L'armadio si apre\n");
		check=1;
	}else{
		printf("Non è possibile aprire %s\n", &comando[i]);
		return 0;
	}

	return check;

}


int apriporta(char comando[]){
	int i, j;
	char dir[6];
	char s[LENOBJ];

	i=0;
	do
		i++;
	while(comando[i]==' ' && comando[i]!='\0');
	if(comando[i]=='\0'){
		printf("Specifica quale porta aprire\n");
		return 0;
	}

	for(j=0; comando[i] != ' ' && j<6 && comando[i]!='\0'; i++, j++)
		dir[j]=comando[i];
	dir[j]='\0';

	if(!strcmp(dir, "nord")){
		switch(player->id){
			case 3: printf("La porta è già aperta\n");
							return 0;
			case 4: printf("La porta è già aperta\n");
							return 0;
			case 6: printf("La porta è già aperta\n");
							return 0;
			case 7:	printf("La porta è già aperta\n");
							return 0;
			case 10: printf("La porta è già aperta\n");
							 return 0;
			case 11: printf("La porta è già aperta\n");
							 return 0;
			case 16: printf("La porta è già aperta\n");
							 return 0;
			case 5: if(player->nordopen){
								printf("La porta è già aperta\n");
								return 0;
							}else{
								do
									i++;
								while(comando[i]==' ' && comando[i]!='\0');
								if(comando[i]=='\0'){
									printf("Specifica con cosa aprire la porta\n");
									return 0;
								}

								for(j=0; comando[i] != ' ' && j<LENOBJ && comando[i]!='\0'; i++, j++)
									s[j]=comando[i];
								s[j]='\0';
								if(strcmp(s, "con")){
									printf("Specifica con cosa aprire la porta\n");
									return 0;
								}

								do
									i++;
								while(comando[i]==' ' && comando[i]!='\0');
								if(comando[i]=='\0'){
									printf("Specifica con cosa aprire la porta\n");
									return 0;
								}

								if(!strcmp(&comando[i], "chiave di ferro")){
									if(findobj(inventario, "chiave di ferro")){
										player->nordopen=1;
										player=player->nord;
										player->sudopen=1;
										player=player->sud;
										printf("La porta si apre\n");
									}else{
										printf("Non hai questo oggetto\n");
										return 0;
									}
								}else
									printf("Non è l'oggetto giusto\n");
							}
							break;
			default: printf("Porta non trovata\n");
							 return 0;
		}
	}else if(!strcmp(dir, "sud")){
		switch(player->id){
			case 0: printf("La porta è già aperta\n");
							return 0;
			case 2: printf("La porta è già aperta\n");
							return 0;
			case 3: printf("La porta è già aperta\n");
							return 0;
			case 6: printf("La porta è già aperta\n");
							return 0;
			case 7: printf("La porta è già aperta\n");
							return 0;
			case 10: printf("La porta è già aperta\n");
							 return 0;
			case 14: printf("La porta è già aperta\n");
							 return 0;
			case 15: printf("La porta è già aperta\n");
							 return 0;
			case 17: printf("La porta è già aperta\n");
							 return 0;
			case 9: if(player->sudopen){
								printf("La porta è già aperta\n");
								return 0;
							}else{
								do
									i++;
								while(comando[i]==' ' && comando[i]!='\0');
								if(comando[i]=='\0'){
									printf("Specifica con cosa aprire la porta\n");
									return 0;
								}

								for(j=0; comando[i] != ' ' && j<LENOBJ && comando[i]!='\0'; i++, j++)
									s[j]=comando[i];
								s[j]='\0';
								if(strcmp(s, "con")){
									printf("Specifica con cosa aprire la porta\n");
									return 0;
								}

								do
									i++;
								while(comando[i]==' ' && comando[i]!='\0');
								if(comando[i]=='\0'){
									printf("Specifica con cosa aprire la porta\n");
									return 0;
								}

								if(!strcmp(&comando[i], "chiave di ferro")){
									if(findobj(inventario, "chiave di ferro")){
										player->sudopen=1;
										player=player->sud;
										player->nordopen=1;
										player=player->nord;
										printf("La porta si apre\n");
									}else{
										printf("Non hai questo oggetto\n");
										return 0;
									}
								}else
									printf("Non è l'oggetto giusto\n");
							}
							break;
			case 11: if(player->sudopen){
								printf("La porta è già aperta\n");
								return 0;
						 	 }else{
							 	do
									i++;
								while(comando[i]==' ' && comando[i]!='\0');
								if(comando[i]=='\0'){
									printf("Specifica con cosa aprire la porta\n");
									return 0;
								}

								for(j=0; comando[i] != ' ' && j<LENOBJ && comando[i]!='\0'; i++, j++)
									s[j]=comando[i];
								s[j]='\0';
								if(strcmp(s, "con")){
									printf("Specifica con cosa aprire la porta\n");
									return 0;
								}

								do
									i++;
								while(comando[i]==' ' && comando[i]!='\0');
								if(comando[i]=='\0'){
									printf("Specifica con cosa aprire la porta\n");
									return 0;
								}

								if(!strcmp(&comando[i], "chiave di bronzo")){
									if(findobj(inventario, "chiave di bronzo")){
										player->sudopen=1;
										player=player->sud;
										player->nordopen=1;
										player=player->nord;
										printf("La porta si apre\n");
									}else{
										printf("Non hai questo oggetto\n");
										return 0;
									}
								}else{
									printf("Non è l'oggetto giusto\n");
									return 0;
								}
							}
							break;
			case 21: if(player->sudopen){
								printf("La porta è già aperta\n");
								return 0;
						 	 }else{
							 	do
									i++;
								while(comando[i]==' ' && comando[i]!='\0');
								if(comando[i]=='\0'){
									printf("Specifica con cosa aprire la porta\n");
									return 0;
								}

								for(j=0; comando[i] != ' ' && j<LENOBJ && comando[i]!='\0'; i++, j++)
									s[j]=comando[i];
								s[j]='\0';
								if(strcmp(s, "con")){
									printf("Specifica con cosa aprire la porta\n");
									return 0;
								}

								do
									i++;
								while(comando[i]==' ' && comando[i]!='\0');
								if(comando[i]=='\0'){
									printf("Specifica con cosa aprire la porta\n");
									return 0;
								}

								if(!strcmp(&comando[i], "chiave dorata")){
									if(findobj(inventario, "chiave dorata")){
										player->sudopen=1;
										player=player->sud;
										player->nordopen=1;
										player=player->nord;
										printf("La porta si apre\n");
									}else{
										printf("Non hai questo oggetto\n");
										return 0;
									}
								}else{
									printf("Non è l'oggetto giusto\n");
									return 0;
								}
							}
							break;
			default: printf("Porta non trovata\n");
							 return 0;
		}
	}else if(!strcmp(dir, "est")){
		switch(player->id){
			case 0: printf("La porta è già aperta\n");
							return 0;
			case 1: printf("La porta è già aperta\n");
							return 0;
			case 5: printf("La porta è già aperta\n");
							return 0;
			case 9: printf("La porta è già aperta\n");
							return 0;
			case 10: printf("La porta è già aperta\n");
							 return 0;
			case 12: printf("La porta è già aperta\n");
							 return 0;
			case 19: printf("La porta è già aperta\n");
							 return 0;
			default: printf("Porta non trovata\n");
							 return 0;
		}
	}else if(!strcmp(dir, "ovest")){
		switch(player->id){
			case 0: printf("La porta è già aperta\n");
							return 0;
			case 10: printf("La porta è già aperta\n");
							 return 0;
			case 11: printf("La porta è già aperta\n");
							 return 0;
			case 13: printf("La porta è già aperta\n");
							 return 0;
			case 18: printf("La porta è già aperta\n");
							 return 0;
			case 2: if(player->ovestopen){
								printf("La porta è già aperta\n");
								return 0;
							}else{
								do
									i++;
								while(comando[i]==' ' && comando[i]!='\0');
								if(comando[i]=='\0'){
									printf("Specifica con cosa aprire la porta\n");
									return 0;
								}

								for(j=0; comando[i] != ' ' && j<LENOBJ && comando[i]!='\0'; i++, j++)
									s[j]=comando[i];
								s[j]='\0';
								if(strcmp(s, "con")){
									printf("Specifica con cosa aprire la porta\n");
									return 0;
								}

								do
									i++;
								while(comando[i]==' ' && comando[i]!='\0');
								if(comando[i]=='\0'){
									printf("Specifica con cosa aprire la porta\n");
									return 0;
								}

								if(!strcmp(&comando[i], "chiave di ottone")){
									if(findobj(inventario, "chiave di ottone")){
										player->ovestopen=1;
										player=player->ovest;
										player->estopen=1;
										player=player->est;
										printf("La porta si apre\n");
									}else{
										printf("Non hai questo oggetto\n");
										return 0;
									}
								}else
									printf("Non è l'oggetto giusto\n");
							}
							break;
			case 6: if(player->ovestopen){
								printf("La porta è già aperta\n");
								return 0;
						 	}else{
								printf("Non è l'oggetto giusto\n");
								return 0;
							}
			default: printf("Porta non trovata\n");
							 return 0;
		}
	}else if(!strcmp(dir, "su")){
		switch(player->id){
			case 8: printf("La porta è già aperta\n");
							return 0;
			case 13: printf("La porta è già aperta\n");
							 return 0;
			case 16: printf("La porta è già aperta\n");
							 return 0;
			case 19: printf("La porta è già aperta\n");
							 return 0;
			default: printf("Porta non trovata\n");
							 return 0;
		}
	}else if(!strcmp(dir, "giù")){
		switch(player->id){
			case 4: printf("La porta è già aperta\n");
							return 0;
			case 8: printf("La porta è già aperta\n");
					  	return 0;
			case 20: printf("La porta è già aperta\n");
							 return 0;
			case 21: printf("La porta è già aperta\n");
							 return 0;
			default: printf("Porta non trovata\n");
							 return 0;
		}
	}else{
		printf("Porta non trovata\n");
		return 0;
	}

	return 1;
}


int suona(char comando[]){
	int i;

	i=0;
	do
		i++;
	while(comando[i]==' ' && comando[i]!='\0');
	if(comando[i]=='\0'){
		printf("Specifica cosa suonare\n");
		return 0;
	}

	if(!strcmp(&comando[i], "arpa")){
		if(findobj(inventario, "arpa")){
			if(player->id==5 && !player->estopen){
				player->estopen=1;
				player=player->est;
				player->ovestopen=1;
				player=player->ovest;
				printf("La porta est si apre per magia!\n");
			}else if(player->id==6 && !player->ovestopen){
				player->ovestopen=1;
				player=player->ovest;
				player->estopen=1;
				player=player->est;
				printf("La porta ovest si apre per magia!\n");
			}else
				printf("Non succede niente...\n");
		}else{
			printf("Non possiedi questo oggetto\n");
			return 0;
		}
	}else{
		printf("Non credo si possa suonare questo oggetto\n");
		return 0;
	}

	return 1;
}




void delcarret(char s[]){
	int i;

	for(i=0; s[i]!='\r' && s[i]!='\0'; i++)
		;
	s[i]='\0';

}

void delnewline(char s[]){
	int i;

	for(i=0; s[i]!='\n' && s[i]!='\0'; i++)
		;
	s[i]='\0';

}

void delspace(char s[]){
	int i;

	if(s[0]==' '){
		for(i=0; s[i]!='\0'; i++)
			s[i]=s[i+1];
	}

}
