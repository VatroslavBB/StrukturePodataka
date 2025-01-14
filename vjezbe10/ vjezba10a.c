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

#define MAX_SIZE 50

typedef struct city* Tposition;
typedef struct city{
    char name[MAX_SIZE];
    int population;
    Tposition left;
    Tposition right;
}City;

Tposition CreateCity(char*, int);
int LinkCity(Tposition, Tposition);
int DeleteCities(Tposition);
int InorderLinkCities(Tposition, Tposition);

typedef struct country* Lposition;
typedef struct country{
    char name[MAX_SIZE];
    Tposition citiesRoot;
    Lposition next;
}Country;

Lposition CreateCountry(char*);
int LinkCountry(Lposition, Lposition);
int DeleteCountries(Lposition);

int ReadFromFile(FILE*, Lposition);

int main(){
    Country head = {
        .name = .0,
        .next = NULL,
        .citiesRoot = NULL
    };

    FILE* countriesFile = NULL;
    countriesFile = fopen("drzave.txt", "r");
    ReadFromFile(countriesFile, &head);


    fclose(countriesFile);

    return 0;
}

Tposition CreateCity(char* name, int pop){
    Tposition new = NULL;
    new = (Tposition)malloc(sizeof(City));
    if(new == NULL){
        printf(("Nema mjesta u memoriji za novi grad\n"));
        return NULL;
    }
    strcpy(new->name, name);
    new->population = pop;
    new->left = NULL;
    new->right = NULL;
    return new;
}

Lposition CreateCountry(char* name){
    Lposition new = NULL;
    new = (Lposition)malloc(sizeof(Country));
    if(new == NULL){
        printf(("Nema mjesta u memoriji za novu drzavu\n"));
        return NULL;
    }
    strcpy(new->name, name);
    City cRoot = {
        .name = .0,
        .population = 0,
        .left = NULL,
        .right = NULL
    };
    new->citiesRoot = &cRoot;
    new->next = NULL;
    return new;
}

int LinkCity(Tposition root, Tposition newCity){
    if(root == NULL){
        root = newCity;
        return EXIT_SUCCESS;
    }
    if(newCity->population > root->population){
        if(root->right == NULL){
            root->right = newCity;
            return EXIT_SUCCESS;
        }
        return LinkCity(root->right, newCity);
    }
    else if(newCity->population < root->population){
        if(root->left == NULL){
            root->left = newCity;
            return EXIT_SUCCESS;
        }
        return LinkCity(root->left, newCity);
    }
    else{
        if(strcmp(newCity->name, root->name) > 0){
            if(root->right == NULL){
                root->right = newCity;
                return EXIT_SUCCESS;
            }
            return LinkCity(root->right, newCity);
        }
        else if(strcmp(newCity->name, root->name) < 0){
            if(root->left == NULL){
                root->left = newCity;
                return EXIT_SUCCESS;
            }
            return LinkCity(root->left, newCity);
        }
        else{
            free(newCity);
            return EXIT_SUCCESS;
        }
    }
}

int LinkCountry(Lposition head, Lposition newCountry){
    Lposition temp = head;
    while(temp->next && (strcmp(newCountry->name, temp->next->name) < 0)){
        temp = temp->next;
    }
    if(temp->next == NULL){
        temp->next = newCountry;
        return EXIT_SUCCESS;
    }
    if(strcmp(newCountry->name, temp->name) == 0){
        InorderLinkCities(newCountry->citiesRoot, temp->citiesRoot);
        return EXIT_SUCCESS;
    }
    newCountry->next = temp->next;
    temp->next = newCountry;
    return EXIT_SUCCESS;
}

int InorderLinkCities(Tposition root, Tposition R){
    if(root == NULL){
        return EXIT_SUCCESS;
    }
    InorderLinkCities(root->left, R);
    LinkCity(R, root);
    InorderLinkCities(root->right, R);
    return EXIT_SUCCESS;
}

int DeleteCities(Tposition root){
    if(root == NULL){
        return EXIT_SUCCESS;
    }
    DeleteCities(root->left);
    DeleteCities(root->right);
    free(root);
    return EXIT_SUCCESS;
}

int DeleteCountries(Lposition head){
    Lposition temp = NULL;
    while(head->next){
        temp = head->next;
        head->next = temp->next;
        free(temp);
    }
    return EXIT_SUCCESS;
}

int ReadFromFile(FILE* file, Lposition head){
    if(file == NULL){
        printf("Nije bilo moguce uci u file");
        return EXIT_SUCCESS;
    }
    char countryName[MAX_SIZE];
    char cityFileName[MAX_SIZE];
    FILE* cityFile = NULL;
    char citiesLine[MAX_SIZE];
    char cityName[MAX_SIZE] = "";
    char s[MAX_SIZE];
    int bytes, population;
    while(fscanf(file, "%s %s", countryName, cityFileName) == 2){
        cityFile = fopen(cityFileName, "r");
        Lposition newCountry = CreateCountry(countryName);
        LinkCountry(head, newCountry);
        while(fgets(citiesLine, sizeof(citiesLine), cityFile)){
            char* ptr = citiesLine;
            bytes = 0;
            population = 0;
            memset(cityName, '\0', sizeof(cityName));
            while(sscanf(ptr, "%s %n", s, &bytes) == 1){
                strcat(cityName, s);
                ptr += bytes;
            }
            sscanf(ptr, ", %d", &population);
            Tposition newCity = CreateCity(cityName, population);
            if(newCountry->citiesRoot == NULL){
                newCountry->citiesRoot = newCity;
            }
            else{
                LinkCity(newCountry->citiesRoot, newCity);
            }
        }
        fclose(cityFile);
    }
    return EXIT_SUCCESS;
}







