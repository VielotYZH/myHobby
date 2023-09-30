#include<stdio.h>
#include<stdbool.h>

#define NODE_NUMBER 3

void explain(void){
    printf("The number of nodes in the current figure is %d\n",NODE_NUMBER);
    printf("Enter data corresponding to each node in sequence\n");
    printf("Each data range is an integer between 0 and 65535\n");
    printf("Separate data with Spaces\n");
}

void initializeCosts(int costs[NODE_NUMBER][NODE_NUMBER]){
    explain();
    for(int i = 0 ;i<NODE_NUMBER;i++){
        for(int j = 0;j<NODE_NUMBER;j++){
            scanf("%d",costs[i][j]);
        }
    }
}

//from array1 to array2
void copyArray(int array1[][NODE_NUMBER],int array2[][NODE_NUMBER]){
    for(int i = 0;i<NODE_NUMBER;i++){
        for(int j = 0;j<NODE_NUMBER;j++){
            array2[i][j]=array1[i][j];
        }
    }
}

void fute(int costs[NODE_NUMBER][NODE_NUMBER]){
    int message[NODE_NUMBER][NODE_NUMBER];
    copyArray(costs,message);
    bool ifChangeAnyCost = false;
    do{
        for(int i =0;i<NODE_NUMBER;i++){
            for(int j = 0;j<NODE_NUMBER;j++){
                if(message[j][i]==1){
                    //neighbour node
                    
                }
            }
        }
    }while(ifChangeAnyCost);
}

void main(){
    int costs[NODE_NUMBER][NODE_NUMBER];
    initializeCosts(costs);
}