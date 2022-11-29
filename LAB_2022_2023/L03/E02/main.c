// Alessio Flamini s268460

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define filename "brani.txt"
#define MAX 256

typedef struct {
    int num_pick;
    char **pick;
}Scelta;

void readFile();
int moltiplicazione(int pos, Scelta *val, char **sol, int n, int cnt);

int main()
{
    readFile();
    return 0;
}

void readFile() {
    Scelta *val;
    char **sol;
    int n, pos = 0, cnt = 0, i, j;
    FILE *fpin = fopen(filename, "r");
    if (fpin != NULL) {
        fscanf(fpin, "%d", &n);

        val = (Scelta *) malloc(n * sizeof(Scelta));
        sol = (char **) malloc(n * sizeof(char *));
        for(i = 0; i < n; i++) {
            sol[i] = (char *) malloc(MAX * sizeof(char));
        }
        for(i = 0; i< n; i++) {
           fscanf(fpin, "%d", &val[i].num_pick);
           val[i].pick = (char **) malloc(val[i].num_pick * sizeof(char *));
           for(j = 0; j < val[i].num_pick; j++) {
            val[i].pick[j] = (char *) malloc(MAX * sizeof(char));
            fscanf(fpin, "%s", val[i].pick[j]);
           }
        }

        cnt = moltiplicazione(pos, val, sol, n, cnt);
        printf("%d", cnt);

        free(val->pick);
        free(val);
        for(i = 0; i < n; i++) {
            free(sol[i]);
        }
        free(sol);
    } else {
        printf("Errore apertura file %s", filename);
    }
    fclose(fpin);
}

int moltiplicazione(int pos, Scelta *val, char **sol, int n, int cnt) {
    int i, j;
    if(pos >= n) {
        for ( j = 0; j < n; j++) {
            printf("%s", sol[j]);
        }
        printf("\n");
        return cnt+1;
    }
    for(i = 0; i < val[pos].num_pick; i++) {
        strcpy(sol[pos], val[pos].pick[i]);
        cnt = moltiplicazione(pos+1, val, sol, n, cnt);
    }
    return cnt;
}
