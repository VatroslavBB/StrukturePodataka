#include<stdio.h>
#include<stdlib.h>

#define EXIT_SUCCESS 0
#define ERROR -1
#define MAX_SIZE 100

typedef struct integerStack* nextInt;

typedef struct integerStack{
    int value;
    nextInt next;
}IntegerStack;

int TopInt(nextInt first);
int PopInt(nextInt first);
int PushInt(nextInt first, int value);
nextInt findlast(nextInt);
int CalculatePostfix(FILE* file, nextInt head);

int main(){
    IntegerStack head = {
        .value = 0,
        .next = NULL
    };

    FILE* file = NULL;
    file = fopen("postfix.txt", "r");
    if(file == NULL){
        printf("Nije bilo moguce otvoriti file");
        return ERROR;
    }
    int sol = CalculatePostfix(file, &head);
    printf("Rješenje je: %d", sol);

    fclose(file);
    return 0;
}

nextInt findlast(nextInt head){
    nextInt last = NULL;
    last = head;
    while(last->next) last = last->next;
    return last;
}

int TopInt(nextInt head){
    return findlast(head)->value;
}

int PopInt(nextInt head){
    nextInt last = findlast(head);
    nextInt prevLast = NULL;
    prevLast = head;
    while(prevLast->next != last) prevLast = prevLast->next;
    prevLast->next = NULL;
    free(last);
    return EXIT_SUCCESS;
}

int PushInt(nextInt head, int value){
    nextInt temp = NULL;
    temp = head;
    if(temp == NULL) return ERROR;
    while(temp->next) temp = temp->next;
    nextInt new = NULL;
    new = (nextInt)malloc(sizeof(IntegerStack));
    if(new == NULL) return ERROR;
    temp->next = new;
    new->next = NULL;
    new->value = value;
    return EXIT_SUCCESS;
}

int CalculatePostfix(FILE* file, nextInt head){
    int val = 0, x = 0, y = 0;
    char op;
    int bytes = 1, inputInt = 1, inputChar = 1;
    char line[MAX_SIZE];
    while(fgets(line, sizeof(line), file)){
        char* ptr = line;
        while((inputInt > 0) || (inputChar > 0)){
            inputInt = sscanf(ptr, " %d%n", &val, &bytes);
            inputChar = 1*(inputInt == 0);
            if(inputInt < 1){
                inputChar = sscanf(ptr, " %c%n", &op, &bytes);
            }
            ptr += bytes;
            if(inputInt > 0){
                PushInt(head, val);
            }
            else if(inputChar > 0){
                x = TopInt(head);
                PopInt(head);
                y = TopInt(head);
                PopInt(head);
                switch(op){
                    case '+':
                        val = x + y;
                        break;
                    case '-':
                        val = y - x;
                        break;
                    case '*':
                        val = x*y;
                        break;
                    default:
                        val = y/x;
                        break;
                }
                PushInt(head, val);
            }
        }
    }
    return head->next->value;
}




