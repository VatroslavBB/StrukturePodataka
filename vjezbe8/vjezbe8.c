/* 
8. Napisati program koji omogućava rad s binarnim stablom pretraživanja. Treba
omogućiti:
unošenje novog elementa .
inorder .
preorder .
postorder .
level order x
brisanje .
pronalaženje nekog elementa . 
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

typedef struct node* position;
typedef struct node{
    int value;
    position left;
    position right;
} Node;

typedef struct qnode* Qposition;
typedef struct qnode{
    position treeNode;
    Qposition next;
}Qnode;

Qposition CreateQnode(position tNode);
position Pop(Qposition head);
position Push(Qposition head, position tNode);
int Qdelete(Qposition head);

position CreateNode(int val);
int LinkToTree(position root, position new);
int InOrder(position root);
int PostOrder(position root);
int PreOrder(position root);
int LevelOrder(position root);
int DeleteNode(position root, int val);
int DeleteTree(position root);
position FindNode(position root, int val);
position FindPrev(position root, position current);
position FindMin(position root);

int main(){

    int arr[] = {4, 2, 27, 1, 3, 5, 41, -7, 11, -10, -3, 17, 27, 5};

    position root = CreateNode(arr[0]);

    for(int i = 1; i < sizeof(arr)/sizeof(int); i++){
        LinkToTree(root, CreateNode(arr[i]));
    }

    InOrder(root);
    printf("\n");
    PostOrder(root);
    printf("\n");
    PreOrder(root);
    printf("\n");
    LevelOrder(root);
    printf("\n");

    /* position found = NULL;
    for(int i = -15; i < 45; i++){
        found = FindPrev(root, FindNode(root, i));
        if(found){
            printf("Prethodnik od %d je %d\n", i, found->value);
        }
        else{
            printf("Vrijednost %d NIJE upisana u stablo\n", i);
        }
    } */

    /* printf("%p\n", FindNode(root, -7));
    DeleteNode(root, -7);
    printf("%p\n", FindNode(root, -7)); */


    //DeleteTree(root);

    return EXIT_SUCCESS;

}

Qposition CreateQnode(position tNode){
    Qposition new = NULL;
    new = (Qposition)malloc(sizeof(Qnode));
    if(new == NULL){
        printf("Nema mjesta u memoriji za novi element na redu\n");
        return NULL;
    }
    new->treeNode = tNode;
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
    position tnode = poped->treeNode;
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

position CreateNode(int val){
    position new = NULL;
    new = (position)malloc(sizeof(Node));
    if(new == NULL){
        printf("Nema mjesta u memoriji za novi cvor na stablu\n");
        return NULL;
    }
    new->value = val;
    new->left = NULL;
    new->right = NULL;
    return new;
}

int LinkToTree(position root, position new){
    if(root == NULL){
        root = new;
        return EXIT_SUCCESS;
    }
    else if(new->value > root->value){
        if(root->right == NULL){
            root->right = new;
            return EXIT_SUCCESS;
        }
        else{
            LinkToTree(root->right, new);
        }
    }
    else if(new->value < root->value){
        if(root->left == NULL){
            root->left = new;
            return EXIT_SUCCESS;
        }
        else{
            LinkToTree(root->left, new);
        }
    }
    else if(new->value == root->value){
        //printf("Vrijednost je vec upisana u stablo\n");
        free(new);
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

int InOrder(position root){
    if(root == NULL){
        return EXIT_SUCCESS;
    }
    InOrder(root->left);
    printf("%d ", root->value);
    InOrder(root->right);
    return EXIT_SUCCESS;
}

int PostOrder(position root){
    if(root == NULL){
        return EXIT_SUCCESS;
    }
    PostOrder(root->left);
    PostOrder(root->right);
    printf("%d ", root->value);
    return EXIT_SUCCESS;
}

int PreOrder(position root){
    if(root == NULL){
        return EXIT_SUCCESS;
    }
    printf("%d ", root->value);
    PreOrder(root->left);
    PreOrder(root->right);
    return EXIT_SUCCESS;
}

int LevelOrder(position root){
    if(root == NULL){
        return EXIT_SUCCESS;
    }
    Qnode qHead = {
        .next = NULL,
        .treeNode = NULL
    };
    Push(&qHead, root);
    while(qHead.next != NULL){
        position current = Pop(&qHead);
        printf("%d ", current->value);
        if(current->left != NULL){
            Push(&qHead, current->left);
        }
        if(current->right != NULL){
            Push(&qHead, current->right);
        }
    }
    return EXIT_SUCCESS;
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

position FindNode(position root, int val){
    if(root == NULL){
        return NULL;
    }
    if(val > root->value) {
        return FindNode(root->right, val);
    }
    else if(val < root->value) {
        return FindNode(root->left, val);
    }
    else{
        return root;
    }
}

position FindPrev(position root, position current){
    if(root == NULL || current == NULL){
        return NULL;
    }
    if(root->left == current || root->right == current){
        return root;
    }
    else if(current->value > root->value){
        return FindPrev(root->right, current);
    }
    else if(current->value < root->value){
        return FindPrev(root->left, current);
    }
}


int DeleteNode(position root, int val){
    position toDelete = NULL;
    toDelete = FindNode(root, val);
    position prev = NULL;
    if(toDelete == NULL){
        return EXIT_SUCCESS;
    }
    prev = FindPrev(root, toDelete);
    int left = (prev->left == toDelete)*1;

    if(toDelete->left == NULL){
        if(toDelete->right != NULL){
            if(left){
                prev->left = toDelete->right;
            }
            else{
                prev->right = toDelete->right;
            }
        }
        free(toDelete);
        return EXIT_SUCCESS;
    }
    if(toDelete->right == NULL){
        if(toDelete->left != NULL){
            if(left){
                prev->left = toDelete->left;
            }
            else{
                prev->right = toDelete->left;
            }
        }
        free(toDelete);
        return EXIT_SUCCESS;
    }
    position min = FindMin(toDelete->right);
    DeleteNode(root, min->value);
    toDelete->value = min->value;
}

position FindMin(position root){
    position temp = NULL;
    temp = root;
    while(temp->left){
        temp = temp->left;
    }
    return temp;
}
