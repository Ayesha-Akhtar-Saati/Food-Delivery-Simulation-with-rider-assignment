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
const char FILENAME_USERS[] = "users.dat";
const char FILENAME_ORDERS[] = "orders.dat";
const char FILENAME_RIDERS[] = "riders.dat";

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
