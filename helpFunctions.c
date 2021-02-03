#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sodium.h>
#include <ctype.h>

int makeDigit(char *s){
    int i, j, l,n;
    char *t;
    l = strlen(s);
    t = malloc(l+1);
    for (i = 0, j = 0; i < l; i++) {
        if (isdigit(s[i]))
            t[j++] = s[i];
    }
    t[j] = '\0';
    n=atoi(t);
    return n;
}
int mod(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
}
