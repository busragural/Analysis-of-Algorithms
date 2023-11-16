#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void swap(int*, int*); 
int partition(int*, int, int, int);
void quickSort(int*, int*, int, int);
void printArray(int*, int);

int main(){ 
	srand(time(NULL));
    int keys[] = {18, 17, 0, 2, 1, 9, 16, 10, 21, 15};
    int locks[] = {9, 10, 16, 15, 1, 0, 21, 18, 2, 17};

    
//    int keys[] = {1,2,3,4,5,6,7,8,9};
//    int locks[] = {1,2,3,4,5,6,7,8,9};

//    int keys[] = {9,8,7,6,5};
//    int locks[] = {9,8,7,6,5};
    
//    int keys[] = {20, 70, 10, 80, 60};
//    int locks[] = {10, 20, 60, 70, 80};

//    int keys[] = {1,2};
//    int locks[] = {2,1};

    
    // @param n -> the number of elements of the array
    int n = sizeof(keys) / sizeof(keys[0]); 	
    
    printf("Before sorting\n");
    printArray(keys, n);
	printArray(locks, n);
	
    quickSort(keys, locks, 0, n - 1);
    
    printf("\nAfter sorting\n");
	printArray(keys, n);
	printArray(locks, n);
	
    return 0; 
}

/* 
	@brief swaps two given numbers using their addresses
	@param firstNum -> the first number to swap
	@param secondNum -> the second number to swap
	@return 
*/
void swap(int* firstNum, int* secondNum) {
  int tmp = *firstNum;
  *firstNum = *secondNum;
  *secondNum = tmp;
}


/* 
	@brief sorts the given array with quick sort from divide&conquer approach
	@param keys -> array that contains keys
	@param locks -> array that contains locks
	@param low -> start of the array
	@param high -> end of the array
	@return 
*/
void quickSort(int* keys, int* locks, int low, int high) {
    if (low<high) {														// checks if the lower bound is lower than the upper bound
		int randomPivot= low + rand() % (high - low);					// choosing the random pivot
		int pLoc = partition(keys, low, high, locks[randomPivot]);		// finds the pivot elements and places in the correct position 
		partition(locks, low, high, keys[pLoc]);						// locks is divided according to the keys and places locks in the appropriate position relative to the keys
        quickSort(keys, locks, low, pLoc - 1);							// sorts the elements to the left of the pivot
        quickSort(keys, locks, pLoc + 1, high);							// sorts the elements to the right of the pivot
    }
}


/* 
	@brief performs partitioning in an array according to a given pivot value
	@param arr -> the given array
	@param low -> start of the array
	@param high -> end of the array
	@param pivot -> element indicating the point at which the array will be divide
	@return the new boundary point for the divided array
*/
//HOARE PARTITION
int partition(int* arr, int low, int high, int pivot){
	int i=low;															// sets 'i' to the lower bound.
	int j=high;															// sets 'j' to the upper bound
	
	while(i<=j){														// while 'i' is less than or equal to 'j', continue the loop
		while((j>=0) && (arr[j]>=pivot)){								// the loop starts until the value at index j of the array is equal to the pivot and j is greater than zero
			if(arr[j]==pivot){											// if the value at index j equal to pivot it swaps [j] and [high] (pivot comes last)
				
				swap(&arr[j], &arr[high]);	
							
			}
			j--;
		}
		while(arr[i]<pivot){											// it is done to ensure that the elements smaller than the pivot value are in the correct position
			i++;
		}
	
		swap(&arr[i], &arr[j]);											// swaps two value to correct position
		
	}
	
	swap(&arr[i], &arr[j]);												// loop is done, pivot is moved to the correct location.
	
	
	swap(&arr[i], &arr[high]);											// pivot is moved to the last place. 
	
	return i;															// the index where the dividing process will take place
}

/* 
	@brief performs partitioning in an array according to a given pivot value
	@param arr -> the given array
	@param low -> start of the array
	@param high -> end of the array
	@param pivot -> element indicating the point at which the array will be divide
	@return the new boundary point for the divided array
*/
//int partition(int* arr, int low, int high, int pivot) {
//    int i = low ;													// sets 'i' to the lower bound
//    int j = high;													// sets 'j' to the upper bound
//	
//    while(i<j) {													// starts the loop, breaks the loop if the 'i' value is equal to or greater than the 'j' value 
//		while(arr[i] < pivot){										// increments i as long as the array element is less than the pivot value
//			i++;
//		}
//        while (arr[j] > pivot){										// decrements j as long as the array element is greater than the pivot value
//        	j--;
//		}
//        swap(&arr[i], &arr[j]);										// swaps the elements at positions 'i' and 'j'
//    }
//    return j;
//}



/* 
	@brief performs partitioning in an array according to a given pivot value
	@param arr -> the given array
	@param low -> start of the array
	@param high -> end of the array
	@param pivot -> element indicating the point at which the array will be divide
	@return the new boundary point for the divided array
*/
//LOMUTO PARTITION
//int partition(int* arr, int low, int high, int pivot) {
//  	int i=low-1;														// sets 'i' to one less than the lower bound.
//	int j;																// sets 'j' to the upper bound
//    for (j=low; j<=high-1; j++) {
//        if (arr[j] < pivot) {											// moves the elements smaller than the pivot value to the correct position
//            i++;
//            swap(&arr[i], &arr[j]);
//        } else if (arr[j]==pivot) {
//            swap(&arr[j], &arr[high]);									// elements equal to the pivot value are kept last.
//            j--;
//        }
//    }
//    swap(&arr[i + 1], &arr[high]);										// moves the pivot value to the correct position
//    return i + 1;														// specifies the location where dividing occurs
//}

/* 
	@brief a function that prints the given array
	@param arr -> the array that to be printed
	@param n -> array length
	@return 
*/

void printArray(int* arr, int n){ 
	int i;
    for ( i=0; i<n; i++) 
        printf("%d ", arr[i]); 
    printf("\n"); 
} 

