#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_SIZE 100
#define DATE_EQ 0
#define DATE_LATER 1
#define DATE_EARLIER -1

int test(){
    printf("\nstiglo do ovde\n");
    return EXIT_SUCCESS;
}

typedef struct registered *RegisteredPosition;

typedef struct registered{
    char name[MAX_SIZE];
    RegisteredPosition next;
}Registered;

Registered RegisteredHead = {
    .name = {0},
    .next = NULL
};

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
    Article ArticleHead;
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
int ReadReceipts(ReceiptPosition head, FILE* file);
int DeleteReceipts(ReceiptPosition head);
int PrintReceipts(ReceiptPosition first);
int MergeReceipts(ReceiptPosition r1, ReceiptPosition r2);

ArticlePosition CreateArticle(char* name, int quantity, float price);
int InsertAfterArticle(ArticlePosition previous, ArticlePosition new);
int SortedInputArticle(ArticlePosition head, ArticlePosition new);
int DeleteArticles(ArticlePosition head);
int PrintArticles(ArticlePosition first);

int question(ReceiptPosition head);
int PrintRegistered(RegisteredPosition first);
RegisteredPosition CreateRegistered(char* name);
int SortedInputRegistered(RegisteredPosition head, RegisteredPosition new);
int InsertAfterRegistered(RegisteredPosition prev, RegisteredPosition new);

int main(){
    Receipt ReceiptHead = {
        .date = NULL_DATE,
        .next = NULL,
        .ArticleHead = {
            .name = {0},
            .next = NULL,
            .quantity = 0,
            .price = 0.0
        }
    };
    FILE* file = NULL;
    file = fopen("racuni.txt", "r");
    ReadReceipts(&ReceiptHead, file);
    //PrintReceipts(ReceiptHead.next);
    fclose(file);

    question(&ReceiptHead);

    DeleteReceipts(&ReceiptHead);
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
    printf("%d.%d.%d", d.day, d.month, d.year);
    return EXIT_SUCCESS;
}

int PrintReceipts(ReceiptPosition first){
    ReceiptPosition temp = NULL;
    temp = first;
    int i = 1;
    while(temp){
        printf("\nRacun%d.txt: \n", i);
        PrintDateTime(temp->date);
        PrintArticles(temp->ArticleHead.next);
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
    new->ArticleHead = NULL_ART;
    
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

int InsertAfterRegistered(RegisteredPosition prev, RegisteredPosition new){
    new->next = prev->next;
    prev->next = new;
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
            SortedInputArticle(&newReceipt->ArticleHead, CreateArticle(name, qnt, price));
            SortedInputRegistered(&RegisteredHead, CreateRegistered(name));
        }
        SortedInputReceipt(head, newReceipt);
        fclose(receipt);
    }
    return EXIT_SUCCESS;
}

int SortedInputReceipt(ReceiptPosition head, ReceiptPosition new){
    ReceiptPosition temp = NULL;
    temp = head;
    int OnHead = 1;
    while(temp->next){
        if(CompareDates(temp->date, new->date) == DATE_EQ){
            MergeReceipts(temp, new);
            return EXIT_SUCCESS;
        }
        else if(CompareDates(temp->next->date, new->date) == DATE_LATER){
            InsertAfterReceipt(temp, new);
            return EXIT_SUCCESS;
        }
        temp = temp->next;
    }
    if(CompareDates(temp->date, new->date) == DATE_EQ){
        MergeReceipts(temp, new);
        return EXIT_SUCCESS;
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
        else if(strcmp(temp->next->name, new->name) > 0){
            InsertAfterArticle(temp, new);
            return EXIT_SUCCESS;
        }
        temp = temp->next;
    }
    if(strcmp(temp->name, new->name) == 0){
        temp->quantity += new->quantity;
        free(new);
        return EXIT_SUCCESS;
    }
    InsertAfterArticle(temp, new);
    return EXIT_SUCCESS;
}

int SortedInputRegistered(RegisteredPosition head, RegisteredPosition new){
    RegisteredPosition temp = NULL;
    temp = head;
    while(temp->next){
        if(strcmp(temp->name, new->name) == 0){
            return EXIT_SUCCESS;
        }
        else if(strcmp(temp->next->name, new->name) > 0){
            InsertAfterRegistered(temp, new);
            return EXIT_SUCCESS;
        }
        temp = temp->next;
    }
    if(strcmp(temp->name, new->name) != 0){
            InsertAfterRegistered(temp, new);
            return EXIT_SUCCESS;
        }
    return EXIT_SUCCESS;
}

int MergeReceipts(ReceiptPosition r1, ReceiptPosition r2){
    ArticlePosition temp = NULL;
    temp = r2->ArticleHead.next;
    while(temp){
        SortedInputArticle(&r1->ArticleHead, CreateArticle(temp->name, temp->quantity, temp->price));
        temp = temp->next;
    }
    free(r2);
    return EXIT_SUCCESS;
}

int DeleteReceipts(ReceiptPosition head){
    ReceiptPosition temp = NULL;
    while(head->next){
        temp = head->next;
        head->next = temp->next;
        DeleteArticles(&temp->ArticleHead);
        free(temp);
    }
    return EXIT_SUCCESS;
}

int DeleteArticles(ArticlePosition head){
    ArticlePosition temp = NULL;
    while(head->next){
        temp = head->next;
        head->next = temp->next;
        free(temp);
    }
    return EXIT_SUCCESS;
}

int question(ReceiptPosition head){
    printf("Kupljeni artikli:\n");
    int max = PrintRegistered(RegisteredHead.next) + 1;
    printf("\nUnesite broj od 1 do %d za unos imena artikla:\n", max);

    int art = 0;
    scanf("%d", &art);
    while(!(art > 0 && art < max+1)){
        printf("\nNiste unijeli odgovarajucu vrijednost, molimo pokusajte ponovo\n");
        scanf("%d", &art);
    }
    DateTime d1 = NULL_DATE, d2 = NULL_DATE;
    int y = 0, m = 0, d = 0;
    printf("Sada unesite dva datuma:\nPrvi Datum:\n");
    scanf("%d.%d.%d.", &d, &m, &y);
    d1 = SetDate(y, m, d);
    printf("Drugi datum:\n");
    scanf("%d.%d.%d.", &d, &m, &y);
    d2 = SetDate(y, m, d);
    while(CompareDates(d1, d2) != DATE_EARLIER){
        printf("Niste unijeli kronoloski ispravne datume, molimo pokusajte ponovo.\n");
        printf("Prvi Datum:\n");
        scanf("%d.%d.%d.", &d, &m, &y);
        d1 = SetDate(y, m, d);
        printf("Drugi datum:\n");
        scanf("%d.%d.%d.", &d, &m, &y);
        d2 = SetDate(y, m, d);
    }
    RegisteredPosition InputedArticle = NULL;   
    InputedArticle = RegisteredHead.next;
    while(art > 1){

        InputedArticle = InputedArticle->next;
        art--;
    }
    ReceiptPosition temp = NULL;
    temp = head->next;
    int bought = 0;
    float cost = 0.0;
    while(temp && (CompareDates(temp->date, d1) != DATE_EARLIER) && (CompareDates(temp->date, d2) != DATE_LATER)){
        ArticlePosition tempArt = NULL;
        tempArt = &temp->ArticleHead;
        while(tempArt->next && strcmp(tempArt->name, InputedArticle->name) != 0){
            tempArt = tempArt->next;
        }
        bought += (strcmp(tempArt->name, InputedArticle->name) == 0)*(tempArt->quantity);
        cost += (strcmp(tempArt->name, InputedArticle->name) == 0)*(tempArt->price)*bought;
        temp = temp->next;
    }
    printf("Artikl %s je u vremenskom razdoblju od ", InputedArticle->name);
    PrintDateTime(d1);
    printf(" do ");
    PrintDateTime(d2);
    printf(" kupljen %d puta i to je kostalo %.3f\n", bought, cost);

    return EXIT_SUCCESS;
}

int PrintRegistered(RegisteredPosition first){
    RegisteredPosition temp = NULL;
    temp = first;
    int i = 0;
    while(temp){
        printf("%d.%s\n", i+1, temp->name);
        temp = temp->next;
        i++;
    }
    return i;
}

RegisteredPosition CreateRegistered(char* name){
    RegisteredPosition new = NULL;
    new = (RegisteredPosition)malloc(sizeof(Registered));
    if(new == NULL){
        printf("Nije bilo mjesta za biljezenje postojeceg artikla.\n");
        return NULL;
    }
    strcpy(new->name, name);
    new->next = NULL;
    return new;
}


