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

void adminLogin();
void signUp(const char *filename);
void signIn(const char *filename);

int main() {
    int role, choice;
    int repeat = 1;

    while (repeat) {
        // Display the main menu and ask the user for their role
        printf("\n----- Food Delivery Login System -----\n");
        printf("1. Admin.\n2. User.\n3. Rider.\n4. Exit.\n");
        printf("Choose your role: ");
        scanf("%d", &role);
        getchar(); // To clear the buffer from the newline after entering a role

        if (role == 1) {
            // If Admin is selected, call adminLogin function
            adminLogin();
        } 
        else if (role == 2 || role == 3) {
            // If User or Rider is selected
            do {
                printf("\n1. Sign-Up\n2. Sign-In\nEnter your choice: ");
                scanf("%d", &choice);
                getchar(); // To clear the buffer from the newline

                const char *filename = (role == 2) ? "users.txt" : "riders.txt";

                if (choice == 1) {
                    // If Sign-Up option is selected, call signUp function
                    signUp(filename);
                    printf("You can now sign in.\n");
                    break; // Break out of the loop after sign-up
                }
                else if (choice == 2) {
                    // If Sign-In option is selected, call signIn function
                    signIn(filename);
                    break; // Break out of the loop after sign-in
                } else {
                    // If neither option is selected, prompt for input again
                    printf("Invalid choice.\n");
                }
            } while(1); // Keep looping until the user either signs up or signs in
        } 
        else if (role == 4) {
            // If Exit option is selected
            printf("Exiting the system.\n");
            repeat = 0;
        } 
        else {
            // If an invalid role is entered, prompt again
            printf("Invalid role selected.\n");
        }
    }

    return 0;
}

void adminLogin() {
    char email[50], password[50];

    // Prompt for Admin login credentials
    printf("\nEnter Admin Email: ");
    gets(email);  // Reading email
    printf("Enter Admin Password: ");
    gets(password);  // Reading password

    // Check if the credentials match the predefined ones
    if (strcmp(email, "admin@food.com") == 0 && strcmp(password, "admin123") == 0) {
        printf("Admin login successful!\n");
    } else {
        printf("Invalid Admin credentials.\n");
    }
}

void signUp(const char *filename) {
    char email[50], password[50];
    FILE *fp = fopen(filename, "a");

    if (!fp) {
        printf("File error.\n");
        return;
    }

    // Collect email and password for sign-up
    printf("\nCreate Email: ");
    gets(email);
    printf("Create Password: ");
    gets(password);

    // Save the new credentials in the file
    fprintf(fp, "%s %s\n", email, password);
    fclose(fp);

    printf("Sign-Up Successful! You can now sign in.\n");
}

void signIn(const char *filename) {
    char email[50], password[50];
    char fileEmail[50], filePass[50];
    int found = 0;

    FILE *fp = fopen(filename, "r");

    if (!fp) {
        // Create the file if it doesn't exist
        fp = fopen(filename, "w");
        fclose(fp);
        printf("File created. Please sign up first.\n");
        return;
    }

    // Collect email and password for sign-in
    printf("\nEnter Email: ");
    gets(email);
    printf("Enter Password: ");
    gets(password);

    // Check if the entered credentials match the stored ones
    while (fscanf(fp, "%s %s", fileEmail, filePass) != EOF) {
        if (strcmp(email, fileEmail) == 0 && strcmp(password, filePass) == 0) {
            found = 1;
            break;
        }
    }

    fclose(fp);

    // Notify user if login is successful or failed
    if (found) {
        printf("Login Successful!\n");
    } else {
        printf("Invalid Email or Password.\n");
    }
}
