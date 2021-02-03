typedef struct{
    unsigned int year,day,month,hour,minute;
}dateTime;

typedef struct{
    char website[100],ID[100],password[100];
    dateTime dateTime;
}Log;
extern Log *content;
extern int numRecords,m,end;
extern char letters[];
