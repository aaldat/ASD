//	Alessio Flamini s268460

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 30
#define H 8
#define D 10
#define RIDEMAX 1000

typedef struct corsa {
	char code[N+1], start[N+1], finish[N+1], time_s[H+1], time_f[H+1];
	int aaaa, mm, gg, delay;
}Corsa;

typedef enum {r_date, r_start, r_dest,
			  r_delay, r_delay_tot, r_end} comand_e;

void readFile(Corsa *vect, int *n);
void openFile(char filename);
comand_e readComand(void);
void selectData(comand_e comando, Corsa corse[RIDEMAX], int n);
void date(Corsa corse[RIDEMAX], int n, comand_e comando);
void start(Corsa corse[RIDEMAX], int n);
void dest(Corsa corse[RIDEMAX], int n);
void delayTot(Corsa corse[RIDEMAX], int n);

int main() {
	Corsa corse[RIDEMAX];
	int dim;
	comand_e comando;

	readFile(corse, &dim);
	do {
        printf("Cosa vuoi visualizzare? Scegli: date/partenza/capolinea/ritardo/ritardo_tot/fine\n");
        comando = readComand();
        selectData(comando, corse, dim);
    }
    while(comando != r_end);

	return 0;
}

void readFile(Corsa *vect, int *n)
{
    int i;
    FILE *fp;

    if((fp = fopen("corse.txt", "r")) == NULL) {
        printf("Errore apertura del file %s.\n", "corse.txt");
        exit(EXIT_FAILURE);
    }

    fscanf(fp, "%d", n);
    for(i = 0; i < *n; i++) {
    	fscanf(fp, "%s %s %s %d/%d/%d %s %s %d", vect[i].code, vect[i].start, vect[i].finish, &vect[i].aaaa, &vect[i].mm, &vect[i].gg, vect[i].time_s, vect[i].time_f, &vect[i].delay);
	}

    fclose(fp);
}

comand_e readComand(void)
{
    char cmd[N+1];
    char comandi[r_end+1][N+1] = {"date", "partenza", "capolinea", "ritardo", "ritardo_tot", "fine"};
    int i;

    scanf("%s", cmd);
    for(i = r_date; i < r_end; i++) {
    	if(strcmp(cmd, comandi[i]) == 0) {
    		return (comand_e)i;
		}
	}
    return (comand_e)i;
}

void selectData(comand_e comando, Corsa corse[RIDEMAX], int n){

	switch(comando) {
		case r_date:
			date(corse, n, comando);
			break;
		case r_start:
			start(corse, n);
			break;
		case r_dest:
			dest(corse, n);
			break;
		case r_delay:
			date(corse, n, comando);
			break;
		case r_delay_tot:
			delayTot(corse, n);
			break;
        case r_end:
            break;
	}
}

void date(Corsa corse[RIDEMAX], int n, comand_e comando){
	int i = 0;
	char data[D+1];
	int anno[n+1], mese[n+1], giorno[n+1];

	for(i = 0; i < 2; i++){
		printf("Inserisci data %d (YYYY/MM/DD): ", i+1);
		scanf("%s", data);
		sscanf(data, "%d/%d/%d", &anno[i], &mese[i], &giorno[i]);
	}

	if(comando == r_delay) {
        printf("Fra il %d/%d/%d e il %d/%d/%d sono partiti i seguenti treni, arrivati poi in ritardo:\n", anno[0], mese[0], giorno[0], anno[1], mese[1], giorno[1]);
	}else if(comando == r_date) {
        printf("Fra il %d/%d/%d e il %d/%d/%d sono partiti i seguenti treni:\n", anno[0], mese[0], giorno[0], anno[1], mese[1], giorno[1]);
	}

	i = 0;

	for(i = 0; i < n; i++){
		if(anno[0] <= corse[i].aaaa && anno[1] >= corse[i].aaaa){
			if(anno[0] == anno[1]){
				if(mese[0] <= corse[i].mm && mese[1] >= corse[i].mm){
					if(mese[0] == mese[1]){
						if(giorno[0] <= corse[i].gg && giorno[1] >= corse[i].gg){
							if(comando == r_delay && corse[i].delay != 0){
								printf("%s\n", corse[i].code);
							}
							if(comando == r_date) {
                                printf("%s\n", corse[i].code);
							}
						}
					}else{
						if(comando == r_delay && corse[i].delay != 0){
                            printf("%s\n", corse[i].code);
                        }
                        if(comando == r_date) {
                            printf("%s\n", corse[i].code);
                        }
					}
				}
			}else{
				if(comando == r_delay && corse[i].delay != 0){
                    printf("%s\n", corse[i].code);
                }
                if(comando == r_date) {
                    printf("%s\n", corse[i].code);
                }
			}
		}
	}

}

void start(Corsa corse[RIDEMAX], int n){
	int i = 0;
	char partenza[N+1];

	printf("Inserire la stazione di partenza: ");
	scanf("%s", partenza);
	printf("Da questa fermata partono i seguenti treni:\n");
	for(i = 0; i < n; i++){
		if(strcmp(partenza, corse[i].start) == 0)
			printf("%s\n", corse[i].code);
	}
}


void dest(Corsa corse[RIDEMAX], int n){
	int i = 0;
	char arrivo[N+1];

	printf("Inserire la stazione di destinazione: ");
	scanf("%s", arrivo);
	printf("Da questa fermata arrivano i seguenti treni:\n");
	for(i = 0; i < n; i++){
		if(strcmp(arrivo, corse[i].finish) == 0)
			printf("%s\n", corse[i].code);
	}
}

void delayTot(Corsa corse[RIDEMAX], int n){
	char code[H];
	int i = 0, rit_tot = 0;
	printf("Inserire il codice della tratta di cui si vuole sapere il ritardo totale: ");
	scanf("%s", code);
	for(i = 0; i < n; i++){
		if(strcmp(code, corse[i].code) == 0)
			rit_tot += corse[i].delay;
	}
	printf("Il ritardo accumulato dalla tratta %s e' : %d\n", code, rit_tot);
}
