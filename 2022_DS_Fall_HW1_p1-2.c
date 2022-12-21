#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct stack{
    int size ;
    int top;
    char *arr;
};

int stackTop(struct stack *sp){
    return sp->arr[sp->top];
}

int isEmpty(struct stack *ptr){
    if(ptr->top == -1){
        return 1;
    }
    else{
        return 0;
    }
}

int isFull(struct stack *ptr){
    if(ptr->top == ptr->size - 1){
        return 1;
    }
    else{
        return 0;
    }
}

void push(struct stack *ptr, char val){
    if(!isFull(ptr)){
        ptr->top ++;
        ptr->arr[ptr->top] = val;
    }
}

char pop(struct stack* ptr){
    if(isEmpty(ptr)){
        return -1;
    }
    else{
        char val = ptr->arr[ptr->top --];
        return val;
    }
}

int precedence(char ch){
    switch(ch){
        case '^':
        return 3;

        case '*':
        case '/':
        return 2;

        case '+':
        case '-':
        return 1;

    }
    return 0;
}

int isOperator(char ch){
    if(ch == '*' || ch == '/' || ch == '+' || ch == '-' || ch == '(' || ch == ')' || ch == '^'){
        return 1;
    }
    return 0;
}

char * postfixToprefix(char* postfix){
    struct stack * sp = (struct stack * ) malloc(sizeof(struct stack));
    sp->size = 100;
    sp->top = -1;
    sp->arr = (char *) malloc(sp->size * sizeof(char));
    char * prefix = (char *) malloc((strlen(postfix) + 1) * sizeof(char));
    char * temp = (char *) malloc((strlen(postfix) + 1) * sizeof(char));
    int n = strlen(postfix);
    int i = 0;
    int j = 0;
    char a;
    for(int k = n - 1 ; k >= 0 ; k --)
    {
        if(!isOperator(postfix[k])){
            temp[j++] = postfix[k];
            //printf("%c 1\n", postfix[k]); 
            while (sp->top != -1 && stackTop(sp) == '#')
            {
                //printf("%c 2\n", postfix[k]);
                a = pop(sp);
                //printf("%c a\n", postfix[k]);
                temp[j ++] = pop(sp);
            }
            push(sp, '#');
                           
        }
        else{
            //printf("%c 3\n", postfix[k]);
            push(sp, postfix[k]);            
        }
    }
    temp[j] = '\0';

    j = strlen(temp) - 1;
    while(temp[i] != '\0'){
        prefix[j --] = temp[i ++];
    }
    prefix[strlen(temp)] = '\0';
    return prefix;
    
}

int main(){
    char * postfix;
    char exp[100];
    scanf("%s", exp);
    postfix = exp;
    printf("%s", postfixToprefix(postfix));
    return 0;
}