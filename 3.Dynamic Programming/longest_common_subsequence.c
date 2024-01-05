#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SEQ_COUNT 100 									// Number of unique sequences to store

void printMatrix(int **, int , int );
void findAllLCS(char *, char *, int **, int, int, char *, int, char **, int *);
int isSequencePrinted(char *, char **, int);
void addPrintedSequence(char *, char **, int *);
int lcs(char *, char *, int, int);

int main(){

    int firstLength, secondLength;
    char *firstWord, *secondWord;

    printf("Please enter the length of the first word: ");			 // Input for the first word
    scanf("%d", &firstLength);
    firstWord = (char*)malloc((firstLength+1)*sizeof(char));
    if (firstWord == NULL){
        printf("\nMemory allocation error for the first word.\n");
        return 1;
    }
    printf("\nPlease enter the first word. ");
    scanf("%s", firstWord);

    printf("\nPlease enter the length of the second word: ");		// Input for the second word
    scanf("%d", &secondLength);
    secondWord = (char*)malloc((secondLength + 1)*sizeof(char));
    if (secondWord == NULL){
        printf("\nMemory allocation error for the second word.\n");
        free(firstWord);
        return 1;
    }
    printf("\nPlease enter the second word. ");
    scanf("%s", secondWord);

    printf("Length of LCS: %d\n", lcs(firstWord, secondWord, firstLength, secondLength));

    free(firstWord);
    free(secondWord);
    
    return 0;
}

/* 
	@brief Prints the elements of a matrix.
	@param matrix -> the matrix to be printed
	@param row -> number of rows in the matrix
	@param column -> number of columns in the matrix
*/
void printMatrix(int **matrix, int row, int column){
    int i, j;
    printf("\n");
    for (i=0; i<row; i++){
        for (j=0; j<column; j++){
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

/* 
	@brief Checks if a sequence is already printed in the given array of sequences.
	@param currentLCS -> the sequence to be checked
	@param printedSeq -> array of printed sequences
	@param seqCount -> number of sequences in the array
	@return 1 if the sequence is printed, 0 otherwise
*/
int isSequencePrinted(char *currentLCS, char **printedSeq, int seqCount){
	int i;
    for (i=0; i<seqCount; i++){
        if (strcmp(currentLCS, printedSeq[i]) == 0){ 						// Sequence is printed already
            return 1; 					
        }
    }
    return 0; 																// Sequence is not printed 
}

/* 
	@brief Adds a unique LCS word to the array of printed sequences.
	@param currentLCS -> the sequence to be added
	@param printedSeq -> array of already printed sequences
	@param seqCount -> pointer to the number of sequences in the array
*/
void addPrintedSequence(char *currentLCS, char **printedSeq, int *seqCount){
    printedSeq[*seqCount] = (char*)malloc((strlen(currentLCS)+1) * sizeof(char));
    strcpy(printedSeq[*seqCount], currentLCS); 								// Unique lcs word to print
    (*seqCount)++;
}

/* 
	@brief Finds all LCS of two words and prints unique sequences.
	@param firstWord -> the first word
	@param secondWord -> the second word
	@param controlMatrix -> the matrix storing the path of backtracked word
	@param i, j -> current indices in the matrix
	@param currentLCS -> current LCS sequence
	@param currentIndex -> current index in the LCS sequence
	@param printedSeq -> array of already printed sequences
	@param seqCount -> pointer to the number of sequences in the array
*/
void findAllLCS(char *firstWord, char *secondWord, int **controlMatrix, int i, int j, char *currentLCS, int currentIndex, char **printedSeq, int *seqCount){

    if (i==0 || j==0){														// If reaching the end of the words
        if (!isSequencePrinted(currentLCS, printedSeq, *seqCount)){			// Check if the sequence is not already printed
            printf("LCS: %s\n", currentLCS);
            addPrintedSequence(currentLCS, printedSeq, seqCount);
        }
        return;
    }
	
    if (controlMatrix[i][j] == 1){												// from the diagonal
        currentLCS[currentIndex] = firstWord[i - 1];
        findAllLCS(firstWord, secondWord, controlMatrix, i - 1, j - 1, currentLCS, currentIndex - 1, printedSeq, seqCount);
    }
    else{
        if (controlMatrix[i][j] == 2 || controlMatrix[i][j] == 4){					// 2-> from the top 4 -> top and left is equal
            findAllLCS(firstWord, secondWord, controlMatrix, i - 1, j, currentLCS, currentIndex, printedSeq, seqCount);
        }
        if (controlMatrix[i][j] == 3 || controlMatrix[i][j] == 4){					// 3-> from the left 4 -> top and left is equal
            findAllLCS(firstWord, secondWord, controlMatrix, i, j - 1, currentLCS, currentIndex, printedSeq, seqCount);
        }
    }
}
/* 
	@brief Finds the length of the Longest Common Subsequence (LCS) of two words.
	@param firstWord -> the first word
	@param secondWord -> the second word
	@param firstLength -> length of the first word
	@param secondLength -> length of the second word
	@return length of the LCS
*/
int lcs(char *firstWord, char *secondWord, int firstLength, int secondLength){
    int i, j;
    int **LCSMatrix = (int**)calloc((firstLength+1), sizeof(int*));				// Matrix to be created with dynamic programming
    int **controlMatrix = (int**)calloc((firstLength+1), sizeof(int*));			// Matrix to check which word is lcs
    char **printedSeq = (char**)malloc(SEQ_COUNT*sizeof(char*));				// Matrix where same lcs words will be checked
    int seqCount = 0;															// Number of sequences

    for (i=0; i<=firstLength; i++){
        LCSMatrix[i] = (int*)calloc((secondLength+1), sizeof(int));
        controlMatrix[i] = (int*)calloc((secondLength+1), sizeof(int));
    }
    
    printf("\nDYNAMIC MATRIX\n");
    printMatrix(LCSMatrix, firstLength+1, secondLength+1);
    printf("\nCONTROL MATRIX\n");
    printMatrix(controlMatrix, firstLength+1, secondLength+1);

    for (i=1; i<=firstLength; i++){
        for (j=1; j<=secondLength; j++){
            if (firstWord[i-1] == secondWord[j-1]){					
                LCSMatrix[i][j] = LCSMatrix[i-1][j-1] + 1;
                controlMatrix[i][j] = 1; 							// The same letters, came from the diagonal
            }
            else{
                if (LCSMatrix[i-1][j]>LCSMatrix[i][j-1]){ 	
                    LCSMatrix[i][j] = LCSMatrix[i-1][j];
                    controlMatrix[i][j] = 2;						// The number above is larger, came from the top row
                }
                else if (LCSMatrix[i-1][j]<LCSMatrix[i][j-1]){
                    LCSMatrix[i][j] = LCSMatrix[i][j-1]; 	
                    controlMatrix[i][j] = 3;						// The number left is larger, came from the left column
                }
                else{
                    LCSMatrix[i][j] = LCSMatrix[i][j-1]; 			
                    controlMatrix[i][j] = 4;						// Top and bottom numbers are equal
                }
            }
        }
        printf("\nDYNAMIC MATRIX\n");
        printMatrix(LCSMatrix, firstLength+1, secondLength+1);
        printf("\nCONTROL MATRIX\n");
        printMatrix(controlMatrix, firstLength+1, secondLength+1);
    }

    char *currentLCS = (char*)calloc((LCSMatrix[firstLength][secondLength]+1),sizeof(char));			// for the lcs word
    findAllLCS(firstWord, secondWord, controlMatrix, firstLength, secondLength, currentLCS, LCSMatrix[firstLength][secondLength]-1, printedSeq, &seqCount);
    
	//Free memory
    
    for (i=0; i<seqCount; i++){
        free(printedSeq[i]);
    }
    free(printedSeq);


    return LCSMatrix[firstLength][secondLength];
}

