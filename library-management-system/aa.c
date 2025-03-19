#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure for Book
typedef struct {
    int id;
    char title[100];
    char author[100];
    int is_borrowed; // 0 = available, 1 = borrowed
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

// Main Menu
void menu() {
    int choice;
    do {
        printf("\n=== Library Management System ===\n");
        printf("1. Add Book\n");
        printf("2. View Books\n");
        printf("3. Add User\n");
        printf("4. View Users\n");
        printf("5. Borrow Book\n");
        printf("6. Return Book\n");
        printf("7. Sort Books by Title\n");
        printf("8. Filter Books by Availability\n");
        printf("9. Save and Exit\n");
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
            case 9:
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
            default: printf("Invalid choice. Try again.\n");
        }
    } while (1);
}

// Add a Book
void add_book() {
    printf("\nEnter Book ID: ");
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
    book_count++;
    printf("Book added successfully!\n");
}

// View All Books
void view_books() {
    printf("\n=== List of Books ===\n");
    for (int i = 0; i < book_count; i++) {
        printf("ID: %d, Title: %s, Author: %s, Status: %s\n",
               books[i].id, books[i].title, books[i].author,
               books[i].is_borrowed ? "Borrowed" : "Available");
    }
}

// Add a User
void add_user() {
    printf("\nEnter User ID: ");
    scanf("%d", &users[user_count].id);
    printf("Enter User Name: ");
    getchar(); // Clear newline
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
    printf("\n=== List of Users ===\n");
    for (int i = 0; i < user_count; i++) {
        printf("ID: %d, Name: %s, Role: %s\n",
               users[i].id, users[i].name, users[i].role);
    }
}

// Borrow a Book
void borrow_book() {
    int book_id;
    printf("\nEnter Book ID to Borrow: ");
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
    printf("\nEnter Book ID to Return: ");
    scanf("%d", &book_id);
    for (int i = 0; i < book_count; i++) {
        if (books[i].id == book_id) {
            if (!books[i].is_borrowed) {
                printf("Book is already available.\n");
                return;
            }
            books[i].is_borrowed = 0;
            printf("Book returned successfully!\n");
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
    printf("\n=== Available Books ===\n");
    for (int i = 0; i < book_count; i++) {
        if (!books[i].is_borrowed) {
            printf("ID: %d, Title: %s, Author: %s\n",
                   books[i].id, books[i].title, books[i].author);
        }
    }
}

// View Borrowed Books
void view_borrowed_books() {
    printf("\n=== Borrowed Books ===\n");
    for (int i = 0; i < book_count; i++) {
        if (books[i].is_borrowed) {
            printf("ID: %d, Title: %s, Author: %s\n",
                   books[i].id, books[i].title, books[i].author);
        }
    }
}

// Save Data to File
void save_data() {
    FILE *file = fopen("library_data.txt", "w");
    if (!file) {
        perror("Error saving data");
        return;
    }
    printf("Data is being saved to: library_data.txt in the current working directory.\n");
    fprintf(file, "%d\n", book_count);
    for (int i = 0; i < book_count; i++) {
        fprintf(file, "%d\n%s\n%s\n%d\n",
                books[i].id, books[i].title, books[i].author, books[i].is_borrowed);
    }
    fprintf(file, "%d\n", user_count);
    for (int i = 0; i < user_count; i++) {
        fprintf(file, "%d\n%s\n%s\n",
                users[i].id, users[i].name, users[i].role);
    }
    fclose(file);
}

// Load Data from File
void load_data() {
    FILE *file = fopen("library_data.txt", "r");
    if (!file) {
        printf("No previous data found. Starting fresh.\n");
        return;
    }
    fscanf(file, "%d", &book_count);
    for (int i = 0; i < book_count; i++) {
        fscanf(file, "%d\n", &books[i].id);
        fgets(books[i].title, 100, file);
        books[i].title[strcspn(books[i].title, "\n")] = '\0';
        fgets(books[i].author, 100, file);
        books[i].author[strcspn(books[i].author, "\n")] = '\0';
        fscanf(file, "%d\n", &books[i].is_borrowed);
    }
    fscanf(file, "%d", &user_count);
    for (int i = 0; i < user_count; i++) {
        fscanf(file, "%d\n", &users[i].id);
        fgets(users[i].name, 100, file);
        users[i].name[strcspn(users[i].name, "\n")] = '\0';
        fscanf(file, "%s\n", users[i].role);
    }
    fclose(file);
}

// Main Function
int main() {
    load_data();
    menu();
    return 0;
}