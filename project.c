/*
 * FILE : project.cpp
 * PROJECT : SENG1000 - Project
 * PROGRAMMER : Fawaz Dogbe(8982570)
 * FIRST VERSION : 2024 - 08 - 12
 * DESCRIPTION :
 *Using tree data structure to manage various product and display output based on
 *specific requirements
 * githubURL: https://github.com/huntdavid175/Tree-DSA-Project
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 127
#define FAILURE 1
#define TEXTLINE 100
#define DESTINATION_SIZE 20
#define WEIGHT_INIT 0
#define VALUATION_INIT 0.0

#pragma warning(disable : 4996)

// Parcel struct
typedef struct Parcel
{
    char *destination;
    int weight;
    float valuation;
    struct Parcel *leftChild;
    struct Parcel *rightChild;
} Parcel;

// HashTable struct
typedef struct HashTable
{
    Parcel *table[TABLE_SIZE];
} HashTable;

// function prototypes
int GenerateHash(char *key);
Parcel *initializeParcelNode(char *destination, int weight, float valuation);
HashTable *initializeHashTable(void);
void insertIntoTable(HashTable *hashTable, char *destination, int weight, float valuation);
Parcel *insertParcelIntoBST(Parcel *root, Parcel *newParcel);
void printParcelBST(Parcel *root);
void printCountryParcelDetails(HashTable *hashTable, char *destination);
void calculateTotalLoadAndValuation(Parcel *root, int *totalWeight, float *totalValuation);
Parcel *findCheapestParcel(Parcel *root);
Parcel *findMostExpensiveParcel(Parcel *root);
Parcel *findLightestParcel(Parcel *root);
Parcel *findHeaviestParcel(Parcel *root);

void displayParcelsByLowOrHighWeight(Parcel *root, int weight, char *filterType);

int main(void)
{
    FILE *fp = NULL;
    char textLine[TEXTLINE] = "";
    char destination[DESTINATION_SIZE] = "";
    int weight = WEIGHT_INIT;
    float valuation = VALUATION_INIT;

    HashTable *hashTable = initializeHashTable();

    fp = fopen("couriers.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening file\n");
        exit(FAILURE);
    }

    while (fgets(textLine, sizeof(textLine), fp) != NULL)
    {
        // Remove newline character if present
        textLine[strcspn(textLine, "\n")] = '\0';

        // Split the line by commas
        char *token = strtok(textLine, ",");
        if (token == NULL)
            continue;
        strcpy(destination, token);

        token = strtok(NULL, ",");
        if (token == NULL)
            continue;
        weight = atoi(token);

        token = strtok(NULL, ",");
        if (token == NULL)
            continue;
        valuation = atof(token);

        insertIntoTable(hashTable, destination, weight, valuation);
    }
    fclose(fp);

    while (1)
    {
        int choice = 0;

        printf("\n\nParcel Management Menu:\n");
        printf("1. Enter country name and display all the parcel details\n");
        printf("2. Enter country and weight pair\n");
        printf("3. Display the total parcel load and valuation for the country\n");
        printf("4. Enter the country name and display cheapest and most expensive parcel’s details\n");
        printf("5. Enter the country name and display lightest and heaviest parcel for the country\n");
        printf("6. Exit the application\n\n");
        printf("Enter your choice: ");

        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
        case 1:
        {
            char countryName[20] = "";
            printf("\nYou selected option 1: Enter country name and display all the parcel details.\n");
            printf("Please enter the country name to display all parcel details: ");

            fgets(countryName, sizeof(countryName), stdin);
            countryName[strcspn(countryName, "\n")] = '\0';

            printf("\nParcel details for %s:\n", countryName);
            printCountryParcelDetails(hashTable, countryName);
            break;
        }
        case 2:
        {
            char countryName[20] = "";
            int filterWeight = 0;
            char choice[10] = "";

            printf("\nYou selected option 2: Enter country and weight pair.\n");
            printf("Please enter the country name: ");

            fgets(countryName, sizeof(countryName), stdin);
            countryName[strcspn(countryName, "\n")] = '\0';

            printf("\nEnter the weight to filter parcels: ");
            scanf("%d", &filterWeight);
            getchar(); // clear the newline left by scanf

            printf("Do you want to see parcels with weight 'higher' or 'lower' than %d? ", filterWeight);
            fgets(choice, sizeof(choice), stdin);
            choice[strcspn(choice, "\n")] = '\0';

            int hash = GenerateHash(countryName);
            Parcel *root = hashTable->table[hash];
            if (root == NULL)
            {
                printf("Country %s has no parcels\n", countryName);
                break;
            }

            printf("\nParcels with weight %s than %d for %s:\n", choice, filterWeight, countryName);
            displayParcelsByLowOrHighWeight(root, filterWeight, choice);
            break;
        }
        case 3:
        {
            {
                char countryName[20] = "";
                int totalWeight = 0;
                float totalValuation = 0.0;

                printf("\nYou selected option 3: Display the total parcel load and valuation for the country.\n");
                printf("Please enter the country name: ");

                fgets(countryName, sizeof(countryName), stdin);
                countryName[strcspn(countryName, "\n")] = '\0';

                int hash = GenerateHash(countryName);
                Parcel *current = hashTable->table[hash];
                if (current == NULL)
                {
                    printf("Country %s has no parcels\n", countryName);
                    break;
                }

                calculateTotalLoadAndValuation(current, &totalWeight, &totalValuation);
                printf("\nTotal load for %s: %d\n", countryName, totalWeight);
                printf("Total valuation for %s: %.2f\n", countryName, totalValuation);
                break;
            }
        }
        case 4:
        {
            char countryName[20] = "";
            printf("\nYou selected option 4: Enter the country name and display cheapest and most expensive parcel’s details.\n");
            printf("Please enter the country name: ");

            fgets(countryName, sizeof(countryName), stdin);
            countryName[strcspn(countryName, "\n")] = '\0';

            int hash = GenerateHash(countryName);
            Parcel *current = hashTable->table[hash];
            if (current == NULL)
            {
                printf("Country %s has no parcels\n", countryName);
                break;
            }

            Parcel *cheapest = findCheapestParcel(current);
            Parcel *mostExpensive = findMostExpensiveParcel(current);

            if (cheapest != NULL)
            {
                printf("\nCheapest parcel details for %s:\n", countryName);
                printf("Weight: %d\n", cheapest->weight);
                printf("Valuation: %.2f\n", cheapest->valuation);
            }
            else
            {
                printf("No parcels found for %s\n", countryName);
            }

            if (mostExpensive != NULL)
            {
                printf("\nMost expensive parcel details for %s:\n", countryName);
                printf("Weight: %d\n", mostExpensive->weight);
                printf("Valuation: %.2f\n", mostExpensive->valuation);
            }
            else
            {
                printf("No parcels found for %s\n", countryName);
            }
            break;
        }
        case 5:
        {
            char countryName[20] = "";
            printf("\nYou selected option 5: Enter the country name and display lightest and heaviest parcel for the country.\n");
            printf("Please enter the country name: ");

            fgets(countryName, sizeof(countryName), stdin);
            countryName[strcspn(countryName, "\n")] = '\0';

            int hash = GenerateHash(countryName);
            Parcel *current = hashTable->table[hash];
            if (current == NULL)
            {
                printf("Country %s has no parcels\n", countryName);
                break;
            }

            Parcel *lightest = findLightestParcel(current);
            Parcel *heaviest = findHeaviestParcel(current);

            if (lightest != NULL)
            {
                printf("\nLightest parcel details for %s:\n", countryName);
                printf("Weight: %d\n", lightest->weight);
                printf("Valuation: %.2f\n", lightest->valuation);
            }
            else
            {
                printf("No parcels found for %s\n", countryName);
            }

            if (heaviest != NULL)
            {
                printf("\nHeaviest parcel details for %s:\n", countryName);
                printf("Weight: %d\n", heaviest->weight);
                printf("Valuation: %.2f\n", heaviest->valuation);
            }
            else
            {
                printf("No parcels found for %s\n", countryName);
            }
            break;
        }
        case 6:
            printf("\nYou selected option 6: Exit the application.\n");
            printf("Exiting...\n");
            return 0;
        default:
            printf("\nInvalid choice. Please try again.\n");
        }
    }

    return 0;
}

//
// FUNCTION : GenerateHash
// DESCRIPTION :
// This Function takes a string and hashes it and returns the hash
// PARAMETERS :
// char* key : a pointer to string
// RETURNS :
// int : the hashed value
//
int GenerateHash(char *key)
{
    int hash = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        hash = (hash + key[i]) % TABLE_SIZE;
    }
    return hash;
}

//
// FUNCTION : initializeParcelNode
// DESCRIPTION :
// This Function creates a new node
// PARAMETERS :
// char* destination : a pointer to destination string
// int weight :an int number representing the weight
// float valuation : valuation as a floating point number
// RETURNS :
// Parcel* : the new node
//
Parcel *initializeParcelNode(char *destination, int weight, float valuation)
{
    Parcel *node = (Parcel *)malloc(sizeof(Parcel));
    if (node == NULL)
    {
        printf("Error allocating memory.\n");
        exit(1);
    }

    node->destination = (char *)malloc(strlen(destination) + 1);
    if (node->destination == NULL)
    {
        printf("Error allocating memory.\n");
        exit(1);
    }
    strcpy(node->destination, destination); // Copy the string into the allocated memory
    node->weight = weight;
    node->valuation = valuation;
    node->leftChild = NULL;
    node->rightChild = NULL;
    return node;
}

//
// FUNCTION : initializeHashTable
// DESCRIPTION :
// This Function initializes the hash table
// PARAMETERS :
// void
// RETURNS :
// HashTable* : the initialized hashtable
//
HashTable *initializeHashTable(void)
{
    HashTable *hashTable = (HashTable *)malloc(sizeof(HashTable));
    if (hashTable == NULL)
    {
        printf("Error allocating memory.\n");
        exit(FAILURE);
    }

    for (int i = 0; i < TABLE_SIZE; i++)
    {
        hashTable->table[i] = NULL;
    }
    return hashTable;
}

//
// FUNCTION : insertIntoTable
// DESCRIPTION :
// This Function inserts into the hash table
// PARAMETERS :
// HashTable* hashTable : a pointer to the hash table
//// char* destination : a pointer to destination string
// int weight :an int number representing the weight
// float valuation : valuation as a floating point number
// RETURNS :
// void
//
void insertIntoTable(HashTable *hashTable, char *destination, int weight, float valuation)
{
    int hash = GenerateHash(destination);
    // printf("Inserting into hash %d: %s\n", hash, destination); // Debug print

    Parcel *newParcel = initializeParcelNode(destination, weight, valuation);

    if (hashTable->table[hash] == NULL)
    {
        hashTable->table[hash] = newParcel;
        return;
    }

    Parcel *current = hashTable->table[hash];
    hashTable->table[hash] = insertParcelIntoBST(current, newParcel);
}

//
// FUNCTION : insertParcelIntoBST
// DESCRIPTION :
// This Function inserts into the BST
// PARAMETERS :
// Parcel* root : a pointer to root of the bst
// RETURNS :
// Parcel* : the root of the bst
//
Parcel *insertParcelIntoBST(Parcel *root, Parcel *newParcel)
{
    if (root == NULL)
    {
        return newParcel;
    }
    if (newParcel->weight < root->weight)
    {
        root->leftChild = insertParcelIntoBST(root->leftChild, newParcel);
    }
    else
    {
        root->rightChild = insertParcelIntoBST(root->rightChild, newParcel);
    }
    return root;
}

//
// FUNCTION : printParcelBST
// DESCRIPTION :
// This Function prints the BST
// PARAMETERS :
// Parcel* root : a pointer to the parcel root bst
// RETURNS :
// void
//
void printParcelBST(Parcel *root)
{
    if (root == NULL)
        return;

    printf("Country: %s\n", root->destination);
    printf("Weight: %d\n", root->weight);
    printf("Valuation: %.2f\n\n", root->valuation);

    printParcelBST(root->leftChild);
    printParcelBST(root->rightChild);
}

//
// FUNCTION : printCountryParcelDetails
// DESCRIPTION :
// This Function prints the BST at a specific bucket
// PARAMETERS :
// HashTable* hashTable : a pointer to the hash table;
// char* destination : a pointer to destination string
// RETURNS :
// void
//
void printCountryParcelDetails(HashTable *hashTable, char *destination)
{
    int hash = GenerateHash(destination);
    // printf("Searching hash %d for country %s\n", hash, destination); // Debug print

    Parcel *current = hashTable->table[hash];
    if (current == NULL)
    {
        printf("Country %s has no parcels\n", destination);
        return;
    }

    printParcelBST(current);
}

//
// FUNCTION : calculateTotalLoadAndValuation
// DESCRIPTION :
// This Function calculates the total load and valuation
// PARAMETERS :
// Parcel* root : a pointer to the root of the BST
// int* totalWeight : a pointer to the total weight
// float* totalValuation : a pointer to the total valuation
// RETURNS :
// void
//
void calculateTotalLoadAndValuation(Parcel *root, int *totalWeight, float *totalValuation)
{
    if (root == NULL)
        return;

    // Accumulate weight and valuation for the current node
    *totalWeight += root->weight;
    *totalValuation += root->valuation;

    // Traverse left and right subtrees
    calculateTotalLoadAndValuation(root->leftChild, totalWeight, totalValuation);
    calculateTotalLoadAndValuation(root->rightChild, totalWeight, totalValuation);
}

//
// FUNCTION : findCheapestParcel
// DESCRIPTION :
// This Function finds the cheapest parcel
// PARAMETERS :
// Parcel* root : a pointer to root of the bst
// RETURNS :
// Parcel* : the item that matches the condition
//
Parcel *findCheapestParcel(Parcel *root)
{
    if (root == NULL)
        return NULL;

    Parcel *cheapest = root;

    Parcel *leftCheapest = findCheapestParcel(root->leftChild);
    if (leftCheapest != NULL && leftCheapest->valuation < cheapest->valuation)
    {
        cheapest = leftCheapest;
    }

    Parcel *rightCheapest = findCheapestParcel(root->rightChild);
    if (rightCheapest != NULL && rightCheapest->valuation < cheapest->valuation)
    {
        cheapest = rightCheapest;
    }

    return cheapest;
}

//
// FUNCTION : findMostExpensiveParcel
// DESCRIPTION :
// This Function finds the most expensive parcel
// PARAMETERS :
// Parcel* root : a pointer to root of the bst
// RETURNS :
// Parcel* : the item that matches the condition
//
Parcel *findMostExpensiveParcel(Parcel *root)
{
    if (root == NULL)
        return NULL;

    Parcel *mostExpensive = root;

    Parcel *leftMostExpensive = findMostExpensiveParcel(root->leftChild);
    if (leftMostExpensive != NULL && leftMostExpensive->valuation > mostExpensive->valuation)
    {
        mostExpensive = leftMostExpensive;
    }

    Parcel *rightMostExpensive = findMostExpensiveParcel(root->rightChild);
    if (rightMostExpensive != NULL && rightMostExpensive->valuation > mostExpensive->valuation)
    {
        mostExpensive = rightMostExpensive;
    }

    return mostExpensive;
}

//
// FUNCTION : findLightestParcel
// DESCRIPTION :
// This Function finds the most lightest parcel
// PARAMETERS :
// Parcel* root : a pointer to root of the bst
// RETURNS :
// Parcel* : the item that matches the condition
//
Parcel *findLightestParcel(Parcel *root)
{
    if (root == NULL)
        return NULL;

    Parcel *lightest = root;

    Parcel *leftLightest = findLightestParcel(root->leftChild);
    if (leftLightest != NULL && leftLightest->weight < lightest->weight)
    {
        lightest = leftLightest;
    }

    Parcel *rightLightest = findLightestParcel(root->rightChild);
    if (rightLightest != NULL && rightLightest->weight < lightest->weight)
    {
        lightest = rightLightest;
    }

    return lightest;
}

//
// FUNCTION : findHeaviestParcel
// DESCRIPTION :
// This Function finds the most heaviest parcel
// PARAMETERS :
// Parcel* root : a pointer to root of the bst
// RETURNS :
// Parcel* : the item that matches the condition
//
Parcel *findHeaviestParcel(Parcel *root)
{
    if (root == NULL)
        return NULL;

    Parcel *heaviest = root;

    Parcel *leftHeaviest = findHeaviestParcel(root->leftChild);
    if (leftHeaviest != NULL && leftHeaviest->weight > heaviest->weight)
    {
        heaviest = leftHeaviest;
    }

    Parcel *rightHeaviest = findHeaviestParcel(root->rightChild);
    if (rightHeaviest != NULL && rightHeaviest->weight > heaviest->weight)
    {
        heaviest = rightHeaviest;
    }

    return heaviest;
}

//
// FUNCTION : displayParcelsByLowOrHighWeight
// DESCRIPTION :
// This Function prints elements that are higher or lower than the given weight
// PARAMETERS :
// Parcel* root : a pointer to root of the bst
// int weight : the given weight
// RETURNS :
// void
//
void displayParcelsByLowOrHighWeight(Parcel *root, int weight, char *filterType)
{
    if (root == NULL)
        return;

    displayParcelsByLowOrHighWeight(root->leftChild, weight, filterType);

    if ((strcmp(filterType, "higher") == 0 && root->weight > weight) ||
        (strcmp(filterType, "lower") == 0 && root->weight < weight))
    {
        printf("Destination: %s, Weight: %d, Valuation: %.2f\n", root->destination, root->weight, root->valuation);
    }

    displayParcelsByLowOrHighWeight(root->rightChild, weight, filterType);
}
