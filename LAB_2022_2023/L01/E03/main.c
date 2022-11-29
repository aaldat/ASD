//	Alessio Flamini s268460

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RIDEMAX 50
#define CHARMAX 100
#define FALSE 0
#define TRUE 1

typedef enum {r_print, r_ord, r_data, r_code, r_start,
			  r_dest, r_search, r_video, r_file, r_end, r_err} select;

typedef struct
{
    char startstation[CHARMAX+1];
    char endstastion[CHARMAX+1];
    char data[CHARMAX+1];
    char starthour[CHARMAX+1];
    char endhour[CHARMAX+1];
    char codice[CHARMAX+1];
    int delay;

}info;

void readFile(info *vect, int *n);
select readCommand();
void selectData(select comando, info *v, int n, select *last_sort);
void orderVector(info *vect, select choice, int n);
void stampa(info *vect, select choice, int n);
int comparison(info e1, info e2, select choice);
void searchLinear(info *vect, int n, char *to_search);
void searchDichotomous(info *vect, int n, char *to_search);

int main()
{
    info corse[RIDEMAX];
    int dim;
    select comando, sort = r_err;

    readFile(corse, &dim);
    do
    {
        printf("Cosa vuoi fare? Scegli:\n- stampa\n- ordinamento\n- ricerca\n- fine\n");
        comando = readCommand();
        selectData(comando, corse, dim, &sort);
    }
    while(comando != r_end);
    return 0;
}

void readFile(info *vect, int *n)
{
    int i;
    FILE *fp;

    if((fp = fopen("corse.txt", "r")) == NULL)
    {
        printf("Errore apertura del file %s.\n", "corse.txt");
        exit(EXIT_FAILURE);
    }

    fscanf(fp, "%d", n);
    for(i = 0; i < *n; i++)
    {
    	fscanf(fp, "%s%s%s%s%s%s%d", vect[i].codice, vect[i].startstation, vect[i].endstastion, vect[i].data, vect[i].starthour, vect[i].endhour, &vect[i].delay);
	}

    fclose(fp);
}

select readCommand()
{
    char cmd[CHARMAX+1];
    char comandi[r_err][CHARMAX+1] = {"stampa", "ordinamento", "data", "codice", "partenza", "arrivo", "ricerca", "video", "file", "fine"};
    int i;

    scanf("%s", cmd);
    for(i = r_print; i < r_err; i++)
    {
    	if(strcmp(cmd, comandi[i]) == 0)
    	{
    		return (select) i;
		}
	}
    return (select) i;
}

void selectData(select comando, info *v, int n, select *last_sort)
{
    select scelta;
    char to_search[CHARMAX+1];

    switch(comando) {
	    case r_print:
	        do {
	            printf("Dove stampare? Scegli:\n- video\n- file\n");
	            scelta = readCommand();
	            if(scelta == r_file) {
	            	stampa(v, scelta, n);
				}
	            else if(scelta == r_video) {
	            	stampa(v, scelta, n);
				}
	            else {
	            	printf("Operazione non valida.\n");
				}
	        }
			while(scelta != r_file && scelta != r_video);
	        break;

	    case r_ord:
	        do {
                printf("Ordinamento per cosa? Scegli:\n- data\n- codice\n- partenza\n- arrivo\n");
                scelta = readCommand();
                if(scelta != r_data && scelta != r_code && scelta != r_start && scelta != r_dest) {
                	printf("Ordinamento per scelta fatta non disponibile.\n");
				} else {
                    orderVector(v, scelta, n);
                    (*last_sort) = scelta;
                }
            }
	        while(scelta != r_data && scelta != r_code && scelta != r_start && scelta != r_dest);
	        break;

	    case r_search:
	        printf("Inserire la stazione di partenza (va bene anche nome non completo).\n");
	        scanf("%s", to_search);
	        if((*last_sort) == r_start){
	        	searchDichotomous(v, n, to_search);
			} else {
	        	searchLinear(v, n, to_search);
			}
	        break;
    }

}

void orderVector(info *vect, select choice, int n)
{
    int i, j, scambio = TRUE;
    info tmp;

    for(i = 0; i < n && scambio; i++) {
        scambio = FALSE;
        for(j = 0; j < n - i - 1; j++) {
        	if(comparison(vect[j], vect[j+1], choice) > 0) {
                tmp = vect[j];
                vect[j] = vect[j+1];
                vect[j+1] = tmp;
                scambio = TRUE;
            }
		}
    }
}

int comparison(info e1, info e2, select choice)
{
    switch(choice) {
        case r_data:
            if(strcmp(e1.data, e2.data) == 0) {
            	return(strcmp(e1.endhour, e2.endhour));
			} else {
            	return(strcmp(e1.data, e2.data));
			}
            break;

        case r_code:
            return(strcmp(e1.codice, e2.codice));
            break;

        case r_start:
            return(strcmp(e1.startstation, e2.startstation));
            break;

        case r_dest:
            return(strcmp(e1.endstastion, e2.endstastion));
            break;
    }
}

void stampa(info *vect, select choice, int n)
{
    int i;
    FILE *fp;

    fp = fopen("file.txt", "w");

    if(choice == r_video) {
        for(i = 0; i < n; i++) {
            printf("%s %s %s %s %s %s, delay di %d minuti.\n", vect[i].codice, vect[i].startstation, vect[i].endstastion, vect[i].data, vect[i].starthour, vect[i].endhour, vect[i].delay);
	   }
    } else {
        for(i = 0; i < n; i++) {
            fprintf(fp, "%s %s %s %s %s %s, delay di %d minuti.\n", vect[i].codice, vect[i].startstation, vect[i].endstastion, vect[i].data, vect[i].starthour, vect[i].endhour, vect[i].delay);
		}
    }
    fclose(fp);
}

void searchLinear(info *vect, int n, char *to_search)
{
    int i, lung, j = 0;
    info match[RIDEMAX];

    lung = strlen(to_search);
    for(i = 0; i < n; i++) {
    	if(strncmp(to_search, vect[i].startstation, lung) == 0) {
            match[j] = vect[i];
            j++;
        }
	}
    stampa(match, r_video, j);
}

void searchDichotomous(info *vect, int n, char *to_search)
{
    int l = 0, r = n-1, i = 0, m, lung, start;
    info corrsipondenze[RIDEMAX];
    lung = strlen(to_search);

    while(l <= r) {
        m = (l + r) / 2;
        if(strncmp(to_search, vect[m].startstation, lung) > 0) {
        	l = m + 1;
		} else if(strncmp(to_search, vect[m].startstation, lung) < 0) {
        	r = m - 1;
		} else {
            corrsipondenze[i++] = vect[m];
            start = m;
            m++;
            while(strncmp(to_search, vect[m].startstation, lung) == 0 && m < n) {
            	corrsipondenze[i++] = vect[m++];
			}
            m = start - 1;
            while(strncmp(to_search, vect[m].startstation, lung) == 0 && m >= 0) {
            	corrsipondenze[i++] = vect[m--];
			}
            stampa(corrsipondenze, r_video, i);
            return;
        }
    }
}
