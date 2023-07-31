#include <stdio.h>
#include <stdlib.h>


struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

struct HashType
{
    struct RecordType* data;
    int size;
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

void insertRecord(struct HashType* pHashArray, struct RecordType record)
{
    int index = hash(record.id, pHashArray -> size);

    while (pHashArray -> data[index].id != -1)
    {
        index = (index + 1) % pHashArray -> size;
    }

    // Insert the record into the hash table
    pHashArray -> data[index] = record;
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
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;

	for (i = 0; i < hashSz; ++i)
	{
		printf("Index %d -> %d, %c, %d\n", i, pHashArray -> data[i].id, pHashArray -> data[i].name, pHashArray -> data[i].order);
		// if index is occupied with any records, print all
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
    struct HashType *hashArray = malloc(sizeof(struct HashType));
    
	hashArray -> data = malloc(sizeof(struct RecordType) * hashSz);

    for (int i = 0; i < hashSz; ++i)
    {
        hashArray -> data[i].id = -1;
        hashArray -> data[i].name = ' ';
        hashArray -> data[i].order = -1;
    }

    hashArray -> size = hashSz;

    for (int i = 0; i < recordSz; ++i)
    {
        insertRecord(hashArray, pRecords[i]);
    }

    displayRecordsInHash(hashArray, hashSz);

    free(pRecords);
    free(hashArray -> data);
    free(hashArray);

    return 0;
}