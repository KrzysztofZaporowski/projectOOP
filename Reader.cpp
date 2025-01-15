#include "Reader.h"

// Constructor that initializes a Reader object by passing login, password, and nickname to the base User class
Reader::Reader(std::string login, std::string password, std::string nickname) : User(login, password, nickname){}

// Setter to update the list of borrowed books
void Reader::setBorrowedBooks(std::vector<Item> borrowed){
	borrowedBooks = borrowed;
}

// Setter to update the renting history
void Reader::setRentingHistory(std::vector<Item> history){
	rentingHistory = history;
}

// Getter to retrieve the renting history
std::vector<Reader::Item> Reader::getRentingHistory(){
	return rentingHistory;
}

// Getter to retrieve the list of borrowed books
std::vector<Reader::Item> Reader::getBorrowedBooks(){
	return borrowedBooks;
}

// Method for borrowing a book, checking if available copies exist and calculating due date
Reader::OutItem Reader::borrowBook(Book& book){
	// Check if the book is available for borrowing
	if (book.getAvailableCopies() > 0) {
		// Reduce the number of available copies
		book.setAvailableCopies(book.getAvailableCopies() - 1);

		// Get the current date
		std::time_t now = std::time(nullptr);
		std::tm* local_time = std::localtime(&now);
		int day = local_time->tm_mday;
		int month = local_time->tm_mon + 1; 
		int year = local_time->tm_year + 1900;
		/// There is new logic because there was problem when due date was in next year
		month += 1; // Increment the month
		if (month > 12) { // Adjust year and month if overflow
			month = 1;
			year += 1;
		}

		// Handle edge cases for days
		static const int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
		if (month == 2 && (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))) {
			// Leap year for February
			if (day > 29) day = 29;
		}
		else if (day > daysInMonth[month - 1]) {
			day = daysInMonth[month - 1];
		}

		// Create an output structure to store the borrowed book information and return it
		OutItem out;
		out.login = login;
		out.ISBN = book.getISBN();
		out.title = book.getTitle();
		out.day = day;
		out.month = month;
		out.year = year;

		// Add the book to the borrowed list
		borrowedBooks.push_back({ book.getTitle(), day, month, year });
		std::cout << "Book borrowed until: " << day << "." << month << "." << year << std::endl;
		return out;
	}
	else {
		std::cout << "Borrowed failed!" << std::endl;
		return {}; // Return empty struct if borrowing failed
	}
}

// Method for returning a book, checking overdue dates and updating records
std::pair<Reader::OutItem, Reader::OutItem> Reader::returnBook(Book& book){
	// Get current date
	std::time_t t = std::time(nullptr);
	std::tm* currentDate = std::localtime(&t);
	int currentDay = currentDate->tm_mday;
	int currentMonth = currentDate->tm_mon + 1; 
	int currentYear = currentDate->tm_year + 1900;
	
	// Search for the book in the borrowed list
	auto it = std::find_if(borrowedBooks.begin(), borrowedBooks.end(), [&](const auto& borrowedBook) {
		return static_cast<Item>(borrowedBook).title == book.getTitle();
		});

	// If the book is found, process the return
	if (it != borrowedBooks.end()) {
		int dueDay = static_cast<Item>(*it).day;
		int dueMonth = static_cast<Item>(*it).month;
		int dueYear = static_cast<Item>(*it).year;

		// Create due date struct and calculate the difference from current date
		std::tm dueDate = {};
		dueDate.tm_mday = dueDay;
		dueDate.tm_mon = dueMonth - 1; 
		dueDate.tm_year = dueYear - 1900;

		std::time_t dueTime = std::mktime(&dueDate);
		std::time_t currentTime = std::mktime(currentDate);
		int daysDifference = std::difftime(currentTime, dueTime) / (60 * 60 * 24);
		
		// Create output structs for borrowed and history info
		OutItem outBorrowed, outHistory;
		outBorrowed.login = login;
		outBorrowed.ISBN = book.getISBN();
		outBorrowed.title = book.getTitle();
		outBorrowed.day = dueDay;
		outBorrowed.month = dueMonth;
		outBorrowed.year = dueYear;
		outHistory.login = login;
		outHistory.ISBN = book.getISBN();
		outHistory.title = book.getTitle();
		outHistory.day = currentDay;
		outHistory.month = currentMonth;
		outHistory.year = currentYear;

		// Remove the book from the borrowed list and add it to the renting history
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
		return { outBorrowed, outHistory};
	}
	else {
		std::cout << "Book '" << book.getTitle() << "' not found in borrowed books." << std::endl;
		OutItem outBorrowed, outHistory;
		return { outBorrowed, outHistory };
	}
}

// Method to display all currently borrowed books and their due dates
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

// Method to display the history of rented books
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

// Display basic information about the reader
void Reader::displayInfo(){
	std::cout << "Reader. Login: " << login << ", nickname: " << nickname << std::endl;
}

bool Reader::operator==(const Reader& other) const{
	return login == other.login || nickname == other.nickname;
}
