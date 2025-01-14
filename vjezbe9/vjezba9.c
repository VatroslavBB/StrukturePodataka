#define CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define MIN -2147483647 - 1

typedef struct node* position;
typedef struct node{
    int number;
    position left;
    position right;
}Node;

typedef struct qnode* Qposition;
typedef struct qnode{
    position tnode;
    Qposition next;
}Qnode;

Qposition CreateQnode(position tNode);
position Pop(Qposition head);
position Push(Qposition head, position tNode);
int Qdelete(Qposition head);

position CreateNode(int);
position insertA(position, int*, int);
position insertB(position, int);
int replace(position);
int LinkNode(position, position);
int DeleteTree(position);
int inorderWrite(position, FILE*);
int sum(position);

int main(){
    int arr[] = {2, 5, 7, 8, 11, 1, 4, 2, 3, 7};
    int len = sizeof(arr)/sizeof(int);

    position root = NULL;

    //pod a)
    //root = insertA(root, arr, len);

    //pod b)
    root = insertB(root, 30);

    FILE* file = NULL;
    file = fopen("zad9.txt", "w");
    if(file == NULL){
        printf("nije se moga otvorit file\n");
        return EXIT_SUCCESS;
    }
    inorderWrite(root, file);
    fprintf(file, "\n");

    replace(root);
    inorderWrite(root, file);

    DeleteTree(root);
    fclose(file);
    return 0;
}

position CreateNode(int num){
    position new = NULL;
    new = (position)malloc(sizeof(Node));
    if(new == NULL){
        printf("Nema mjesta u memoriji\n");
        return NULL;
    }
    new->number = num;
    new->right = NULL;
    new->left = NULL;
    return new;
}

int LinkNode(position root, position new){
    if(new->number >= root->number){
        if(root->left){
            return LinkNode(root->left, new);
        }
        else{
            root->left = new;
            return EXIT_SUCCESS;
        }
    }
    else{
        if(root->right){
            return LinkNode(root->right, new);
        }
        else{
            root->right = new;
            return EXIT_SUCCESS;
        }
    }
}

position insertA(position root, int arr[], int len){
    root = CreateNode(arr[0]);
    for(int i = 1; i < len; i++){
        LinkNode(root, CreateNode(arr[i]));
    }
    return root;
}

position insertB(position root, int n){
    root = CreateNode(rand()%79 + 11);
    for(int i = 1; i < n; i++){
        LinkNode(root, CreateNode(rand()%79 + 11));
    }
    return root;
}

int replace(position root){
    if(root == NULL){
        return EXIT_SUCCESS;
    }
    Qnode head = {
        .next = NULL,
        .tnode = NULL
    };
    Push(&head, root);
    position current = NULL;
    while(head.next){
        current = Pop(&head);
        current->number = sum(current);
        if(current->left){
            Push(&head, current->left);
        }
        if(current->right){
            Push(&head, current->right);
        }
    }
    return EXIT_SUCCESS;
}

int sum(position root){
    int sol = 0;
    if(root == NULL){
        return sol;
    }
    Qnode head = {
        .tnode = NULL,
        .next = NULL
    };
    Push(&head, root);
    position current = NULL;
    while(head.next){
        current = Pop(&head);
        sol += current->number;
        if(current->left){
            Push(&head, current->left);
        }
        if(current->right){
            Push(&head, current->right);
        }
    }
    return sol - root->number;
}

Qposition CreateQnode(position tNode){
    Qposition new = NULL;
    new = (Qposition)malloc(sizeof(Qnode));
    if(new == NULL){
        printf("Nema mjesta u memoriji za novi element na redu\n");
        return NULL;
    }
    new->tnode = tNode;
    new->next = NULL;
    return new;
}

position Pop(Qposition head){
    if(head == NULL){
        return NULL;
    }
    else if(head->next == NULL){
        return NULL;
    }
    Qposition poped = NULL;
    poped = head->next;
    position tnode = poped->tnode;
    head->next = poped->next;
    free(poped);
    return tnode;
}

position Push(Qposition head, position tNode){
    Qposition temp = NULL;
    temp = head;
    while(temp->next){
        temp = temp->next;
    }
    Qposition new = CreateQnode(tNode);
    temp->next = new;
    new->next = NULL;
    return tNode;
}

int Qdelete(Qposition head){
    while(head){
        Qposition temp = head;
        head = head->next;
        free(temp);
    }
    return 0;
}

int DeleteTree(position root){
    if(root == NULL){
        return EXIT_SUCCESS;
    }
    DeleteTree(root->left);
    DeleteTree(root->right);
    free(root);
    return EXIT_SUCCESS;
}

int inorderWrite(position root, FILE* file){
    if(root == NULL){
        return EXIT_SUCCESS;
    }
    inorderWrite(root->left, file);
    fprintf(file, "%d ", root->number);
    inorderWrite(root->right, file);
    return EXIT_SUCCESS;
}
