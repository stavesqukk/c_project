#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h> // For getch()
#include <time.h>

// Structure for Book
typedef struct {
    int id;
    char title[100];
    char author[100];
    int is_borrowed; // 0 = available, 1 = borrowed
    char borrower[50]; // Name of the borrower
    char due_date[11]; // Format: YYYY-MM-DD
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
int validate_user(const char *username, const char *password, int role);
void save_user(const char *username, const char *password, const char *email);
int find_book_by_id(int id);
void to_lowercase(char *str);
void get_hidden_password(char *password);
int validate_username(const char *username);
void forgot_password();
void generate_statistics();
void user_menu();
void export_data_to_file();
void backup_data();
void restore_data();
int calculate_fine(const char *due_date);
void save_user_with_role(const char *username, const char *password, const char *email, int role);
void save_admin(const char *username, const char *password, const char *email);
void preload_books();

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"

// Print Colored Menu
void print_colored_menu() {
    printf(GREEN "=========================================\n" RESET);
    printf(YELLOW "         LIBRARY MANAGEMENT SYSTEM       \n" RESET);
    printf(GREEN "=========================================\n" RESET);
    printf("| " BLUE "1" RESET " | Login                             |\n");
    printf("| " BLUE "2" RESET " | Sign Up                           |\n");
    printf(GREEN "=========================================\n" RESET);
}

// Main Menu
void menu() {
    int choice;
    do {
        printf(GREEN "\n=========================================\n" RESET);
        printf(YELLOW "         LIBRARY MANAGEMENT SYSTEM       \n" RESET);
        printf(GREEN "=========================================\n" RESET);
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
        printf("| 9  | Generate Statistics            |\n");
        printf("| 10 | Export Data to File            |\n");
        printf("| 11 | Backup Data                    |\n");
        printf("| 12 | Restore Data                   |\n");
        printf("| 13 | Save and Exit                  |\n");
        printf(GREEN "=========================================\n" RESET);
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
            case 9: generate_statistics(); break;
            case 10: export_data_to_file(); break;
            case 11: backup_data(); break;
            case 12: restore_data(); break;
            case 13:
                save_books();
                printf(GREEN "Data saved. Exiting...\n" RESET);
                exit(0);
            default:
                printf(RED "Invalid choice. Try again.\n" RESET);
        }
    } while (1);
}

// Load Books from File
void load_books() {
    FILE *file = fopen("books.txt", "r");
    if (!file) {
        printf("No previous book data found. Starting fresh.\n");
        return;
    }

    book_count = 0;
    while (!feof(file)) {
        char line[256]; // Buffer to read each line
        if (fgets(line, sizeof(line), file)) {
            // Parse the line
            int fields = sscanf(line, "%d,%99[^,],%99[^,],%d,%49[^,],%10[^\n]",
                                &books[book_count].id,
                                books[book_count].title,
                                books[book_count].author,
                                &books[book_count].is_borrowed,
                                books[book_count].borrower,
                                books[book_count].due_date);

            // Handle cases with missing Borrower and Due Date
            if (fields >= 4) { // At least ID, Title, Author, and Status must be present
                if (fields < 5) {
                    strcpy(books[book_count].borrower, "N/A");
                }
                if (fields < 6) {
                    strcpy(books[book_count].due_date, "N/A");
                }

                // Debug print to verify loaded data
                printf("DEBUG: Loaded book: ID=%d, Title=%s, Author=%s, Status=%d, Borrower=%s, Due Date=%s\n",
                       books[book_count].id, books[book_count].title, books[book_count].author,
                       books[book_count].is_borrowed, books[book_count].borrower, books[book_count].due_date);

                book_count++;
            } else {
                printf("DEBUG: Skipping invalid line: %s", line);
            }
        }
    }

    fclose(file);
    printf("Books loaded successfully from books.txt! Total books: %d\n", book_count);
}

// Save Books to File
void save_books() {
    FILE *file = fopen("books.txt", "w");
    if (!file) {
        perror("Error saving book data");
        return;
    }

    for (int i = 0; i < book_count; i++) {
        fprintf(file, "%d,%s,%s,%d,%s\n",
                books[i].id,
                books[i].title,
                books[i].author,
                books[i].is_borrowed,
                books[i].is_borrowed ? books[i].due_date : "");
    }

    fclose(file);
    printf("Books saved successfully to books.txt!\n");
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

    printf("Enter Book Title (max 99 characters): ");
    getchar(); // Clear input buffer
    fgets(books[book_count].title, 100, stdin);
    books[book_count].title[strcspn(books[book_count].title, "\n")] = '\0'; // Remove newline

    printf("Enter Book Author (max 99 characters): ");
    fgets(books[book_count].author, 100, stdin);
    books[book_count].author[strcspn(books[book_count].author, "\n")] = '\0'; // Remove newline

    books[book_count].is_borrowed = 0; // Mark as available
    book_count++;
    printf("Book added successfully!\n");
}

// View All Books
void view_books() {
    if (book_count == 0) {
        printf("No books available in the library.\n");
        return;
    }

    printf("\n=========================================\n");
    printf("         BOOK MANAGEMENT SYSTEM          \n");
    printf("=========================================\n");
    printf("+----------+------------------------------+----------------------+------------+-----------------+------------+\n");
    printf("| ID       | Title                        | Author               | Status     | Borrower        | Due Date   |\n");
    printf("+----------+------------------------------+----------------------+------------+-----------------+------------+\n");
    for (int i = 0; i < book_count; i++) {
        printf("| %-8d | %-28s | %-20s | %-10s | %-15s | %-10s |\n",
               books[i].id, 
               books[i].title[0] ? books[i].title : "N/A", // Handle empty title
               books[i].author[0] ? books[i].author : "N/A", // Handle empty author
               books[i].is_borrowed ? "Borrowed" : "Available",
               books[i].is_borrowed ? books[i].borrower : "N/A",
               books[i].is_borrowed ? books[i].due_date : "N/A");
    }
    printf("+----------+------------------------------+----------------------+------------+-----------------+------------+\n");
    printf("Total books to display: %d\n", book_count);
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
    printf("Sort by:\n1. Title\n2. Author\n3. ID\n4. Status\nEnter choice: ");
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
            } else if (choice == 4) {
                compare = books[i].is_borrowed - books[j].is_borrowed;
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
    query[strcspn(query, "\n")] = '\0'; // Remove newline character

    // Convert the query to lowercase for case-insensitive search
    to_lowercase(query);

    int found = 0; // Flag to track if any book matches the query

    printf("+----------+------------------------------+----------------------+------------+\n");
    printf("| ID       | Title                        | Author               | Status     |\n");
    printf("+----------+------------------------------+----------------------+------------+\n");

    for (int i = 0; i < book_count; i++) {
        char field[100];

        // Search by Title or Author based on user choice
        if (choice == 1) {
            strcpy(field, books[i].title);
        } else if (choice == 2) {
            strcpy(field, books[i].author);
        } else {
            printf("Invalid choice. Please try again.\n");
            return;
        }

        // Convert the field to lowercase for case-insensitive comparison
        to_lowercase(field);

        // Check if the query is a substring of the field
        if (strstr(field, query)) {
            printf("| %-8d | %-28s | %-20s | %-10s |\n",
                   books[i].id, books[i].title, books[i].author,
                   books[i].is_borrowed ? "Borrowed" : "Available");
            found = 1; // Mark that a match was found
        }
    }

    printf("+----------+------------------------------+----------------------+------------+\n");

    if (!found) {
        printf("No books found matching the query.\n");
    }
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

    printf("Enter due date (YYYY-MM-DD): ");
    scanf("%s", books[index].due_date);

    books[index].is_borrowed = 1;
    printf("Book issued successfully! Due date: %s\n", books[index].due_date);
}

// Return a Book
void return_book() {
    int id;
    printf("Enter the ID of the book to return: ");
    scanf("%d", &id);

    // Find the book by ID
    int index = find_book_by_id(id);
    if (index == -1) {
        printf("Error: Book with ID %d not found.\n", id);
        return;
    }

    // Check if the book is borrowed
    if (!books[index].is_borrowed) {
        printf("Error: This book is not currently borrowed.\n");
        return;
    }

    // Calculate fine if the book is overdue
    int fine = calculate_fine(books[index].due_date);
    if (fine > 0) {
        printf("You have a fine of $%d for late return.\n", fine);
        return; // Do not proceed to mark the book as returned
    }

    // Mark the book as returned
    books[index].is_borrowed = 0;
    strcpy(books[index].borrower, "N/A");
    strcpy(books[index].due_date, "N/A");

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
        print_colored_menu();
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
    int role; // 1 for Admin, 2 for User

    printf(YELLOW "Select Role:\n" RESET);
    printf(GREEN "1. Admin\n" RESET);
    printf(BLUE "2. User\n" RESET);
    printf("Enter your choice: ");
    scanf("%d", &role);

    if (role != 1 && role != 2) {
        printf(RED "Invalid role selected. Exiting...\n" RESET);
        exit(1);
    }

    while (1) {
        printf("Enter username: ");
        scanf("%s", username);

        printf("Enter password: ");
        get_hidden_password(password);

        if (validate_user(username, password, role)) {
            strcpy(current_user, username);
            printf(GREEN "\nLogin successful. Welcome, %s!\n" RESET, current_user);

            // Redirect to the appropriate menu based on the role
            if (role == 1) {
                printf(GREEN "Logged in as Admin.\n" RESET);
                menu(); // Admin menu
            } else if (role == 2) {
                printf(BLUE "Logged in as User.\n" RESET);
                user_menu(); // User menu
            }
        } else {
            attempts++;
            printf(RED "\nInvalid credentials. Please try again.\n" RESET);

            if (attempts >= 3) {
                printf(RED "Too many failed attempts. Exiting...\n" RESET);
                exit(1);
            }
        }
    }
}

// Sign-Up Function
void signup() {
    char username[50], password[50], email[100];
    int role; // 1 for Admin, 2 for User

    while (1) {
        printf("Enter a new username: ");
        scanf("%s", username);

        // Check if the username is already taken
        if (validate_username(username)) {
            printf(RED "Error: Username '%s' is already taken. Please try a different username.\n" RESET, username);
        } else {
            break; // Username is available
        }
    }

    printf("Enter a new password: ");
    get_hidden_password(password);

    printf("Enter your email (for password recovery): ");
    scanf("%s", email);

    // Prompt the user to select their role
    printf(YELLOW "Select Role:\n" RESET);
    printf(GREEN "1. Admin\n" RESET);
    printf(BLUE "2. User\n" RESET);
    printf("Enter your choice: ");
    scanf("%d", &role);

    if (role != 1 && role != 2) {
        printf(RED "Invalid role selected. Please try again.\n" RESET);
        return;
    }

    // Save the user credentials to the appropriate file
    if (role == 1) {
        save_admin(username, password, email);
    } else {
        save_user(username, password, email);
    }

    printf(GREEN "\nSign-up successful! You can now log in.\n" RESET);
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
int validate_user(const char *username, const char *password, int role) {
    FILE *file;
    if (role == 1) {
        file = fopen("admins.txt", "r"); // Check in admins.txt for Admins
    } else {
        file = fopen("users.txt", "r"); // Check in users.txt for Users
    }

    if (!file) {
        return 0;
    }

    char stored_username[50], stored_password[50], stored_email[100];
    while (fscanf(file, "%s %s %s", stored_username, stored_password, stored_email) == 3) {
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

// Save New User Credentials with Role
void save_user_with_role(const char *username, const char *password, const char *email, int role) {
    FILE *file = fopen("users.txt", "a");
    if (!file) {
        perror("Error saving user data");
        exit(1);
    }

    // Save the role as "Admin" or "User"
    const char *role_str = (role == 1) ? "Admin" : "User";
    fprintf(file, "%s %s %s %s\n", username, password, email, role_str);
    fclose(file);
}

// Save Admin Credentials
void save_admin(const char *username, const char *password, const char *email) {
    FILE *file = fopen("admins.txt", "a");
    if (!file) {
        perror("Error saving admin data");
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

// Generate Statistics
void generate_statistics() {
    int total_books = book_count;
    int issued_books = 0, available_books = 0;

    for (int i = 0; i < book_count; i++) {
        if (books[i].is_borrowed) {
            issued_books++;
        } else {
            available_books++;
        }
    }

    printf("\nStatistics:\n");
    printf("Total Books: %d\n", total_books);
    printf("Issued Books: %d\n", issued_books);
    printf("Available Books: %d\n", available_books);
}

// User Menu Function
void user_menu() {
    int choice;
    do {
        printf(GREEN "\n=========================================\n" RESET);
        printf(YELLOW "               USER MENU                 \n" RESET);
        printf(GREEN "=========================================\n" RESET);
        printf("| 1 | View Books                        |\n");
        printf("| 2 | Search Books                      |\n");
        printf("| 3 | Issue Book                        |\n");
        printf("| 4 | Return Book                       |\n");
        printf("| 5 | Exit                              |\n");
        printf(GREEN "=========================================\n" RESET);
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: view_books(); break;
            case 2: search_books(); break;
            case 3: issue_book(); break;
            case 4: return_book(); break;
            case 5: save_books(); exit(0);
            default: printf(RED "Invalid choice. Try again.\n" RESET);
        }
    } while (1);
}

// Export Data to File
void export_data_to_file() {
    FILE *file = fopen("report.txt", "w");
    if (!file) {
        perror("Error creating report file");
        return;
    }

    fprintf(file, "ID,Title,Author,Status\n");
    for (int i = 0; i < book_count; i++) {
        fprintf(file, "%d,%s,%s,%s\n", books[i].id, books[i].title, books[i].author,
                books[i].is_borrowed ? "Borrowed" : "Available");
    }

    fclose(file);
    printf("Data exported successfully to report.txt\n");
}

// Backup Data to File
void backup_data() {
    FILE *file = fopen("backup.dat", "wb");
    if (!file) {
        perror("Error creating backup file");
        return;
    }

    fwrite(&book_count, sizeof(int), 1, file);
    fwrite(books, sizeof(Book), book_count, file);
    fclose(file);
    printf("Backup created successfully!\n");
}

// Restore Data from Backup
void restore_data() {
    FILE *file = fopen("backup.dat", "rb");
    if (!file) {
        printf("No backup file found.\n");
        return;
    }

    fread(&book_count, sizeof(int), 1, file);
    fread(books, sizeof(Book), book_count, file);
    fclose(file);
    printf("Data restored successfully from backup!\n");
}

// Calculate Fine
int calculate_fine(const char *due_date) {
    // Parse the due date (YYYY-MM-DD) manually
    int year, month, day;
    if (sscanf(due_date, "%d-%d-%d", &year, &month, &day) != 3) {
        printf("Error: Invalid due date format.\n");
        return 0;
    }

    // Get the current date
    time_t now = time(NULL);
    struct tm *current_date = localtime(&now);

    // Calculate the difference in days
    struct tm due = {0};
    due.tm_year = year - 1900; // tm_year is years since 1900
    due.tm_mon = month - 1;    // tm_mon is 0-based
    due.tm_mday = day;

    time_t due_time = mktime(&due);
    if (due_time == -1) {
        printf("Error: Unable to calculate due date.\n");
        return 0;
    }

    double difference = difftime(now, due_time) / (60 * 60 * 24); // Difference in days

    if (difference > 0) {
        return (int)difference * 10; // Fine: $10 per day
    }
    return 0; // No fine
}

// Main Function
int main() {
    load_books(); // Load books from books.txt
    login_or_signup(); // Handle login or sign-up
    return 0; // Exit after login or sign-up
}