#pragma once
#include <iostream>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>
#include <string>
#include "User.h"
#include "Book.h"
#include "Reader.h"
#include "Admin.h"

struct Item {
	std::string login;
	std::string isbn;
	std::string title;
	int day;
	int month;
	int year;
};

class LibraryDB{
public:
	LibraryDB(std::string username, std::string password, std::string databaseName, std::string tableBooksName, std::string tableReadersName,
		std::string tableAdminsName, std::string rentingHistorytableName, std::string borrowedBooksTableName);
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

