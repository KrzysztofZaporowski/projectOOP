#pragma once
#include <string>
#include <vector>
#include "User.h"
#include "Reader.h"


class Admin : public User{
public:
	Admin(std::string login, std::string password, std::string nickname);
	bool addUser(std::vector<Reader>& readers, const Reader& reader);
	bool deleteUser(std::vector<Reader>& readers, const Reader& reader);
	void updateBookCopiesNumber(Book& book, int new_amount);
	bool addBook(std::vector<Book>& books, const Book& book);
	bool deleteBook(std::vector<Book>& books, const Book& book);
	bool addAdmin(std::vector<Admin>& admins, const Admin& admin);
	bool deleteAdmin(std::vector<Admin>& admins, const Admin& admin);
	void displayInfo() override;
	bool operator==(const Admin& other) const;
};

