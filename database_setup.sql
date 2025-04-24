CREATE DATABASE library_db;
USE library_db;

CREATE TABLE books (
    id INT AUTO_INCREMENT PRIMARY KEY,
    title VARCHAR(255),
    author VARCHAR(255),
    is_issued BOOLEAN DEFAULT FALSE,
    borrower VARCHAR(100) DEFAULT NULL
);

