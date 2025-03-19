CC = gcc
CFLAGS = -Iinclude -Wall -Wextra
SRC = src/main.c src/book_management.c src/user_management.c src/borrow_return.c src/fine_calculation.c src/authentication.c src/search.c src/sorting_filtering.c src/history.c src/error_handling.c src/utils.c
OBJ = $(SRC:.c=.o)
EXEC = library_management_system

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)

run: $(EXEC)
	./$(EXEC)