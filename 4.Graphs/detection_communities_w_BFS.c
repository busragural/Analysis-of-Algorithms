#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#define MAX_NODES 100
#include <limits.h>

void findCommunities(int**, int**, int*, int, int, int);
int** findEdgeCount(int**, int, int, int*, int**) ;
void printMatrix(int**, int);
int findMaxEdge(int, int**, int*, int*);
int** resetEdgeCounts(int**, int);
int **roadPassed(int **, int);

int main() {
    int n; 													// number of nodes
    int round,minMemCount; 									// round -> number of rounds  // minMemCount-> minimum number of members
	int i,j;
    int *distances = (int*)malloc(MAX_NODES*sizeof(int));   // the distance between two nodes
    
	printf("Enter the number of nodes: ");
    scanf("%d", &n);
    
    int **graph =  (int**)malloc(n*sizeof(int*));			// the given matrix for the graph
    for(i=0; i<n; i++){
    	graph[i] = (int*)malloc(n*sizeof(int));
	}
	
	int **edgeCounts = (int**)calloc(n,sizeof(int*));		// the matrix consist of the value of edge betweenness
    for(i=0; i<n; i++){
    	edgeCounts[i] = (int*)calloc(n,sizeof(int));
	}
    
    FILE *file = fopen("graf3.txt", "r"); 					// opens file

    if (file == NULL) {
        printf("Error.\n");
        return 1;
    }

    for (i = 0; i < n; ++i) {								// reads file
        for (j = 0; j < n; ++j) {
            fscanf(file, "%d", &graph[i][j]);
        }
    }

    fclose(file);											// closes file
    
    printf("\nFor how many rounds should communities be terminated when they remain the same? Enter: ");
    scanf("%d", &round);
    printf("\nWhat is the minimum number of members in communities? Enter: ");
    scanf("%d", &minMemCount);
    
    findCommunities(graph, edgeCounts, distances, n, round, minMemCount);
    
    for (i = 0; i < n; i++) {
	    free(graph[i]);
	}
	free(graph);
	
	for (i = 0; i < n; i++) {
	    free(edgeCounts[i]);
	}
	free(edgeCounts);
    free(distances);
    return 0;
}

/* 
    @brief Function to print the elements of a matrix.
    @param matrix -> the matrix to be printed
    @param n -> the size of the matrix (number of rows and columns are equal)
*/
void printMatrix(int** matrix, int n) {
	int i,j;
	printf("\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }
}

/* 
    @brief Function to find the maximum edge count in a array.
    @param n -> the size of the array 
    @param edgeCounts -> array to store the value of edges traversed between nodes
    @param maxI -> pointer to store the row index of the maximum edge count
    @param maxJ -> pointer to store the column index of the maximum edge count
    @return The maximum edge count found in the array.
*/
int findMaxEdge(int n, int **edgeCounts, int *maxI, int *maxJ) {
    int i, j;
    int max = 0;

    for (i = 0; i<n; ++i) {
        for (j = i+1; j<n; ++j) {
            if (edgeCounts[i][j] > max) {
                max = edgeCounts[i][j];
                *maxI = i;
                *maxJ = j;
            }
        }
    }
    return max;
}

/* 
    @brief Function to reset all edge counts in a array to zero.
    @param edgeCounts -> thearray containing edge counts between vertices
    @param n -> the size of the array (number of rows and columns are equal)
    @return A pointer to the modified array with all edge counts reset to zero.
*/
int** resetEdgeCounts(int **edgeCounts, int n){
	int i,j;
	for(i=0; i<n; i++){
		for(j=0; j<n; j++){
			edgeCounts[i][j]=0;
		}
	}	
	return edgeCounts;	
}

/* 
    @brief Performs a breadth-first search (BFS) on a graph starting from a given node.
    @param graph -> the adjacency matrix representing the graph
    @param start -> the starting node for BFS
    @param n -> the number of nodes in the graph
    @param distances -> array to store the shortest distances from the starting node to each node
    @param edgeCounts -> array to store the value of edges traversed between nodes
    @return A pointer to the modified array after performing BFS.
*/
int** findEdgeCount(int **graph, int start, int n, int *distances, int **edgeCounts) {
	
	int i,j,current;
    bool* visited = (bool*)malloc(n * sizeof(bool)); 						// Used to avoid visiting the same node again
    for (i = 0; i < n; i++) {												
        visited[i] = false;
    }
  													
    int *parent = (int*)malloc(MAX_NODES * sizeof(int));					// Records where a node was reached.
	for (i = 0; i < MAX_NODES; i++) {
	    parent[i] = -1;
	}
	
    visited[start] = true;													// The starting node was reached.
    parent[start] = -1; 													// There is no parent for the starting node.
    distances[start] = 0;													// The distance from the starting node to itself is zero.
    												
    int *queue = (int*)malloc(n*sizeof(int));								// the data structure for the bfs algorithm
	int front = -1, rear = -1;												// first, the queue is empty.

    queue[++rear] = start;													// Adds the starting node to the end of the queue

    while (front != rear) {
    	
        current = queue[++front];											// Removes a node from the queue

        for (i=0; i<n; ++i) {												// Controls the neighbour nodes
            if (graph[current][i] == 1 && !visited[i]) {					// If there is a neighbor -> 1 && if not visited
                visited[i] = true;											// The node was reached.
                //distances[i] = distances[current] + 1;						// Calculates the distances
                queue[++rear] = i;											// Adds the node to the end of the queue
                parent[i] = current;										// Indicates which node reached to the current neighbor.
					
				int temp = i;
                while (parent[temp] != -1) {								// The value of each route taken until reaching the relevant neighbor is increased.
                	if(parent[temp] <= temp){								// Condition for the 0-1 and 1-0 (it is the same)
                		edgeCounts[parent[temp]][temp]++;
					}
					else{
						edgeCounts[temp][parent[temp]]++;
					}
                    
                    temp = parent[temp];
                }       
            }
        }
    }

    for (i=0; i<n; ++i) {
    	int temp = i;
    	//printf("Shortest distance [%d->%d] = %d >>>Nodes passed: ", start, i, distances[i]);
        printf("[%d->%d] >>> Nodes passed: ", start, i);
		while (temp != -1) {
            printf("%d ", temp);
            temp = parent[temp];
        }
        printf("\n");
    }
    printf("\n");
    
    return edgeCounts;
}

/* 
    @brief Calculates and prints the number of times each road was passed, based on edgeCounts.
    @param edgeCounts -> array containing value of edges between nodes
    @param n -> the number of nodes (size of the edgeCounts array)
    @return A pointer to the modified array after updating the counts.
*/
int **roadPassed(int **edgeCounts, int n){
	int i,j;
	for (i=0; i<n; ++i) {
	    for (j=i+1; j<n; ++j) {
	        if(i<=j){
	        	if(edgeCounts[i][j] !=0){
	        		edgeCounts[i][j] /=2;
	        		printf("The road [%d-%d] was passed %d times.\n", i, j, edgeCounts[i][j]);
				}	
			}
	    }
	}
	return edgeCounts;
}

/* 
    @brief Finds communities in a graph based on edge counts.
    @param graph -> the adjacency matrix representing the graph
    @param edgeCounts -> array to store the value of edges traversed between nodes
    @param distances -> array to store the shortest distances from nodes to each other
    @param n -> the number of nodes in the graph
    @param round -> the number of consecutive rounds with the same number of communities for termination
    @param t -> minimum number of members required in each community for termination
*/
void findCommunities(int **graph, int **edgeCounts, int *distances, int n, int round, int t){
	
	int i,j;
	int newCommCount, oldCommCount = -1;									// The number of communities (two separate variables for the stopping condition)
	int maxEdge;															// Most traveled road
	int maxI, maxJ;															// Nodes at both ends of the most traveled road
	
	int sameRounds = 0;														// How many rounds have communities remained the same?
	
	int *queue = (int*)malloc(n*sizeof(int));
	int front, rear, current;
	bool* visited = (bool*)malloc(n * sizeof(bool)); 						// Used to avoid visiting the same node again
	
	int currMembers;
	int *currCommunity = (int*)malloc(n*sizeof(int));						// Current community whose members are calculated
	
	int *commMembers = (int*)malloc(n*sizeof(int));							// Keeps the number of members in all communities.
	
	int **comms = (int**)calloc(n,sizeof(int*));							// Keeps all communities and its members
	for(i=0; i<n; i++){
        comms[i] =  (int*)calloc(n, sizeof(int));
    }
	
	while(true){
		newCommCount = 0;
		
	    for(j =0; j<n; j++){												// Calculates the shortest paths to other nodes for each node
    		edgeCounts = findEdgeCount(graph, j, n, distances,edgeCounts);
		}	
		
	    edgeCounts = roadPassed(edgeCounts,n);								// Calculates which roads have been passed and how many times.
	    
	    findMaxEdge(n, edgeCounts, &maxI, &maxJ);							// Finds the most traveled path.
	    printf("Path [%d-%d] is deleted\n", maxI, maxJ);							
		
		graph[maxI][maxJ] = 0;												// Deleted the most traveled path.
    	graph[maxJ][maxI] = 0;												// Deleted the most traveled path.
		
		printf("\nGraph:\n");
		printMatrix(graph, n);
		printf("\nEdges:\n");
		printMatrix(edgeCounts, n);
		
		edgeCounts = resetEdgeCounts(edgeCounts, n);						// The values of the paths are reset.
		
	    for (i = 0; i < n; i++) {												
	        visited[i] = false;
	    }								
	
		for(i=0; i<n; i++){													// All nodes are selected as starting node respectively
			if(!visited[i]){												// If not visited before, queue operation will set.		
				front = -1;													
				rear = -1;
				visited[i] = true;
				queue[++rear] = i;
				currMembers = 0;											// Number of members in the community
				
				while (front != rear) {					
			        current = queue[++front];
					currCommunity[currMembers] = current;					// The current node is added to the current community.
			    	currMembers++;
			    	
			        for (j=0; j<n; ++j) {									// It goes from the starting point to the last possible place.
			            if (graph[current][j]==1 && !visited[j]) { 
			                visited[j] = true;
			                queue[++rear] = j;       
			            }
			        }
		    	}

		    	commMembers[newCommCount]=currMembers;						// Adds the calculated number of members to the array containing the number of members in all communities
				for(j=0; j<currMembers; j++){								// Adds the members of the community according to the number of members to the matrix... 
					comms[newCommCount][j]=currCommunity[j];				// ...where all members of the community are kept
        		}
        		
        		newCommCount++;	
			}
			
		}

        if(newCommCount==oldCommCount){										// If the number of communities has not changed
			sameRounds++;
            if(sameRounds==round){												// The number of rounds remained the same for round consecutive rounds.
                
				printf("\nThe algorithm is terminated because the number of communities remains the same for %d consecutive rounds.", round);
		        printf("\nNumber of Communities: %d", newCommCount);
		        for(i=0; i<newCommCount; ++i){
					printf("\nCommunity Number -> %d", i);
					printf("\nMembers -> ");
		            for(j=0; j < commMembers[i]; ++j){
						printf("%d ", comms[i][j]);
		            }
		            printf("\n");
		        }				
                return;
            }
        }
		else{
			sameRounds = 0;
        }

        for(i=0; i<newCommCount; i++){									 	// The number of members of each community is controlled.
			if(commMembers[i]<= t){
				printf("\nThe algorithm is terminated because the number of members in one of the communities is at least %d.", t);
		        printf("\nNumber of Communities: %d", newCommCount);
		        for(i=0; i<newCommCount; ++i){
					printf("\nCommunity Number -> %d", i);
					printf("\nMembers -> ");
		            for(j=0; j<commMembers[i]; ++j){
						printf("%d ", comms[i][j]);
		            }
		            printf("\n");
		        }
                return;
            }
        }
        oldCommCount = newCommCount;
	}
}

