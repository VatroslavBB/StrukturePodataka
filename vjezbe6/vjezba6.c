#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_SIZE 100
#define DATE_EQ 0
#define DATE_LATER 1
#define DATE_EARLIER -1

typedef struct datetime{
    int year;
    int month;
    int day;
}DateTime;

DateTime NULL_DATE = {
    .year = 0,
    .month = 0,
    .day = 0
};

typedef struct article *ArticlePosition;

typedef struct article{
    char name[MAX_SIZE];
    int quantity;
    float price;
    ArticlePosition next;
}Article;

Article NULL_ART = {
    .name = {0},
    .price = 0.0,
    .quantity = 0,
    .next = NULL
};

typedef struct receipt* ReceiptPosition;

typedef struct receipt{
    DateTime date;
    ArticlePosition ArticleHead;
    ReceiptPosition next;
}Receipt;

int CompareDates(DateTime d1, DateTime d2);
DateTime SetDate(int year, int month, int day);
int PrintDateTime(DateTime d);

ReceiptPosition CreateReceipt(DateTime date);
int InsertAfterReceipt(ReceiptPosition previous, ReceiptPosition newReceipt);
int ReadReceipts(ReceiptPosition first, FILE* file);
int SortedInputReceipt(ReceiptPosition head, ReceiptPosition new);
int PrintReceipts(ReceiptPosition first);
int MergeReceipts(ReceiptPosition r1, ReceiptPosition r2);
int ReadReceipts(ReceiptPosition head, FILE* file);
int DeleteReceipts(ReceiptPosition first);
int PrintReceipts(ReceiptPosition first);

ArticlePosition CreateArticle(char* name, int quantity, float price);
int InsertAfterArticle(ArticlePosition previous, ArticlePosition new);
int SortedInputArticle(ArticlePosition head, ArticlePosition new);
int DeleteArticles(ArticlePosition first);
int PrintArticles(ArticlePosition first);








int main(){
    Receipt ReceiptHead = {
        .date = NULL_DATE,
        .next = NULL,
        .ArticleHead = &NULL_ART
    };

    FILE* file = NULL;
    file = fopen("racuni.txt", "r");

    ReadReceipts(&ReceiptHead, file);

    PrintReceipts(ReceiptHead.next);

    printf("\noashfoashdf\n");

    fclose(file);
    return 0;
}








int CompareDates(DateTime d1, DateTime d2){
    if(d1.year > d2.year){
        return DATE_LATER;
    }else if(d1.year < d2.year){
        return DATE_EARLIER;
    }else{
        if(d1.month > d2.month){
            return DATE_LATER;
        }else if(d1.month < d2.month){
            return DATE_EARLIER;
        }else{
            if(d1.day > d2.day){
                return DATE_LATER;
            }else if(d1.day < d2.day){
                return DATE_EARLIER;
            }else{
                return DATE_EQ;
            }
        }
    }
}

DateTime SetDate(int year, int month, int day){
    DateTime newDate = NULL_DATE;
    newDate.year = year;
    newDate.month = month;
    newDate.day = day;
    return newDate;
}

int PrintDateTime(DateTime d){
    printf("%d.%d.%d\n", d.day, d.month, d.year);
    return EXIT_SUCCESS;
}

int PrintReceipts(ReceiptPosition first){
    ReceiptPosition temp = NULL;
    temp = first;
    int i = 1;
    while(temp){
        printf("\nRacun%d.txt: \n", i);
        PrintDateTime(temp->date);
        PrintArticles(temp->ArticleHead->next);
        i++;
        temp = temp->next;
    }
    return EXIT_SUCCESS;
}

int PrintArticles(ArticlePosition first){
    ArticlePosition temp = NULL;
    temp = first;
    while(temp){
        printf("Naziv artikla: %s\n", temp->name);
        printf("Kolicina artikla: %d\n", temp->quantity);
        printf("Cijena artikla: %.3f\n", temp->price);
        temp = temp->next;
    }
    return EXIT_SUCCESS;
}

ReceiptPosition CreateReceipt(DateTime d){
    ReceiptPosition new = NULL;
    new = (ReceiptPosition)malloc(sizeof(Receipt));
    if(new == NULL){
        printf("Nije bilo mjesta u memoriji za novi racun.\n");
        return NULL;
    }
    new->date = d;
    new->next = NULL;
    new->ArticleHead = &NULL_ART;
    
    return new;
}

int InsertAfterReceipt(ReceiptPosition previous, ReceiptPosition newReceipt){
    newReceipt->next = previous->next;
    previous->next = newReceipt;
    return EXIT_SUCCESS;
}

ArticlePosition CreateArticle(char* name, int quantity, float price){
    ArticlePosition new = NULL;
    new = (ArticlePosition)malloc(sizeof(Article));
    if(new == NULL){
        printf("Nije bilo mjesta u memoriji za novi racun.\n");
        return NULL;
    }
    strcpy(new->name, name);
    new->quantity = quantity;
    new->price = price;
    new->next = NULL;
    return new;
}

int InsertAfterArticle(ArticlePosition previous, ArticlePosition new){
    new->next = previous->next;
    previous->next = new;
    return EXIT_SUCCESS;
}

int ReadReceipts(ReceiptPosition head, FILE* file){
    FILE* receipt = NULL;
    if(file == NULL){
        printf("Nije bilo moguce otvoriti file.\n");
        return EXIT_FAILURE;
    }
    char FileLine[MAX_SIZE];
    int year = 0, month = 0, day = 0, qnt = 0;
    char name[MAX_SIZE];
    float price = 0.0;
    while(fscanf(file, "%s", FileLine) == 1){
        receipt = fopen(FileLine, "r");
        if(fscanf(receipt, "%d-%d-%d", &year, &month, &day) != 3){
            printf("Nije bilo moguce procitati racun %s\n", FileLine);
            continue;
        }
        ReceiptPosition newReceipt = NULL;
        newReceipt = CreateReceipt(SetDate(year, month, day));
        while(fscanf(receipt, "%s %d %f", name, &qnt, &price) == 3){
            SortedInputArticle(newReceipt->ArticleHead, CreateArticle(name, qnt, price));
        }
        SortedInputReceipt(head, newReceipt);
        fclose(receipt);
        free(newReceipt);
    }
    return EXIT_SUCCESS;
}

int SortedInputReceipt(ReceiptPosition head, ReceiptPosition new){
    ReceiptPosition temp = NULL;
    temp = head;
    int state = 7;
    while(temp->next){
        state = CompareDates(new->date, temp->date);
        if(state != DATE_LATER){
            if(state == DATE_EQ){
                MergeReceipts(temp, new);
            }
            else{
                InsertAfterReceipt(temp, new);
            }
            return EXIT_SUCCESS;
        }
        temp = temp->next;
    }
    InsertAfterReceipt(temp, new);
    return EXIT_SUCCESS;
}

int SortedInputArticle(ArticlePosition head, ArticlePosition new){
    ArticlePosition temp = NULL;
    temp = head;
    while(temp->next){
        if(strcmp(temp->name, new->name) == 0){
            temp->quantity += new->quantity;
            free(new);
            return EXIT_SUCCESS;
        }
        temp = temp->next;
    }
    InsertAfterArticle(temp, new);
    return EXIT_SUCCESS;
}

int MergeReceipts(ReceiptPosition r1, ReceiptPosition r2){
    ArticlePosition head = NULL, ToCopy = NULL;
    head = r1->ArticleHead;
    ToCopy = r2->ArticleHead->next;
    while(ToCopy){
        SortedInputArticle(head, ToCopy);
        ToCopy = ToCopy->next;
    }
    free(r2);
    return EXIT_SUCCESS;
}

