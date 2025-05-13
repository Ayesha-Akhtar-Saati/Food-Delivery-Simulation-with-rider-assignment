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

