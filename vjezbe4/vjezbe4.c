#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_SIZE 200
#define ERROR -5

struct poly;
typedef struct poly* position;

typedef struct poly{
    int coeff;
    int exp;
    position next;
}Poly;

position CreateElement(int k, int e);

int SortedInput(position head, position NewElement);

int InsertAfter(position p, position new);

int DeleteAfter(position p);

int ReadFromFile(FILE* fp, position head);

int SumPoly(position head1, position head2, position headSum);

int MultiplyPoly(position head1, position head2, position headMult);

int PrintPoly(position first);

int InputPoly(position head);

int main(){
    Poly head1 = {
        .coeff = 0,
        .exp = 0,
        .next = NULL
    };
    Poly head2 = {
        .coeff = 0,
        .exp = 0,
        .next = NULL
    };
    Poly head3 = {
        .coeff = 0,
        .exp = 0,
        .next = NULL
    };
    Poly headSum = {
        .coeff = 0,
        .exp = 0,
        .next = NULL
    };
    Poly headMult = {
        .coeff = 0,
        .exp = 0,
        .next = NULL
    };
    

    FILE* file = NULL;
    file = fopen("polinom.txt", "r");
    if(file == NULL){
        printf("nije bilo moguce otvorit file\n");
        return ERROR;
    }
    ReadFromFile(file, &head2);
    PrintPoly(head2.next);
    fclose(file);

    file = NULL;
    file = fopen("polinom2.txt", "r");
    if(file == NULL){
        printf("nije bilo moguce otvorit file\n");
        return ERROR;
    }
    ReadFromFile(file, &head1);
    PrintPoly(head1.next);
    fclose(file);

    SumPoly(&head1, &head2, &headSum);
    PrintPoly(headSum.next);

    MultiplyPoly(&head1, &head2, &headMult);
    PrintPoly(headMult.next);

    return EXIT_SUCCESS;
}

position CreateElement(int k, int e){
    position NewElement = NULL;
    NewElement = (position)malloc(sizeof(Poly));
    if(NewElement == NULL){
        printf("nema dovoljno mjesta u memoriji\n");
        return NULL;
    }
    NewElement->coeff = k;
    NewElement->exp = e;
    NewElement->next = NULL;
    return NewElement;
}

int SortedInput(position head, position NewElement) {
    position temp = head;
    while (temp->next && (temp->next->exp < NewElement->exp)) {
        temp = temp->next;
    }
    if (temp->next && (temp->next->exp == NewElement->exp)) {
        temp->next->coeff += NewElement->coeff;
        if (temp->next->coeff == 0) {
            DeleteAfter(temp);
        }
        free(NewElement);
    } else {
        InsertAfter(temp, NewElement);
    }
    return EXIT_SUCCESS;
}

int InsertAfter(position p, position new){
    new->next = p->next;
    p->next = new;
    return EXIT_SUCCESS;
}

int DeleteAfter(position p){
    position p_ = p->next;
    p->next = p_->next;
    free(p_);
    return EXIT_SUCCESS;
}

int ReadFromFile(FILE* file, position head){
    if(file == NULL){
        printf("nije bilo moguce otvorit file\n");
        return ERROR;
    }
    int c, e;
    int bytes;
    char line[MAX_SIZE];
    while(fgets(line, sizeof(line), file)){
        char* ptr = line;
        while(sscanf(ptr, " %d %d %n", &c, &e, &bytes) == 2){
            SortedInput(head, CreateElement(c, e));
            ptr += bytes;
        }
    }
    return EXIT_SUCCESS;
}

int SumPoly(position head1, position head2, position headSum){
    position i = NULL, j = NULL;
    i = head1;
    j = head2;
    if(i == NULL){
        printf("polinom1 nepostoji\n");
        return EXIT_SUCCESS;
    }
    if(j == NULL){
        printf("polinom2 nepostoji\n");
        return EXIT_SUCCESS;
    }
    while(i->next){
        while(j->next){
            int k = 1;
            if(i->exp == j->exp){
                k = i->coeff + j->coeff;
                if(k == 0){
                    DeleteAfter(i);
                }
                else{
                    SortedInput(headSum, i);
                }
                DeleteAfter(j);
            }
            j = j->next;
        }
        i = i->next;
    }
    return EXIT_SUCCESS;
}

int MultiplyPoly(position head1, position head2, position headMul){
    position i = NULL, j = NULL;
    i = head1->next;
    j = head2->next;
    if(i == NULL){
        printf("polinom1 nepostoji\n");
        return EXIT_SUCCESS;
    }
    if(j == NULL){
        printf("polinom2 nepostoji\n");
        return EXIT_SUCCESS;
    }
    while(i){
        while(j){
            int k = 1, e = 0;
            k = i->coeff * j->coeff;
            e = i->exp + j->exp;
            position temp = CreateElement(k, e);
            SortedInput(headMul, temp);
            j = j->next;
        }
        j = head2->next;
        i = i->next;
    }
    return EXIT_SUCCESS;
}

int PrintPoly(position first){
    position temp = NULL;
    temp = first;
    if(first == NULL){
        printf("polinom nepostoji\n");
        return ERROR;
    }
    printf("Polinom: ");
    while(temp){
        printf("%dx^%d + ", temp->coeff, temp->exp);
        temp = temp->next;
    }
    printf("\n");
    return EXIT_SUCCESS;
}

int InputPoly(position head) {
    int coeff, exp;
    char choice;
    do {
        printf("Enter coefficient: ");
        scanf("%d", &coeff);
        printf("Enter exponent: ");
        scanf("%d", &exp);
        position newElement = CreateElement(coeff, exp);
        SortedInput(head, newElement);
        printf("Do you want to add another term? (y/n): ");
        scanf(" %c", &choice);
    } while (choice == 'y' || choice == 'Y');
    return EXIT_SUCCESS;
}


