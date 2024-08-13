#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
void insertIntoTable(HashTable *hashTable, char *destination, int weight, float valuation);
Parcel *InsertElementIntoBST(Parcel *root, Parcel *newParcel);
void printParcelBST(Parcel *root);
void printCountryParcelDetails(HashTable *hashTable, char *destination);
void calculateTotalLoadAndValuation(Parcel *root, int *totalWeight, float *totalValuation);
Parcel *findCheapestParcel(Parcel *root);
Parcel *findMostExpensiveParcel(Parcel *root);

int main(void)
{
    FILE *fp = NULL;
    char textLine[100] = "";
    char destination[20] = "";
    int weight = 0;
    float valuation = 0.0;

    HashTable *hashTable = initializeHashTable();

    fp = fopen("couriers.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening file\n");
        exit(FAILURE);
    }

    while (fgets(textLine, sizeof(textLine), fp) != NULL)
    {
        int parsed = sscanf(textLine, "%s %d %f", destination, &weight, &valuation);

        if (parsed != 3)
        {
            printf("Error parsing line: %s\n", textLine);
            continue;
        }

        insertIntoTable(hashTable, destination, weight, valuation);
    }
    fclose(fp);

    while (1)
    {
        int choice = 0;

        printf("Parcel Management Menu:\n");
        printf("1. Enter country name and display all the parcel details\n");
        printf("2. Enter country and weight pair\n");
        printf("3. Display the total parcel load and valuation for the country\n");
        printf("4. Enter the country name and display cheapest and most expensive parcel’s details\n");
        printf("5. Enter the country name and display lightest and heaviest parcel for the country\n");
        printf("6. Exit the application\n");
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

            printf("Parcel details for %s:\n", countryName);
            printCountryParcelDetails(hashTable, countryName);
            break;
        }
        case 2:
            printf("You selected option 2: Enter country and weight pair.\n");
            printf("Please enter the country name and weight to filter parcels.\n");
            printf("You can specify if you want parcels with weight higher or lower than the given weight.\n");
            break;
        case 3:
        {
            {
                char countryName[20] = "";
                int totalWeight = 0;
                float totalValuation = 0.0;

                printf("You selected option 3: Display the total parcel load and valuation for the country.\n");
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
            printf("You selected option 4: Enter the country name and display cheapest and most expensive parcel’s details.\n");
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
                printf("Cheapest parcel details for %s:\n", countryName);
                printf("Weight: %d\n", cheapest->weight);
                printf("Valuation: %.2f\n", cheapest->valuation);
            }
            else
            {
                printf("No parcels found for %s\n", countryName);
            }

            if (mostExpensive != NULL)
            {
                printf("Most expensive parcel details for %s:\n", countryName);
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
            printf("You selected option 5: Enter the country name and display lightest and heaviest parcel for the country.\n");
            printf("Please enter the country name to find and display the lightest and heaviest parcel details.\n");
            break;
        case 6:
            printf("You selected option 6: Exit the application.\n");
            printf("Exiting...\n");
            return 0;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

int GenerateHash(char *key)
{
    int hash = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        hash = (hash + key[i]) % TABLE_SIZE;
    }
    return hash;
}

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
        root->leftChild = InsertElementIntoBST(root->leftChild, newParcel);
    }
    else
    {
        root->rightChild = InsertElementIntoBST(root->rightChild, newParcel);
    }
    return root;
}

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
