#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct{
    unsigned int year,day,month,hour,minute;
}dateTime;

typedef struct{
    char website[100],ID[100],password[100];
    dateTime dateTime;
}Log;

Log *content;
int numRecords=0;

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

void Load(){
    FILE *f=fopen("Manager.txt","r");
    char buffer[300];
    while(fgets(buffer,sizeof(buffer),f)){
        numRecords++;
    }
    fseek(f,0l,SEEK_SET);
    content=(Log *)calloc(numRecords,sizeof(Log));
    for(int i=0;i<numRecords;i++){
        fgets(buffer,sizeof(buffer),f);
        char *tok=strtok(buffer,",-/:");
        strcpy(content[i].website,tok);
        for(int j=0;j<7;j++){
            tok=strtok(NULL,",-/:");
            switch(j){
            case 0:
                strcpy(content[i].ID,tok);
                break;
            case 1:
                strcpy(content[i].password,tok);
                break;
            case 2:
                content[i].dateTime.day=makeDigit(tok);
                break;
            case 3:
                content[i].dateTime.month=makeDigit(tok);
                break;
            case 4:
                content[i].dateTime.year=makeDigit(tok);
                break;
            case 5:
                content[i].dateTime.hour=makeDigit(tok);
                break;
            case 6:
                content[i].dateTime.minute=makeDigit(tok);
                break;
            }
        }
    fclose(f);
    }
}
char *Encrypt(char *key,char *password){
    int l1,l2,i=0,j=0;
    l1=strlen(key);
    l2=strlen(password);
    char *encryptedPass=calloc(l2,sizeof(char));
    while(i<l2){
        if(j>=l1){
            j=0;
        }
        char x=(password[i]+key[j])%126;
        encryptedPass[i]=x;
        i++;
        j++;
    }
    return encryptedPass;
}

char *Decrypt(char *key,char *password){
    int l1,l2,i=0,j=0;
    l1=strlen(key);
    l2=strlen(password);
    char *decryptedPass=calloc(l2,sizeof(char));
    while(i<l2){
        if(j>=l1){
            j=0;
        }
        char x=(password[i]-key[j]);
        decryptedPass[i]=x+126;
        i++;
        j++;
    }
    return decryptedPass;
}
void Add(){
    time_t t ;
    struct tm *tmp ;
    char MY_TIME[50];
    time( &t );
    char website[100],ID[100],password[100],key[100];
    unsigned int year,day,month,hour,minute;
    printf("Enter the website name: ");
    scanf("%s",website);
    printf("Enter the ID: ");
    scanf("%s",ID);
    printf("Enter the password: ");
    scanf("%s",password);
    printf("What is the key you want to use(you have to remember it and keep it secret, you can reuse it): ");
    scanf("%s",key);
    strcpy(password,Encrypt(key,password));
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

int main()
{
    Load();
    Add();
    return 0;
}
