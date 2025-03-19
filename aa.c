#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure for Book
typedef struct {
    int id;
    char title[100];
    char author[100];
    int is_borrowed; // 0 = available, 1 = borrowed
    int reserved_by; // ID of the user who reserved the book
} Book;

// Structure for User
typedef struct {
    int id;
    char name[100];
    char role[20]; // "student" or "faculty"
} User;

// Global Arrays for Books and Users
Book books[100];
User users[100];
int book_count = 0;
int user_count = 0;

// Function Prototypes
void add_book();
void view_books();
void add_user();
void view_users();
void borrow_book();
void return_book();
void sort_books_by_title();
void filter_books_by_availability();
void save_data();
void load_data();
void view_borrowed_books();
void delete_recent_book();

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
        printf("| 3  | Add User                       |\n");
        printf("| 4  | View Users                     |\n");
        printf("| 5  | Borrow Book                    |\n");
        printf("| 6  | Return Book                    |\n");
        printf("| 7  | Sort Books by Title            |\n");
        printf("| 8  | Filter Books by Availability   |\n");
        printf("| 9  | View Borrowed Books            |\n");
        printf("| 10 | Save and Exit                  |\n");
        printf("| 11 | Delete Recently Added Book     |\n");
        printf("=========================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: add_book(); break;
            case 2: view_books(); break;
            case 3: add_user(); break;
            case 4: view_users(); break;
            case 5: borrow_book(); break;
            case 6: return_book(); break;
            case 7: sort_books_by_title(); break;
            case 8: filter_books_by_availability(); break;
            case 9: view_borrowed_books(); break;
            case 10:
                printf("Are you sure you want to save and exit? (y/n): ");
                char confirm;
                scanf(" %c", &confirm);
                if (confirm == 'y' || confirm == 'Y') {
                    save_data();
                    printf("Data saved. Exiting...\n");
                    exit(0);
                } else {
                    printf("Returning to menu...\n");
                }
                break;
            case 11: delete_recent_book(); break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while (1);
}

// Add a Book
void add_book() {
    printf("\n=========================================\n");
    printf("             ADD A NEW BOOK              \n");
    printf("=========================================\n");
    printf("Enter Book ID: ");
    scanf("%d", &books[book_count].id);

    // Clear the input buffer
    while (getchar() != '\n');

    printf("Enter Book Title: ");
    fgets(books[book_count].title, 100, stdin);
    books[book_count].title[strcspn(books[book_count].title, "\n")] = '\0'; // Remove newline

    printf("Enter Book Author: ");
    fgets(books[book_count].author, 100, stdin);
    books[book_count].author[strcspn(books[book_count].author, "\n")] = '\0'; // Remove newline

    books[book_count].is_borrowed = 0; // Mark as available
    books[book_count].reserved_by = 0; // No reservation
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

// Add a User
void add_user() {
    printf("\n=========================================\n");
    printf("             ADD A NEW USER              \n");
    printf("=========================================\n");
    printf("Enter User ID: ");
    scanf("%d", &users[user_count].id);

    // Clear the input buffer
    while (getchar() != '\n');

    printf("Enter User Name: ");
    fgets(users[user_count].name, 100, stdin);
    users[user_count].name[strcspn(users[user_count].name, "\n")] = '\0'; // Remove newline

    do {
        printf("Enter User Role (student/faculty): ");
        scanf("%s", users[user_count].role);
        if (strcmp(users[user_count].role, "student") != 0 && strcmp(users[user_count].role, "faculty") != 0) {
            printf("Invalid role. Please enter 'student' or 'faculty'.\n");
        } else {
            break;
        }
    } while (1);

    user_count++;
    printf("User added successfully!\n");
}

// View All Users
void view_users() {
    printf("\n=========================================\n");
    printf("         USER MANAGEMENT SYSTEM          \n");
    printf("=========================================\n");
    printf("+----------+----------------------+------------+\n");
    printf("| ID       | Name                 | Role       |\n");
    printf("+----------+----------------------+------------+\n");
    for (int i = 0; i < user_count; i++) {
        printf("| %-8d | %-20s | %-10s |\n",
               users[i].id, users[i].name, users[i].role);
    }
    printf("+----------+----------------------+------------+\n");
}

// Borrow a Book
void borrow_book() {
    int book_id;
    printf("\n=========================================\n");
    printf("             BORROW A BOOK               \n");
    printf("=========================================\n");
    printf("Enter Book ID to Borrow: ");
    scanf("%d", &book_id);
    for (int i = 0; i < book_count; i++) {
        if (books[i].id == book_id) {
            if (books[i].is_borrowed) {
                printf("Book is already borrowed.\n");
                return;
            }
            books[i].is_borrowed = 1;
            printf("Book borrowed successfully!\n");
            return;
        }
    }
    printf("Book not found.\n");
}

// Return a Book
void return_book() {
    int book_id;
    printf("\n=========================================\n");
    printf("             RETURN A BOOK               \n");
    printf("=========================================\n");
    printf("Enter Book ID to Return: ");
    scanf("%d", &book_id);
    for (int i = 0; i < book_count; i++) {
        if (books[i].id == book_id) {
            if (!books[i].is_borrowed) {
                printf("Book is already available.\n");
                return;
            }
            books[i].is_borrowed = 0;
            if (books[i].reserved_by != 0) {
                printf("Book returned successfully! It is now reserved for User ID: %d.\n", books[i].reserved_by);
                books[i].reserved_by = 0; // Clear reservation after notifying
            } else {
                printf("Book returned successfully!\n");
            }
            return;
        }
    }
    printf("Book not found.\n");
}

// Sort Books by Title
void sort_books_by_title() {
    for (int i = 0; i < book_count - 1; i++) {
        for (int j = i + 1; j < book_count; j++) {
            if (strcmp(books[i].title, books[j].title) > 0) {
                Book temp = books[i];
                books[i] = books[j];
                books[j] = temp;
            }
        }
    }
    printf("Books sorted by title successfully!\n");
}

// Filter Books by Availability
void filter_books_by_availability() {
    printf("\n=========================================\n");
    printf("         AVAILABLE BOOKS LIST            \n");
    printf("=========================================\n");
    printf("%-10s %-30s %-20s\n", "ID", "Title", "Author");
    printf("=========================================\n");
    for (int i = 0; i < book_count; i++) {
        if (!books[i].is_borrowed) {
            printf("%-10d %-30s %-20s\n",
                   books[i].id, books[i].title, books[i].author);
        }
    }
    printf("=========================================\n");
}

// View Borrowed Books
void view_borrowed_books() {
    printf("\n=========================================\n");
    printf("         BORROWED BOOKS LIST             \n");
    printf("=========================================\n");
    printf("+----------+------------------------------+----------------------+------------+\n");
    printf("| ID       | Title                        | Author               | Status     |\n");
    printf("+----------+------------------------------+----------------------+------------+\n");
    for (int i = 0; i < book_count; i++) {
        if (books[i].is_borrowed) {
            printf("| %-8d | %-28s | %-20s |\n",
                   books[i].id, books[i].title, books[i].author);
        }
    }
    printf("+----------+------------------------------+----------------------+------------+\n");
}

// Save Data to File
void save_data() {
    FILE *file = fopen("library_data.txt", "w");
    if (!file) {
        perror("Error saving data");
        return;
    }
    if (fprintf(file, "%d\n", book_count) < 0) {
        perror("Error writing book count");
        fclose(file);
        return;
    }
    for (int i = 0; i < book_count; i++) {
        if (fprintf(file, "%d\n%s\n%s\n%d\n%d\n",
                    books[i].id, books[i].title, books[i].author,
                    books[i].is_borrowed, books[i].reserved_by) < 0) {
            perror("Error writing book data");
            fclose(file);
            return;
        }
    }
    if (fprintf(file, "%d\n", user_count) < 0) {
        perror("Error writing user count");
        fclose(file);
        return;
    }
    for (int i = 0; i < user_count; i++) {
        if (fprintf(file, "%d\n%s\n%s\n",
                    users[i].id, users[i].name, users[i].role) < 0) {
            perror("Error writing user data");
            fclose(file);
            return;
        }
    }
    fclose(file);
    printf("Data saved successfully!\n");
}

// Load Data from File
void load_data() {
    FILE *file = fopen("library_data.txt", "r");
    if (!file) {
        printf("No previous data found. Starting fresh.\n");
        return;
    }
    if (fscanf(file, "%d", &book_count) != 1) {
        printf("Error reading book count. Starting fresh.\n");
        fclose(file);
        return;
    }
    for (int i = 0; i < book_count; i++) {
        if (fscanf(file, "%d\n", &books[i].id) != 1 ||
            fgets(books[i].title, 100, file) == NULL ||
            fgets(books[i].author, 100, file) == NULL ||
            fscanf(file, "%d\n%d\n", &books[i].is_borrowed, &books[i].reserved_by) != 2) {
            printf("Error reading book data. Starting fresh.\n");
            fclose(file);
            return;
        }
        books[i].title[strcspn(books[i].title, "\n")] = '\0';
        books[i].author[strcspn(books[i].author, "\n")] = '\0';
    }
    if (fscanf(file, "%d", &user_count) != 1) {
        printf("Error reading user count. Starting fresh.\n");
        fclose(file);
        return;
    }
    for (int i = 0; i < user_count; i++) {
        if (fscanf(file, "%d\n", &users[i].id) != 1 ||
            fgets(users[i].name, 100, file) == NULL ||
            fscanf(file, "%s\n", users[i].role) != 1) {
            printf("Error reading user data. Starting fresh.\n");
            fclose(file);
            return;
        }
        users[i].name[strcspn(users[i].name, "\n")] = '\0';
    }
    fclose(file);
}

// Delete Recently Added Book
void delete_recent_book() {
    if (book_count == 0) {
        printf("\nNo books available to delete.\n");
        return;
    }

    printf("\n=========================================\n");
    printf("         DELETE RECENTLY ADDED BOOK       \n");
    printf("=========================================\n");
    printf("The following book will be deleted:\n");
    printf("ID: %d, Title: %s, Author: %s\n",
           books[book_count - 1].id, books[book_count - 1].title, books[book_count - 1].author);

    // Ask for confirmation on the same line
    printf("Are you sure you want to delete this book? (y/n): ");
    char confirm;
    scanf(" %c", &confirm);

    if (confirm == 'y' || confirm == 'Y') {
        book_count--; // Decrease the book count to remove the last book
        printf("The book has been deleted successfully.\n");
    } else {
        printf("Deletion canceled. Returning to menu.\n");
    }
}

// Main Function
int main() {
    load_data();
    menu();
    return 0;
}