#pragma once
#include <string>
#include <iostream>

class Book{
public:
    Book(std::string isbn, std::string title, std::string authorName,
        std::string authorLastName, int publishYear, int availableCopies);
    std::string getISBN() const;
    std::string getTitle() const;
    int getPublishYear() const;
    std::string getAuthorName() const;
    std::string getAuthorLastname() const;
    int getAvailableCopies() const;
    void setAvailableCopies(int amount);
    void displayBookInfo() const;
    bool operator==(const Book& other) const;
private:
    std::string isbn;
    std::string title;
    std::string authorName;
    std::string authorLastName;
    int publishYear;
    int availableCopies;
};

