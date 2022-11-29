//	Alessio Flamini s268460

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define N 30

char *cercaRegexp(char *src, char *regexp);

int main()
{
    char src[N], regexp[N];

    printf("Inserisci la stringa: ");
    scanf("%s", src);
    printf("Inserisci la regexp: ");
    scanf("%s", regexp);

    printf("Ho trovato: %s", cercaRegexp(src, regexp));
    return 0;
}

char *cercaRegexp(char *src, char *regexp){
    int i = 0, j = 0, l = 0, t = 0, found = 0, flag;
    while (src[i] != '\0' && !found) {
        j = l = 0;
        found = 1;
        while(regexp[j] != '\0' && src[i+l] != '\0' && found) {
            if(regexp[j] == '[') {
                if(regexp[j+1] != '^') {
                    flag = 0;
                    for(t = j+1; regexp[t] != ']'; t++) {
                        if(src[i+l] == regexp[t])
                            flag = 1;
                    }
                    if(flag == 1) {
                        j = t;
                    } else {
                        found = 0;
                    }
                } else {
                    flag = 0;
                    for(t = j+2; regexp[t] != ']' && !flag; t++) {
                        if(src[i+l] == regexp[t])
                            flag = 1;
                    }
                    if(flag == 1) {
                        found = 0;
                    } else {
                        j = t;
                    }
                }
            } else if(regexp[j] == '\\') {
                if(regexp[j+1] == 'a') {
                    if(!islower(src[i+l]))
                        found = 0;
                }else if(regexp[j] == 'A'){
                    if(!isupper(src[i+l]))
                        found = 0;
                }
            } else if(regexp[j] == '.') {
                if(regexp[j+1] != src[i+l+1])
                    found = 0;
            }
            j++; l++;
        }
        i++;
        if(src[i] == '\0' && regexp[j] != '\0')
            found = 0;
    }
    if(!found) return NULL;
    return &src[--i];
}
