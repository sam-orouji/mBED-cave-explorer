// ============================================
// The HashTable class file
//
// Copyright 2022 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================


/*
Student Name: Sam Orouji
Date: 11/7/25
=======================
ECE 2035 Project 2-1:
=======================
This file provides definition for the structs and functions declared in the
header file. It also contains helper functions that are not accessible from
outside of the file.

FOR FULL CREDIT, BE SURE TO TRY MULTIPLE TEST CASES and DOCUMENT YOUR CODE.

===================================
Naming conventions in this file:
===================================
1. All struct names use camel case where the first letter is capitalized.
    e.g. "HashTable", or "HashTableEntry"

2. Variable names with a preceding underscore "_" will not be called directly.
    e.g. "_HashTable", "_HashTableEntry"

    Recall that in C, we have to type "struct" together with the name of the struct
    in order to initialize a new variable. To avoid this, in hash_table.h
    we use typedef to provide new "nicknames" for "struct _HashTable" and
    "struct _HashTableEntry". As a result, we can create new struct variables
    by just using:
        - "HashTable myNewTable;"
    or
        - "HashTableEntry myNewHashTableEntry;"
    
    The preceding underscore "_" simply provides a distinction between the names
    of the actual struct defition and the "nicknames" that we use to initialize
    new structs.
    [See Hidden Definitions section for more information.]

    ** _HashTable is for the struct, and HashTable would be the name of variable thats
    a _HashTable type

3. Functions, their local variables and arguments are named with camel case, where
    the first letter is lower-case.
    e.g. "createHashTable" is a function. One of its arguments is "numBuckets".
        It also has a local variable called "newTable".

4. The name of a struct member is divided by using underscores "_". This serves
    as a distinction between function local variables and struct members.
    e.g. "num_buckets" is a member of "HashTable".
*/


/****************************************************************************
 * Include the Public Interface
 *
 * By including the public interface at the top of the file, the compiler can
 * enforce that the function declarations in the the header are not in
 * conflict with the definitions in the file. This is not a guarantee of
 * correctness, but it is better than nothing!
 * 
 * this means these function definitions match the functions talked about in the header
 * hash_table.h file
 ***************************************************************************/
#include "hash_table.h"

/****************************************************************************
 * Include other private dependencies
 *
 * These other modules are used in the implementation of the hash table module,
 * but are not required by users of the hash table.
 ***************************************************************************/
#include <stdlib.h> // For malloc and free
#include <stdio.h>  // For printf

/****************************************************************************
 * Hidden Definitions
 *
 * These definitions are not available outside of this file. However, because
 * the are forward declared in hash_table.h, the type names are
 * available everywhere and user code can hold pointers to these structs.
 * 
 * what we are defining
 ***************************************************************************/
/**
 * This structure represents a hash table.
 * Use "HashTable" instead when you are creating a new variable. [See top comments]
 */
struct _HashTable
{
    /** The array of pointers to the head of a singly linked list, whose nodes
        are HashTableEntry objects */
        // our implementation is an ARRAY of linked lists JUST like 1332
    HashTableEntry **buckets;

    /** The hash function pointer */
    HashFunction hash;

    /** The number of buckets in the hash table */
    unsigned int num_buckets;
};

/**
 * This structure represents a hash table entry.
 * Use "HashTableEntry" instead when you are creating a new variable. [See top comments]
 */
struct _HashTableEntry
{
    /** The key for the hash table entry */
    unsigned int key;

    /** The value associated with this hash table entry */
    void *value;

    /**
     * A pointer pointing to the next hash table entry
     * NULL means there is no next entry (i.e. this is the tail)
     */
    HashTableEntry *next;
};





/****************************************************************************
 * Private Functions
 *
 * These functions are not available outside of this file, since they are not
 * declared in hash_table.h.
 ***************************************************************************/
/**
 * createHashTableEntry
 *
 * Helper function that creates a hash table entry by allocating memory for it on
 * the heap. It initializes the entry with key and value, initialize pointer to
 * the next entry as NULL, and return the pointer to this hash table entry.
 *
 * @param key The key corresponds to the hash table entry
 * @param value The value stored in the hash table entry
 * @return The pointer to the hash table entry
 */
static HashTableEntry *createHashTableEntry(unsigned int key, void *value)
{
    // 1. alloc memory on heap
    HashTableEntry* entry = (HashTableEntry*)malloc(sizeof(HashTableEntry));

    // edge case prevents null items being passed
    if (entry == NULL) return NULL;

    // initialize entry of key, value and next pointer to null
    entry-> key = key;
    entry-> value = value;
    entry-> next = NULL;

    // return pointer to hash table entry
    return entry;
}

/**
 * findItem
 *
 * Helper function that checks whether there exists the hash table entry that
 * contains a specific key.
 *
 * @param hashTable The pointer to the hash table.
 * @param key The key corresponds to the hash table entry
 * @return The pointer to the hash table entry, or NULL if key does not exist
 */
static HashTableEntry *findItem(HashTable *hashTable, unsigned int key)
{
    // use hash value to find which bucket to look through, then loop through it
    unsigned int hashValue = hashTable->hash(key);
    unsigned int numberOfBuckets = hashTable->num_buckets;
    unsigned int bucketIndex = hashValue % numberOfBuckets; // which linked list to look in

    // get head of bucket we need to traverse
    HashTableEntry* current = hashTable->buckets[bucketIndex];

    // if not found return null
    while (current != NULL) {
        if (current->key == key) {
            return current; // return pointer to hash table entry
        }
        // iterate
        current = current->next;
    }
    return NULL; // if not found
}





/****************************************************************************
 * Public Interface Functions
 *
 * These functions implement the public interface as specified in the header
 * file, and make use of the private functions and hidden definitions in the
 * above sections.
 ****************************************************************************/
// The createHashTable is provided for you as a starting point.
HashTable *createHashTable(HashFunction hashFunction, unsigned int numBuckets)
{
    // The hash table has to contain at least one bucket. Exit gracefully if
    // this condition is not met.
    if (numBuckets == 0)
    {
        printf("Hash table has to contain at least 1 bucket...\n");
        exit(1);
    }

    // Allocate memory for the new HashTable struct on heap.
    HashTable *newTable = (HashTable *)malloc(sizeof(HashTable));

    // Initialize the components of the new HashTable struct.
    newTable->hash = hashFunction;
    newTable->num_buckets = numBuckets;
    newTable->buckets = (HashTableEntry **)malloc(numBuckets * sizeof(HashTableEntry *));

    // As the new buckets are empty, init each bucket as NULL.
    unsigned int i;
    for (i = 0; i < numBuckets; ++i)
    {
        newTable->buckets[i] = NULL;
    }

    // Return the new HashTable struct.
    return newTable;
}

void destroyHashTable(HashTable *hashTable)
{
    // 1. Loop through each bucket of the hash table to remove all items.
    //      1a. set temp to be the first entry of the ith bucket
    //      1b. delete all entries

    // loop through all buckets
    for (unsigned int i = 0; i < hashTable->num_buckets; i++) {
        // loop current bucket until we delete all entries
        HashTableEntry * current = hashTable->buckets[i];
        // free ALL entries in this bucket
        while (current != NULL) {
            HashTableEntry* temp = current; // save current
            current = current-> next;
            free(temp); // delete item without memory leak successfully
        }

    }
       
    // 2. Free buckets array
    free(hashTable->buckets);
    
    // 3. Free hash table
    free(hashTable);
}

void *insertItem(HashTable *hashTable, unsigned int key, void *value)
{    //1. First, we want to check if the key is present in the hash table.
    HashTableEntry* present = findItem(hashTable, key);


    // num of bucketzz
    unsigned int numberOfBuckets = hashTable->num_buckets;

    //2. If the key is present in the hash table, store new value and return old value
    if (present != NULL) {
        // set present (the item found) to the new update value
        void* oldValue = present->value;
        present->value = value;
        return oldValue;
    }

    else {
    //3. If not, create entry for new value and return NULL
    // add item to head of the bucket this entry is hashed to
        unsigned int hashValue = hashTable->hash(key);
        unsigned int bucketIndex = hashValue % numberOfBuckets; // which linked list to look in
        HashTableEntry* current = hashTable->buckets[bucketIndex];
        // we are at the head rn

        // create new entry
        HashTableEntry* newEntry = createHashTableEntry(key, value);

        // insert at head of bucket
        newEntry-> next = current; // point new entry to head
        hashTable->buckets[bucketIndex] = newEntry; // Update new entry to head

        return NULL;
    }
}

void *getItem(HashTable *hashTable, unsigned int key)
{
    //1. First, we want to check if the key is present in the hash table.
    HashTableEntry* doesItExist = findItem(hashTable, key);
    //2. If the key exist, return the value
    if (doesItExist != NULL) {
        return doesItExist->value;
    }
    //3. If not. just return NULL
    return NULL;
}

void *removeItem(HashTable *hashTable, unsigned int key)
{
    //1. Get the bucket number and the head entry
    unsigned int hashValue = hashTable->hash(key);
    unsigned int numberOfBuckets = hashTable->num_buckets;
    unsigned int bucketIndex = hashValue % numberOfBuckets; // bucket number
    HashTableEntry* current = hashTable->buckets[bucketIndex]; // head entry

    //2. If the head holds the key, change the head to the next value, and return the old value
    if (current!= NULL &&  current->key == key) {
        HashTableEntry* holder = current;
        hashTable->buckets[bucketIndex] = current->next;
        void * value = holder->value;
        free (holder);
        return value;
    } else {
    //3. If not the head, search for the key to be removed
    // NEED to traverse manually, CANT traverse using findItem bc we need item BEFORE
        HashTableEntry* prev = NULL;
        HashTableEntry* current = hashTable->buckets[bucketIndex]; // head

         // Search for the key
        while (current != NULL) {
        if (current->key == key) {
            void * value = current->value; // save value for returning
            prev-> next = current->next; // prev node links to node after the one we delete
            free(current); // free deleted element
            return value; // unlink node
        }
        prev = current;
        current = current->next;
    }
        //4. If the key is not present in the list, return NULL
        return NULL;
    }

}   
    

void deleteItem(HashTable *hashTable, unsigned int key)
{
    //1. Remove the entry and free the returned data
    void* data = removeItem(hashTable, key);
    
    // Free the data if it exists
    if (data != NULL) {
        free(data);
    }
}
