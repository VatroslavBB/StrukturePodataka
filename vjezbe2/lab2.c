#define _CRT_SECURE_NO_WARNINGS

#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#define MAX_SIZE 50
#define ERROR -1

struct _struct;
typedef struct _struct* position;

typedef struct _struct{
    char name[MAX_SIZE];
    char lastName[MAX_SIZE];
    int birthYear;
    position next;
}Person;

position CreatePerson(char* name, char* lastName, char birthYear);

int PrependList(position head, char* name, char* lastName, int birthYear);

int PrintList(position first);

int AppendList(position head, char* name, char* lastName, int birthYear);

position FindLast(position head);

int InsertAfter(position previous, position np);

int Delete(position head, char* name);

position findPrevious(position head, char* name);

position FindBySurname(position first, char *surname);





int main(){
    Person head = {
        .name = {0},
        .lastName = {0},
        .birthYear = 0,
        .next = NULL
    };

    //dio pod a)
    PrependList(&head, "Ivan", "Ivic", 1983);

    //dio pod b)
    PrintList(head.next);

    //dio pod c)
    AppendList(&head, "Ana", "Anic", 2001);
    PrintList(head.next);

    //dio pod d)
    FindBySurname(head.next, "Anic");

    //dio pod e)
    Delete(&head, "Ivan");
    PrintList(head.next);

    return 0;
}





position CreatePerson(char* name, char* lastName, char birthYear){
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

position findPrevious(position head, char* name){
    position current = NULL;
    current = head;
    while(current->next != NULL){
        if(strcmp((current->next)->name, name) == 0) return current;
    }
    return NULL;
}

int Delete(position head, char* name){
    position previous = findPrevious(head, name);
    position current = previous->next;
    previous->next = current->next;
    free(current);
    return EXIT_SUCCESS;
}
