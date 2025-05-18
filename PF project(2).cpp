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

void adminDashboard(User* admin) {
    int choice;
    while (1) {
        printf("\n===== ADMIN DASHBOARD =====\n");
        printf("1. View All Orders\n");
        printf("2. Update Order Status\n");
        printf("3. Assign Rider to Order\n");
        printf("4. Logout\n");
        printf("Enter your choice: ");
       
        char input[10];
        scanf("%s", input);
       
        if (!validateInput(input, 1)) {
            printf("Invalid input! Please enter a number.\n");
            continue;
        }
       
        choice = atoi(input);
       
        switch (choice) {
            case 1: viewAllOrders(admin); break;
            case 2: updateOrderStatus(admin); break;
            case 3: assignRider(admin); break;
            case 4: return;
            default: printf("Invalid choice! Try again.\n");
        }
    }
}

void riderDashboard(User* rider) {
    int choice;
    while (1) {
        printf("\n===== RIDER DASHBOARD =====\n");
        printf("1. View Assigned Deliveries\n");
        printf("2. Update Delivery Status\n");
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
            case 1: viewAssignedDeliveries(rider); break;
            case 2: updateDeliveryStatus(rider); break;
            case 3: return;
            default: printf("Invalid choice! Try again.\n");
        }
    }
}

void placeOrder(User* user) {
    if (orderCount >= MAX_ORDERS) {
        printf("Maximum order limit reached!\n");
        return;
    }
   
    Order newOrder;
    newOrder.id = orderCount + 1;
    newOrder.userId = user->id;
    newOrder.orderTime = time(NULL);
    newOrder.itemCount = 0;
    newOrder.total = 0;
    strcpy(newOrder.status, "preparing");
    newOrder.riderId = -1;
   
    int choice;
    char input[10];
   
    printf("\n===== PLACE NEW ORDER =====\n");
   
    while (1) {
        printf("\nMenu Categories:\n");
        printf("1. SOUPS\n");
        printf("2. APPETIZERS\n");
        printf("3. MAIN COURSE\n");
        printf("4. Finish Order\n");
        printf("Enter category choice: ");
       
        scanf("%s", input);
       
        if (!validateInput(input, 1)) {
            printf("Invalid input! Please enter a number.\n");
            continue;
        }
       
        choice = atoi(input);
       
        if (choice == 4) break;
       
        if (choice < 1 || choice > 3) {
            printf("Invalid choice! Try again.\n");
            continue;
        }
       
        const char* category;
        switch (choice) {
            case 1: category = "SOUPS"; break;
            case 2: category = "APPETIZERS"; break;
            case 3: category = "MAIN COURSE"; break;
        }
       
        printf("\n%s Menu:\n", category);
        for (int i = 0; i < menuItemCount; i++) {
            if (strcmp(menuItems[i].category, category) == 0) {
                printf("%d. %s - Rs %.2f\n", menuItems[i].id, menuItems[i].name, menuItems[i].price);
            }
        }
       
        printf("Enter item ID to add (0 to go back): ");
        scanf("%s", input);
       
        if (!validateInput(input, 1)) {
            printf("Invalid input! Please enter a number.\n");
            continue;
        }
       
        int itemId = atoi(input);
       
        if (itemId == 0) continue;
       
        MenuItem* selectedItem = NULL;
        for (int i = 0; i < menuItemCount; i++) {
            if (menuItems[i].id == itemId && strcmp(menuItems[i].category, category) == 0) {
                selectedItem = &menuItems[i];
                break;
            }
        }
       
        if (selectedItem == NULL) {
            printf("Invalid item ID!\n");
            continue;
        }
       
        printf("Enter quantity: ");
        scanf("%s", input);
       
        if (!validateInput(input, 1)) {
            printf("Invalid input! Please enter a number.\n");
            continue;
        }
       
        int quantity = atoi(input);
       
        if (quantity <= 0) {
            printf("Quantity must be positive!\n");
            continue;
        }
       
        // Add item to order
        newOrder.items[newOrder.itemCount].itemId = selectedItem->id;
        newOrder.items[newOrder.itemCount].quantity = quantity;
        newOrder.itemCount++;
        newOrder.total += selectedItem->price * quantity;
       
        printf("Added %d x %s to your order.\n", quantity, selectedItem->name);
    }
   
    if (newOrder.itemCount > 0) {
        // Select payment method
        printf("\nSelect payment method:\n");
        printf("1. Cash on Delivery\n");
        printf("2. Credit/Debit Card\n");
        printf("Enter your choice: ");
       
        scanf("%s", input);
          
        if (!validateInput(input, 1)) {
            printf("Invalid input! Using Cash on Delivery as default.\n");
            strcpy(newOrder.paymentMethod, "cash");
        } else {
            choice = atoi(input);
            if (choice == 2) {
                strcpy(newOrder.paymentMethod, "card");
                printf("Card payment selected. Payment will be processed when order is confirmed.\n");
            } else {
                strcpy(newOrder.paymentMethod, "cash");
                printf("Cash on Delivery selected.\n");
            }
        }
       
        orders[orderCount++] = newOrder;
        saveOrdersToFile();
        printf("\nOrder placed successfully! Order ID: %d, Total: Rs %.2f\n", newOrder.id, newOrder.total);
    } else {
        printf("\nNo items were added to the order.\n");
    }
}

void viewOrders(User* user) {
    printf("\n===== YOUR ORDERS =====\n");
   
    int found = 0;
    for (int i = 0; i < orderCount; i++) {
        if (orders[i].userId == user->id) {
            found = 1;
            printf("\nOrder ID: %d\n", orders[i].id);
            printf("Order Time: %s", ctime(&orders[i].orderTime));
            printf("Status: %s\n", orders[i].status);
            printf("Payment Method: %s\n", orders[i].paymentMethod);
           
            if (orders[i].riderId != -1) {
                for (int j = 0; j < riderCount; j++) {
                    if (riders[j].id == orders[i].riderId) {
                        for (int k = 0; k < userCount; k++) {
                            if (users[k].id == riders[j].userId) {
                                printf("Assigned Rider: %s (%s)\n", users[k].name, users[k].phone);
                                break;
                            }
                        }
                        break;
                    }
                }
            }
           
            printf("Items:\n");
            for (int j = 0; j < orders[i].itemCount; j++) {
                for (int k = 0; k < menuItemCount; k++) {
                    if (menuItems[k].id == orders[i].items[j].itemId) {
                        printf("- %d x %s (Rs %.2f each)\n",
                               orders[i].items[j].quantity,
                               menuItems[k].name,
                               menuItems[k].price);
                        break;
                    }
                }
            }
            printf("Total: Rs %.2f\n", orders[i].total);
        }
    }
   
    if (!found) {
        printf("You have no orders yet.\n");
    }
}

void updateOrderStatus(User* admin) {
    printf("\n===== UPDATE ORDER STATUS =====\n");
    
    if (orderCount == 0) {
        printf("No orders available.\n");
        return;
    }
    
    viewAllOrders(admin);
    
    char input[10];
    printf("Enter Order ID to update: ");
    scanf("%s", input);
    
    if (!validateInput(input, 1)) {
        printf("Invalid input! Please enter a number.\n");
        return;
    }
    
    int orderId = atoi(input);
    Order* order = NULL;
    
    for (int i = 0; i < orderCount; i++) {
        if (orders[i].id == orderId) {
            order = &orders[i];
            break;
        }
    }
    
    if (order == NULL) {
        printf("Order not found!\n");
        return;
    }
    
    printf("\nCurrent Status: %s\n", order->status);
    printf("Select new status:\n");
    printf("1. Preparing\n");
    printf("2. Ready\n");
    printf("3. Assigned\n");
    printf("4. Delivered\n");
    printf("Enter choice: ");
    
    scanf("%s", input);
    
    if (!validateInput(input, 1)) {
        printf("Invalid input! Please enter a number.\n");
        return;
    }
    
    int choice = atoi(input);
    
    switch (choice) {
        case 1: strcpy(order->status, "preparing"); break;
        case 2: strcpy(order->status, "ready"); break;
        case 3: strcpy(order->status, "assigned"); break;
        case 4: strcpy(order->status, "delivered"); break;
        default: printf("Invalid choice!\n"); return;
    }
    
    saveOrdersToFile();
    printf("Order status updated successfully!\n");
}

void assignRider(User* admin) {
    printf("\n===== ASSIGN RIDER TO ORDER =====\n");
   
    if (orderCount == 0) {
        printf("No orders available.\n");
        return;
    }
   
    if (riderCount == 0) {
        printf("No riders available.\n");
        return;
    }
   
    viewAllOrders(admin);
   
    char input[10];
    printf("Enter Order ID to assign rider: ");
    scanf("%s", input);
   
    if (!validateInput(input, 1)) {
        printf("Invalid input! Please enter a number.\n");
        return;
    }
   
    int orderId = atoi(input);
    Order* order = NULL;
   
    for (int i = 0; i < orderCount; i++) {
        if (orders[i].id == orderId) {
            order = &orders[i];
            break;
        }
    }
    
    if (order == NULL) {
        printf("Order not found!\n");
        return;
    }
   
    printf("\nAvailable Riders:\n");
    for (int i = 0; i < riderCount; i++) {
        if (strcmp(riders[i].status, "available") == 0) {
            for (int j = 0; j < userCount; j++) {
                if (users[j].id == riders[i].userId) {
                    printf("%d. %s (%s)\n", riders[i].id, users[j].name, users[j].phone);
                    break;
                }
            }
        }
    }
   
    printf("Enter Rider ID to assign: ");
    scanf("%s", input);
   
    if (!validateInput(input, 1)) {
        printf("Invalid input! Please enter a number.\n");
        return;
    }
   
    int riderId = atoi(input);
    Rider* rider = NULL;
   
    for (int i = 0; i < riderCount; i++) {
        if (riders[i].id == riderId && strcmp(riders[i].status, "available") == 0) {
            rider = &riders[i];
            break;
        }
    }
   
    if (rider == NULL) {
        printf("Invalid rider ID or rider not available!\n");
        return;
    }
   
    order->riderId = rider->id;
    strcpy(order->status, "assigned");
    strcpy(rider->status, "on-delivery");
    rider->currentOrderId = order->id;
   
    saveOrdersToFile();
    saveRidersToFile();
   
    printf("Rider assigned successfully to order %d!\n", order->id);
}

void viewAllOrders(User* admin) {
    printf("\n===== ALL ORDERS =====\n");
   
    if (orderCount == 0) {
        printf("No orders available.\n");
        return;
    }
   
    for (int i = 0; i < orderCount; i++) {
        printf("\nOrder ID: %d\n", orders[i].id);
       
        for (int j = 0; j < userCount; j++) {
            if (users[j].id == orders[i].userId) {
                printf("Customer: %s (%s)\n", users[j].name, users[j].phone);
                break;
            }
        }
       
        printf("Order Time: %s", ctime(&orders[i].orderTime));
        printf("Status: %s\n", orders[i].status);
        printf("Payment Method: %s\n", orders[i].paymentMethod);
       
        if (orders[i].riderId != -1) {
            for (int j = 0; j < riderCount; j++) {
                if (riders[j].id == orders[i].riderId) {
                    for (int k = 0; k < userCount; k++) {                       	
                        if (users[k].id == riders[j].userId) {
                            printf("Assigned Rider: %s (%s)\n", users[k].name, users[k].phone);
                            break;
                        }
                    }
                    break;
                }
            }
        }
       
        printf("Items:\n");
        for (int j = 0; j < orders[i].itemCount; j++) {
            for (int k = 0; k < menuItemCount; k++) {
                if (menuItems[k].id == orders[i].items[j].itemId) {
                    printf("- %d x %s (Rs %.2f each)\n",
                           orders[i].items[j].quantity,
                           menuItems[k].name,
                           menuItems[k].price);
                    break;
                }
            }
        }
        printf("Total: Rs %.2f\n", orders[i].total);
    }
}
