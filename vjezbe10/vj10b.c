/* Napisati program koji čita datoteku drzave.txt u kojoj su zapisani nazivi pojedinih država. 
Uz ime države u datoteci se nalazi i ime dodatne datoteke u kojoj se nalaze gradovi pojedine države. 
Svaka datoteka koja predstavlja državu sadrži popis gradova u formatu naziv_grada, broj_stanovnika.

a) Potrebno je formirati sortiranu vezanu listu država po nazivu. Svaki čvor vezane liste sadrži stablo 
gradova sortirano po broju stanovnika, zatim po nazivu grada.

b) Potrebno je formirati stablo država sortirano po nazivu. Svaki čvor stabla sadrži vezanu listu gradova 
sortiranu po broju stanovnika, zatim po nazivu grada.

Nakon formiranja podataka potrebno je ispisati države i gradove te omogućiti korisniku putem tastature 
pretragu gradova određene države koji imaju broj stanovnika veći od unosa na tastaturi.
 */

#define CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_SIZE 100

typedef struct city* cityPosition;
typedef struct city{
    char name[MAX_SIZE];
    int population;
    cityPosition next;
}City;

typedef struct country* countryPosition;
typedef struct country{
    char name[MAX_SIZE];
    City cityHead;
    countryPosition left;
    countryPosition right;
}Country;

countryPosition CreateCountry(char*);
cityPosition CreateCity(char*, int);
int LinkCity(cityPosition head, cityPosition new);
int LinkCountry(countryPosition root, countryPosition new);
int InorderCountry(countryPosition root);
int PrintCities(cityPosition head);

int ReadFromFile(FILE*, countryPosition);


int main(){
    FILE* file = NULL;
    file = fopen("drzave.txt", "r");

    char firstCountry[MAX_SIZE];
    char firstCityFileName[MAX_SIZE];
    fscanf(file, "%s %s", firstCountry, firstCityFileName);
    countryPosition root = CreateCountry(firstCountry);
    int population = 0;
    char cityName[MAX_SIZE];
    FILE* cityFile = fopen(firstCityFileName, "r");
    cityPosition newCity = NULL;
    while(fscanf(cityFile, "%99[^,], %d\n", cityName, &population) == 2){
        newCity = CreateCity(cityName, population);
        LinkCity(&root->cityHead, newCity);
    }

    /* cityPosition temp = &root->cityHead;
    while(temp){
        printf("%s\n", temp->name);
        temp = temp->next;
    } */

    ReadFromFile(file, root);

    InorderCountry(root);

    fclose(file);
    return 0;
}

int PrintCities(cityPosition head){
    cityPosition temp = head->next;
    while(temp){
        printf("%s\n", temp->name);
        temp = temp->next;
    }
    return EXIT_SUCCESS;
}

int InorderCountry(countryPosition root){
    if(root == NULL){
        return EXIT_SUCCESS;
    }
    InorderCountry(root->left);
    printf("\nU drzavi %s nalaze se sljedeci gradovi:\n\n", root->name);
    PrintCities(&root->cityHead);
    InorderCountry(root->right);
    return EXIT_SUCCESS;
}

countryPosition CreateCountry(char* name){
    countryPosition new = NULL;
    new = (countryPosition)malloc(sizeof(Country));
    if(new == NULL){
        printf("Nema mjesta u memoriji za novu drzavu\n");
        return NULL;
    }
    strcpy(new->name, name);
    City cHead = {
        .name = {0},
        .population = 0,
        .next = NULL
    };
    new->cityHead = cHead;
    new->left = NULL;
    new->right = NULL;
    return new;
}

cityPosition CreateCity(char* name, int population){
    cityPosition new = NULL;
    new = (cityPosition)malloc(sizeof(City));
    if(new == NULL){
        printf("Nema mjesta u memoriji za novi grad\n");
        return NULL;
    }
    strcpy(new->name, name);
    new->population = population;
    new->next = NULL;
    return new;
}

int LinkCity(cityPosition head, cityPosition new){
    if(head->next == NULL){
        head->next = new;
        new->next = NULL;
        return 0;
    }
    cityPosition temp = head, prev;
    while(temp->next){
        if(new->population < temp->next->population){
            new->next = temp->next;
            temp->next = new;
            return 0;
        }
        if(new->population == temp->next->population){
            if(strcmp(new->name, temp->next->name) > 0){
                new->next = temp->next->next;
                temp->next->next = new;
                return 0;
            }
            else{
                new->next = temp->next;
                temp->next = new;
                return 0;
            }
        }
        prev = temp;
        temp = temp->next;
    }
    if(new->population == temp->population){
        if(strcmp(new->name, temp->name) > 0){
            new->next = temp->next;
            temp->next = new;
            return 0;
        }
        else{
            new->next = prev->next;
            prev->next = new;
            return 0;
        }
    }
    new->next = temp->next;
    temp->next = new;
    return EXIT_SUCCESS;
}

int LinkCountry(countryPosition root, countryPosition new){
    if(root == NULL){
        root = new;
        return EXIT_SUCCESS;
    }
    if(strcmp(root->name, new->name) > 0){
        if(root->left){
            return LinkCountry(root->left, new);
        }
        else{
            root->left = new;
            return EXIT_SUCCESS;
        }
    }
    else if(strcmp(root->name, new->name) < 0){
        if(root->right){
            return LinkCountry(root->right, new);
        }
        else{
            root->right = new;
            return EXIT_SUCCESS;
        }
    }
}

int ReadFromFile(FILE* countryFile, countryPosition root){
    if(countryFile == NULL){
        printf("Nije se mogla ocitati datoteka s drzavama\n");
        return EXIT_SUCCESS;
    }
    char countryName[MAX_SIZE];
    char cityFileName[MAX_SIZE];
    FILE* cityFile = NULL;
    int population = 0;
    char cityName[MAX_SIZE];
    cityPosition newCity = NULL;
    countryPosition newCountry = NULL;

    while(fscanf(countryFile, "%s %s", countryName, cityFileName) == 2){
        newCountry = CreateCountry(countryName);
        LinkCountry(root, newCountry);
        cityFile = NULL;
        cityFile = fopen(cityFileName, "r");
        while(fscanf(cityFile, "%99[^,], %d", cityName, &population) == 2){
            newCity = CreateCity(cityName, population);
            if(newCity){
                LinkCity(&newCountry->cityHead, newCity);
            }
        }
        fclose(cityFile);
    }
    return EXIT_SUCCESS;
}


