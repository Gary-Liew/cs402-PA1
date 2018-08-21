//
//  warmup1.c
//  PA1 code
//
//  Created by YING LIU on 2017/1/20.
//  Copyright © 2017年 YING LIU. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/stat.h>
#include "cs402.h"
#include <errno.h>
#include "my402list.h"
//define a  structure of transaction
typedef struct tagMy402banktransaction{
    char type;
    time_t time;
    unsigned long int amount;
    char* description;
}My402transaction;

//long int balance=0;
//remove the beginning space and end space with '\n'.
char *removespace(char *str){
    if(str==NULL) return NULL;
    while(*str==' ') str++;
    if(*str=='\0'){
        return str;
    }
    char *end;
    end= str+strlen(str)-1;
    while((end>str)&&((*end==' ')||(*end=='\n'))) end--;
    *(end+1)='\0';
    return str;
}
//determine if command input is valid.
int commandline(int argc, char *argv[]){
    int returnvalue=0;
    if(argc<2||argc>3){
        fprintf(stderr,"The number of argument in command line is invalid. Usage: warmup1 sort [tfile]. \n");
        exit(1);
    }
    else if(strcmp(argv[1], "sort")){
        fprintf(stderr,"Invalid command line argument,sort should appear in the second argument.Usage: warmup1 sort [tfile].\n");
        exit(1);
    }
    else if(argc==2){
        returnvalue=0;
    }
    else{
        returnvalue=1;
    }
    return returnvalue;
}
//process type in one transaction.
void process_type(char *str,My402transaction* transaction,int line){
    int i=0;
    if(strlen(str)>1){
        fprintf(stderr,"Transaction should have only one character.The error line is %d\n",line);
        exit(1);
    }
    else if(str[i]!='-'&&str[i]!='+'){
        fprintf(stderr,"Transaction should be + or -.The error line is %d\n",line);
        exit(1);
    }
    else if(!strcmp(str,"-")){
        transaction->type='-';
    }
    else{
        transaction->type='+';
    }
}
//process timestamp of one transaction
void process_timestamp(char *str,My402transaction* transaction,int line){
    if(strlen(str)>10){
        fprintf(stderr,"Transaction timestamp should less than 11 digit.The error line is %d\n",line);
        exit(1);
    }
    char *ptr;
    unsigned long timestamp=(unsigned long)strtoul(str, &ptr, 0);
    unsigned long currentime=(unsigned long)time(NULL);
    int i=0;
    unsigned long k=strlen(str);
    while(i<k){
        if(str[i]<'0'||str[i]>'9'){
            fprintf(stderr,"Transaction timestamp should be number.The error line is %d\n",line);
            exit(1);
        }
        i++;
    }
    /*if(ptr!=NULL){
        fprintf(stderr,"Transaction timestamp should be number.\n");
        exit(1);
    }*/
        if(timestamp>currentime){
            fprintf(stderr,"Transaction timestamp should between 0 and the current time.The error line is %d\n",line);
            exit(1);
        }
        else{
            transaction->time=(time_t)timestamp;
        }
}
//process amount of one transaction
void process_amount(char *str,My402transaction* transaction,int line){
    char character;
    int j=0;
    int k=0;
    char transamount[10];
    unsigned long totalamount = 0;
    while(str[j]!='.'){
        character=str[j];
        if(j>=7){
            fprintf(stderr,"Transaction amount is too large.The error line is %d\n",line);
            exit(1);
        }
        else if(!(character>= '0' && character <= '9')){
            fprintf(stderr,"Transaction amount must be a number.The error line is %d\n",line);
            exit(1);
        }
        else{
            totalamount=totalamount*10+(unsigned long)atoi(&character);
            transamount[k]=character;
            ++j;
            ++k;
        }
    }
    if(j==0){
        fprintf(stderr,"Transaction amount's format is wrong.The error line is %d\n",line);
        exit(1);
    }
    j++;
    int lengthafterpoint = 0;
    while(str[j]!='\0'){
        character=str[j];
        if(!(character>= '0' && character <= '9')){
            fprintf(stderr,"Transaction amount must be a number.The error line is %d\n",line);
            exit(1);
        }
        lengthafterpoint++;
        totalamount=totalamount*10+(unsigned long)atoi(&character);
        transamount[k]=character;
        ++k;
        ++j;
    }
    transamount[k]='\0';
    if(lengthafterpoint!=2){
        fprintf(stderr,"Transaction amount is too long.The error line is %d\n",line);
        exit(1);
    }
    transaction->amount=totalamount;
}
//process descripition of one transaction.
void process_description(char *str,My402transaction* transaction,int line){
    char *newstr=removespace(str);
    if(strlen(newstr)==0){
        fprintf(stderr,"Transaction description cannot be empty.The error line is %d\n",line);
        exit(1);
    }
    else{
        transaction->description=newstr;
    }
}
//read all the data into one element of My402List
int readdata(char *str,My402List *mylist,int line){
    char *delimeter="\t";
    char *token=NULL;
    int count=0;
    char *item[4];
    int i=0;
    for(i=0;i<4;i++){
        item[i]=NULL;
    }
    char *temp=malloc(sizeof(char) * strlen(str));
    strcpy(temp,str);
    temp=removespace(temp);
    if(temp==NULL){
        return 0;
    }
    else if(temp=='\0'){
        fprintf(stderr,"Transaction's one line cannot be empty.The error line is %d\n",line);
        exit(1);
    }
    else{
        strcpy(str,temp);
        free(temp);
        char *temp1=malloc(sizeof(char) * strlen(str));
        strcpy(temp1,str);
        token=strtok(temp1,delimeter);
        while(token!=NULL){
            if(count<4){
            item[count]=strdup(token);
            count++;
            token=strtok(NULL,delimeter);
            }
            else{
                fprintf(stderr,"Transaction have more than 4 fields.The error line is %d\n",line);
                exit(1);
            }
        }
        free(temp1);
        if(count!=4){
            fprintf(stderr,"Transaction should have 4 fields.The error line is %d\n",line);
            exit(1);
        }
        else{
            My402transaction* transaction=(My402transaction*)malloc(sizeof(My402transaction));
            process_type(item[0],transaction,line);
            process_timestamp(item[1],transaction,line);
            process_amount(item[2],transaction,line);
            process_description(item[3],transaction,line);
            transaction=(void *)transaction;
            My402ListPrepend(mylist, transaction);
            return 1;
        }
        
    }
}
// read from file to My402List
int readfromfile(FILE *file,My402List *mylist,int line){
    char buf[1026];
    while(fgets(buf,sizeof(buf),file)!=NULL){
        line++;
        if(strcmp(buf,"\n")!=0){
            if(strlen(buf)>1024){
                fprintf(stderr,"Transaction length cannot more than 1024 character.The erroe line is %d\n",line);
                exit(1);

            }
            else{
                char *tempbuf=malloc(sizeof(char) * strlen(buf)+1);
                strcpy(tempbuf,buf);
                int readresult;
                readresult=readdata(tempbuf, mylist,line);
                if(readresult==0){
                    fprintf(stderr,"Cannot read the file.\n");
                    exit(1);
                }

            }
        }
        
    }
    return line;
}

//switch two elements of one My402List
static
void BubbleForward(My402List *pList, My402ListElem **pp_elem1, My402ListElem **pp_elem2)
/* (*pp_elem1) must be closer to First() than (*pp_elem2) */
{
    My402ListElem *elem1=(*pp_elem1), *elem2=(*pp_elem2);
    void *obj1=elem1->obj, *obj2=elem2->obj;
    My402ListElem *elem1prev=My402ListPrev(pList, elem1);
    /*  My402ListElem *elem1next=My402ListNext(pList, elem1); */
    /*  My402ListElem *elem2prev=My402ListPrev(pList, elem2); */
    My402ListElem *elem2next=My402ListNext(pList, elem2);
    
    My402ListUnlink(pList, elem1);
    My402ListUnlink(pList, elem2);
    if (elem1prev == NULL) {
        (void)My402ListPrepend(pList, obj2);
        *pp_elem1 = My402ListFirst(pList);
    } else {
        (void)My402ListInsertAfter(pList, obj2, elem1prev);
        *pp_elem1 = My402ListNext(pList, elem1prev);
    }
    if (elem2next == NULL) {
        (void)My402ListAppend(pList, obj1);
        *pp_elem2 = My402ListLast(pList);
    } else {
        (void)My402ListInsertBefore(pList, obj1, elem2next);
        *pp_elem2 = My402ListPrev(pList, elem2next);
    }
}

//sort My402List according to the value of timestamp
static void sorting(My402List *plist){
    My402ListElem *elem=NULL;
    int i=0;
        for (i=0; i < My402ListLength(plist); i++) {
        int j=0, something_swapped=FALSE;
        My402ListElem *next_elem=NULL;
        
        for (elem=My402ListFirst(plist), j=0; j < My402ListLength(plist)-i-1; elem=next_elem, j++) {
            int cur_val=(int)(((My402transaction*)elem->obj)->time), next_val=0;
            
            next_elem=My402ListNext(plist, elem);
            next_val = (int)(((My402transaction*)next_elem->obj)->time);
            
            if (cur_val > next_val) {
                BubbleForward(plist, &elem, &next_elem);
                something_swapped = TRUE;
            }
            else if(cur_val==next_val){
                fprintf(stderr,"Transaction timestamp is duplicate.\n");
                exit(1);
            }
        }
        if (!something_swapped) break;
    }
    

}

//print the first line;
void print_line(){
    fprintf(stdout,"+-----------------+--------------------------+----------------+----------------+\n");
}

//print the first part.
void print_title(){
    print_line();
    fprintf(stdout,"|       Date      | Description              |         Amount |        Balance |\n");
    print_line();
}

//print the timestamp part of one element in My402List
void print_time(My402ListElem *elem){
    time_t timer=((My402transaction *)elem->obj)->time;
    char *timer1=ctime(&timer);
    char timestamp[16];
    int i=0;
    int j=0;
    while(i<15){
        if(i>10){j=i+9;}
        timestamp[i++]=timer1[j++];
    }
    timestamp[i]='\0';
    fprintf(stdout,"| %s |",timestamp);
}

//print description part of one element in My402List
void print_description(My402ListElem *elem){
    int i=0;
    char * desc=((My402transaction *)elem->obj)->description;
    char description[25];
    strncpy(description,desc,24);
    while(description[i] !='\0' && i<24){
        i++;
    }
    int j=0;
    for(j=i;j<24;j++){
        description[j]=' ';
    }
    description[j]='\0';
    fprintf(stdout," %s |",description);
}

//print amount part of one element in the My402List according to the rule
void process_value(char type,unsigned long int amount){
    char array[15];
    if(type=='+'){
        if(amount>=1000000000){
            fprintf(stdout,"  \?,\?\?\?,\?\?\?.\?\?  |");
        }
        else{
            array[0]=' ';
            array[13]=' ';
            int i=12;
            int j=1;
            while(j<10){
                int remainder=amount%10;
                amount=amount/10;
                if(j==3) array[i--]='.';
                else if((j-3)%3==0) array[i--]=',';
                array[i--]='0'+remainder;
                j++;
                if(amount==0){
                    break;
                }
            }
            if(i==11){
                array[i]='0';i--;
                array[i]='.';i--;
                array[i]='0';i--;
            }
            if(i==10){
                array[i]='.';i--;
                array[i]='0';i--;
            }
            while(i>0){
                array[i]=' ';
                i--;
            }
            array[14]='\0';
            fprintf(stdout," %s |",array);
        }
    }
    else{
        if(amount>=1000000000){
            fprintf(stdout," (\?,\?\?\?,\?\?\?.\?\?) |");
        }
        else{
            array[0]='(';
            array[13]=')';
            int i=12;
            int j=1;
            while(j<10){
                int remainder=amount%10;
                amount=amount/10;
                if(j==3) array[i--]='.';
                else if((j-3)%3==0) array[i--]=',';
                array[i--]='0'+remainder;
                j++;
                if(amount==0){
                    break;
                }
            }
            if(i==11){
                array[i]='0';i--;
                array[i]='.';i--;
                array[i]='0';i--;
            }
            if(i==10){
                array[i]='.';i--;
                array[i]='0';i--;
            }
            while(i>0){
                array[i]=' ';
                i--;
            }
            array[14]='\0';
            fprintf(stdout," %s |",array);

        }

    }
}

//print amount part of one element in the My402List
void print_amount(My402ListElem *elem){
    unsigned long int amount=((My402transaction *)elem->obj)->amount;
    char type=((My402transaction *)elem->obj)->type;
    process_value(type,amount);
}


void process_balance(long int bal){
    char list[15];
    if(bal>=0){
        if(bal>=1000000000){
            fprintf(stdout,"  \?,\?\?\?,\?\?\?.\?\?  |\n");
        }
        else{
            list[0]=' ';
            list[13]=' ';
            int i=12;
            int j=1;
            while(j<10){
                int remainder=bal%10;
                bal=bal/10;
                if(j==3) list[i--]='.';
                else if((j-3)%3==0) list[i--]=',';
                list[i--]='0'+remainder;
                j++;
                if(bal==0){
                    break;
                }
            }
            if(i==11){
                list[i]='0';i--;
                list[i]='.';i--;
                list[i]='0';i--;
            }
            if(i==10){
                list[i]='.';i--;
                list[i]='0';i--;
            }
            while(i>0){
                list[i]=' ';
                i--;
            }
            list[14]='\0';
            fprintf(stdout," %s |\n",list);

        }
    }
    else{
        bal=bal*(-1);
        if(bal>=1000000000){
            fprintf(stdout," (\?,\?\?\?,\?\?\?.\?\?) |\n");
        }
        else{
            list[0]='(';
            list[13]=')';
            int i=12;
            int j=1;
            while(j<10){
                int remainder=bal%10;
                bal=bal/10;
                if(j==3) list[i--]='.';
                else if((j-3)%3==0) list[i--]=',';
                list[i--]='0'+remainder;
                j++;
                if(bal==0){
                    break;
                }
            }
            if(i==11){
                list[i]='0';i--;
                list[i]='.';i--;
                list[i]='0';i--;
            }
            if(i==10){
                list[i]='.';i--;
                list[i]='0';i--;
            }
            while(i>0){
                list[i]=' ';
                i--;
            }
            list[14]='\0';
            fprintf(stdout," %s |\n",list);
        }
    }
}

//print balance part of one element in the My402List according to the rule
long int print_balance(My402ListElem *elem,long int balance){
    unsigned long int amount=((My402transaction *)elem->obj)->amount;
    char type=((My402transaction *)elem->obj)->type;
    if(type=='+') balance=balance+amount;
    else balance= balance-amount;
    long int bal =balance;
    process_balance(bal);
    return balance;
}

//print balance part of one element in the My402List
void printall(My402List *mylist){
    My402ListElem *elem=NULL;
    long int balance=0;
    print_title();
    //print_time(elem);
    for(elem=My402ListFirst(mylist);elem!=NULL;elem=My402ListNext(mylist,elem)){
        print_time(elem);
        print_description(elem);
        print_amount(elem);
        balance=print_balance(elem,balance);
    }
    print_line();
}


int main(int argc,char *argv[]){
    struct stat info;
    FILE *file=NULL;
    int line=0;
    if(commandline(argc, argv)){
        stat(argv[2],&info);
        if(S_ISDIR(info.st_mode)){
            fprintf(stderr,"Input argument is a directory.\n");
            exit(1);
        }
        My402List mylist;
        memset(&mylist, 0, sizeof(My402List));
        (void)My402ListInit(&mylist);
        file=fopen(argv[2],"r");
        if(argv[2][0]=='-'){
            if(file==NULL){
                fprintf(stderr,"input file \"%s\" does not exist.Usage: warmup1 sort [tfile]\n",argv[2]);
                exit(1);

            }
        }
        if(file==NULL){
            fprintf(stderr,"%s\n",strerror(errno));
            exit(1);
        }
        else{
            while(!feof(file)){
                readfromfile(file, &mylist,line);
            }
            fclose(file);
            sorting(&mylist);
            printall(&mylist);
        }
        
        
    }
    else{
        file=stdin;
        My402List mylist;
        memset(&mylist, 0, sizeof(My402List));
        (void)My402ListInit(&mylist);
        if(file==NULL){
            fprintf(stderr,"%s\n",strerror(errno));
            exit(1);
        }
        else{
            while(!feof(file)){
                readfromfile(file, &mylist,line);
            }
            fclose(file);
            sorting(&mylist);
            printall(&mylist);
        }
    }
    return(0);
}








