#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CHAR_LENGTH 255
#define FILE_LENGTH 30
#define CLASS_LENGTH 10

#define DEFAULT_ERROR "Failed to open the file, exiting the program!\n"
#define MEMORY_ERROR "Failed to allocate memory.\n" 
#define ANALYSIS_ERROR "Nothing to analyze. Read a file before analysis.\n"
#define WRITE_ERROR "No writeable information. Analyze the data before writing.\n"
#define UNKNOWN_CHOICE_ERROR "Unknown choice. Try again!\n"

typedef struct content{
    char *pWord;
    int iClass;
    struct content *pNext;
}CONTENT;

int makeChoice(void){
    int iChoice;
    printf("Choose one of the following actions:\n");
    printf("1) Read a file\n");
    printf("2) Analyze the information\n");
    printf("3) Write the results\n");
    printf("0) End the program\n");
    printf("Make your choice: ");
    scanf("%d", &iChoice);
    return iChoice;
}
void fileName(char *cFile){
    char aName[CHAR_LENGTH];
    printf("Give the name of the file: ");
    scanf("%s", aName);
    strcpy(cFile, aName);
    return;
}
CONTENT *readFile(char *cReadContent){
    FILE *pFile;
    char aRivi[CHAR_LENGTH];
    char *pFileWord;
    char *pPart;
    CONTENT *pNew = NULL, *pFirst = NULL, *pLast = NULL;
    if((pFile = fopen(cReadContent, "r")) == NULL){
        perror(DEFAULT_ERROR);
        exit(0);
    }
    fgets(aRivi, CHAR_LENGTH, pFile);
    while(fgets(aRivi, CHAR_LENGTH, pFile) != NULL){
        if((pNew = (CONTENT*)malloc(sizeof(CONTENT))) == NULL){
            perror(MEMORY_ERROR);
            exit(0);
        }
        pFileWord = strtok(aRivi, ";");
        if((pNew->pWord = (char*)malloc(sizeof(char)*strlen(pFileWord)+1)) == NULL){
            perror(MEMORY_ERROR);
            exit(0);
        }
        strcpy(pNew->pWord, pFileWord);
        pPart = strtok(NULL, ";");
        pNew->iClass = atoi(pPart);
        pNew->pNext = NULL;
        if(pFirst == NULL){
            pFirst = pNew;
            pLast = pNew;
        } else {
            pLast->pNext = pNew;
            pLast = pNew;
        }
    }
    printf("The file '%s' has been read.\n", cReadContent);
    fclose(pFile);
    return pFirst;
}
int calculation(CONTENT *contents, int i){
    CONTENT *ptr = contents;
    int iQuantity = 0; 
    while(ptr != NULL){
        if(ptr->iClass == i){
            iQuantity = iQuantity + 1;
        }
        ptr = ptr->pNext;
    }
    return iQuantity;
}
int *analysis(CONTENT *contents, int *pQuantities){
    CONTENT *ptr = contents;
    CONTENT *cBeginning = NULL, *cEnd = NULL;
    int i;
    int iSum = 0;
    for(i=1; i<=10; i++){
        pQuantities[i] = calculation(contents, i);
        iSum = iSum + pQuantities[i];
    }
    while(ptr != NULL){
        if(cBeginning == NULL){
            ptr = ptr->pNext;
            cBeginning = ptr;
            cEnd = ptr;
        } else{
            cEnd = ptr;
        }
        ptr = ptr->pNext;
    }
    printf("Analyzed %d data element, from the word '%s' to the word '%s'.\n", iSum, cBeginning->pWord, cEnd->pWord);
    return pQuantities;
}
void writeFile(char *cWriteable, int *pQuantities){
    FILE *file;
    int i;
    if((file = fopen(cWriteable, "w")) == NULL){
        perror(DEFAULT_ERROR);
        exit(0);
    }
    fprintf(file, "Word class;Quantity\n");
    for(i=1; i<=10; i++){ //We have ten word classes
        fprintf(file, "Class %d;%d\n", i, pQuantities[i]);
    }
    printf("The file '%s' has been written.\n", cWriteable);
    fclose(file);
    return;
}
void freeMemory(CONTENT *pContents){
    CONTENT *ptr = pContents;
    while(pContents != NULL){
        ptr = pContents;
        free(ptr->pWord);
        free(ptr);
        pContents = pContents->pNext;
    }
    return;
}
int *resetNumber(int *iQuantities){
    int *ptr = iQuantities;
    int i;
    if(sizeof(ptr) != 0){
        for(i=0; i<=sizeof(iQuantities); i++){
            iQuantities[i] = 0;
        }
    }
    return(iQuantities);
}
int checkAnalysis(int *pQuantities){
    int i;
    int sum=0;
    for(i=0; i<10; i++){
        if(pQuantities[i] == 0){
            sum++;
        }
    }
    if(sum == 10){
        return(0);
    } else{
        return(1);
    }
}
int main(void){
    CONTENT *pContentList;
    CONTENT *pBackUp;
    int iChoice;
    char cFile[FILE_LENGTH];
    int iQuantities[CLASS_LENGTH]; //This is for the ten different word classes
    int *pQuantities = iQuantities;
    int iCriteria; //Making sure the data has been analyzed
    pQuantities = resetNumber(iQuantities); 
    do{
        iChoice = makeChoice();
        if(iChoice == 1){
            freeMemory(pContentList);
            pContentList = NULL;
            fileName(cFile);
            pContentList = readFile(cFile);
            printf("\n");
        } else if(iChoice == 2){
            if(pContentList == NULL){
                printf(ANALYSIS_ERROR);
                printf("\n");
            } else{
                pQuantities = resetNumber(iQuantities);
                pQuantities = analysis(pContentList, pQuantities);
            }
            printf("\n");
        } else if(iChoice == 3){
            iCriteria = checkAnalysis(iQuantities);
            if(iCriteria == 0){
                printf(WRITE_ERROR);
            } else{
                fileName(cFile);
                writeFile(cFile, iQuantities);
            }
            printf("\n");
        } else if(iChoice == 0){
            printf("Ending the program.\n");
            break;
        } else{
                printf(UNKNOWN_CHOICE_ERROR);
                printf("\n");
                continue;
            }
    } while(iChoice != 0);
    pQuantities = NULL;
    freeMemory(pContentList);
    pContentList = NULL;

    printf("Thank you for using the program!.\n");    
    return(0);
}