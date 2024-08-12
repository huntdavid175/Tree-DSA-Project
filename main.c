#include <stdio.h>

#define TABLE_SIZE 127

typedef struct Parcel
{
    char *destination;
    int weight;
    float value;
    struct Parcel *leftChild;
    struct Parcel *rightChild;
} Parcel;

typedef struct HashTable
{
    Parcel *table[TABLE_SIZE];
} HashTable;

int GenerateHash(char *key);
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