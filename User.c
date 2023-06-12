#include "basis.h"
#include "User.h"
#include "Moderator.h"

User *login() {
    printf("\n1. Moderator\n2. Member\nChoose your role: ");
    int role;
    scanf("%d", &role);

    if (role == 1) {
        // Moderator login
        printf("Enter password: ");
        char password[20];
        scanf("%s", password);

        if (strcmp(password, "fourfools") == 0) {
            Moderator *moderator = createModerator(password);
            // Access to all the functions
            if (moderator == NULL) {
                printf("Failed to create Moderator. Exiting.\n");
                exit(EXIT_FAILURE);
            }
            User *user = malloc(sizeof(User));
            user->type = USER_MODERATOR;
            user->user = moderator;
            return user;
        } else {
            printf("Wrong password. Try again.\n");
            return login();
        }
    } else if (role == 2) {
        // Member login
        Member *member = createMember();
        if (member == NULL) {
            printf("Failed to register Member. Exiting.\n");
            exit(EXIT_FAILURE);
        }
        User *user = malloc(sizeof(User));
        user->type = USER_MEMBER;
        user->user = member;
        return user;
    } else {
        printf("Invalid choice. Try again.\n");
        return login();
    }
}

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

void borrowItem(Member *member, char *sportName, char *itemName) {
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

    if (item->quantity > 0) {
        item->quantity--;
        item->borrowed++;
        member->borrowedItems = realloc(member->borrowedItems, sizeof(struct Item *) * ++member->numItems);
        member->borrowedItems[member->numItems - 1] = item;
        printf("You borrowed %s.\n", itemName);
    } else {
        printf("Item is out of stock.\n");
    }
}

void returnItem(Member *member, char *itemName) {
    for (int i = 0; i < member->numItems; i++) {
        if (strcmp(member->borrowedItems[i]->name, itemName) == 0) {
            member->borrowedItems[i]->quantity++;
            member->borrowedItems[i]->borrowed--;
            printf("You returned %s.\n", itemName);
            return;
        }
    }

    printf("You did not borrow this item.\n");
}

Member *createMember() {
    
    Member *member = (Member *) malloc(sizeof(Member));
    if (member == NULL) {
        printf("Invalid User.\n");
        login();
    }
    member->borrowedItems = (struct Item **) malloc(sizeof(struct Item *));
    member->numItems = 0;
    return member;
}

void deleteMember(Member *member) {
    if (member != NULL) {
        for (int i = 0; i < member->numItems; i++) {
            free(member->borrowedItems[i]);
        }
        free(member->borrowedItems);
        free(member);
    }
}

void addSport(char *name) {
    struct Sport *sport = malloc(sizeof(struct Sport));
    sport->name = strdup(name);
    sport->items = NULL;

    int index = hash(name);
    sport->next = sports[index];
    sports[index] = sport;
}

void freeSport(struct Sport* sport) {
    if (sport != NULL) {
        freeSport(sport->next);
        freeItem(sport->items);
        free(sport->name);
        free(sport);
    }
}

void cleanup() {
    for (int i = 0; i < MAX_SPORTS; i++) {
        freeSport(sports[i]);
        sports[i] = NULL;
    }
}

int hash(char *str) {
    int hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;

    return hash % MAX_SPORTS;
}

void continueOrExit() {
    printf("\nPress 1 to continue browsing\nPress 2 to exit back to menu\n");
    int choice;
    scanf("%d", &choice);
    if (choice == 1) {
        // continue browsing
    } else if (choice == 2) {
        // exit to menu
        login();
    } else {
        printf("Invalid choice. Try again.\n");
        continueOrExit();
    }
}