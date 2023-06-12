#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SPORTS 20

struct Sport *sports[MAX_SPORTS] = { NULL };

typedef enum {
    USER_MEMBER,
    USER_MODERATOR,
} UserType;

struct Item {
    char *name;
    int quantity;
    int borrowed;
    struct Item *left, *right;
};

struct Sport {
    char *name;
    struct Item *items;
    struct Sport *next;
};

typedef struct {
    struct Item **borrowedItems;
    int numItems;
} Member;

typedef struct {
    char *password;
    struct Sport **accessToSports;
    int numSports;
} Moderator;

typedef struct {
    UserType type;
    void *user;
} User;
