/*****************************************************************************************
Prologue

UCSC Extension: Advanced C Programming

Instructor: Rajainder A. Yeldandi

Author: Kevin Geiszler

Assignment Number: 9, Queues

Topics: Queues

File Name: assignment9_KevinGeiszler.c

Date: 2/22/2017

Objective: This program scans an input file and either adds a number to the queue, or
		   removes a number from the queue based on which action the program is told to
		   do. All output is written to the output file.
		   
Comments:  "E" is used for "enqueue". It adds the specified number to the back of the
		   queue. "D" is used for dequeue. It removes the item that's at the front of the
		   queue.

*****************************************************************************************/

//Preprocessor directives
#include <stdio.h>
#include <stdlib.h>

#define MAXQUEUE 50
#define TRUE 1
#define FALSE 0
#define LINESIZE 64

typedef struct lineFeed {
	char action;
	int intInput;
} ACTIONSTRUCT;

typedef struct queue {
	ACTIONSTRUCT queueElement[MAXQUEUE];
	int front, rear;
} INPUTQUEUE;

/****************************************************************************************/

//Function prototypes
int empty(INPUTQUEUE *);
char removeFromQueue(INPUTQUEUE *);
void insert(INPUTQUEUE *, int);
void editQueue(FILE *, FILE *, INPUTQUEUE *, char []);

/****************************************************************************************/

//The main function
int main(int argc, char *argv[])
{
	INPUTQUEUE inQueue, *pInQueue;
	
	//initialize inQueue.front and inQueue.rear to -1
	inQueue.front = inQueue.rear = -1;
	
	pInQueue = &inQueue;
	FILE *pInFile, *pOutFile;
	int intTest;
	char charTest;
	char myString[LINESIZE];
	
	if (argc != 3)
	{
		printf("Error, need the following file usage: %s inputFile outputFile\n", argv[0]);
		exit(1);
	}
	
	if ((pInFile = fopen(argv[1], "r")) == NULL)
	{
		printf("Error opening input file: %s\n", argv[1]);
		exit(1);
	}
	
	if ((pOutFile = fopen(argv[2], "w")) == NULL)
	{
		printf("Error opening output file: %s\n", argv[2]);
		exit(1);
	}
	
	rewind(pInFile);
	
	editQueue(pInFile, pOutFile, pInQueue, myString);
	
	printf("\nOpen output file (%s) to see this program's output.\n\n", argv[2]);
	
	fclose(pInFile);
	fclose(pOutFile);
	
	return 0;
}

/****************************************************************************************/

//Checks if the queue is empty

int empty(INPUTQUEUE *pInQ)
{
	if (pInQ->front == pInQ->rear)
		return TRUE;
	else
		return FALSE;
}

/****************************************************************************************/

//Removes and returns the item at the front of the queue. Checks for queue underflow
//before removing anything from the queue.

char removeFromQueue(INPUTQUEUE *pInQ)
{
	///check for underflow condition
	if (empty(pInQ))
	{
		printf("queue underflow\n");
		exit(1);
	}
	//bring front back to 0 when it reaches the end of the array
	if (pInQ->front == MAXQUEUE - 1)
		pInQ->front = 0;
	else
		//each time an item is removed from the queue, front is incremented
		(pInQ->front)++;
	
	return (pInQ->queueElement[pInQ->front].intInput);
}

/****************************************************************************************/

//Places and item in the back of the queue. Checks for queue overflow before placing
//the item into the queue.

void insert(INPUTQUEUE *pInQ, int x)
{
	//bring rear back to 0 when it reaches the end of the array
	if (pInQ->rear == MAXQUEUE - 1)
		pInQ->rear = 0;
	else
		//each time an item is added to the queue, rear is incremented
		(pInQ->rear)++;
	
	//check for overflow condition
	if (pInQ->rear == pInQ->front)
	{
		printf("queue overflow\n");
		exit(1);
	}
	else
		pInQ->queueElement[pInQ->rear].intInput = x;
		
	return;
}

/****************************************************************************************/

//Edits the queue based on the actin read from the file. "E" is used for "enqueue", and
//"D" is used for "dequeue". The queue is displayed after each action, and the final queue
//is displayed at the end of the function. All output is written to a file.

void editQueue(FILE *pIn, FILE *pOut, INPUTQUEUE *pInQ, char string[])
{
	int fileInt, removedInt;
	char fileChar;
	
	int *pFront = &pInQ->front;
	int *pRear  = &pInQ->rear;
	
	fprintf(pOut, "---------------------------\n\n");

	while (fgets(string, LINESIZE, pIn) != NULL)
	{
		sscanf(string, "%c %d", &fileChar,
								  &fileInt);
								  
		if (fileChar == 'E')
		{
			insert(pInQ, fileInt);
			fprintf(pOut, "Action: Enqueue\n\n%d was inserted into the queue.\n", fileInt);
		}
		else if (fileChar == 'D')
		{
			removedInt = removeFromQueue(pInQ);
			fprintf(pOut, "Action: Dequeue\n\n%d was removed from the queue.\n", removedInt);
		}
				
		fprintf(pOut, "\nCurrent queue: ");
		
		for (int i = *pFront + 1 ; i <= *pRear; i++)
			fprintf(pOut, "%d ", pInQ->queueElement[i].intInput);
			
		fprintf(pOut, "\n\n---------------------------\n\n");
	}
	
	fprintf(pOut, "The queue process has ended.\n\nThe final queue is: ");

	for (int i = *pFront + 1; i <= *pRear; i++)
		fprintf(pOut, "%d ", pInQ->queueElement[i].intInput);
	
	fprintf(pOut, "\n\n");
}

/****************************************************************************************/