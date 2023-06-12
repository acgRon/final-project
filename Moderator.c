#include "basis.h"
#include "User.h"
#include "Moderator.h"

struct Sport *findSport(char *name) {
    int index = hash(name);
    struct Sport *sport = sports[index];

    while (sport != NULL) {
        if (strcmp(sport->name, name) == 0)
            return sport;

        sport = sport->next;
    }

    return NULL;
}

Moderator *createModerator(char *password) {
    Moderator *moderator = (Moderator *) malloc(sizeof(Moderator));
    if (moderator == NULL) {
        printf("Invalid User.\n");
        login();
    }
    moderator->password = password;
    moderator->accessToSports = sports; // Give moderator full access
    moderator->numSports = MAX_SPORTS;
    return moderator;
}

void deleteModerator(Moderator *moderator) {
    if (moderator != NULL) {
        free(moderator);
    }
    else if(moderator == NULL){
        printf("Not find the Moderator");
    }
}

void printItems(Moderator *moderator) {
    if (moderator != NULL) {
        for (int i = 0; i < MAX_SPORTS; i++) {
            struct Sport *sport = sports[i];
            while (sport != NULL) {
                // Implement printItemsForSport function
                printItemsForSport(sport);
                sport = sport->next;
            }
        }
    }
}


struct Item *createItem(char *name, int quantity) {
    struct Item *item = malloc(sizeof(struct Item));
    item->name = strdup(name);
    item->quantity = quantity;
    item->borrowed = 0;
    item->left = NULL;
    item->right = NULL;

    return item;
}

struct Item* minValueNode(struct Item* node) {
    struct Item* current = node;

    /* loop down to find the leftmost leaf */
    while (current && current->left != NULL)
        current = current->left;

    return current;
}

struct Item* deleteNode(struct Item* root, char *name) {
    // base case
    if (root == NULL) return root;

    // If the name to be deleted is smaller than the root's name,
    // then it lies in left subtree
    if (strcmp(name, root->name) < 0)
        root->left = deleteNode(root->left, name);

    // If the name to be deleted is greater than the root's name,
    // then it lies in right subtree
    else if (strcmp(name, root->name) > 0)
        root->right = deleteNode(root->right, name);

    // if name is same as root's name, then This is the node
    // to be deleted
    else {
        // node with only one child or no child
        if (root->left == NULL) {
            struct Item *temp = root->right;
            free(root->name);
            free(root);
            return temp;
        } else if (root->right == NULL) {
            struct Item *temp = root->left;
            free(root->name);
            free(root);
            return temp;
        }

        // node with two children: Get the inorder successor (smallest
    // in the right subtree)
    struct Item* temp = minValueNode(root->right);

    // Swap the inorder successor's content with this node
    char* tempName = root->name;
    root->name = temp->name;
    temp->name = tempName;
    root->quantity = temp->quantity;
    root->borrowed = temp->borrowed;

    // Delete the inorder successor
    root->right = deleteNode(root->right, temp->name);
    }
    return root;
}

void addItem(struct Sport *sport, char *name, int quantity) {
    struct Item *new_item = createItem(name, quantity);

    if (sport->items == NULL) {
        sport->items = new_item;
    } else {
        struct Item *parent, *current = sport->items;

        while (current != NULL) {
            parent = current;
            if (strcmp(name, current->name) < 0)
                current = current->left;
            else
                current = current->right;
        }

        if (strcmp(name, parent->name) < 0)
            parent->left = new_item;
        else
            parent->right = new_item;
    }
}

void deleteItem(struct Sport *sport, char *name) {
    sport->items = deleteNode(sport->items, name);
}

void freeItem(struct Item* item) {
    if (item != NULL) {
        freeItem(item->left);
        freeItem(item->right);
        free(item->name);
        free(item);
    }
}

struct Item *findItem(struct Item *root, char *name) {
    if (root == NULL)
        return NULL;

    if (strcmp(name, root->name) == 0)
        return root;

    if (strcmp(name, root->name) < 0)
        return findItem(root->left, name);
    else
        return findItem(root->right, name);
}

void checkBorrowedStatus(Moderator *moderator, char *sportName, char *itemName) {
    struct Sport *sport = findSport(sportName);

    if (sport == NULL) {
        printf("Sport not found.\n");
        return;
    }

    struct Item *item = findItem(sport->items, itemName);

    if (item == NULL) {
        printf("Item not found.\n");
        return;
    }

    printf("%s: %d borrowed, %d in stock.\n", itemName, item->borrowed, item->quantity);
}

void importItemsFromFile(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Could not open file %s\n", filename);
        return;
    }

    char sportName[20];
    char itemName[20];
    int quantity;
    while (fscanf(file, "%s %s %d", sportName, itemName, &quantity) == 3) {
        struct Sport *sport = findSport(sportName);
        if (sport == NULL) {
            addSport(sportName);
            sport = findSport(sportName);
        }
        addItem(sport, itemName, quantity);
    }

    fclose(file);
}

void exportItemsToFile(char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Could not open file %s\n", filename);
        return;
    }

    for (int i = 0; i < MAX_SPORTS; i++) {
        struct Sport *sport = sports[i];
        while (sport != NULL) {
            struct Item *item = sport->items;
            if (item != NULL) {
                fprintf(file, "%s %s %d\n", sport->name, item->name, item->quantity);
            }
            sport = sport->next;
        }
    }

    fclose(file);
}