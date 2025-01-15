#pragma once
#include <iostream>
#include <vector>
#include <iostream>
#include <chrono>
#include "Book.h"
#include "User.h"
#include "Reader.h"
#include "LibraryDB.h"


class MenuHandler{
public:
	MenuHandler();
	void menu();
	void displayCurrentDate();
	void signIn(std::vector<Reader>& readers);
	void readerLogIn(std::vector<Reader>& readers, std::vector<Book>& books, std::vector<LibraryDB::Item>& borrowedBooks, std::vector<LibraryDB::Item>& rentingHistory);
	void readerOperations(std::vector<Book>& books, Reader& reader, std::vector<LibraryDB::Item>& borrowedBooks, std::vector<LibraryDB::Item>& rentingHistory);
	void adminLogIn(std::vector<Reader>& readers, std::vector<Admin>& admins, std::vector<Book>& books);
	void adminOperations(std::vector<Reader>& readers, std::vector<Admin>& admins, std::vector<Book>& books, Admin& admin);
};

