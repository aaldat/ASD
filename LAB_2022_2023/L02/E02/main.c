// Alessio Flamini s268460

#include <stdio.h>
#include <stdlib.h>

void malloc2dP(int ***mp, int nr, int nc);
void readFile(int ***mp, int *nr, int *nc);
void separa(int **m, int **white_p, int **black_p, int nr, int n);

int main() {
    int nr, nc, i, *black, *white, **mp;

    readFile(&mp, &nr, &nc);
    separa(mp, &white, &black, nr, nc);

    free(mp);

    printf("Neri:\n");
    for(i = 0; i < (nr * nc) / 2; i++) {
        printf("%d\t", black[i]);
    }

    printf("\n\n");

    printf("Bianchi:\n");
    for(i = 0; i < (nr * nc) / 2; i++) {
        printf("%d\t", white[i]);
    }
    if((nr * nc) % 2 != 0) {
        printf("%d\t", white[i]); //stampo l'ultimo carattere in caso di scacchiera dispari
    }

    free(white);
    free(black);

    return 0;
}

void readFile(int ***mp, int *nr, int *nc) {
    int i, j, r, c, **m;
    FILE *fp;

    fp = fopen("mat.txt", "r");
    fscanf(fp, "%d %d", &r, &c);

    malloc2dP(&m, r, c);

    for(i = 0; i < r; i++) {
        for(j = 0; j < c; j++) {
            fscanf(fp, "%d", &m[i][j]); //riempio la matrice
        }
    }
    fclose(fp); //chiudo il file fp
    *nr = r;
    *nc = c;
    *mp = m;
}

void malloc2dP(int ***mp, int nr, int nc) { //alloco la matrice dinamicamente tramite puntatore passato per riferimento
    int **m, i;

    m = (int **) malloc(nr*sizeof(int *));
    for(i = 0; i < nr; i++) {
        m[i] = (int *) malloc(nc*sizeof(int));
    }
    *mp = m;
}

void separa(int **m, int **white_p, int **black_p, int nr, int nc) {
    int x, y, i, j, n = 0, b = 0;

    x = ((nc * nr) % 2);
    y = ((nr * nc) / 2);

    switch(x) {
    case 0: //x pari
        *white_p = (int *) malloc(y *sizeof(int));
        *black_p = (int *) malloc(y * sizeof(int));
        break;

    default: //x dispari
        *white_p = (int *) malloc((y + 1) *sizeof(int));
        *black_p = (int *) malloc(y * sizeof(int));
        break;
    }

    for(i = 0; i < nr; i++) {
        for(j = 0; j < nc; j++) {
            if((i + j) % 2 == 0) {
                x = m[i][j];
                (*white_p)[b] = m[i][j];
                x = (*white_p)[b];
                b++;
            } else {
                (*black_p)[n] = m[i][j];
                n++;
            }
        }
    }
}
