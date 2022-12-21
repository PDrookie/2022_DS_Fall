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
        char val = ptr->arr[ptr->top];
        ptr->top--;
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
    else
        return 0;
}

char * infixTopostfix(char* infix){
    struct stack * sp = (struct stack * ) malloc(sizeof(struct stack));
    sp->size = 100;
    sp->top = -1;
    sp->arr = (char *) malloc(sp->size * sizeof(char));
    char * postfix = (char *) malloc((strlen(infix) + 1) * sizeof(char));
    int i = 0;
    int j = 0;
    while (infix[i] != '\0')
    {
        if(!isOperator(infix[i])){
            //printf("%c 1\n", infix[i]);
            postfix[j] = infix[i];
            i ++;
            j ++;
        }
        else if(infix[i] == '('){
            //printf("%c 2\n", infix[i]);
            push(sp, infix[i]);
            i ++;        
        }
        else if(infix[i] == ')'){
            while(!isEmpty(sp) && stackTop(sp) != '('){
                //printf("%c 3\n", infix[i]);
                postfix[j] = pop(sp);
                j ++;
            }
            i ++;            
            if(!isEmpty(sp) && stackTop(sp) != '('){
                return 0;
            }
            else{
                //printf("I POP\n");
                pop(sp);
            }            
        }
        else{
            while (!isEmpty(sp) && precedence(infix[i]) <= precedence(stackTop(sp)))
            {
                //printf("%c preRUN\n", infix[i]);
                postfix[j] = pop(sp);                    
                j ++;    
            }
            //printf("%c 4\n", infix[i]);
            push(sp, infix[i]);
            i ++;
        }
    }
    while (!isEmpty(sp))
    {
        postfix[j] = pop(sp);
        j ++;
    }
    

    postfix[j] = '\0';
    return postfix;
    
}

int main(){
    char * infix;
    char exp[100];
    scanf("%s", exp);
    infix = exp;
    printf("%s", infixTopostfix(infix));
    return 0;
}