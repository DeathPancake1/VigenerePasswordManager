#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "main.h"

void Encrypt(char *key,char *password){
    int l1,l2,i=0,j=0;
    l1=strlen(key);
    l2=strlen(password);
    int passwordInt[l2],keyInt[l1];
    for(int l=0;l<l2;l++){
        for(int k=0;k<m;k++){
            if(password[l]==letters[k]){
                passwordInt[l]=k;
            }
        }
    }
    for(int l=0;l<l1;l++){
        for(int k=0;k<m;k++){
            if(key[l]==letters[k]){
                keyInt[l]=k;
            }
        }
    }
    while(i<l2){
        if(j>=l1){
            j=0;
        }
        int x=mod((passwordInt[i]+keyInt[j]),m);
        password[i]=letters[x];
        i++;
        j++;
    }
}

void Decrypt(char *key,char *password){
    int l1,l2,i=0,j=0;
    l1=strlen(key);
    l2=strlen(password);
    int passwordInt[l2],keyInt[l1];
    for(int l=0;l<l2;l++){
        for(int k=0;k<m;k++){
            if(password[l]==letters[k]){
                passwordInt[l]=k;
            }
        }
    }
    for(int l=0;l<l1;l++){
        for(int k=0;k<m;k++){
            if(key[l]==letters[k]){
                keyInt[l]=k;
            }
        }
    }
    while(i<l2){
        if(j>=l1){
            j=0;
        }
        int x=mod((passwordInt[i]-keyInt[j]),m);
        password[i]=letters[x];
        i++;
        j++;
    }
}
