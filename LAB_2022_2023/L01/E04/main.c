//	Alessio Flamini s268460

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHARMAX 100
#define RIDEMAX 50
#define FALSE 0
#define TRUE 1

typedef enum{r_print, r_ord, r_data, r_code, r_start,
      		 r_dest, r_search, r_video, r_file, r_end, r_err} select;

typedef struct
{
    char starstation[CHARMAX+1];
    char endstation[CHARMAX+1];
    char data[CHARMAX+1];
    char starthour[CHARMAX+1];
    char endhour[CHARMAX+1];
    char codice[CHARMAX+1];
    int delay;
}info;

typedef struct
{
    info *refData[RIDEMAX];
    info *refCode[RIDEMAX];
    info *refStart[RIDEMAX];
    info *refEnd[RIDEMAX];
}ref;

void readFile(info *vect, int *n);
select readComman();
void selectData(select comando, ref *list, int n, int *ordinato);
info **findPVect(select choice, ref *list);
void stampa(info **vect, select choice, int n);
void orderVector(info **vect, select choice, int n);
int refCompare(info *e1, info *e2, select choice);
int refNCompare(char *stringa, info *el, select choice, int n);
void searchLinear(info **vect, char *to_search, select choice, int n);
void searchDichotomous(info **vect, char *to_search, select choice, int n);

int main()
{
    info corse[RIDEMAX];
    ref list;
    int ind, dim, right[6] = {FALSE};
    select comando;

    readFile(corse, &dim);

    for(ind = 0; ind < dim; ind++)
    {
        list.refData[ind] = &corse[ind];
        list.refCode[ind] = &corse[ind];
        list.refStart[ind] = &corse[ind];
        list.refEnd[ind] = &corse[ind];
    }
    do
    {
        printf("Cosa vuoi fare? Scegli:\n- stampa\n- ordinamento\n- ricerca\n- fine\n");
        comando = readComman();
        selectData(comando, &list, dim, right);
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
    	fscanf(fp, "%s%s%s%s%s%s%d", vect[i].codice, vect[i].starstation, vect[i].endstation, vect[i].data, vect[i].starthour, vect[i].endhour, &vect[i].delay);
	}
    fclose(fp);
}

select readComman()
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

void selectData(select comando, ref *list, int n, int *right)
{
    select scelta1, scelta2;
    char to_search[CHARMAX+1];

    switch(comando)
    {
        case r_print:
            do
            {
                printf("Dove stampare? Scegli:\n- video\n- file\n");
                scelta1 = readComman();
                if(scelta1 == r_file || scelta1 == r_video)
                {
                    printf("Ordinamento per cosa? Scegli:\n- data\n- codice\n- partenza\n- arrivo\n");
                    scelta2 = readComman();
                    stampa(findPVect(scelta2, list), scelta1, n);
                }
                else
                {
                	printf("Operazione non valida.\n");
				}
            }
            while(scelta1 != r_file && scelta1 != r_video);
            break;

        case r_ord:
            do
            {
                printf("Ordinamento per cosa? Scegli:\n- data\n- codice\n- partenza\n- arrivo\n");
                scelta1 = readComman();
                if(scelta1 != r_data && scelta1 != r_code && scelta1 != r_start && scelta1 != r_dest)
                {
                	printf("Ordinamento per scelta fatta non disponibile.\n");
				}
                else
                {
                    orderVector(findPVect(scelta1, list), scelta1, n);
                    right[scelta1] = TRUE;
                }
            }
            while(scelta1 != r_data && scelta1 != r_code && scelta1 != r_start && scelta1 != r_dest);
            break;

        case r_search:
            do
            {
                printf("Ricerca per cosa? Scegli:\n- data\n- codice\n- partenza\n- arrivo\n");
                scelta1 = readComman();
                if(scelta1 != r_data && scelta1 != r_code && scelta1 != r_start && scelta1 != r_dest)
                {
                	printf("Ordinamento per scelta fatta non disponibile.\n");
				}
                else
		        {
	                printf("Inserisci chiave ricerca: ");
	                scanf("%s", to_search);
	                if(right[scelta1])
	                {
	                	searchDichotomous(findPVect(scelta1, list), to_search, scelta1, n);
					}

	                else
	                {
	                	searchLinear(findPVect(scelta1, list), to_search, scelta1, n);
					}
            	}
            }
            while(scelta1 != r_data && scelta1 != r_code && scelta1 != r_start && scelta1 != r_dest);
            break;

        case r_end:
            break;

        default:
            printf("Comando inserito non disponibile. Riprova.\n");
    }
}

info **findPVect(select choice, ref *list)
{
    switch(choice)
    {
        case r_data:
            return(list->refData);
            break;

        case r_code:
            return(list->refCode);
            break;

        case r_start:
            return(list->refStart);
            break;

        case r_dest:
            return(list->refEnd);
            break;

        default:
            return(NULL);
    }
}

void stampa(info **vect, select choice, int n)
{
    FILE *fp;
    int i;

    if(choice == r_video)
    {
        for(i = 0; i < n; i++)
        {
            printf("%s %s %s %s %s %s, ritardo di %d minuti.\n", vect[i]->codice, vect[i]->starstation, vect[i]->endstation, vect[i]->data, vect[i]->starthour, vect[i]->endhour, vect[i]->delay);
		}
    }
    else
    {
        fp = fopen("file.txt", "w");
        for(i = 0; i < n; i++)
        {
            fprintf(fp, "%s %s %s %s %s %s, ritardo di %d minuti.\n", vect[i]->codice, vect[i]->starstation, vect[i]->endstation, vect[i]->data, vect[i]->starthour, vect[i]->endhour, vect[i]->delay);
		}
    }
}

void orderVector(info **vect, select choice, int n)
{
    int i, j;
    info *tmp;

    for(i = 1; i < n; i++)
    {
        tmp = vect[i];
        j = i - 1;
        while(j >= 0 && refCompare(tmp, vect[j], choice) < 0)
        {
            vect[j + 1] = vect[j];
            j--;
        }
        vect[j + 1] = tmp;
    }
}

int refCompare(info *e1, info *e2, select choice)
{
    switch(choice)
    {
        case r_data:
            if(strcmp(e1->data, e2->data) == 0)
            {
            	return(strcmp(e1->starthour, e2->starthour));
			}
            return(strcmp(e1->data, e2->data));
            break;

        case r_code:
            return(strcmp(e1->codice, e2->codice));
            break;

        case r_start:
            return(strcmp(e1->starstation, e2->starstation));
            break;

        case r_dest:
            return(strcmp(e1->endstation, e2->endstation));
            break;

        default:
            return(-1);
    }
}

int refNCompare(char *stringa, info *el, select choice, int n)
{
    switch(choice)
    {
        case r_data:
            return(strncmp(stringa, el->data, n));
            break;

        case r_code:
            return(strncmp(stringa, el->codice, n));
            break;

        case r_start:
            return(strncmp(stringa, el->starstation, n));
            break;

        case r_dest:
            return(strncmp(stringa, el->endstation, n));
            break;

        default:
            return(-1);
    }
}

void searchLinear(info **vect, char *to_search, select choice, int n)
{
    int i, lung, j = 0;
    info *match[RIDEMAX];

    lung = strlen(to_search);
    for(i = 0; i < n; i++)
    {
    	if(refNCompare(to_search, vect[i], choice, lung) == 0)
        {
            match[j] = vect[i];
            j++;
        }
	}

    stampa(match, r_video, j);
}

void searchDichotomous(info **vect, char *to_search, select choice, int n)
{
    int l = 0, r = n-1, m, lung, i = 0, start;
    info *match[RIDEMAX];

    lung = strlen(to_search);

    while(l <= r)
    {
        m = (l + r) / 2;
        if(refNCompare(to_search, vect[m], choice, lung) > 0)
        {
        	l = m + 1;
		}
        else if(refNCompare(to_search, vect[m], choice, lung) < 0)
        {
        	r = m - 1;
		}
        else
        {
            match[i++] = vect[m];
            start = m++;
            while(refNCompare(to_search, vect[m], choice, lung) == 0 && m < n)
            {
            	match[i++] = vect[m++];
			}
            m = start - 1;
            while(m >= 0 && refNCompare(to_search, vect[m], choice, lung) == 0)
            {
            	match[i++] = vect[m--];
			}
            stampa(match, r_video, i);
            return;
        }
    }
}
