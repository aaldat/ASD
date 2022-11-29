//	Alessio Flamini s268460

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHARMAX 100
#define RIDEMAX 50
#define FALSE 0
#define TRUE 1

typedef struct
{
    char *startstation;
    char *endstation;
    char *data;
    char *starthour;
    char *endhour;
    char *codice;
    int *delay;
}info;

typedef struct
{
    info **RefDate;
    info **RefCode;
    info **RefStart;
    info **RefDest;

}ref;

typedef enum{r_print, r_ord, r_data, r_code, r_start,
      		 r_dest, r_search, r_video, r_file, r_end, r_insert, r_err} select;

void readFile(info **vect, int *n, char *nomefile);
void readStruct(info *singleS, FILE *fp);
char *allocString(char tmp[]);
void allocPuntVect(ref *list, int n);
select readCommand();
void selectData(select comando, ref *list, int *n, int *right, info **corse);
void dealloc(info *corse, int n, ref *list);
info **findPuntVect(select choice, ref *list);
void print(info **vect, select choice, int n);
void sortVect(info **vect, select choice, int n);
int refCompare(info *e1, info *e2, select choice);
int refNCompare(char *stringa, info *elem, select choice, int n);
void searchLinear(info **vect, char *to_search, select choice, int n);
void earchDichotomous(info **vect, char *to_search, select choice, int n);

int main()
{
    info *corse;
    ref list;
    int dim, ind, right[6] = {FALSE};
    select comando;
    char file1[9] = {"corse.txt"};

    readFile(&corse, &dim, file1);
    allocPuntVect(&list, dim);
    for(ind = 0; ind < dim; ind++)
    {
        list.RefDate[ind] = &corse[ind];
        list.RefCode[ind] = &corse[ind];
        list.RefStart[ind] = &corse[ind];
        list.RefDest[ind] = &corse[ind];
    }
    do
    {
        printf("Cosa vuoi fare? Scegli:\n- stampa\n- ordinamento\n- ricerca\n- lettura\n- fine\n");
        comando = readCommand();
        selectData(comando, &list, &dim, right, &corse);
    }
    while(comando != r_end);

    return 0;
}

void readFile(info **vect, int *n, char *nomefile)
{
    int i;
    FILE *fp;

    if((fp = fopen(nomefile, "r")) == NULL)
    {
        printf("Errore apertura del file %s.\n", nomefile);
        exit(EXIT_FAILURE);
    }

    fscanf(fp, "%d", n);
    (*vect) =(info *)malloc((*n) * sizeof(info));
    for(i = 0; i < *n; i++)
    {
    	readStruct(&((*vect)[i]), fp);
	}

    fclose(fp);
}

void readStruct(info *singleS, FILE *fp)
{
    char tmp[CHARMAX];

    fscanf(fp, "%s", tmp);
    singleS->codice = allocString(tmp);

    fscanf(fp, "%s", tmp);
    singleS->startstation = allocString(tmp);

    fscanf(fp, "%s", tmp);
    singleS->endstation = allocString(tmp);

    fscanf(fp, "%s", tmp);
    singleS->data = allocString(tmp);

    fscanf(fp, "%s", tmp);
    singleS->starthour = allocString(tmp);

    fscanf(fp, "%s", tmp);
    singleS->endhour = allocString(tmp);

    fscanf(fp, "%d", &(singleS->delay));
}

char *allocString(char tmp[])
{
    char *p;
    p = (char *)malloc((strlen(tmp)+1) * sizeof(char));
    strcpy(p, tmp);
    return p;
}

void allocPuntVect(ref *list, int n)
{
    list->RefCode = (info **) malloc(n * sizeof(info*));
    list->RefDate = (info **) malloc(n * sizeof(info*));
    list->RefStart = (info **) malloc(n * sizeof(info*));
    list->RefDest = (info **) malloc(n * sizeof(info*));
}

select readCommand()
{
    char cmd[CHARMAX+1];
    char comandi[r_err][CHARMAX+1] = {"stampa", "ordinamento", "data", "codice", "partenza", "arrivo", "ricerca", "video", "file", "fine", "lettura"};
    int i;

    scanf("%s", cmd);
    for(i = r_print; i < r_err; i++)
    {
    	if(strcmp(cmd, comandi[i]) == 0)
        {
        	return (select)i;
		}
	}

    return (select) i;
}

void selectData(select comando, ref *list, int *n, int *right, info **corse)
{
    select scelta1, scelta2;
    char to_search[CHARMAX+1], file[CHARMAX+1];
    int i;

    switch(comando)
    {
        case r_print:
            do
            {
	            printf("Dove stampare? Scegli:\n- video\n- file\n");
	            scelta1 = readCommand();
	            if(scelta1 == r_file || scelta1 == r_video)
	            {
	                printf("Ordinamento per cosa? Scegli:\n- data\n- codice\n- partenza\n- arrivo\n");
	                scelta2 = readCommand();
	                print(findPuntVect(scelta2, list), scelta1, *n);
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
	            scelta1 = readCommand();
	            if(scelta1 != r_data && scelta1 != r_code && scelta1 != r_start && scelta1 != r_dest)
	            {
	            	printf("Ordinamento per scelta fatta non disponibile.\n");
				}
	            else
                {
	                sortVect(findPuntVect(scelta1, list), scelta1, *n);
	                right[scelta1] = TRUE;
                }
	        }
	        while(scelta1 != r_data && scelta1 != r_code && scelta1 != r_start && scelta1 != r_dest);
	        break;

	    case r_search:
	        do
            {
	            printf("Ricerca per cosa? Scegli:\n- data\n- codice\n- partenza\n- arrivo\n");
	            scelta1 = readCommand();
	            if(scelta1 != r_data && scelta1 != r_code && scelta1 != r_start && scelta1 != r_dest)
	            {
	            	printf("Ordinamento per scelta fatta non disponibile.\n");
				}
	            else
                {
	                scanf("%s", to_search);
	                if(right[scelta1])
	                {
	                	earchDichotomous(findPuntVect(scelta1, list), to_search, scelta1, *n);
					}
	                else
	                {
	                	searchLinear(findPuntVect(scelta1, list), to_search, scelta1, *n);
					}
                }
            }
	        while(scelta1 != r_data && scelta1 != r_code && scelta1 != r_start && scelta1 != r_dest);
	        break;

	    case r_insert:
	        printf("Quale file da cui leggere i dati? Scegli:\n- log.txt\n- log2.txt\n");
	        scanf("%s", file);
	        dealloc(*corse, *n, list);
	        readFile(corse, n, file);
	        allocPuntVect(list, *n);
	        for(i = 0; i < *n; i++)
	        {
		        list->RefDate[i] = &((*corse)[i]);
		        list->RefCode[i] = &((*corse)[i]);
		        list->RefStart[i] = &((*corse)[i]);
		        list->RefDest[i] = &((*corse)[i]);
		        right[i] = FALSE;
	        }
	        break;

	    case r_end:
	        break;

	    default:
	        printf("Comando non disponibile.\n");
    }
}

void dealloc(info *corse, int n, ref *list)
{
    int i;

    for(i = 0; i < n; i++)
    {
        free(corse[i].codice);
        free(corse[i].startstation);
        free(corse[i].endstation);
        free(corse[i].data);
        free(corse[i].starthour);
        free(corse[i].endhour);
    }
    free(corse);

    free(list->RefCode);
    free(list->RefDate);
    free(list->RefStart);
    free(list->RefDest);
}

info **findPuntVect(select choice, ref *list)
{
    switch(choice)
    {
        case r_data:
            return(list->RefDate);
            break;

        case r_code:
            return(list->RefCode);
            break;

        case r_start:
            return(list->RefStart);
            break;

        case r_dest:
            return(list->RefDest);
            break;

        default:
            return(NULL);
    }
}

void print(info **vect, select choice, int n)
{
    FILE *fp;
    int i;

    if(choice == r_video)
    {
    	for(i = 0; i < n; i++)
    	{
            printf("%s %s %s %s %s %s, ritardo di %d minuti.\n", vect[i]->codice, vect[i]->startstation, vect[i]->endstation, vect[i]->data, vect[i]->starthour, vect[i]->endhour, vect[i]->delay);
		}
    }
    else
    {
    	fp = fopen("file.txt", "w");
    	for(i = 0; i < n; i++)
    	{
        	fprintf(fp, "%s %s %s %s %s %s, ritardo di %d minuti.\n", vect[i]->codice, vect[i]->startstation, vect[i]->endstation, vect[i]->data, vect[i]->starthour, vect[i]->endhour, vect[i]->delay);
		}
    }
}

void sortVect(info **vect, select choice, int n)
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
            return(strcmp(e1->startstation, e2->startstation));
            break;

        case r_dest:
            return(strcmp(e1->endstation, e2->endstation));
            break;

        default:
            return(-1);
    }
}

int refNCompare(char *stringa, info *elem, select choice, int n)
{
        switch(choice)
        {
	        case r_data:
	            return(strncmp(stringa, elem->data, n));
	            break;

	        case r_code:
	            return(strncmp(stringa, elem->codice, n));
	            break;

	        case r_start:
	            return(strncmp(stringa, elem->startstation, n));
	            break;

	        case r_dest:
	            return(strncmp(stringa, elem->endstation, n));
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

    print(match, r_video, j);
}

void earchDichotomous(info **vect, char *to_search, select choice, int n)
{
    int l = 0, r = n - 1, m, lung, i = 0, start;
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
            print(match, r_video, i);
            return;
        }
    }
}
