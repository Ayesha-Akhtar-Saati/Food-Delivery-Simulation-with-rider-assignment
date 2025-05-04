#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int MAX_CATEGORIES = 3;
const int MAX_ITEMS = 10;
const int MAX_NAME_LENGTH = 50;
const int MAX_CART_ITEMS = 20;


typedef struct {
    char name[MAX_NAME_LENGTH];
    int price;
    int quantity;
} MenuItem;

typedef struct {
    char category[MAX_NAME_LENGTH];
    MenuItem items[MAX_ITEMS];
    int itemCount;
} MenuCategory;

typedef struct {
    MenuItem items[MAX_CART_ITEMS];
    int itemCount;
    int total;
} ShoppingCart;
