#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sodium.h>
#include <ctype.h>

typedef struct{
    unsigned int year,day,month,hour,minute;
}dateTime;

typedef struct{
    char website[100],ID[100],password[100];
    dateTime dateTime;
}Log;

Log *content;
int numRecords=0,m=0;
char letters[]={"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 !@#$%"};

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

void Load(){
    m=strlen(letters);
    FILE *f=fopen("Manager.txt","r");
    char buffer[300];
    while(fgets(buffer,sizeof(buffer),f)){
        numRecords++;
    }
    fseek(f,0l,SEEK_SET);
    content=(Log *)calloc(numRecords,sizeof(Log));
    for(int i=0;i<numRecords;i++){
        fgets(buffer,sizeof(buffer),f);
        char *tok=strtok(buffer,",/-:");
        strcpy(content[i].website,tok);
        for(int j=0;j<7;j++){
            tok=strtok(NULL,",/:-");
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
    }
    fclose(f);
}

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

void Search(){
    char key[100],query[100],decrypted[100];;
    printf("what is the key you used to decrypt the password?\n");
    scanf("%s",key);
    printf("Enter the website name or the ID: ");
    scanf("%s",query);
    int f=0;
    for(int i=0;i<numRecords;i++){
        if(strcmp(content[i].website,query)==0||strcmp(content[i].ID,query)==0){
            printf("\nWebsite:%s ID:%s Date/Time:%d/%d/%d-%00d:%00d\n",content[i].website,content[i].ID,content[i].dateTime.day,content[i].dateTime.month,content[i].dateTime.year,content[i].dateTime.hour,content[i].dateTime.minute);
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
        sprintf(buffer,"%s,%s,%s,%d/%d/%d-%00d:%00d\n",content[i].website,content[i].ID,content[i].password,content[i].dateTime.day,content[i].dateTime.month,content[i].dateTime.year,content[i].dateTime.hour,content[i].dateTime.minute);
        fputs(buffer,f);
    }
    fclose(f);
}

void Quit(){
    printf("Are you sure you want to quit?\n1)Yes\n2)No\n");
    int choice;
    scanf("%d",&choice);
    switch(choice){
        case 1:
            exit(0);
            break;
        case 2:
            return;
            break;
        default:
            printf("Enter a valid choice");
            Quit();
    }
}

void ShowAll(){
    printf("-----------------All Entries-----------------\n");
    for(int i=0;i<numRecords;i++){
        printf("Website:%s ID:%s Date/Time:%d/%d/%d-%00d:%00d\n",content[i].website,content[i].ID,content[i].dateTime.day,content[i].dateTime.month,content[i].dateTime.year,content[i].dateTime.hour,content[i].dateTime.minute);
    }
}

void Modify(){
    char query[100],decrypted[100];;
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

int main()
{
    Load();
    Modify();
    ShowAll();
    Quit();
    return 0;
}
