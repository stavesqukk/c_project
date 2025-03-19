# Library Management System

## Overview
The Library Management System is a C-based application designed to facilitate the management of library resources, including books and users. It provides functionalities for book management, user management, borrowing and returning books, fine calculation, and tracking borrowing history.

## Features
- **Book Management**: Add, delete, update, and search for books.
- **User Management**: Register, update, and remove users (students or faculty).
- **Borrow and Return System**: Users can borrow and return books, with due date management.
- **Fine Calculation**: Automatically calculates fines for late returns.
- **Authentication System**: Login and logout functionalities for both admin and regular users.
- **Search Functionality**: Search for books by title, author, or ID.
- **Sorting and Filtering**: Sort books by title, author, or ID and filter results.
- **Borrowing History**: Users can view their borrowing history.
- **Error Handling**: Validates user input to prevent crashes.

## Data Storage
The application uses binary files for persistent storage:
- `data/books.dat`: Stores book details.
- `data/users.dat`: Stores user details.
- `data/history.dat`: Stores borrowing history.

## Project Structure
```
library-management-system
├── src
│   ├── main.c
│   ├── book_management.c
│   ├── user_management.c
│   ├── borrow_return.c
│   ├── fine_calculation.c
│   ├── authentication.c
│   ├── search.c
│   ├── sorting_filtering.c
│   ├── history.c
│   ├── error_handling.c
│   └── utils.c
├── include
│   ├── book_management.h
│   ├── user_management.h
│   ├── borrow_return.h
│   ├── fine_calculation.h
│   ├── authentication.h
│   ├── search.h
│   ├── sorting_filtering.h
│   ├── history.h
│   ├── error_handling.h
│   └── utils.h
├── data
│   ├── books.dat
│   ├── users.dat
│   └── history.dat
├── Makefile
└── README.md
```

## Setup Instructions
1. Clone the repository or download the project files.
2. Navigate to the project directory.
3. Compile the project using the provided Makefile:
   ```
   make
   ```
4. Run the application:
   ```
   ./library_management_system
   ```

## Usage Guidelines
- Follow the on-screen menu to navigate through different functionalities.
- Ensure to authenticate as an admin or user to access specific features.
- Use the search and filter options to easily find books.
- Keep track of borrowed books and return them on time to avoid fines.

## Optional Features
- Graphical User Interface (GUI) for enhanced user experience.
- Database integration for more robust data management.
- Multiple user roles for differentiated access levels.
- Book reservation system for managing book availability.

## Contributing
Contributions are welcome! Please submit a pull request or open an issue for any suggestions or improvements.