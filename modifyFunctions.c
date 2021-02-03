#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sodium.h>
#include <ctype.h>
#include "encrypt.h"
#include "helpFunctions.h"
#include "main.h"

void Add(){
    time_t t ;
    struct tm *tmp ;
    char MY_TIME[50];
    time( &t );
    char website[100],ID[100],password[100],key[100];
    uint32_t myInt[16];
    int i;
    unsigned int year,day,month,hour,minute,choice;
    printf("Enter the website name: ");
    scanf("%s",website);
    printf("Enter the ID: ");
    scanf("%s",ID);
    printf("Do you want to insert a password or have a password generated for you?\n1)Enter password\n2)Generate password\n");
    scanf("%d",&choice);
    switch(choice){
        case 1:
            printf("Enter the password: ");
            scanf("%s",password);
            break;
        case 2:
            if (sodium_init() == -1) {
                return;
            }
            for(i=0;i<16;i++){
                myInt[i] = randombytes_uniform(m);
            }
            for(i=0;i<16;i++){
                password[i]=letters[myInt[i]];
            }
            password[i]='\0';
            printf("Your password is %s\n",password);
            break;
        default:
            printf("Enter a valid choice\n");
            return;
    }
    printf("What is the key you want to use(you have to remember it and keep it secret, you can reuse it): ");
    scanf("%s",key);
    Encrypt(key,password);
    tmp=localtime(&t);
    strftime(MY_TIME,sizeof(MY_TIME),"%x-%H:%M",tmp);
    char *tok=strtok(MY_TIME,"-:/");
    month=makeDigit(tok);
    for(int i=0;i<4;i++){
        tok=strtok(NULL,"-:/");
        switch(i){
            case 0:
                day=makeDigit(tok);
                break;
            case 1:
                year=makeDigit(tok);
                break;
            case 2:
                hour=makeDigit(tok);
                break;
            case 3:
                minute=makeDigit(tok);
                break;
        }
    }
    numRecords++;
    content=(Log *)realloc(content,numRecords*sizeof(Log));
    strcpy(content[numRecords-1].website,website);
    strcpy(content[numRecords-1].password,password);
    strcpy(content[numRecords-1].ID,ID);
    content[numRecords-1].dateTime.year=year;
    content[numRecords-1].dateTime.month=month;
    content[numRecords-1].dateTime.day=day;
    content[numRecords-1].dateTime.hour=hour;
    content[numRecords-1].dateTime.minute=minute;
}
void Modify(){
    char query[100];
    printf("Enter the website name or the ID of the entry you want to modify: ");
    scanf("%s",query);
    int f=0,index;
    for(int i=0;i<numRecords;i++){
        if(strcmp(content[i].website,query)==0||strcmp(content[i].ID,query)==0){
            index=i;
            f=1;
        }
    }
    if(f==0){
        printf("Unable to find %s in the file.\n",query);
        return;
    }
    time_t t ;
    struct tm *tmp ;
    char MY_TIME[50];
    time( &t );
    char website[100],ID[100],password[100],key[100];
    uint32_t myInt[16];
    int i;
    unsigned int year,day,month,hour,minute,choice;
    printf("Enter the website name: ");
    scanf("%s",website);
    printf("Enter the ID: ");
    scanf("%s",ID);
    printf("Do you want to insert a password or have a password generated for you?\n1)Enter password\n2)Generate password\n");
    scanf("%d",&choice);
    switch(choice){
        case 1:
            printf("Enter the password: ");
            scanf("%s",password);
            break;
        case 2:
            if (sodium_init() == -1) {
                return;
            }
            for(i=0;i<16;i++){
                myInt[i] = randombytes_uniform(m);
            }
            for(i=0;i<16;i++){
                password[i]=letters[myInt[i]];
            }
            password[i]='\0';
            printf("Your password is %s\n",password);
            break;
        default:
            printf("Enter a valid choice\n");
            return;
    }
    printf("What is the key you want to use(you have to remember it and keep it secret, you can reuse it): ");
    scanf("%s",key);
    Encrypt(key,password);
    tmp=localtime(&t);
    strftime(MY_TIME,sizeof(MY_TIME),"%x-%H:%M",tmp);
    char *tok=strtok(MY_TIME,"-:/");
    month=makeDigit(tok);
    for(int i=0;i<4;i++){
        tok=strtok(NULL,"-:/");
        switch(i){
            case 0:
                day=makeDigit(tok);
                break;
            case 1:
                year=makeDigit(tok);
                break;
            case 2:
                hour=makeDigit(tok);
                break;
            case 3:
                minute=makeDigit(tok);
                break;
        }
    }
    strcpy(content[index].website,website);
    strcpy(content[index].password,password);
    strcpy(content[index].ID,ID);
    content[index].dateTime.year=year;
    content[index].dateTime.month=month;
    content[index].dateTime.day=day;
    content[index].dateTime.hour=hour;
    content[index].dateTime.minute=minute;
}

void Delete(){
    char query[100];
    printf("Enter the website name or the ID of the entry you want to delete: ");
    scanf("%s",query);
    int f=0,index;
    for(int i=0;i<numRecords;i++){
        if(strcmp(content[i].website,query)==0||strcmp(content[i].ID,query)==0){
            index=i;
            f=1;
        }
    }
    if(f==0){
        printf("Unable to find %s in the file.\n",query);
        return;
    }
    else{
        for(int i=0,j=0;i<numRecords;i++){
            if(i!=index){
                content[j]=content[i];
                j++;
            }
        }
    }
    numRecords--;
    content=(Log *)realloc(content,numRecords*sizeof(Log));
}

void Search(){
    char key[100],query[100],decrypted[100];;
    printf("what is the key you used to decrypt the password?\n");
    scanf("%s",key);
    printf("Enter the website name or the ID: ");
    scanf("%s",query);
    int f=0;
    for(int i=0;i<numRecords;i++){
        if(strcmp(content[i].website,query)==0||strcmp(content[i].ID,query)==0){
            printf("\nWebsite:%s ID:%s Date/Time:%d/%d/%d-%0d:%0d\n",content[i].website,content[i].ID,content[i].dateTime.day,content[i].dateTime.month,content[i].dateTime.year,content[i].dateTime.hour,content[i].dateTime.minute);
            strcpy(decrypted,content[i].password);
            Decrypt(key,decrypted);
            printf("The password is:'%s'\n",decrypted);
            f=1;
        }
    }
    if(f==0){
        printf("Unable to find %s in the file.\n",query);
    }
}

void Save(){
    FILE *f=fopen("Manager.txt","w");
    char buffer[100];
    for(int i=0;i<numRecords;i++){
        sprintf(buffer,"%s,%s,%s,%d/%d/%d-%0d:%0d\n",content[i].website,content[i].ID,content[i].password,content[i].dateTime.day,content[i].dateTime.month,content[i].dateTime.year,content[i].dateTime.hour,content[i].dateTime.minute);
        fputs(buffer,f);
    }
    fclose(f);
}

void ShowAll(){
    printf("-----------------All Entries-----------------\n");
    for(int i=0;i<numRecords;i++){
        printf("Website:%s ID:%s Date/Time:%d/%d/%d-%0d:%0d\n",content[i].website,content[i].ID,content[i].dateTime.day,content[i].dateTime.month,content[i].dateTime.year,content[i].dateTime.hour,content[i].dateTime.minute);
    }
}
