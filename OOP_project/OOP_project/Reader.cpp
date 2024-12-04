#include "Reader.h"

Reader::Reader(std::string login, std::string password, std::string nickname) : User(login, password, nickname){}

void Reader::setBorrowedBooks(std::vector<Item> borrowed){
	borrowedBooks = borrowed;
}

void Reader::setRentingHistory(std::vector<Item> history){
	rentingHistory = history;
}

std::vector<Item> Reader::getRentingHistory(){
	return rentingHistory;
}

std::vector<Item> Reader::getBorrowedBooks(){
	return borrowedBooks;
}

void Reader::borrowBook(Book& book){
	if (book.getAvailableCopies() > 0) {
		book.setAvailableCopies(book.getAvailableCopies() - 1);
		std::time_t now = std::time(0);
		std::tm* local_time = std::localtime(&now);
		int day = local_time->tm_mday;        
		int month = local_time->tm_mon + 1;   
		int year = local_time->tm_year + 1900;
		local_time->tm_mon += 1;
		std::mktime(local_time);

		int newDay = local_time->tm_mday;
		int newMonth = local_time->tm_mon + 1; 
		int newYear = local_time->tm_year + 1900;
		borrowedBooks.push_back({ book.getTitle(), newDay, newMonth, newYear });
		std::cout << "Book borrowed until: " << newDay << "." << newMonth << "." << newYear << std::endl;
	}
	else {
		std::cout << "Borrowed failed!" << std::endl;
	}
}

void Reader::returnBook(Book& book){
	std::time_t t = std::time(nullptr);
	std::tm* currentDate = std::localtime(&t);
	int currentDay = currentDate->tm_mday;
	int currentMonth = currentDate->tm_mon + 1; 
	int currentYear = currentDate->tm_year + 1900;
	
	auto it = std::find_if(borrowedBooks.begin(), borrowedBooks.end(), [&](const auto& borrowedBook) {
		return std::get<0>(borrowedBook) == book.getTitle();
		});
	if (it != borrowedBooks.end()) {
		int dueDay = static_cast<Item>(*it).day;
		int dueMonth = static_cast<Item>(*it).month;
		int dueYear = static_cast<Item>(*it).month;

		std::tm dueDate = {};
		dueDate.tm_mday = dueDay;
		dueDate.tm_mon = dueMonth - 1; 
		dueDate.tm_year = dueYear - 1900;

		std::time_t dueTime = std::mktime(&dueDate);
		std::time_t currentTime = std::mktime(currentDate);

		int daysDifference = std::difftime(currentTime, dueTime) / (60 * 60 * 24);

		borrowedBooks.erase(it); 
		rentingHistory.push_back({ book.getTitle(), currentDay, currentMonth, currentYear });
		std::cout << "Book '" << book.getTitle() << "' returned successfully." << std::endl;
		if (daysDifference < 0){
			std::cout << "Congratulation! You returned this book: " << -daysDifference << " days before due date which was: " <<
				dueDay << "." << dueMonth << "." << dueYear << std::endl;
		}
		else if (daysDifference > 0){
			std::cout << "This isn't good! You returned this book: " << daysDifference << " days after due date which was: " <<
				dueDay << "." << dueMonth << "." << dueYear << std::endl;
		}
		else {
			std::cout << "You returned this book on due day. Good job!" << std::endl;
		}

		book.setAvailableCopies(book.getAvailableCopies() + 1);
	}
	else {
		std::cout << "Book '" << book.getTitle() << "' not found in borrowed books." << std::endl;
	}
}

void Reader::displayCurrentBorrowedBooks() const{
	if (!borrowedBooks.empty()) {
		for (const auto& book : borrowedBooks) {
			std::cout << "Title: " << book.title << ", due date: " << book.day << "." << book.month << "." << book.year
				<< std::endl;
		}
	}
	else {
		std::cout << "Currentlly you don't rent any book." << std::endl;
	}
}

void Reader::displayBorrowHistory() const{
	if (!rentingHistory.empty()) {
		for (const auto& book : rentingHistory) {
			std::cout << "Title: " << book.title << ", you returned this book - return date: " << book.day << "." << book.month << "." << book.year
				<< std::endl;
		}
	}
	else {
		std::cout << "Your renting history is empty." << std::endl;
	}
}

bool Reader::operator==(const Reader& other) const{
	return login == other.login && nickname == other.nickname;
}
