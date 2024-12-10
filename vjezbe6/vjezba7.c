/* 7. Napisati program koji pomoću vezanih listi (stabala) predstavlja strukturu direktorija. 
Omogućiti unos novih direktorija i pod-direktorija, ispis sadržaja direktorija i
povratak u prethodni direktorij. Točnije program treba preko menija simulirati 
korištenje DOS naredbi: 1- "md", 2 - "cd dir", 3 - "cd..", 4 - "dir" i 5 – izlaz. */

#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_SIZE 50
#define COMMON_CHILD 0
#define UNIQUE_CHILD 1

typedef struct directory* DirectoryPosition;
typedef struct directory{
    char name[MAX_SIZE];
    DirectoryPosition child;
    DirectoryPosition parent;
    DirectoryPosition next;
}Directory;

Directory DirRoot = {
        .name = "C:",
        .child = NULL,
        .parent = NULL,
        .next = NULL
};
DirectoryPosition current = &DirRoot;

DirectoryPosition CreateDir(char* name);
int LinkDir(DirectoryPosition newDir);
int ReturnToPrev();
int PrintDir(DirectoryPosition dir, int depth);
int PrintDirEl(char* name);
int ChangeDir(char* name);
int solution();

int main(){
    
    solution();

    printf("\ngotovo.\n:)\n");
    return 0;
}

DirectoryPosition CreateDir(char* name){
    DirectoryPosition newDir = NULL;
    newDir = (DirectoryPosition)malloc(sizeof(Directory));
    if(newDir == NULL){
        printf("Nije bilo mjesta za stvaranje novog direktorija.\n");
        return NULL;
    }
    strcpy(newDir->name, name);
    newDir->next = NULL;
    newDir->parent = NULL;
    newDir->child = NULL;
    return newDir;
}

int LinkDir(DirectoryPosition newDir){
    DirectoryPosition currentChild = NULL;
    currentChild = current->child;
    if(currentChild == NULL){
        newDir->parent = current;
        current->child = newDir;
        return UNIQUE_CHILD;
    }
    while(currentChild->next){
        currentChild = currentChild->next;
    }
    newDir->parent = current;
    currentChild->next = newDir;
    return COMMON_CHILD;
}

int ReturnToPrev(){
    if(current->parent == NULL){
        printf("Vracanje na prethodni direktorij nije moguce.\n");
        return EXIT_FAILURE;
    }
    current = current->parent;
    return EXIT_SUCCESS;
}

int PrintDir(DirectoryPosition dir, int depth){
    DirectoryPosition temp = NULL;
    temp = dir->child;
    while(temp){
        for(int i = 0; i < depth; i++){
            printf("\t");
        }
        PrintDirEl(temp->name);
        PrintDir(temp, depth + 1);
        temp = temp->next;
    }
    return EXIT_SUCCESS;
}

int PrintDirEl(char* name){
    printf("|--> %s\n", name);
    return EXIT_SUCCESS;
}

int ChangeDir(char* name){
    DirectoryPosition temp = NULL;
    if(current->child == NULL){
        return EXIT_SUCCESS;
    }
    else{
        temp = current->child;
    }
    while(temp){
        if(strcmp(temp->name, name) == 0){
            current = temp;
            PrintDirEl(current->name);
            return EXIT_SUCCESS;
        }
        temp = temp->next;
    }
    printf("Taj direktorij nepostoji.\n");
    return EXIT_FAILURE;
}

int solution(){
    int quit = 0;
    int wrongChoice = 0;
    char choice[MAX_SIZE], name[MAX_SIZE];
    memset(choice, -1, sizeof(choice));
    memset(name, -1, sizeof(name));
    while(quit == 0){
        if(wrongChoice == 1){
            wrongChoice = 0;
            printf("Ta naredba nepostoji, pokušajte ponovo.\n");
        }
        printf("Odaberite jednu od sljedecih naredbi:\n");
        printf("1 - md\n");
        printf("2 - cd\n");
        printf("3 - cd..\n");
        printf("4 - dir\n");
        printf("5 - quit\n");
        scanf("%s", choice);
        if(strcmp(choice, "md") == 0){
            printf("Unesite ime novog direktorija: ");
            scanf("%s", name);
            LinkDir(CreateDir(name));
        }
        else if(strcmp(choice, "cd") == 0){
            printf("Unesite ime direktorija kojeg zelite mijenjati: ");
            scanf("%s", name);
            ChangeDir(name);
        }
        else if(strcmp(choice, "cd..") == 0){
            ReturnToPrev();
        }
        else if(strcmp(choice, "dir") == 0){
            PrintDirEl(current->name);
            PrintDir(current, 1);
        }
        else if(strcmp(choice, "quit") == 0){
            quit = 1;
        }
        else{
            wrongChoice = 1;
        }
    }
    return EXIT_SUCCESS;
}

