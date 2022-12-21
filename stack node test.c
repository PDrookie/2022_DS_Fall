#include<stdio.h>

struct Node
{
    char data;
    struct Node* link;
};

struct Node* top = NULL;

void Push(char x){
    struct Node* temp = (struct Node*)malloc(sizeof(struct Node*));
    temp -> data = x;
    temp -> link = top;
    top = temp;
}

int Pop(){
    struct Node *temp;
    if(top == NULL){
        return;
    }
   /* if(top == '(' || top == ')'){
        temp = findTheC(temp, top);
        print(temp); 
        free(temp);
    }*/
    {
        temp = top;
        char temp_data = top -> data;
        top = top -> link;
        print(temp);
        free(temp);
        return temp_data; 
    }
    
}

/*char findTheC(char temp, char h){
    if(top -> link != '(' || top -> link != ')'){
        return temp = top -> link;
    }
    else {
        findTheC(top -> link);
    }
}*/

void print(char x){
    printf("%c\n", x);
}

int main(int argc, char* argv[]){
    int read;
    Push('A');
    Push('B');
    Push('D');
    Pop();
    Pop();    
    Push('E');
    Pop();
    Pop();    
    /*do{
      scanf("%c", &read);
      Push(read);
      //if(read != '+' || read != '-' || read != '*' || read != '/' || read != '(' || read != ')'){
        Pop();
      //}  
    }while(read != '\0');*/
    return 0;
}
