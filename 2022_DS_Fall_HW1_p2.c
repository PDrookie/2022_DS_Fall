#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int size = 0;
int data[1005] = {0};
int flag = 0;

void quicksort(int number[50],int first,int last){
   int i, j, pivot, temp;
   if(first<last){
      pivot=first;
      i=first;
      j=last;
      while(i<j){
         while(number[i]<=number[pivot]&&i<last)
         i++;
         while(number[j]>number[pivot])
         j--;
         if(i<j){
            temp=number[i];
            number[i]=number[j];
            number[j]=temp;
         }
      }
      temp=number[pivot];
      number[pivot]=number[j];
      number[j]=temp;
      quicksort(number,first,j-1);
      quicksort(number,j+1,last);
    }

}

void insert(int x){
    size ++;
    int i = size;
    //printf("%d size\n", size);
    for(int j = 0 ; j < size ; j ++){
        if(data[j] == x){
            flag = 1;
        }
    }
    if(flag != 1){
        data[-- i] = x;
        //printf("%d in\n", data[size - 1]);
    }
    quicksort(data, 0, size - 1);
}

void change(int x, int y){
    /*for(int j = 0 ; j < size ; j ++){
        if(data[j] == y){
            flag = 1;
        }
    }*/
    if(flag != 1){
        data[x] = y;
        quicksort(data, 0, size - 1);
    }
    quicksort(data, 0, size - 1);
    }


int main(){
    char action[10];
    int heapN, i, j;
    char trash, comma;
    int result[1005];
    int n = 0;
    for(;;)
    {
        for(int i = 0 ; i < size ; i ++){
        printf("%d ", data[i]);
        }
        printf(" (qsort)\n");

        for(int k = 0 ; k < 10 ; k ++){
            action[k] = '\0';
        }
        scanf("%s", action);        
        //printf("%s\n", action);
        if(action[0] == 'i'){
            scanf("%d%c", &heapN, &trash);
            //printf("%d NN\n", heapN);
            insert(heapN);
        }
        else if(action[0] == 'r'){
            if(size == 0){
                result[n] = 0;
                n ++;
                continue;
            }
            result[n] = data[0];
            n ++;
            size --;
            for(int i = 0 ; i < size ; i ++){                
                data[i] = data[i + 1];
                //printf("%d(%d) ", data[i], i);
            }
            //rintf("\n");
        }
        else if(action[0] == 'c'){
            scanf("%d%c%d", &i, &comma, &j, &trash);
            if(i >= size || i < 0){                
                result[n] = -2;
                //printf("%d\n", result[n]);
                n ++;
            }
            else
                change(i, j);
            }
        if(action[0] == 'q'){
            for(int k = 0 ; k < n ; k ++){
                if(result[k] == 0){
                    //printf("%d\n", result[k]);
                    printf("empty\n");
                }
                else if(result[k] == -2){
                    printf("out of range\n");
                }
                else
                    printf("%d\n", result[k]);
            }
            exit(0);
        }
    }
    return 0;
}
