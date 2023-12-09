#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#define HORNERS_CONST 31
 
struct record{															
	char* userName;
	int deleted;
};

int hornerMethod(char*, int);
int firstHashFunc(int, int);
int secondHashFunc(int, int);
void insertToHash(struct record *, int, char*, int);
int searchInHash(struct record *, int, char *, int);
struct record *rehash(struct record *, int, int);
void modeOperations(int, struct record *, int);
void printHash(struct record*, int);
//int isPrime(int);
//int findNextPrime(int);

int main(){
		
	int option, operation, exitMode =1; 								// Variables for menu navigation and control
	float  loadFactor;													// The load factor entered by the user
	int numberOfElements,tableSize;										// Number of elements and the calculated table size
	char *userName = (char*)malloc(20*sizeof(char));					// Buffer for user input
	int i;
	
	printf("Please enter the number of elements you will insert to hash table.\n");
	scanf("%d", &numberOfElements);
		
	printf("Please enter the load factor\n");
	scanf("%f", &loadFactor);
		
//	tableSize = findNextPrime((int)(numberOfElements/loadFactor)+1);
//	printf("\nTable size will be %d.\n", tableSize);

	printf("Please enter the table size. It should be higher than %f.\n",numberOfElements/loadFactor);
	scanf("%d", &tableSize);
		
	struct record *hashTable = (struct record*)calloc(tableSize, sizeof(struct record));
		
	// Initialize the hash table
	for(i=0; i<tableSize; i++){
		hashTable[i].deleted = 0;
		hashTable[i].userName = "";
	}
		
	do{
		printf("\n1-Normal Mode\n2-Debug Mode\n0-Exit\nEnter: ");
		scanf("%d", &option);
			
		if(option == 1){
			printf("\n***NORMAL MODE***\n");
			modeOperations(option, hashTable, tableSize); 				// Call the function for normal mode operations		
		}
		else if(option == 2){
			printf("\n***DEBUG MODE***\n");
			modeOperations(option, hashTable, tableSize);				// Call the function for debug mode operations	
		}
		else if(option == 0){	
			exitMode = 0;												// Set exit flag for program termination
		}
		else{
			printf("Invalid. Try again.\n");
		}	
	}while(exitMode);
		
	return 0;
}

/* 
	@brief Performs various operations based on the selected mode.
	@param option -> the selected mode (NORMAL_MODE or DEBUG_MODE)
	@param hashTable -> pointer to the hash table
	@param tableSize -> size of the hash table
	@return 
*/
void modeOperations(int option, struct record *hashTable, int tableSize){
	
	char userName[20];
	int operation, found, edit, exitOp = 1; 								// Variables for menu navigation and control
	do{
		printf("\n1-Insertion\n2-Search\n3-Deletion\n4-Edit\n0-Back\nEnter: ");
		scanf("%d", &operation);
		if(operation == 1){													
			printf("\n**INSERTION**\n");
			printf("\nPlease enter a username for insertion. Enter: ");
			scanf("%s", &userName);
		    getchar();														// Clear the input buffer (for enter)
			insertToHash(hashTable, tableSize, userName, option);			// Call the function for insertion
		}	
		else if(operation == 2){
			printf("\n**SEARCH**\n");
			printf("\nPlease enter a username for search. Enter: ");
			scanf("%s", &userName);
		    getchar();
			found = searchInHash(hashTable, tableSize, userName, option);	// Call the function for search
					
			if(found == -1){
				printf("\nElement '%s' was not found in the table.\n", userName);
			}else{
				printf("\nThe element '%s' is found at address %d.\n", userName, found);
			}		
		}
		else if(operation == 3){
			printf("\n**DELETION**\n");
			printf("\nPlease enter a username for deletion. Enter: ");
			scanf("%s", &userName);
		    getchar();
					
			found = searchInHash(hashTable, tableSize, userName, option);	// Find the element will be deleted
					
			if(found == -1){												// Was not found
				printf("\nElement '%s' is not deleted because it does not exist in the table.\n", userName);
			}else{
						
				hashTable[found].deleted = 1;								// Was deleted
				printf("\nThe '%s' element at address %d was deleted.\n", userName, found);
						
			}
		}
		else if(operation == 4){
			
			printf("\nThe table is edited.");
			hashTable = rehash(hashTable, tableSize, option);				// Rehash
			printHash(hashTable, tableSize);	
		}
		else if(operation == 0){
			exitOp = 0;
		}
		else{
			printf("\nInvalid. Try again.\n");
		}
	}while(exitOp);	
}

/* 
	@brief Applies Horner's method to convert a string into a hash value.
	@param name -> the input string
	@return the hash value calculated using Horner's method
*/
int hornerMethod(char* name, int tableSize){ 
    int res = name[0] - 'a' + 1; 									// initialize the result with the first character's value
 	int i=1;
    
    while(name[i] != '\0'){
		res = res * HORNERS_CONST + (name[i] - 'a' + 1);			// Apply Horner's method for each character	
		res %= tableSize;											// For overflow
		i++;
	}
    return res; 
} 

/* 
	@brief Applies the first hash function to calculate the hash index.
	@param key -> the key for hashing
	@param tableSize -> the size of the hash table
	@return the calculated hash index
*/
int firstHashFunc(int key, int tableSize){
	return key % tableSize;
}

/* 
	@brief Applies the second hash function to calculate the hash index.
	@param key -> the key for hashing
	@param tableSize -> the size of the hash table
	@return the calculated hash index
*/
int secondHashFunc(int key, int tableSize){
	return  1 + (key % (tableSize-2));	
}

/* 
	@brief Inserts an element into the hash table using open addressing and double hashing.
	@param table -> pointer to the hash table
	@param tableSize -> size of the hash table
	@param name -> the element to insert
	@param option -> the selected mode (NORMAL_MODE or DEBUG_MODE)
	@return 
*/
void insertToHash(struct record *table, int tableSize, char* name, int option){
	int i=1;														// Counter for probing attempts
	int hornerInt = hornerMethod(name, tableSize);					// Calculate the hash value using Horner's method
	int hIndex1 = firstHashFunc(hornerInt, tableSize);				// Calculate the first hash index
	int hIndex2 = secondHashFunc(hornerInt, tableSize);				// Calculate the second hash index
	int hashIndex = hIndex1;										// Initialize the hash index
	
	// Debug mode: Display information
	if(option == 2){
		printf("\nD: After Horner's Method: %s -> %d", name, hornerInt);
		printf("\nD: h1('%s') = %d", name, hIndex1);
		printf("\nD: h2('%s') = %d", name, hIndex2);
		printf("\nD: h('%s', 0) = %d", name, hashIndex);
	}

	// Search for an available slot
	while(table[hashIndex].userName != "" && strcmp(table[hashIndex].userName, name) && i<tableSize){
		if(option == 2){											// Debug mode: Display information if the word is not placed at the current address
			printf("\nD: The word '%s' is not placed at the address %d.", name, hashIndex);
		}
		hashIndex += hIndex2;										// Update the hash index using the second hash function	
		hashIndex %= tableSize;										// Ensure the hash index is within bounds of the table
		i++;
	}
	if(i >= tableSize){												// Check if the insertion is not possible due to a full table
		printf("\nThe insertion operation could not be performed because the table is full.");
	}
	else{
		if(table[hashIndex].userName == ""){						 // Check if the slot is empty
			table[hashIndex].userName = (char*)malloc(strlen(name)* sizeof(char));
			strcpy(table[hashIndex].userName, name);
			if(option == 2){										// Debug mode: Display information about the successful placement
				printf("\nD: The word '%s' is placed at address %d.\n", name, hashIndex);
			}
			printf("\nYour element is placed at address %d.\n", hashIndex);
		}
		else{
			if(table[hashIndex].deleted == 1){						// Check if the slot was marked as deleted
				table[hashIndex].deleted = 0;						// Reset the deleted flag
				if(option == 2){									// Debug mode: Display information about the successful placement
					printf("\nD: The word '%s' is placed at address %d.\n", name, hashIndex);
				}
				printf("\nYour element is placed at address %d.\n", hashIndex);
			}
			else{
				printf("\nThe insertion operation could not be performed because the element already exists in the table.\n");
			}
		}	
	}
}

/* 
	@brief Searches for an element in the hash table using open addressing and double hashing.
	@param table -> pointer to the hash table
	@param tableSize -> size of the hash table
	@param name -> the element to search
	@param option -> the selected mode (NORMAL_MODE or DEBUG_MODE)
	@return the address of the found element or -1 if not found
*/
int searchInHash(struct record *table, int tableSize, char *name, int option){
	
	int i=1;														// Counter for probing attempts
	int hornerInt = hornerMethod(name, tableSize);					// Calculate the hash value using Horner's method
	int hIndex1 = firstHashFunc(hornerInt, tableSize);				// Calculate the first hash index
	int hIndex2 = secondHashFunc(hornerInt, tableSize);				// Calculate the second hash index
	int hashIndex = hIndex1;										// Initialize the hash index
	
	// Debug mode: Display information
	if(option == 2){
		printf("\nD: After Horner's Method: %s -> %d", name, hornerInt);
		printf("\nD: h1('%s') = %d", name, hIndex1);
		printf("\nD: h2('%s') = %d", name, hIndex2);
	}
	
	// Search for the word in the hash table
	while(strcmp(table[hashIndex].userName, name) != 0 && i<tableSize){		// Debug mode: Display information if the word is not found at the current address
		if (option == 2) {
            printf("\nD: The word '%s' is not found at address %d.", name, hashIndex);
        }
		hashIndex += hIndex2; 									// Update the hash index using the second hash function
        hashIndex %= tableSize; 								// Ensure the hash index is within bounds of the table
		i++;
	}
		
	if (i >= tableSize) {										// Check if the word is not found after probing the entire table
        if (option == 2) {
            printf("\nD: The word '%s' is not found at address %d.", name, hashIndex);
        }
        return -1;
    } else {													// Check if the word is found at the current address
        if (table[hashIndex].deleted == 1) {					// Check if the word was marked as deleted
            if (option == 2) {	
                printf("\nD: The word '%s' has been already deleted at address %d.", name, hashIndex);
            }
            return -1;
        } else {												// Debug mode: Display information about the successful search
            if (option == 2) {
                printf("\nD: The word '%s' is found at address %d.", name, hashIndex);
            }
            return hashIndex;
        }
    }
}

/* 
	@brief Rehashes the elements from the old hash table to a new hash table.
	@param oldTable -> pointer to the old hash table
	@param tableSize -> size of the old hash table
	@param option -> the selected mode (NORMAL_MODE or DEBUG_MODE)
	@return pointer to the new hash table
*/
struct record *rehash(struct record *oldTable, int tableSize, int option){
	
	int i;
	struct record *newTable = (struct record*)calloc(tableSize, sizeof(struct record));
	
	for(i=0; i<tableSize; i++){
		newTable[i].deleted = 0;								// Initialize the new hash table
		newTable[i].userName = "";
	}
	
	for(i=0; i<tableSize; i++){
		
		if(oldTable[i].userName != ""){							// Check if the username in the old table is not empty
			if(oldTable[i].deleted != 1){						// Check if the element was not marked as deleted
				
				if(option ==  2){								// Debug mode: Display information about the rehashing process (not deleted)
					printf("\n\nD: Username: %s\nD: Old Address: %d\nD: Deleted: NO", oldTable[i].userName, i );
				}
				insertToHash(newTable, tableSize, oldTable[i].userName, option);	// Insert the element into the new hash table
			}
			else{
				if(option == 2){								// Debug mode: Display information about the rehashing process (marked as deleted)
					printf("\n\nD: Username: %s\nD: Old Address: %d\nD: Deleted: YES", oldTable[i].userName, i );
				}
			}
		}
	}
	free(oldTable);												// Free the memory allocated for the old hash table
	return newTable;
}

/* 
	@brief Prints the contents of the hash table.
	@param hashTable -> pointer to the hash table
	@param size -> size of the hash table
*/
void printHash(struct record* hashTable, int size){
	int i;
	printf("\nTable:");
    for(i=0; i<size; i++){ 
		printf("\n%d: %s -> %d", i, hashTable[i].userName, hashTable[i].deleted); 
    } 
    printf("\n"); 
} 



///* 
//	@brief Checks if a given number is a prime number.
//	@param num -> the number to check for primality
//	@return 1 if the number is prime, 0 otherwise
//*/
//int isPrime(int num) {
//	int i;
//    if (num <= 1) {												// Numbers less than or equal to 1 are not prime
//        return 0;
//    }
//    for (i=2; i*i<=num; i++) {									// Check for factors up to the square root of the number
//        if (num % i == 0) {
//            return 0;
//        }
//    }
//    return 1;
//}
//
///* 
//	@brief Finds the next prime number greater than or equal to a given starting number.
//	@param start -> the starting number
//	@return the next prime number
//*/
//int findNextPrime(int start) {
//    int num = start;
//    while (!isPrime(num)) {										// Keep incrementing the number until a prime number is found
//        num++;
//    }
//    return num;
//}


