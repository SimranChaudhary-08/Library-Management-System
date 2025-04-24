#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;

void connectDB() {
    conn = mysql_init(NULL);
    if (conn == NULL) {
        fprintf(stderr, "MySQL init failed\n");
        exit(1);
    }

    if (mysql_real_connect(conn, "localhost", "root", "password", "library_db", 0, NULL, 0) == NULL) {
        fprintf(stderr, "Connection failed: %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(1);
    }
}

void addBook() {
    char title[255], author[255];
    getchar();
    printf("Enter Book Title: ");
    fgets(title, sizeof(title), stdin);
    title[strcspn(title, "\n")] = 0;
    printf("Enter Author Name: ");
    fgets(author, sizeof(author), stdin);
    author[strcspn(author, "\n")] = 0;

    char query[600];
    sprintf(query, "INSERT INTO books (title, author) VALUES ('%s', '%s')", title, author);
   
    if (mysql_query(conn, query)) {
        printf("Add failed: %s\n", mysql_error(conn));
    } else {
        printf("Book added successfully.\n");
    }
}

void viewBooks() {
    if (mysql_query(conn, "SELECT * FROM books")) {
        printf("View failed: %s\n", mysql_error(conn));
        return;
    }

    res = mysql_store_result(conn);
    printf("\n%-5s %-30s %-30s %-10s %-20s\n", "ID", "Title", "Author", "Issued", "Borrower");
    printf("-------------------------------------------------------------------------------------------\n");

    while ((row = mysql_fetch_row(res))) {
        printf("%-5s %-30s %-30s %-10s %-20s\n", row[0], row[1], row[2], strcmp(row[3], "0") == 0 ? "No" : "Yes", row[4] ? row[4] : "None");
    }

    mysql_free_result(res);
}

void issueBook() {
    int id;
    char name[100];
    printf("Enter Book ID to issue: ");
    scanf("%d", &id);
    getchar();
    printf("Enter your name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;

    char query[300];
    sprintf(query, "UPDATE books SET is_issued = TRUE, borrower = '%s' WHERE id = %d AND is_issued = FALSE", name, id);

    if (mysql_query(conn, query)) {
        printf("Issue failed: %s\n", mysql_error(conn));
    } else if (mysql_affected_rows(conn) > 0) {
        printf("Book issued to %s.\n", name);
    } else {
        printf("Book not available or already issued.\n");
    }
}

void returnBook() {
    int id;
    printf("Enter Book ID to return: ");
    scanf("%d", &id);

    char query[200];
    sprintf(query, "UPDATE books SET is_issued = FALSE, borrower = NULL WHERE id = %d AND is_issued = TRUE", id);

    if (mysql_query(conn, query)) {
        printf("Return failed: %s\n", mysql_error(conn));
    } else if (mysql_affected_rows(conn) > 0) {
        printf("Book returned successfully.\n");
    } else {
        printf("Book is not currently issued.\n");
    }
}

int main() {
    connectDB();

    int choice;
    while (1) {
        printf("\n=== Library Management System ===\n");
        printf("1. Add Book\n");
        printf("2. View Books\n");
        printf("3. Issue Book\n");
        printf("4. Return Book\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addBook(); break;
            case 2: viewBooks(); break;
            case 3: issueBook(); break;
            case 4: returnBook(); break;
            case 5: mysql_close(conn); exit(0);
            default: printf("Invalid choice.\n");
        }
    }

    return 0;
}

