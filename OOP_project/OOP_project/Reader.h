#pragma once
#include <vector>
#include <tuple>
#include <string>
#include <ctime>
#include "User.h"
#include "Book.h"

struct Item {
	std::string title; 
	int day; 
	int month; 
	int year;
};

class Reader : public User{
public:
	Reader(std::string login, std::string password, std::string nickname);
	void setBorrowedBooks(std::vector<Item> borrowed);
	void setRentingHistory(std::vector<Item> history);
	std::vector<Item> getRentingHistory();
	std::vector<Item> getBorrowedBooks();
	void borrowBook(Book& book);
	void returnBook(Book& book);
	void displayCurrentBorrowedBooks() const;
	void displayBorrowHistory() const;
	bool operator==(const Reader& other) const;
private:
	std::vector<Item> borrowedBooks;
	std::vector<Item> rentingHistory;
};

