#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

// Constants
const int MAX_USERS = 100;
const int MAX_ORDERS = 100;
const int MAX_RIDERS = 20;
const int MAX_ITEMS = 20;
const char FILENAME_USERS[] = "users.txt";
const char FILENAME_ORDERS[] = "orders.txt";
const char FILENAME_RIDERS[] = "riders.txt";

// Structure definitions
typedef struct {
    int id;
    char name[50];
    char type[20]; // "user", "admin", or "rider"
    char username[50];
    char password[50];
    char phone[15];
    char address[100];
} User;

typedef struct {
    int id;
    char name[50];
    float price;
    char category[30];
} MenuItem;

typedef struct {
    int itemId;
    int quantity;
} OrderItem;

typedef struct {
    int id;
    int userId;
    time_t orderTime;
    OrderItem items[MAX_ITEMS];
    int itemCount;
    float total;
    char status[30]; // "preparing", "ready", "assigned", "delivered"
    int riderId;
} Order;

typedef struct {
    int id;
    int userId;
    char status[30]; // "available", "on-delivery"
    int currentOrderId;
} Rider;

// Function declarations
void initializeSystem();
void loadMenuItems();
int validateInput(const char* input, int isNumeric);
void mainMenu();
void userLogin();
void adminLogin();
void riderLogin();
void registerUser();
void registerRider();
User* authenticateUser(const char* username, const char* password, const char* userType);
void userDashboard(User* user);
void adminDashboard(User* admin);
void riderDashboard(User* rider);
void placeOrder(User* user);
void viewOrders(User* user);
void updateOrderStatus(User* admin);
void assignRider(User* admin);
void viewAllOrders(User* admin);
void viewAssignedDeliveries(User* rider);
void updateDeliveryStatus(User* rider);
void saveUsersToFile();
void loadUsersFromFile();
void saveOrdersToFile();
void loadOrdersFromFile();
void saveRidersToFile();
void loadRidersFromFile();

// Global variables
User users[MAX_USERS];
Order orders[MAX_ORDERS];
Rider riders[MAX_RIDERS];
MenuItem menuItems[20];
int userCount = 0;
int orderCount = 0;
int riderCount = 0;
int menuItemCount = 0;

int main() {
    initializeSystem();
    mainMenu();
    return 0;
}

void initializeSystem() {
    loadUsersFromFile();
    loadOrdersFromFile();
    loadRidersFromFile();
    loadMenuItems();
    
    // Create default admin if none exists
    if (userCount == 0) {
        User admin = {1, "Admin", "admin", "admin", "admin123", "1234567890", "Admin Address"};
        users[userCount++] = admin;
        
        // Create a default rider for testing
        User rider = {2, "Test Rider", "rider", "rider", "rider123", "9876543210", "Rider Address"};
        users[userCount++] = rider;
        
        Rider riderRecord = {1, rider.id, "available", -1};
        riders[riderCount++] = riderRecord;
        
        saveUsersToFile();
        saveRidersToFile();
    }
}

void loadMenuItems() {
 
    // SOUPS
    menuItems[menuItemCount++] = (MenuItem){1, "Hot and Sour Soup", 447.0, "SOUPS"};
    menuItems[menuItemCount++] = (MenuItem){2, "Chicken Corn Soup", 155.0, "SOUPS"};
    menuItems[menuItemCount++] = (MenuItem){3, "Thai Soup", 484.0, "SOUPS"};
   
    // APPETIZERS
    menuItems[menuItemCount++] = (MenuItem){4, "Dynamite Prawns", 1220.0, "APPETIZERS"};
    menuItems[menuItemCount++] = (MenuItem){5, "Crispy Shrimp Balls", 761.0, "APPETIZERS"};
    menuItems[menuItemCount++] = (MenuItem){6, "Basket of Fries", 633.0, "APPETIZERS"};
    menuItems[menuItemCount++] = (MenuItem){7, "Crispy Mini Rolls", 547.0, "APPETIZERS"};
    menuItems[menuItemCount++] = (MenuItem){8, "Stuffed Chilli", 863.0, "APPETIZERS"};
    menuItems[menuItemCount++] = (MenuItem){9, "Chicken Drumsticks", 871.0, "APPETIZERS"};
    menuItems[menuItemCount++] = (MenuItem){10, "Fish and Chips", 556.0, "APPETIZERS"};
      
    // MAIN COURSE
    menuItems[menuItemCount++] = (MenuItem){11, "Black Pepper Chicken", 1140.0, "MAIN COURSE"};
    menuItems[menuItemCount++] = (MenuItem){12, "Sizzling Chicken", 1377.0, "MAIN COURSE"};
    menuItems[menuItemCount++] = (MenuItem){13, "Mongolian Beef", 1016.0, "MAIN COURSE"};
    menuItems[menuItemCount++] = (MenuItem){14, "Lotus Fish", 1028.0, "MAIN COURSE"};
    menuItems[menuItemCount++] = (MenuItem){15, "Egg Fried Rice", 945.0, "MAIN COURSE"};
    menuItems[menuItemCount++] = (MenuItem){16, "Singaporean Noodles", 1237.0, "MAIN COURSE"};
}

int validateInput(const char* input, int isNumeric) {
    if (strlen(input) == 0) return 0;
   
    if (isNumeric) {
        for (int i = 0; input[i] != '\0'; i++) {
            if (!isdigit(input[i])) return 0;
        }
        return 1;
    } else {
        for (int i = 0; input[i] != '\0'; i++) {
            if (!isalpha(input[i]) && input[i] != ' ') return 0;
        }
        return 1;
    }
}

void mainMenu() {
    int choice;
    while (1) {
        printf("\n===== FOOD DELIVERY SYSTEM =====\n");
        printf("1. User Login\n");
        printf("2. Admin Login\n");
        printf("3. Rider Login\n");
        printf("4. Register as User\n");
        printf("5. Register as Rider\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        
        char input[10];
        scanf("%s", input);
        
        if (!validateInput(input, 1)) {
            printf("Invalid input! Please enter a number.\n");
            continue;
        }
        
        choice = atoi(input);
        
        switch (choice) {
            case 1: userLogin(); break;
            case 2: adminLogin(); break;
            case 3: riderLogin(); break;
            case 4: registerUser(); break;
            case 5: registerRider(); break;
            case 6: 
                saveUsersToFile();
                saveOrdersToFile();
                saveRidersToFile();
                exit(0);
            default: printf("Invalid choice! Try again.\n");
        }
    }
}
void userLogin() {
    char username[50], password[50];
   
    printf("\n===== USER LOGIN =====\n");
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);
   
    User* user = authenticateUser(username, password, "user");
    if (user != NULL) {
        printf("\nLogin successful! Welcome, %s.\n", user->name);
        userDashboard(user);
    } else {
        printf("\nInvalid username or password.\n");
    }
}

void adminLogin() {
    char username[50], password[50];
   
    printf("\n===== ADMIN LOGIN =====\n");
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);
   
    User* admin = authenticateUser(username, password, "admin");
    if (admin != NULL) {
        printf("\nLogin successful! Welcome, %s.\n", admin->name);
        adminDashboard(admin);
    } else {
        printf("\nInvalid username or password.\n");
    }
}

void riderLogin() {
    char username[50], password[50];
   
    printf("\n===== RIDER LOGIN =====\n");
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);
   
    User* rider = authenticateUser(username, password, "rider");
    if (rider != NULL) {
        printf("\nLogin successful! Welcome, %s.\n", rider->name);
        riderDashboard(rider);
    } else {
        printf("\nInvalid username or password.\n");
    }
}

void registerUser() {
    if (userCount >= MAX_USERS) {
        printf("Maximum user limit reached!\n");
        return;
    }
   
    User newUser;
    char input[100];
   
    printf("\n===== USER REGISTRATION =====\n");
   
    newUser.id = userCount + 1;
    strcpy(newUser.type, "user");
   
    while (1) {
        printf("Full Name: ");
        getchar(); // Clear input buffer
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
       
        if (validateInput(input, 0)) {
            strcpy(newUser.name, input);
            break;
        }
        printf("Invalid name! Only alphabets and spaces allowed.\n");
    }
   
    printf("Username: ");
    scanf("%s", newUser.username);
   
    printf("Password: ");
    scanf("%s", newUser.password);
   
    while (1) {
        printf("Phone Number: ");
        scanf("%s", input);
        if (validateInput(input, 1) && strlen(input) == 10) {
            strcpy(newUser.phone, input);
            break;
        }
        printf("Invalid phone number! Must be 10 digits.\n");
    }
   
    printf("Address: ");
    getchar(); // Clear input buffer
    fgets(newUser.address, sizeof(newUser.address), stdin);
    newUser.address[strcspn(newUser.address, "\n")] = '\0';
   
    users[userCount++] = newUser;
    saveUsersToFile();
   
    printf("\nRegistration successful! You can now login.\n");
}

void registerRider() {
    if (userCount >= MAX_USERS || riderCount >= MAX_RIDERS) {
        printf("Maximum user or rider limit reached!\n");
        return;
    }
   
    User newUser;
    char input[100];
   
    printf("\n===== RIDER REGISTRATION =====\n");
   
    newUser.id = userCount + 1;
    strcpy(newUser.type, "rider");
   
    while (1) {
        printf("Full Name: ");
        getchar(); // Clear input buffer
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
       
        if (validateInput(input, 0)) {
            strcpy(newUser.name, input);
            break;
        }
        printf("Invalid name! Only alphabets and spaces allowed.\n");
    }
   
    printf("Username: ");
    scanf("%s", newUser.username);
   
    printf("Password: ");
    scanf("%s", newUser.password);
   
    while (1) {
        printf("Phone Number: ");
        scanf("%s", input);
        if (validateInput(input, 1) && strlen(input) == 10) {
            strcpy(newUser.phone, input);
            break;
        }
        printf("Invalid phone number! Must be 10 digits.\n");
    }
   
    printf("Address: ");
    getchar(); // Clear input buffer
    fgets(newUser.address, sizeof(newUser.address), stdin);
    newUser.address[strcspn(newUser.address, "\n")] = '\0';
   
    // Add to users
    users[userCount++] = newUser;
   
    // Create rider record
    Rider newRider = {riderCount + 1, newUser.id, "available", -1};
    riders[riderCount++] = newRider;
   
    saveUsersToFile();
    saveRidersToFile();
   
    printf("\nRegistration successful! You can now login as a rider.\n");
}

User* authenticateUser(const char* username, const char* password, const char* userType) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0 && 
            strcmp(users[i].password, password) == 0 &&
            strcmp(users[i].type, userType) == 0) {
            
            // For riders, verify there's a rider record
            if (strcmp(userType, "rider") == 0) {
                int riderFound = 0;
                for (int j = 0; j < riderCount; j++) {
                    if (riders[j].userId == users[i].id) {
                        riderFound = 1;
                        break;
                    }
                }
                if (!riderFound) {
                    return NULL;
                }
            }
            
            return &users[i];
        }
    }
    return NULL;
}


void userDashboard(User* user) {
    int choice;
    while (1) {
        printf("\n===== USER DASHBOARD =====\n");
        printf("1. Place New Order\n");
        printf("2. View My Orders\n");
        printf("3. Logout\n");
        printf("Enter your choice: ");
       
        char input[10];
        scanf("%s", input);
       
        if (!validateInput(input, 1)) {
            printf("Invalid input! Please enter a number.\n");
            continue;
        }
       
        choice = atoi(input);
       
        switch (choice) {
            case 1: placeOrder(user); break;
            case 2: viewOrders(user); break;
            case 3: return;
            default: printf("Invalid choice! Try again.\n");
        }
    }
}
