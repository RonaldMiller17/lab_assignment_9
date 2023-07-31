#include <stdio.h>
#include <stdlib.h>


struct RecordType
{
	int id;
	char name;
	int order; 
};

struct HashType
{
	struct RecordType* data;
	struct HashType* next;
};

// Compute the hash function
int hash(int x, int size)
{
	return x % size;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

void insertRecord(struct HashType** pHashArray, struct RecordType* record, int size)
{
	struct HashType* newNode = malloc(sizeof(struct HashType));
    newNode -> data = record;
    newNode -> next = NULL;

    int hashKey = hash(record -> id, size);
	// printf("has h key %d", record.id % size);

    if(pHashArray[hashKey] == NULL) {
        pHashArray[hashKey] = newNode;
	}
	else
    {
        struct HashType *temp = pHashArray[hashKey];
        while(temp -> next != NULL)
        {
            temp = temp -> next;
        }

        temp -> next = newNode;
    }
}


// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType **pHashArray, int hashSz)
{
	for (int i = 0; i < hashSz; ++i)
	{
		struct HashType* chain = pHashArray[i];
        while (chain != NULL)
		{
			printf("Index %d -> %d, %c, %d\n", i, chain -> data -> id, chain -> data -> name, chain -> data -> order);
			chain = chain -> next;
		}
	}
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);

	// Your hash implementation
	int hashSz = recordSz * 2;
	struct HashType **hashArray = malloc(sizeof(struct HashType) * hashSz);

	for (int i = 0; i < hashSz; i++)
    {
        hashArray[i] = NULL;
    }

	for (int i = 0; i < recordSz; ++i)
	{
		insertRecord(hashArray, &pRecords[i], hashSz);
	}

	displayRecordsInHash(hashArray, hashSz);

	free(pRecords);
	for (int i = 0; i < hashSz; i++)
    {
        struct HashType* temp = hashArray[i];
        while (temp != NULL)
        {
            struct HashType* next = temp->next;
            free(temp);
            temp = next;
        }
    }
	free(hashArray);

	return 0;
}