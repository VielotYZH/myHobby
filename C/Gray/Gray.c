#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

int length,number;

char** init(){
    printf("Please enter the length of the gray code you want:");
    scanf("%d",&length);
    number = (int)pow(2,length);
    char** gray = (char**)malloc(number*sizeof(char*));
    for(int i = 0;i<number;i++){
        gray[i]=(char*)malloc((length+1)*sizeof(char));
        memset(gray[i],'0',length);
        gray[i][length]='\0';
    }
    return gray;
}

void singleOperate(int digital_number,char** gray){
    if(digital_number==1){
        gray[1][length-1] = '1';
    }else{
        int range = (int)pow(2,digital_number);
        for(int i = 0;i<range/2;i++){
            for(int j = length-1;j>=length-digital_number+1;j--){
                gray[range-1-i][j]=gray[i][j];
                gray[range-1-i][length-digital_number]='1';
            }
        }
    }
}

void encode(char** gray){
    for(int i = 1;i<=length;i++){
        singleOperate(i,gray);
    }
}

char* decimalToBinary(int decimal){
    char* binary = (char*)malloc((length+1)*sizeof(char));
    memset(binary,'0',length);
    binary[length]='\0';
    int index = length-1;
    while(decimal>1){
        int quatient = decimal/2;
        binary[index--] = decimal-quatient*2==1?'1':'0';
        decimal=quatient;
    }
    binary[index] = decimal==1?'1':'0';
    return binary;
}

void main(){
    char**gray = init();
    encode(gray);
    printf("Number\tGray\tBinary\n");
    for(int i = 0;i<number;i++){
        printf("%d\t%s\t%s\n",i,gray[i],decimalToBinary(i));
    }
}