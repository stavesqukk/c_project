#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h> // For getch()

// Structure for Book
typedef struct {
    int id;
    char title[100];
    char author[100];
    int is_borrowed; // 0 = available, 1 = borrowed
} Book;

// Global Variables
Book books[100];
int book_count = 0;
char current_user[50];

// Function Prototypes
void load_books();
void save_books();
void add_book();
void view_books();
void edit_book();
void delete_book();
void sort_books();
void search_books();
void issue_book();
void return_book();
void login_or_signup();
void login();
void signup();
int validate_user(const char *username, const char *password);
void save_user(const char *username, const char *password, const char *email);
int find_book_by_id(int id);
void to_lowercase(char *str);
void get_hidden_password(char *password);
int validate_username(const char *username);
void forgot_password();

// Main Menu
void menu() {
    int choice;
    do {
        printf("\n=========================================\n");
        printf("         LIBRARY MANAGEMENT SYSTEM       \n");
        printf("=========================================\n");
        printf("| %-2s | %-30s |\n", "No", "Option");
        printf("|----|--------------------------------|\n");
        printf("| 1  | Add Book                       |\n");
        printf("| 2  | View Books                     |\n");
        printf("| 3  | Edit Book Details              |\n");
        printf("| 4  | Delete Book                    |\n");
        printf("| 5  | Sort Books                     |\n");
        printf("| 6  | Search Books                   |\n");
        printf("| 7  | Issue Book                     |\n");
        printf("| 8  | Return Book                    |\n");
        printf("| 9  | Save and Exit                  |\n");
        printf("=========================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: add_book(); break;
            case 2: view_books(); break;
            case 3: edit_book(); break;
            case 4: delete_book(); break;
            case 5: sort_books(); break;
            case 6: search_books(); break;
            case 7: issue_book(); break;
            case 8: return_book(); break;
            case 9:
                save_books();
                printf("Data saved. Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (1);
}

// Load Books from File
void load_books() {
    FILE *file = fopen("books.txt", "r");
    if (!file) {
        printf("No previous data found. Starting fresh.\n");
        return;
    }
    while (fscanf(file, "%d\n%[^\n]\n%[^\n]\n%d\n", &books[book_count].id, books[book_count].title, books[book_count].author, &books[book_count].is_borrowed) == 4) {
        book_count++;
    }
    fclose(file);
}

// Save Books to File
void save_books() {
    FILE *file = fopen("books.txt", "w");
    if (!file) {
        perror("Error saving data");
        return;
    }
    for (int i = 0; i < book_count; i++) {
        fprintf(file, "%d\n%s\n%s\n%d\n", books[i].id, books[i].title, books[i].author, books[i].is_borrowed);
    }
    fclose(file);
    printf("Data saved successfully!\n");
}

// Add a Book
void add_book() {
    printf("\n=========================================\n");
    printf("             ADD A NEW BOOK              \n");
    printf("=========================================\n");
    int id;
    printf("Enter Book ID: ");
    scanf("%d", &id);

    // Check for duplicate ID
    if (find_book_by_id(id) != -1) {
        printf("Error: A book with this ID already exists.\n");
        return;
    }

    books[book_count].id = id;

    printf("Enter Book Title: ");
    getchar(); // Clear input buffer
    fgets(books[book_count].title, 100, stdin);
    books[book_count].title[strcspn(books[book_count].title, "\n")] = '\0'; // Remove newline

    printf("Enter Book Author: ");
    fgets(books[book_count].author, 100, stdin);
    books[book_count].author[strcspn(books[book_count].author, "\n")] = '\0'; // Remove newline

    books[book_count].is_borrowed = 0; // Mark as available
    book_count++;
    printf("Book added successfully!\n");
}

// View All Books
void view_books() {
    printf("\n=========================================\n");
    printf("         BOOK MANAGEMENT SYSTEM          \n");
    printf("=========================================\n");
    printf("+----------+------------------------------+----------------------+------------+\n");
    printf("| ID       | Title                        | Author               | Status     |\n");
    printf("+----------+------------------------------+----------------------+------------+\n");
    for (int i = 0; i < book_count; i++) {
        printf("| %-8d | %-28s | %-20s | %-10s |\n",
               books[i].id, books[i].title, books[i].author,
               books[i].is_borrowed ? "Borrowed" : "Available");
    }
    printf("+----------+------------------------------+----------------------+------------+\n");
}

// Edit Book Details
void edit_book() {
    int id;
    printf("Enter the ID of the book to edit: ");
    scanf("%d", &id);

    int index = find_book_by_id(id);
    if (index == -1) {
        printf("Error: Book not found.\n");
        return;
    }

    printf("Editing Book ID: %d\n", books[index].id);
    printf("Enter new title (leave blank to keep current): ");
    getchar(); // Clear input buffer
    char new_title[100];
    fgets(new_title, 100, stdin);
    if (strcmp(new_title, "\n") != 0) {
        new_title[strcspn(new_title, "\n")] = '\0'; // Remove newline
        strcpy(books[index].title, new_title);
    }

    printf("Enter new author (leave blank to keep current): ");
    char new_author[100];
    fgets(new_author, 100, stdin);
    if (strcmp(new_author, "\n") != 0) {
        new_author[strcspn(new_author, "\n")] = '\0'; // Remove newline
        strcpy(books[index].author, new_author);
    }

    printf("Book details updated successfully!\n");
}

// Delete a Book
void delete_book() {
    int id;
    printf("Enter the ID of the book to delete: ");
    scanf("%d", &id);

    int index = find_book_by_id(id);
    if (index == -1) {
        printf("Error: Book not found.\n");
        return;
    }

    for (int i = index; i < book_count - 1; i++) {
        books[i] = books[i + 1];
    }
    book_count--;
    printf("Book deleted successfully!\n");
}

// Sort Books
void sort_books() {
    printf("Sort by:\n1. Title\n2. Author\n3. ID\nEnter choice: ");
    int choice;
    scanf("%d", &choice);

    for (int i = 0; i < book_count - 1; i++) {
        for (int j = i + 1; j < book_count; j++) {
            int compare = 0;
            if (choice == 1) {
                compare = strcmp(books[i].title, books[j].title);
            } else if (choice == 2) {
                compare = strcmp(books[i].author, books[j].author);
            } else if (choice == 3) {
                compare = books[i].id - books[j].id;
            }
            if (compare > 0) {
                Book temp = books[i];
                books[i] = books[j];
                books[j] = temp;
            }
        }
    }
    printf("Books sorted successfully!\n");
}

// Search Books
void search_books() {
    printf("Search by:\n1. Title\n2. Author\nEnter choice: ");
    int choice;
    scanf("%d", &choice);

    char query[100];
    printf("Enter search query: ");
    getchar(); // Clear input buffer
    fgets(query, 100, stdin);
    query[strcspn(query, "\n")] = '\0'; // Remove newline
    to_lowercase(query);

    printf("+----------+------------------------------+----------------------+------------+\n");
    printf("| ID       | Title                        | Author               | Status     |\n");
    printf("+----------+------------------------------+----------------------+------------+\n");
    for (int i = 0; i < book_count; i++) {
        char field[100];
        if (choice == 1) {
            strcpy(field, books[i].title);
        } else if (choice == 2) {
            strcpy(field, books[i].author);
        }
        to_lowercase(field);
        if (strstr(field, query)) {
            printf("| %-8d | %-28s | %-20s | %-10s |\n",
                   books[i].id, books[i].title, books[i].author,
                   books[i].is_borrowed ? "Borrowed" : "Available");
        }
    }
    printf("+----------+------------------------------+----------------------+------------+\n");
}

// Issue a Book
void issue_book() {
    int id;
    printf("Enter the ID of the book to issue: ");
    scanf("%d", &id);

    int index = find_book_by_id(id);
    if (index == -1) {
        printf("Error: Book not found.\n");
        return;
    }

    if (books[index].is_borrowed) {
        printf("Error: Book is already borrowed.\n");
        return;
    }

    books[index].is_borrowed = 1;
    printf("Book issued successfully!\n");
}

// Return a Book
void return_book() {
    int id;
    printf("Enter the ID of the book to return: ");
    scanf("%d", &id);

    int index = find_book_by_id(id);
    if (index == -1) {
        printf("Error: Book not found.\n");
        return;
    }

    if (!books[index].is_borrowed) {
        printf("Error: Book is not currently borrowed.\n");
        return;
    }

    books[index].is_borrowed = 0;
    printf("Book returned successfully!\n");
}

// Find Book by ID
int find_book_by_id(int id) {
    for (int i = 0; i < book_count; i++) {
        if (books[i].id == id) {
            return i;
        }
    }
    return -1;
}

// Convert String to Lowercase
void to_lowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

// User Login or Sign-Up
void login_or_signup() {
    int choice;
    while (1) {
        printf("\n=========================================\n");
        printf("         LIBRARY MANAGEMENT SYSTEM       \n");
        printf("=========================================\n");
        printf("| 1 | Login                             |\n");
        printf("| 2 | Sign Up                           |\n");
        printf("=========================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: login(); return;
            case 2: signup(); return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

// Login Function
void login() {
    char username[50], password[50];
    int attempts = 0;

    while (1) {
        printf("Enter username: ");
        scanf("%s", username);

        printf("Enter password: ");
        get_hidden_password(password);

        if (validate_user(username, password)) {
            strcpy(current_user, username);
            printf("\nLogin successful. Welcome, %s!\n", current_user);
            break; // Exit the loop on successful login
        } else {
            attempts++;
            printf("\nInvalid credentials. Please try again.\n");

            // Offer "Forgot Password" option after invalid credentials
            printf("Forgot Password? Enter '1' to recover your password or '2' to try again: ");
            int choice;
            scanf("%d", &choice);

            if (choice == 1) {
                forgot_password();
                return; // Exit login after password recovery
            }

            // Optional: Limit the number of attempts
            if (attempts >= 3) {
                printf("Too many failed attempts. Exiting...\n");
                exit(1);
            }
        }
    }
}

// Sign-Up Function
void signup() {
    char username[50], password[50], email[100];

    while (1) {
        printf("Enter a new username: ");
        scanf("%s", username);

        // Check if the username is already taken
        if (validate_username(username)) {
            printf("Error: Username '%s' is already taken. Please try a different username.\n", username);
        } else {
            break; // Username is available
        }
    }

    printf("Enter a new password: ");
    get_hidden_password(password);

    printf("Enter your email (for password recovery): ");
    scanf("%s", email);

    save_user(username, password, email);
    printf("\nSign-up successful! You can now log in.\n");
    login();
}

// Validate if Username is Already Taken
int validate_username(const char *username) {
    FILE *file = fopen("users.txt", "r");
    if (!file) {
        return 0; // File doesn't exist, so username is not taken
    }

    char stored_username[50], stored_password[50], stored_email[100];
    while (fscanf(file, "%s %s %s", stored_username, stored_password, stored_email) == 3) {
        if (strcmp(username, stored_username) == 0) {
            fclose(file);
            return 1; // Username is taken
        }
    }

    fclose(file);
    return 0; // Username is not taken
}

// Function to Get Hidden Password
void get_hidden_password(char *password) {
    int i = 0;
    char ch;

    while (1) {
        ch = getch(); // Read a single character without echoing it
        if (ch == '\r') { // Enter key pressed
            password[i] = '\0';
            printf("\n"); // Move to the next line
            break;
        } else if (ch == '\b') { // Backspace key pressed
            if (i > 0) {
                i--;
                printf("\b \b"); // Erase the last character on the console
            }
        } else if (i < 49) { // Limit password length to 49 characters
            password[i++] = ch;
            printf("*"); // Print '*' for each character entered
        }
    }
}

// Validate User Credentials
int validate_user(const char *username, const char *password) {
    FILE *file = fopen("users.txt", "r");
    if (!file) {
        return 0;
    }

    char stored_username[50], stored_password[50];
    while (fscanf(file, "%s %s", stored_username, stored_password) == 2) {
        if (strcmp(username, stored_username) == 0 && strcmp(password, stored_password) == 0) {
            fclose(file);
            return 1; // User found
        }
    }

    fclose(file);
    return 0; // User not found
}

// Save New User Credentials
void save_user(const char *username, const char *password, const char *email) {
    FILE *file = fopen("users.txt", "a");
    if (!file) {
        perror("Error saving user data");
        exit(1);
    }

    fprintf(file, "%s %s %s\n", username, password, email);
    fclose(file);
}

// Forgot Password Function
void forgot_password() {
    char email[100], stored_email[100], stored_username[50], stored_password[50];
    printf("Enter your registered email: ");
    scanf("%s", email);

    FILE *file = fopen("users.txt", "r");
    if (!file) {
        printf("No user data found. Please sign up first.\n");
        return;
    }

    while (fscanf(file, "%s %s %s", stored_username, stored_password, stored_email) == 3) {
        if (strcmp(email, stored_email) == 0) {
            printf("Your username: %s\n", stored_username);
            printf("Your password: %s\n", stored_password);
            fclose(file);
            return;
        }
    }

    fclose(file);
    printf("No account found with the provided email.\n");
}

// Main Function
int main() {
    login_or_signup();
    load_books();
    menu();
    return 0;
}