#include <stdio.h>//To show messages on the screen and take input from the user.
#include <stdlib.h>//To use functions for memory, random numbers, and exiting the program
#include <string.h>//To work with words and text, like copying or comparing strings.
#include <ctype.h>//To check or change characters, like making letters uppercase
#include <time.h>//To work with date and time, like getting the current time or creating delays

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
    char paymentMethod[20];
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
//Initializes the system by loading saved data and creating a default admin and rider if no users exist.
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
//This function checks if the input is correct:
//If isNumeric is true, it makes sure the input has only numbers.
//If isNumeric is false, it makes sure the input has only letters and spaces.
//If the input is empty or has wrong characters, it returns 0 (invalid).
//If it's correct, it returns 1 (valid).
int validateInput(const char* input, int isNumeric) {
    if (strlen(input) == 0) return 0;//If the input is empty (no characters), it's invalid ? return 0.
   
    if (isNumeric) {
        for (int i = 0; input[i] != '\0'; i++) {
            if (!isdigit(input[i])) return 0;//If any character is not a digit (0-9), return 0 (invalid).
        }
        return 1;//If all characters are digits, return 1 (valid).
    } else {
        for (int i = 0; input[i] != '\0'; i++) {
            if (!isalpha(input[i]) && input[i] != ' ') return 0; //If a character is not a letter and not a space, return 0 (invalid)
        }
        return 1;
    }
}
//Shows the main menu, takes user input, and runs the selected option like login, registration, or exit.
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
//Registers a new user by collecting and validating their details, then saving them to the system
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
//Checks if a user's username, password, and type are correct, and returns the user if valid (especially verifies rider accounts exist).
User* authenticateUser(const char* username, const char* password, const char* userType) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0 && 
            strcmp(users[i].password, password) == 0 &&
            strcmp(users[i].type, userType) == 0) {
            
            // For riders, verify there's a rider record 
            if (strcmp(userType, "rider") == 0) {
                int riderFound = 0;
                for (int j = 0; j < riderCount; j++) {
                    if (riders[j].userId == users[i].id) {//It compares the rider’s userId with the user’s id
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

//Shows the user dashboard menu where users can place orders, view their orders, or log out
void userDashboard(User* user) {
    int choice;
    while (1) {
        printf("\n===== USER DASHBOARD =====\n");
        printf("1. Place New Order\n");
        printf("2. View My Orders\n");
        printf("3. Logout\n");
        printf("Enter your choice: ");
       
        char input[10];//In an array, space is already reserved for the input, so we don’t use & in scanf.
        scanf("%s", input);//Takes the user’s choice as a string (instead of directly as an integer) to allow for validation.
       
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
    while (1) {//It shows the Admin Dashboard menu again and again (inside a loop) until the admin chooses to logout
        printf("\n===== ADMIN DASHBOARD =====\n");
        printf("1. View All Orders\n");
        printf("2. Update Order Status\n");
        printf("3. Assign Rider to Order\n");
        printf("4. Logout\n");
        printf("Enter your choice: ");
       
        char input[10];
        scanf("%s", input);//In an array, space is already reserved for the input, so we don’t use & in scanf.
       
        if (!validateInput(input, 1)) {////Takes the user’s choice as a string (instead of directly as an integer) to allow for validation
            printf("Invalid input! Please enter a number.\n");
            continue;
        }
       
        choice = atoi(input);//it convert string into number
       
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
//Allows a user to browse the menu, select items, choose a payment method, and place a new food order
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
//Displays all past orders placed by the currently logged-in user with full details
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
//Allows the admin to view all orders and update the status of a selected order
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
//Allows an admin to assign an available rider to a selected order and update both the order and rider records accordingly.
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
//Displays all customer orders for an admin, including: Customer details, Order time and status, Payment method, Assigned rider (if any), Ordered items and total cost
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
//This function displays all orders assigned to the currently logged-in rider, including: Customer details, Delivery address, Order contents and total
void viewAssignedDeliveries(User* rider) {
    printf("\n===== YOUR ASSIGNED DELIVERIES =====\n");
    
    // Find the rider record
    Rider* riderRecord = NULL;
    for (int i = 0; i < riderCount; i++) {
        if (riders[i].userId == rider->id) {
            riderRecord = &riders[i];
            break;
        }
    }
    
    if (riderRecord == NULL) {
        printf("Rider record not found!\n");
        return;
    }
    
    int found = 0;
    for (int i = 0; i < orderCount; i++) {
        if (orders[i].riderId == riderRecord->id) {
            found = 1;
            printf("\nOrder ID: %d\n", orders[i].id);
            
            for (int j = 0; j < userCount; j++) {
                if (users[j].id == orders[i].userId) {
                    printf("Customer: %s (%s)\n", users[j].name, users[j].phone);
                    printf("Address: %s\n", users[j].address);
                    printf("Payment Method: %s\n", orders[i].paymentMethod);
                    break;
                }
            }
            
            printf("Order Time: %s", ctime(&orders[i].orderTime));
            printf("Status: %s\n", orders[i].status);
            
            printf("Items:\n");
            for (int j = 0; j < orders[i].itemCount; j++) {
                for (int k = 0; k < menuItemCount; k++) {
                    if (menuItems[k].id == orders[i].items[j].itemId) {
                        printf("- %d x %s\n", orders[i].items[j].quantity, menuItems[k].name);
                        break;
                    }
                }
            }
            printf("Total: Rs %.2f\n", orders[i].total);
        }
    }
    
    if (!found) {
        printf("You have no assigned deliveries.\n");
    }
}
//This function lets a rider update the delivery status of their current assigned order, marking it as either: "on the way" (picked up), "delivered"
void updateDeliveryStatus(User* rider) {
    printf("\n===== UPDATE DELIVERY STATUS =====\n");
   
    // Find the rider record
    Rider* riderRecord = NULL;
    for (int i = 0; i < riderCount; i++) {
        if (riders[i].userId == rider->id) {
            riderRecord = &riders[i];
            break;
        }
    }
   
    if (riderRecord == NULL) {
        printf("Rider record not found!\n");
        return;
    }
   
    if (riderRecord->currentOrderId == -1) {
        printf("You have no assigned deliveries to update.\n");
        return;
    }
   
    Order* order = NULL;
    for (int i = 0; i < orderCount; i++) {
        if (orders[i].id == riderRecord->currentOrderId) {
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
    printf("1. Picked Up\n");
    printf("2. Delivered\n");
    printf("Enter choice: ");
   
    char input[10];
    scanf("%s", input);
      
    if (!validateInput(input, 1)) {
        printf("Invalid input! Please enter a number.\n");
        return;
    }
   
    int choice = atoi(input);
   
    switch (choice) {
        case 1:
            strcpy(order->status, "on the way");
            printf("Status updated to 'on the way'.\n");
            break;
        case 2:
            strcpy(order->status, "delivered");
            strcpy(riderRecord->status, "available");
            riderRecord->currentOrderId = -1;
           
            // If payment was by card, process payment
            if (strcmp(order->paymentMethod, "card") == 0) {
                printf("Card payment processed successfully.\n");
            }
           
            printf("Status updated to 'delivered'. You are now available for new deliveries.\n");
            break;
        default:
            printf("Invalid choice!\n");
            return;
    }
   
    saveOrdersToFile();
    saveRidersToFile();
}
//This function writes all users' data (such as ID, name, username, etc.) to a file defined by FILENAME_USERS.
void saveUsersToFile() {
    FILE* file = fopen(FILENAME_USERS, "w");
    if (file == NULL) {
        printf("Error saving users data!\n");
        return;
    }
   
    fprintf(file, "%d\n", userCount);
    for (int i = 0; i < userCount; i++) {
        fprintf(file, "%d\n%s\n%s\n%s\n%s\n%s\n%s\n",
                users[i].id,
                users[i].name,
                users[i].type,
                users[i].username,
                users[i].password,
                users[i].phone,
                users[i].address);
    }
   
    fclose(file);
}
//Reads users from a file (like "users.txt") and loads them into the global users[] array
void loadUsersFromFile() {
    FILE* file = fopen(FILENAME_USERS, "r");
    if (file == NULL) {
        return; // File doesn't exist yet
    }
    
    fscanf(file, "%d", &userCount);
    for (int i = 0; i < userCount; i++) {
        fscanf(file, "%d", &users[i].id);
        fgetc(file); // Consume newline
        fgets(users[i].name, sizeof(users[i].name), file);
        users[i].name[strcspn(users[i].name, "\n")] = '\0';
        fgets(users[i].type, sizeof(users[i].type), file);
        users[i].type[strcspn(users[i].type, "\n")] = '\0';
        fgets(users[i].username, sizeof(users[i].username), file);
        users[i].username[strcspn(users[i].username, "\n")] = '\0';
        fgets(users[i].password, sizeof(users[i].password), file);
        users[i].password[strcspn(users[i].password, "\n")] = '\0';
        fgets(users[i].phone, sizeof(users[i].phone), file);
        users[i].phone[strcspn(users[i].phone, "\n")] = '\0';
        fgets(users[i].address, sizeof(users[i].address), file);
        users[i].address[strcspn(users[i].address, "\n")] = '\0';
    }
    
    fclose(file);
}
//"This function saves all the orders from memory into a file, so the data doesn't get lost when the program is closed."
void saveOrdersToFile() {
    FILE* file = fopen(FILENAME_ORDERS, "w");
    if (file == NULL) {
        printf("Error saving orders data!\n");
        return;
    }
   
    fprintf(file, "%d\n", orderCount);
    for (int i = 0; i < orderCount; i++) {
        fprintf(file, "%d\n%d\n%ld\n%d\n%.2f\n%s\n%d\n%s\n",
                orders[i].id,
                orders[i].userId,
                orders[i].orderTime,
                orders[i].itemCount,
                orders[i].total,
                orders[i].status,
                orders[i].riderId,
                orders[i].paymentMethod);
       
        for (int j = 0; j < orders[i].itemCount; j++) {
            fprintf(file, "%d %d\n", orders[i].items[j].itemId, orders[i].items[j].quantity);
        }
    }
   
    fclose(file);
}
//This function loads all saved orders from a file back into memory when the program starts, so you can continue working with the previous data.
void loadOrdersFromFile() {
    FILE* file = fopen(FILENAME_ORDERS, "r");
    if (file == NULL) {
        return; // File doesn't exist yet
    }
   
    fscanf(file, "%d", &orderCount);
    for (int i = 0; i < orderCount; i++) {
        fscanf(file, "%d", &orders[i].id);
        fscanf(file, "%d", &orders[i].userId);
        fscanf(file, "%ld", &orders[i].orderTime);
        fscanf(file, "%d", &orders[i].itemCount);
        fscanf(file, "%f", &orders[i].total);
        fscanf(file, "%s", orders[i].status);
        fscanf(file, "%d", &orders[i].riderId);
        fscanf(file, "%s", orders[i].paymentMethod);
       
        for (int j = 0; j < orders[i].itemCount; j++) {
            fscanf(file, "%d %d", &orders[i].items[j].itemId, &orders[i].items[j].quantity);
        }
    }
   
    fclose(file);
}
//"This function saves all rider information from memory into a file, so the data is not lost when the program is closed."
void saveRidersToFile() {
    FILE* file = fopen(FILENAME_RIDERS, "w");
    if (file == NULL) {
        printf("Error saving riders data!\n");
        return;
    }
    
    fprintf(file, "%d\n", riderCount);
    for (int i = 0; i < riderCount; i++) {
        fprintf(file, "%d\n%d\n%s\n%d\n", 
                riders[i].id, 
                riders[i].userId, 
                riders[i].status, 
                riders[i].currentOrderId);
    }
    
    fclose(file);
}
//"This function loads all rider information from a file back into memory, so the program can continue using the saved data."
void loadRidersFromFile() {
    FILE* file = fopen(FILENAME_RIDERS, "r");
    if (file == NULL) {
        return; // File doesn't exist yet
    }
    
    fscanf(file, "%d", &riderCount);
    for (int i = 0; i < riderCount; i++) {
        fscanf(file, "%d", &riders[i].id);
        fscanf(file, "%d", &riders[i].userId);
        fscanf(file, "%s", riders[i].status);
        fscanf(file, "%d", &riders[i].currentOrderId);
    }
    
    fclose(file);
}
