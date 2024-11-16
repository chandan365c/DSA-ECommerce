#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 101
#define MAX_USERS 100

// Data Structures
typedef struct User
{
     int id;
     char name[50];
     char email[100];
     char password[50];
     struct User *next;
} User;

typedef struct Product
{
     int id;
     char name[50];
     double price;
     struct Product *next;
} Product;

typedef struct PurchaseHistoryNode
{
     int productId;
     struct PurchaseHistoryNode *next;
} PurchaseHistoryNode;

typedef struct UserHistory
{
     int userId;
     PurchaseHistoryNode *purchaseHistory;
} UserHistory;

typedef struct BrowsingHistoryNode
{
     int productId;
     struct BrowsingHistoryNode *next;
} BrowsingHistoryNode;

typedef struct UserBrowsingHistory
{
     int userId;
     BrowsingHistoryNode *browsingHistory;
} UserBrowsingHistory;

// Hash Tables
User *userTable[HASH_SIZE] = {NULL};
Product *productTable[HASH_SIZE] = {NULL};

// User Histories
UserHistory userHistories[MAX_USERS];
UserBrowsingHistory browsingHistories[MAX_USERS];

// Hash Function
int hash(int id)
{
     return id % HASH_SIZE;
}

// User Management
void addUser(int id, const char *name, const char *email, const char *password)
{
     int index = hash(id);
     User *newUser = (User *)malloc(sizeof(User));
     newUser->id = id;
     strcpy(newUser->name, name);
     strcpy(newUser->email, email);
     strcpy(newUser->password, password);
     newUser->next = userTable[index];
     userTable[index] = newUser;
     userHistories[id].userId = id;
     userHistories[id].purchaseHistory = NULL;
     browsingHistories[id].userId = id;
     browsingHistories[id].browsingHistory = NULL;
     printf("User '%s' added successfully.\n", name);
}

User *getUserByEmail(const char *email)
{
    for (int i = 0; i < HASH_SIZE; i++)
    {
        User *user = userTable[i];
        while (user)
        {
            if (strcmp(user->email, email) == 0)
            {
                return user;
            }
            user = user->next;
        }
    }
    return NULL;
}

User *getUserByID(int ID)
{
    for(int i=0; i< HASH_SIZE; i++)
    {
        User *user = userTable[i];
        while(user)
        {
            if(user->id == ID)
            {
                return user;
            }
            user = user->next;
        }
    }
    return NULL;
}

// Product Management
void addProduct(int id, const char *name, double price)
{
     int index = hash(id);
     Product *newProduct = (Product *)malloc(sizeof(Product));
     newProduct->id = id;
     strcpy(newProduct->name, name);
     newProduct->price = price;
     newProduct->next = productTable[index];
     productTable[index] = newProduct;
     printf("Product '%s' added successfully.\n", name);
}

void addProductInteractive()
{
     int id;
     char name[50];
     double price;
     printf("Enter Product ID: ");
     scanf("%d", &id);
     printf("Enter Product Name: ");
     scanf(" %[^\n]%*c", name);
     printf("Enter Product Price: ");
     scanf("%lf", &price);
     addProduct(id, name, price);
}

Product *getProduct(int id)
{
     int index = hash(id);
     Product *product = productTable[index];
     while (product && product->id != id)
     {
          product = product->next;
     }
     return product;
}

void displayAllProducts()
{
     printf("\nAvailable Products:\n");
     for (int i = 0; i < HASH_SIZE; i++)
     {
          Product *product = productTable[i];
          while (product)
          {
               printf("Product ID: %d, Name: %s, Price: $%.2f\n", product->id, product->name, product->price);
               product = product->next;
          }
     }
}

// Purchase History
PurchaseHistoryNode *addToPurchaseHistory(PurchaseHistoryNode *head, int productId)
{
     PurchaseHistoryNode *newNode = (PurchaseHistoryNode *)malloc(sizeof(PurchaseHistoryNode));
     newNode->productId = productId;
     newNode->next = head;
     return newNode;
}

void recordPurchaseHistory(int userId, int productId)
{
     userHistories[userId].purchaseHistory = addToPurchaseHistory(userHistories[userId].purchaseHistory, productId);
     printf("Purchase history updated for user ID %d.\n", userId);
}

void displayPurchaseHistory(int userId)
{
     printf("\nPurchase History for User ID %d:\n", userId);
     PurchaseHistoryNode *current = userHistories[userId].purchaseHistory;
     if (!current)
     {
          printf("No purchase history available.\n");
     }
     else
     {
          while (current)
          {
               Product *product = getProduct(current->productId);
               if (product)
               {
                    printf("  - %s ($%.2f)\n", product->name, product->price);
               }
               current = current->next;
          }
     }
}

// Browsing History
BrowsingHistoryNode *addToBrowsingHistory(BrowsingHistoryNode *head, int productId)
{
     BrowsingHistoryNode *newNode = (BrowsingHistoryNode *)malloc(sizeof(BrowsingHistoryNode));
     newNode->productId = productId;
     newNode->next = head;
     return newNode;
}

void recordBrowsingHistory(int userId, int productId)
{
     browsingHistories[userId].browsingHistory = addToBrowsingHistory(browsingHistories[userId].browsingHistory, productId);
     printf("Product ID %d added to browsing history for User ID %d.\n", productId, userId);
}

void displayBrowsingHistory(int userId)
{
     printf("\nBrowsing History for User ID %d:\n", userId);
     BrowsingHistoryNode *current = browsingHistories[userId].browsingHistory;
     if (!current)
     {
          printf("No browsing history available.\n");
     }
     else
     {
          while (current)
          {
               Product *product = getProduct(current->productId);
               if (product)
               {
                    printf("  - %s ($%.2f)\n", product->name, product->price);
               }
               current = current->next;
          }
     }
}

// Recommendations
void recommendProducts(int userId)
{
     printf("\nRecommended Products for User ID %d:\n", userId);
     for (int i = 0; i < HASH_SIZE; i++)
     {
          Product *product = productTable[i];
          while (product)
          {
               int alreadyPurchased = 0;
               PurchaseHistoryNode *current = userHistories[userId].purchaseHistory;
               while (current)
               {
                    if (current->productId == product->id)
                    {
                         alreadyPurchased = 1;
                         break;
                    }
                    current = current->next;
               }
               if (!alreadyPurchased)
               {
                    printf("  - %s ($%.2f)\n", product->name, product->price);
               }
               product = product->next;
          }
     }
}

void displayUserData(int userID)
{
    printf("Details of user %d: \n", userID);
    User *user = getUserByID(userID);
    printf("-----------------------\n");
    printf("\nUserName: %s \n", user->name);
    printf("\nEmail:  %s \n", user->email);
    printf("\nPassword:  %s \n", user->password);
    printf("--------------------------\n");
}

// User Login
int loginUser(char *email, char *password)
{
     User *user = getUserByEmail(email);
     if (user && strcmp(user->password, password) == 0)
     {
          printf("Login successful!\n");
          return user->id;
     }
     else
     {
          printf("Invalid email or password.\n");
          return -1;
     }
}

void logoutUser(int *loggedInUser)
{
     if (*loggedInUser != -1)
     {
          *loggedInUser = -1;
          printf("User logged out successfully.\n");
     }
     else
     {
          printf("No user is logged in.\n");
     }
}

// Buy Product
void buyProduct(int userId)
{
    displayAllProducts();
    int productId;
    printf("Enter Product ID to Buy: ");
    scanf("%d", &productId);

    Product *product = getProduct(productId);
    if (product)
    {
        char buyChoice;
        printf("Do you want to buy this product? (y/n): ");
        scanf(" %c", &buyChoice);

        if (buyChoice == 'y' || buyChoice == 'Y')
        {
            recordPurchaseHistory(userId, productId);
            printf("User ID %d bought the product: %s for $%.2f\n", userId, product->name, product->price);
        }
        else
        {
            recordBrowsingHistory(userId, productId);
        }
    }
    else
    {
        printf("Product not found.\n");
    }
}

// Main Function
int main()
{
    int choice;
    int userId, productId;
    char name[50], email[100], password[50];
    int loggedInUser = -1;

    while (1)
    {
        printf("\nE-Commerce System\n");
        printf("1. Register User\n");
        printf("2. Login\n");
        printf("3. Display User Details\n");
        printf("4. Add Product\n");
        printf("5. Buy Product\n");
        printf("6. Display Browsing History\n");
        printf("7. Display Purchase History\n");
        printf("8. Recommend Products\n");
        printf("9. Logout\n");
        printf("10. Exit\n");

        if (loggedInUser != -1)
        {
            printf("Logged in as User ID %d\n", loggedInUser);
        }

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
            {
                if (loggedInUser == -1)
                {
                    printf("Enter User ID: ");
                    scanf("%d", &userId);
                    printf("Enter Name: ");
                    scanf(" %[^\n]%*c", name);
                    printf("Enter Email: ");
                    scanf("%s", email);
                    printf("Enter Password: ");
                    scanf("%s", password);
                    addUser(userId, name, email, password);
                }
                else
                {
                    printf("You are already logged in.\n");
                }
                break;
            }
            case 2:
            {
                if (loggedInUser == -1)
                {
                    printf("Enter Email: ");
                    scanf("%s", email);
                    printf("Enter Password: ");
                    scanf("%s", password);
                    loggedInUser = loginUser(email, password);
                }
                else
                {
                    printf("You are already logged in.\n");
                }
                break;
            }
            case 3:
            {
                if(loggedInUser != -1)
                {
                    displayUserData(loggedInUser);
                }
                else
                {
                    printf("Log in to view user details.\n");
                }
                break;
            }
            case 4:
            {
                if (loggedInUser != -1)
                {
                    addProductInteractive();
                }
                else
                {
                    printf("Log in to add products.\n");
                }
                break;
            }
            case 5:
            {
                if (loggedInUser != -1)
                {
                    buyProduct(loggedInUser);
                }
                else
                {
                    printf("Log in to buy products.\n");
                }
                break;
            }
            case 6:
            {
                if (loggedInUser != -1)
                {
                    displayPurchaseHistory(loggedInUser);
                }
                else
                {
                    printf("Log in to view purchase history.\n");
                }
                break;
            }
            case 7:
            {
                if (loggedInUser != -1)
                {
                    displayBrowsingHistory(loggedInUser);
                }
                else
                {
                    printf("Log in to view browsing history.\n");
                }
                break;
            }
            case 8:
            {
                if (loggedInUser != -1)
                {
                    recommendProducts(loggedInUser);
                }
                else
                {
                    printf("Log in to view product recommendations.\n");
                }
                break;
            }
            case 9:
            {
                logoutUser(&loggedInUser);
                break;
            }
            case 10:
            {
                printf("Exiting system...\n");
                exit(0);
                break;
            }
            default:
            {
                printf("Invalid choice, please try again.\n");
                break;
            }
        }
    }

    return 0;
}