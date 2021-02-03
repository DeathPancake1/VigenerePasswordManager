#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "helpFunctions.h"
#include "main.h"

void Load(){
    m=strlen(letters);
    FILE *f=fopen("Manager.txt","a+");
    fseek(f,0l,SEEK_SET);
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

void Quit(){
    printf("Are you sure you want to quit?\n1)Yes\n2)No\n");
    int choice;
    scanf("%d",&choice);
    switch(choice){
        case 1:
            end=1;
            return;
            break;
        case 2:
            return;
            break;
        default:
            printf("Enter a valid choice\n");
            Quit();
    }
}
void MENU(){
    Load();
    int choice;
    while(end!=1){
        printf("What to you want to do?\n");
        printf("1)Search the file.\n2)Add an entry.\n3)Delete an entry.\n4)Modify an existing entry.\n5)Print all your entries\n6)Exit\n");
        scanf("%d",&choice);
        switch(choice){
            case 1:
                Search();
                break;
            case 2:
                Add();
                break;
            case 3:
                Delete();
                break;
            case 4:
                Modify();
                break;
            case 5:
                ShowAll();
                break;
            case 6:
                Save();
                Quit();
                break;
            default:
                printf("Enter a valid choice");
        }
    }
}
