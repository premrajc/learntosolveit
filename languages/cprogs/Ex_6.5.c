#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Keeping hashtable size smaller value to 10.
#define HASHSIZE 10

//Sturtcure of node.
struct nlist {
    struct nlist *next;
    char *name;
    char *defn;
};

// Array of pointer to the nodes.
static struct nlist *hashtab[HASHSIZE];

//Hash function to get index value of hashtab[].
unsigned hash(char *s) {
    unsigned hashval;       // hashval is index in hashtab[].
    for(hashval=0; *s!='\0'; s++)
        hashval = *s + 31 * hashval;
    return hashval % HASHSIZE;
}

//Function to look for the specific name in the hashtab[].
struct nlist *lookup(char *s) {
    struct nlist *np;
	// for loop, in case there are multinode on single index of hashtab.
    for(np=hashtab[hash(s)]; np != NULL; np = np -> next)
        if(strcmp(s, np ->name) == 0)
            return np;
    return NULL;
}

/*
	Function to add new node to the hashtab[], if already exist then simply 
	changes the 'defn' value of that node.
*/
struct nlist *install(char *name, char *defn) {
    struct nlist *np;
    unsigned hashval;
    if((np = lookup(name)) == NULL) {
        np = (struct nlist *) malloc(sizeof(struct nlist));
        if (np == NULL || (np -> name =strdup(name)) == NULL)
            return NULL;
        hashval = hash(name);
        np -> next = hashtab[hashval];
        hashtab[hashval] = np;
    } else
        free((void *) np -> defn);
    if((np -> defn = strdup(defn)) == NULL)
        return NULL;
    return np;
}

/* 
	Function to remove entry from hashtab[], given by 'name'.
	Prints error message if node not found else deletes the entry from list.
*/
void undef(char *name) {
    struct nlist *np, *temp;
    unsigned hashval;
    if((np = lookup(name)) == NULL) {
        printf("No pair found with name %s.\n", name);
        return;
    }
    hashval = hash(name);
    temp = hashtab[hashval];
    if(temp == np) {
        hashtab[hashval] = np -> next;
        free(np);
        return;
    }
    while(temp -> next != np)
        temp = temp -> next;
    temp -> next = np -> next;
    free(np);
}

/*
	Function to print hashtab[], funtion is being called everytime the undef 
	function is called.
*/
void print_hashtable() {
    int i;
    struct nlist *np;
    for(i=0; i<HASHSIZE; i++) {
        if(hashtab[i] == NULL)
            printf("hashtab[%d] : NULL\n", i);
        else {
            np = hashtab[i];
            printf("hashtab[%d] : ", i);
            while (np -> next != NULL) {
                    printf("[(%s, %s)] --> ", np -> name, np -> defn);
                    np = np -> next;
            }
            printf("[(%s, %s)]\n", np -> name, np -> defn);
        }
    }
}

//Function to free up all memory used by malloc().
void free_hashtable_memory() {
    int i;
    struct nlist *np, *temp;
    for(i=0; i<HASHSIZE; i++) {
        if(hashtab[i] != NULL) {
            np = hashtab[i];
            while(np -> next != NULL) {
                temp = np -> next;
                free(np);
                np = temp;
            }
            free(np);
            hashtab[i] = NULL;
        }
    }
}

// main function.
int main() {
    int i;
    char key[10], val[10], choice;
    struct nlist *table[HASHSIZE] = {0};
    printf("Enter the key value pair to create hash table(maximum 10 pairs):\n");
    for(i=0; i<HASHSIZE; i++) {
        printf("Enter key: ");
        fpurge(stdin);
        fgets(key, 11, stdin);
        key[strlen(key)-1] = '\0';
        printf("Enter val: ");
        fpurge(stdin);
        fgets(val, 11, stdin);
        val[strlen(val)-1] = '\0';
        table[i] = install(key, val);
        while(1) {
            printf("Want ot enter more...? Press y to continue or n to exit.\n");
            fpurge(stdin);
            scanf("%c", &choice);
            if(choice == 'y' || choice == 'n')
                break;
            else
                printf("Invalid choice.\n");
        }
        if(choice == 'n')
            break;
    }
    printf("\nHash Table: \n");
    print_hashtable();
    while(1) {
        printf("\nEnter the key to deleted : ");
        fpurge(stdin);
        fgets(key, 11, stdin);
        key[strlen(key)-1] = '\0';
        undef(key);
        printf("\nNew hashtable:\n");
        print_hashtable();
        while(1) {
            printf("\nWant to delete more..? Press y to continue or n to exit.\n");
            fpurge(stdin);
            scanf("%c", &choice);
            if(choice == 'y' || choice == 'n')
                break;
            else
                printf("Invalid choice.\n");
        }
        if(choice == 'n')
            break;
    }
    free_hashtable_memory();
    return 0;
}
