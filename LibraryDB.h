#pragma once
#include <iostream>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>
#include <string>
#include <mutex>
#include "User.h"
#include "Book.h"
#include "Reader.h"
#include "Admin.h"



class LibraryDB{
public:
	struct Item {
		std::string login;
		std::string isbn;
		std::string title;
		int day;
		int month;
		int year;
		bool operator==(const Item& other) const {
			return login == other.login &&
				isbn == other.isbn &&
				title == other.title &&
				day == other.day &&
				month == other.month &&
				year == other.year;
		}
	};

	// Get the Singleton instance with default MySQL connection data
	static LibraryDB& getInstance(const std::string& username = "root",
		const std::string& password = "Krzysiu13",
		const std::string& databaseName = "library",
		const std::string& tableBooksName = "books",
		const std::string& tableReadersName = "readers",
		const std::string& tableAdminsName = "admins",
		const std::string& rentingHistoryTableName = "renting_history",
		const std::string& borrowedBooksTableName = "borrowed_books");

	void loadData();
	std::vector<Book> getBooks();
	std::vector<Admin> getAdmins();
	std::vector<Reader> getReaders();
	std::vector<Item> getRentingHistory();
	std::vector<Item> getBorrowedBooks();
	void saveBooksToDB(std::vector<Book>& books);
	void saveAdminsToDB(std::vector<Admin>& admins);
	void saveReadersToDB(std::vector<Reader>& readers);
	void saveToRentingHistoryTable(std::vector<Item>& renting);
	void saveToBorrowedBooksTable(std::vector<Item>& borrowed);
private:
	// Private constructor
	LibraryDB(const std::string& username,
		const std::string& password,
		const std::string& databaseName,
		const std::string& tableBooksName,
		const std::string& tableReadersName,
		const std::string& tableAdminsName,
		const std::string& rentingHistoryTableName,
		const std::string& borrowedBooksTableName);

	// Prevent copying
	LibraryDB(const LibraryDB&) = delete;
	LibraryDB& operator=(const LibraryDB&) = delete;

	// Singleton instance
	static std::unique_ptr<LibraryDB> instance;
	static std::once_flag initFlag;

	std::vector<Book> books;
	std::vector<Admin> admins;
	std::vector<Reader> readers;
	std::vector<Item> rentingHistory;
	std::vector<Item> borrowedBooks;
	std::string username;
	std::string password;
	std::string databaseName;
	std::string tableBooksName;
	std::string tableReadersName;
	std::string tableAdminsName;
	std::string rentingHistorytableName;
	std::string borrowedBooksTableName;
};