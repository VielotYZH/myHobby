#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>
#include<string.h>
#include<Windows.h>

#define PAGES_NUMBER 10000
#define MAX_PAGE_TYPES 5
#define PAGE_FRAMES_NUMBER 3

int missingPageNumber = 0;

void randomlyGeneratePages(char arr[], int length){
    srand((unsigned int)time(NULL));
    for(int i = 0; i < length ; i++){
        arr[i] = rand() % MAX_PAGE_TYPES + 65;//The ASCII code of A is 65
    }
}

//Queries elements in an array whose indexes range from start to end-1
int contains(char arr[], int start, int end, char element){
    for(int i = start;i < end;i++){
        if(arr[i] == element){
            return i;
        }
    }
    return -1;
}

//nowrap
void printPageFrames(char arr[], int length){
    for(int i = 0; i < length; i++){
        printf("%c\t\t",arr[i]);
    }
}

//Optimal Replacement
bool OPT(char pages[], int pagesLength, int currentIndex, char pageFrames[], int pageFramesLength){
    if(contains(pageFrames, 0, PAGE_FRAMES_NUMBER, pages[currentIndex]) == -1){
        missingPageNumber++;
        int availableIndex = contains(pageFrames, 0, PAGE_FRAMES_NUMBER, 0);
        if(availableIndex != -1){
            pageFrames[availableIndex] = pages[currentIndex];
        }else{
            int targetIndex = 0;
            int maxIndex = 0;
            for(int i = 0 ; i < pageFramesLength ; i++){
                int tempIndex = contains(pages, currentIndex + 1, pagesLength, pageFrames[i]);
                if(tempIndex == -1){
                    targetIndex = i;
                    break;
                }else if(tempIndex > maxIndex){
                    maxIndex = tempIndex;
                    targetIndex = i;
                }
            }
            pageFrames[targetIndex] = pages[currentIndex];
        }
        return true;
    }
    return false;
}

//First In First Out
bool FIFO(char page, char pageFrames[]){
    static int times[PAGE_FRAMES_NUMBER];
    static bool hasInit = false;
    if(!hasInit){
        for(int i = 0; i < PAGE_FRAMES_NUMBER; i++){
            times[i] = -1;
        }
        hasInit = true;
    }
    if(contains(pageFrames, 0, PAGE_FRAMES_NUMBER, page) == -1){
        missingPageNumber++;
        int availableIndex = contains(pageFrames, 0, PAGE_FRAMES_NUMBER, 0);
        if(availableIndex != -1){
            pageFrames[availableIndex] = page;
            times[availableIndex] = 0;
        }else{
            int targetIndex = 0;
            int i = 1;
            for(; i < PAGE_FRAMES_NUMBER ; i++){
                if(times[i] > times[targetIndex]){
                    targetIndex = i;
                }
            }
            pageFrames[targetIndex] = page;
            times[targetIndex] = 0;
        }
        for(int i = 0 ; i < PAGE_FRAMES_NUMBER ; i++){
            if(times[i] != -1){
                times[i]++;
            }
        }
        return true;
    }
    return false;
}

//Least Recently Used
bool LRU(char page, char pageFrames[]){
    bool missingPage = false;
    static int times[PAGE_FRAMES_NUMBER];
    static bool hasInit = false;
    if(!hasInit){
        for(int i = 0; i < PAGE_FRAMES_NUMBER; i++){
            times[i] = -1;
        }
        hasInit = true;
    }
    int index = contains(pageFrames, 0, PAGE_FRAMES_NUMBER, page);
    if(index == -1){
        missingPageNumber++;
        missingPage = true;
        int availableIndex = contains(pageFrames, 0, PAGE_FRAMES_NUMBER, 0);
        if(availableIndex != -1){
            pageFrames[availableIndex] = page;
            times[availableIndex] = 0;
        }else{
            int targetIndex = 0;
            int i = 1;
            for(; i < PAGE_FRAMES_NUMBER ; i++){
                if(times[i] > times[targetIndex]){
                    targetIndex = i;
                }
            }
            pageFrames[targetIndex] = page;
            times[targetIndex] = 0;
        }
    }else{
        times[index] = 0;
    }
    for(int i = 0 ; i < PAGE_FRAMES_NUMBER ; i++){
        if(times[i] != -1){
            times[i]++;
        }
    }
    return missingPage;
}

//Least Frequently Used
bool LFU(char page, char pageFrames[]){
    static int times[PAGE_FRAMES_NUMBER];
    static bool hasInit = false;
    if(!hasInit){
        for(int i = 0 ; i < PAGE_FRAMES_NUMBER ; i++){
            times[i] = -1;
        }
        hasInit = true;
    }
    int index = contains(pageFrames, 0, PAGE_FRAMES_NUMBER, page);
    if(index == -1){
        missingPageNumber++;
        int availableIndex = contains(pageFrames, 0, PAGE_FRAMES_NUMBER, 0);
        if(availableIndex != -1){
            pageFrames[availableIndex] = page;
            times[availableIndex]++;
        }else{
            int targetIndex = 0;
            for(int i = 1 ; i < PAGE_FRAMES_NUMBER ; i++){
                if(times[i] < times[targetIndex]){
                    targetIndex = i;
                }
            }
            pageFrames[targetIndex] = page;
            for(int i = 0 ; i < PAGE_FRAMES_NUMBER ; i++){
                times[i] = 0;
            }
        }
        return true;
    }else{
        times[index]++;
        return false;
    }
}

int runOPT(bool (*p)(char[], int, int, char[], int), bool printProcedure, char pages[], int pagesLength){
    char pageFrames[PAGE_FRAMES_NUMBER];
    memset(pageFrames, 0, PAGE_FRAMES_NUMBER);
    if(printProcedure){
        for(int i = 0 ; i < PAGE_FRAMES_NUMBER ; i++){
            printf("pageFrame%d\t", i + 1);
        }
        printf("pageMissing\n");
    }
    for(int i = 0 ; i < pagesLength ; i++){
        bool missingPage = p(pages, PAGES_NUMBER, i, pageFrames, PAGE_FRAMES_NUMBER);
        if(printProcedure){
            printPageFrames(pageFrames, PAGE_FRAMES_NUMBER);
            if(missingPage){
                printf("*");
            }
            printf("\n");
        }
    }
    // printf("OPT missing page number:%d\n", missingPageNumber);
    // printf("OPT page fault rate:%.2f%%\n", (double)missingPageNumber/pagesLength*100);
    // printf("\n\n");
    int result = missingPageNumber;
    missingPageNumber = 0;
    return result;
}

int runFunction(bool (*p)(char, char[]), char* functionName, bool printProcedure, char pages[], int pagesLength){
    char pageFrames[PAGE_FRAMES_NUMBER];
    memset(pageFrames, 0, PAGE_FRAMES_NUMBER);
    if(printProcedure){
        for(int i = 0 ; i < PAGE_FRAMES_NUMBER ; i++){
            printf("pageFrame%d\t", i + 1);
        }
        printf("pageMissing\n");
    }
    for(int i = 0 ; i < pagesLength ; i++){
        bool missingPage = p(pages[i], pageFrames);
        if(printProcedure){
            printPageFrames(pageFrames, PAGE_FRAMES_NUMBER);
            if(missingPage){
                printf("*");
            }
            printf("\n");
        }
    }
    printf("%s missing page number:%d\n",functionName, missingPageNumber);
    printf("%s page fault rate:%.2f%%\n",functionName, (double)missingPageNumber/pagesLength*100);
    printf("\n\n");
    int result = missingPageNumber;
    missingPageNumber = 0;
    return result;
}

void testOPT(bool (*p)(char[], int, int, char[], int)){
    char arr[12] = {'A','B','C','D','A','B','E','A','B','C','D','E'};

    runOPT(p, true, arr, 12);

    exit(0);
}

void test(bool (*p)(char, char[]), char* functionName){
    char arr[12] = {'A','B','C','D','A','B','E','A','B','C','D','E'};

    runFunction(p, functionName, true, arr, 12);

    exit(0);
}

void main(){
    bool (*OPTp)(char[], int, int, char[], int) = OPT;
    bool (*FIFOp)(char, char[]) = FIFO;
    bool (*LRUp)(char, char[]) = LRU;
    bool (*LFUp)(char, char[]) = LFU;

    //testOPT(OPTp);
    //test(FIFOp, "FIFO");
    //test(LRUp,"LRU");
    test(LFUp, "LFU");

    int everyResult[4][3];
    for(int i = 0 ; i < 3 ; i++){
        char pages[PAGES_NUMBER];
        randomlyGeneratePages(pages, PAGES_NUMBER);

        everyResult[0][i] = runOPT(OPTp, false, pages, PAGES_NUMBER);

        everyResult[1][i] = runFunction(FIFOp, "FIFO", false, pages, PAGES_NUMBER);

        everyResult[2][i] = runFunction(LRUp, "LRU", false, pages, PAGES_NUMBER);

        everyResult[3][i] = runFunction(LFUp, "LFU", false, pages, PAGES_NUMBER);

        Sleep(700);
    }

    int result[4];
    for(int i = 0 ; i < 4 ; i++){
        int sum = 0;
        for(int j = 0 ; j < 3 ; j++){
            sum += everyResult[i][j];
        }
        result[i] = sum / 3;
        printf("%.2f ",(double)result[i]/PAGES_NUMBER*100);
    }    
}