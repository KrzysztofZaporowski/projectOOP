#pragma once
#include <vector>
#include <string>
#include <ctime>
#include <chrono>
#include "User.h"
#include "Book.h"


class Reader : public User{
public:
	struct Item {
		std::string title;
		int day;
		int month;
		int year;
	};
	struct OutItem {
		std::string login;
		std::string ISBN;
		std::string title;
		int day = 0;
		int month = 0;
		int year = 0;

		OutItem() : login(""), ISBN(""), title(""), day(0), month(0), year(0) {}
	};
	Reader(std::string login, std::string password, std::string nickname);
	void setBorrowedBooks(std::vector<Item> borrowed);
	void setRentingHistory(std::vector<Item> history);
	std::vector<Item> getRentingHistory();
	std::vector<Item> getBorrowedBooks();
	OutItem borrowBook(Book& book);
	std::pair<OutItem, OutItem> returnBook(Book& book);
	void displayCurrentBorrowedBooks() const;
	void displayBorrowHistory() const;
	void displayInfo() override;
	bool operator==(const Reader& other) const;
private:
	std::vector<Item> borrowedBooks;
	std::vector<Item> rentingHistory;
};

