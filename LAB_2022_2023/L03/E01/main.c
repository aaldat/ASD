#include <stdio.h>
#include <stdlib.h>

int searchMajority(int *num, int numSize);
int searchMajorityR(int *num, int l, int r);
int count(int num[], int n, int l, int r);

int main()
{
    int dim, i, res;
    int *v;

    printf("Inserisci dimensione: ");
    scanf("%d", &dim);

    v = (int *) malloc(dim * sizeof(int));

    for(i = 0; i < dim; i++) {
        printf("Inserisci elemento %d: ", i);
        scanf("%d", v+i);
    }

    res = searchMajority(v, dim);

    if(res != -1) {
        printf("Elemento maggioritario: %d", res);
    } else {
        printf("Nessun elemento maggioritario!");
    }

    free(v);
    return 0;
}

int searchMajority(int *num, int numSize) {
    return searchMajorityR(num, 0, numSize-1);
}

int searchMajorityR(int *num, int l, int r) {
    int m = (r-l)/2 + l;
    int sx, dx, sxcnt, dxcnt;

    if(l==r) return num[l];

    sx = searchMajorityR(num, l, m);
    dx = searchMajorityR(num, m+1, r);

    if(sx == dx) return sx;

    sxcnt = count(num, sx, l, r);
    dxcnt = count(num, dx, l, r);

    return sxcnt > dxcnt ? sx : dx;
}

int count(int num[], int n, int l, int r) {
    int cnt = 0;
    for(int i = l; i <= r; i++) {
        if(num[i] == n) {
            cnt++;
        }
    }
    return cnt;
}
