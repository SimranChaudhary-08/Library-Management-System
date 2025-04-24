# Library Management System in C using MySQL

A basic CLI-based library system built in C that uses MySQL for database operations.

## Features
- Add new books
- View all books
- Issue a book to a student
- Return a book

## Setup Instructions

### 1. MySQL
Run this in MySQL:

```sql
CREATE DATABASE library_db;
USE library_db;

CREATE TABLE books (
    id INT AUTO_INCREMENT PRIMARY KEY,
    title VARCHAR(255),
    author VARCHAR(255),
    is_issued BOOLEAN DEFAULT FALSE,
    borrower VARCHAR(100) DEFAULT NULL
);
## compile and run
gcc library_management.c -o library -lmysqlclient
./library
