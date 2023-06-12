#include "basis.h"
#include "User.h"
#include "Moderator.h"

int main() {
    User *user = NULL;

    while (1) {
        printf("Welcome to the Sports Club Management System!\n");
        printf("1. Login\n2. Import items from file\n3. Export items to file\n4. Exit\n");
        printf("Enter your choice: ");
        int choice;
        scanf("%d", &choice);
        getchar(); // consume newline

        if (choice == 1) {
            user = login();
            if (user != NULL) {
                printf("Login successful!\n");
            }
        } else if (choice == 2) {
            char filename[20];
            printf("Enter filename: ");
            fgets(filename, sizeof(filename), stdin);
            // remove newline character
            filename[strcspn(filename, "\n")] = 0;
            importItemsFromFile(filename);
        } else if (choice == 3) {
            char filename[20];
            printf("Enter filename: ");
            fgets(filename, sizeof(filename), stdin);
            // remove newline character
            filename[strcspn(filename, "\n")] = 0;
            exportItemsToFile(filename);
        } else if (choice == 4) {
            break;
        } else {
            printf("Invalid choice, try again\n");
        }
    }

    if (user != NULL) {
        if (user->type == USER_MODERATOR) {
            deleteModerator(user->user);
        } else {
            deleteMember(user->user);
        }
        free(user);
    }

    cleanup();
    printf("Thank you for using the Sports Club Management System!\n");

    return 0;
}