#define _CRT_SECURE_NO_WARNINGS

#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#define MAX_SIZE 50
#define ERROR -1

struct person;
typedef struct person* position;

typedef struct person{
    char name[MAX_SIZE];
    char lastName[MAX_SIZE];
    int birthYear;
    position next;
}Person;

position CreatePerson(char* name, char* lastName, int birthYear);

int PrependList(position head, char* name, char* lastName, int birthYear);

int PrintList(position first);

int AppendList(position head, char* name, char* lastName, int birthYear);

position FindLast(position head);

int InsertAfter(position previous, position np);

int Delete(position head, char* name);

position findPrevious(position head, char* name);

position FindBySurname(position first, char *surname);

int AddAfter(position head, char* surname, char* newName, char* newSurname, int newBY);

int AddBefore(position head, char* surname, char* newName, char* newSurname, int newBY);

int SortBySurname(position first);

int CopyToFile(position first);

int ReadFromFile(FILE* file);

int main(){
    Person head = {
        .name = {0},
        .lastName = {0},
        .birthYear = 0,
        .next = NULL
    };

    PrependList(&head, "Ivana", "Ivic", 1983);
    PrependList(&head, "Mate", "Matic", 2001);
    PrependList(&head, "Jure", "Juric", 1995);
    PrependList(&head, "Marta", "Martic", 1987);
    PrependList(&head, "Josko", "Jolic", 2002);
    printf("initial list\n");
    PrintList(head.next);

    //dio pod a)
    AddAfter(&head, "Martic", "Ante", "Antic", 1985);
    printf("\ndio pod a) je izvrsen\n");
    PrintList(head.next);

    //dio pod b)
    AddBefore(&head, "Martic", "Lucija", "Lucijevic", 1997);
    printf("\ndio pod b) je izvrsen\n");
    PrintList(head.next);
    
    //dio pod c)
    SortBySurname(&head);
    printf("\ndio pod c) je izvrsen\n");
    PrintList(head.next);

    //dio pod d)
    CopyToFile(head.next);
    printf("\ndio pod d) je izvrsen\n");

    //dio pod e)
    FILE* file = NULL;
    file = fopen("InputListe.txt", "r");
    if(file == NULL){
        printf("Nije bilo moguce otvorit file");
        return ERROR;
    }
    ReadFromFile(file);
    printf("\ndio pod e) je izvrsen\n");
    fclose(file);
    return 0;
}





position CreatePerson(char* name, char* lastName, int birthYear){
    position newPerson = NULL;
    newPerson = (position)malloc(sizeof(Person));
    if(newPerson == NULL){
        printf("Nema mista u memoriji\n");
        return NULL;
    }
    strcpy(newPerson->name, name);
    strcpy(newPerson->lastName, lastName);
    newPerson->birthYear = birthYear;
    newPerson->next = NULL;
    return newPerson;
}

int PrependList(position head, char* name, char* lastName, int birthYear){
    position newPerson = NULL;
    newPerson = CreatePerson(name, lastName, birthYear);
    if(newPerson == NULL){
        printf("Nije bilo moguce kreirat novu osobu");
        return ERROR;
    }
    InsertAfter(head, newPerson);
    return EXIT_SUCCESS;
}

int PrintList(position first){
    position temp = NULL;
    temp = first;
    printf("\n");
    while(temp != NULL){
        printf("%s %s %d\n", temp->name, temp->lastName, temp->birthYear);
        temp = temp->next;
    }
    return EXIT_SUCCESS;
}

int AppendList(position head, char* name, char* lastName, int birthYear){
    position newPerson = NULL;
    newPerson = CreatePerson(name, lastName, birthYear);
    if(newPerson == NULL){
        printf("Nije bilo moguce dodat novu osobu");
        return ERROR;
    }
    position last = NULL;
    last = FindLast(head);
    InsertAfter(last, newPerson);
    return EXIT_SUCCESS;
}

position FindLast(position head){
    position last = NULL;
    last = head;
    while(last->next != NULL){
        last = last->next;
    }
    return last;
}

int InsertAfter(position previous, position np){
    np->next = previous->next;
    previous->next = np;
    return EXIT_SUCCESS;
}

position FindBySurname(position first, char *surname){
    position temp = NULL;
    temp = first;
    while(temp){
        if(strcmp(surname, temp->lastName) == 0) return temp;
        temp = temp->next;
    }
    return NULL;
}

position findPrevious(position head, char* surname){
    position current = NULL;
    current = head;
    while(current->next != NULL){
        if(strcmp((current->next)->lastName, surname) == 0) return current;
        current = current->next;
    }
    return NULL;
}

int Delete(position head, char* surname){
    position previous = findPrevious(head, surname);
    position current = previous->next;
    previous->next = current->next;
    free(current);
    return EXIT_SUCCESS;
}

int AddAfter(position head, char* surname, char* newName, char* newSurname, int newBY){
    position newPerson = CreatePerson(newName, newSurname, newBY);
    position temp = FindBySurname(head, surname);
    InsertAfter(temp, newPerson);
    return EXIT_SUCCESS;
}

int AddBefore(position head, char* surname, char* newName, char* newSurname, int newBY){
    position NewPerson = CreatePerson(newName, newSurname, newBY);
    position previous = findPrevious(head, surname);
    InsertAfter(previous, NewPerson);
    return EXIT_SUCCESS;
}

int SortBySurname(position head){
    position temp, i, iPrev, end;
    end = NULL;

    while(head->next != end){
        iPrev = head;
        i = head->next;
        while(i->next != end){
            if(strcmp(i->lastName, (i->next)->lastName) == 1){
                temp = i->next;
                iPrev->next = temp;
                i->next = temp->next;
                temp->next = i;
                i = temp;
            }
            iPrev = i;
            i = i->next;
        }
        end = i;
    }
    return EXIT_SUCCESS;
}

int CopyToFile(position first){
    if(first == NULL){
        printf("Lista je prazna, nema nista za upisat");
        return EXIT_FAILURE;
    }
    FILE* file = NULL;
    file = fopen("InputListe.txt", "w");
    if(file == NULL){
        printf("Nije bilo moguce otvorit file");
        return ERROR;
    }
    position temp = NULL;
    temp = first;
    while(temp != NULL){
        fprintf(file, "%s %s %d\n", temp->name, temp->lastName, temp->birthYear);
        temp = temp->next;
    }
    fclose(file);
    return EXIT_SUCCESS;
}

int ReadFromFile(FILE* file){

    Person newHead = {
        .name = {0},
        .lastName = {0},
        .birthYear = 0,
        .next = NULL
    };

    char* buff, currName, currSurname;
    int currBy;

    while(!feof(file)){
        fscanf(file, "%s", buff);
        sscanf(buff, "%s %s %d", currName, currSurname, currBy);
        AppendList(&newHead, currName, currSurname, currBy);
    }

    return EXIT_SUCCESS;
}

