#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sodium.h>
#include <ctype.h>
#include "encrypt.h"
#include "helpFunctions.h"
#include "modifyFunctions.h"
#include "main.h"
#include "menu.h"

Log *content;
int numRecords=0,m=0,end=0;
char letters[]={"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 !@#$%"};


int main()
{
    printf("Vigenere C password manager\n");
    MENU();
    return 0;
}
