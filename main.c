#include <stdio.h>

#define TABLE_SIZE 127
#define FAILURE 1

typedef struct Parcel
{
    char *destination;
    int weight;
    float valuation;
    struct Parcel *leftChild;
    struct Parcel *rightChild;
} Parcel;

typedef struct HashTable
{
    Parcel *table[TABLE_SIZE];
} HashTable;

int GenerateHash(char *key);
Parcel *initializeParcelNode(char *destination, int weight, float valuation);
HashTable *initializeHashTable(void);
void *insertIntoTable(HashTable *hashTable, char *destination, int weight, float valuation);
Parcel *InsertElementIntoBST(Parcel *root, Parcel *newParcel);

int main(void)
{
    return 1;
}

int GenerateHash(char *key)
{
    int hash = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        int asciiValue = key[i];
        hash = (hash + asciiValue) % TABLE_SIZE;
    }
    return hash;
}

Parcel *initializeParcelNode(char *destination, int weight, float valuation)
{

    Parcel *node = (Parcel *)malloc(sizeof(Parcel));
    if (node == NULL)
    {
        printf("EOM");
        exit(1);
    }

    node->destination = (char *)malloc(strlen(destination) + 1);
    if (node->destination == NULL)
    {
        printf("EOM");
        exit(1);
    }
    node->destination = destination;
    node->weight = weight;
    node->valuation = valuation;
    node->leftChild = NULL;
    node->rightChild = NULL;
    return node;
}

HashTable *initializeHashTable(void)
{
    HashTable *hashTable = (HashTable *)malloc(sizeof(HashTable));
    if (hashTable == NULL)
    {
        printf("EOM");
        exit(FAILURE);
    }

    for (int i = 0; i < TABLE_SIZE; i++)
    {
        hashTable->table[i] = NULL;
    }
    return hashTable;
}

void *insertIntoTable(HashTable *hashTable, char *destination, int weight, float valuation)
{
    int hash = GenerateHash(destination);

    Parcel *newParcel = initializeParcelNode(destination, weight, valuation);

    if (hashTable->table[hash] == NULL)
    {
        hashTable->table[hash] = newParcel;
        return;
    }

    Parcel *current = hashTable->table[hash];

    hashTable->table[hash] = InsertElementIntoBST(current, newParcel);
}

Parcel *InsertElementIntoBST(Parcel *root, Parcel *newParcel)
{
    if (root == NULL)
    {
        return newParcel;
    }
    if (newParcel->weight < root->weight)
    {
        root->leftChild = insertParcel(root->leftChild, newParcel);
    }
    else
    {
        root->rightChild = insertParcel(root->rightChild, newParcel);
    }
    return root;
}
