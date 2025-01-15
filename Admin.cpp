#include "Admin.h"

// Constructor for the Admin class
Admin::Admin(std::string login, std::string password, std::string nickname) : User(login, password, nickname){}

bool Admin::addUser(std::vector<Reader>& readers, const Reader& reader){
	for (const auto& existingReader : readers) {
		if (existingReader == reader) {
			std::cout << "Reader with this login already exists!" << std::endl;
			return false;
		}
	}
	// Add the new user to the vector and display a message
	readers.push_back(reader);
	std::cout << "Reader " << reader.getNick() << " was added!" << std::endl;
	return true; // Return true if the user was successfully added
}

bool Admin::deleteUser(std::vector<Reader>& readers, const Reader& reader){
	for (const auto& existingReader : readers) {
		if (reader == existingReader) {
			// Remove the user using std::remove and display a message
			readers.erase(std::remove(readers.begin(), readers.end(), reader), readers.end());
			std::cout << "Reader " << reader.getNick() << " has been succesfully deleted!" << std::endl;
			return true; // Return true if the user was successfully deleted
		}
	}
	// If the user is not found, display an error message
	std::cout << "User " << reader.getNick() << " can't be deleted. Are you sure you gave good data, because this reader doesn't exit in our databse." << std::endl;
	return false;
}

void Admin::updateBookCopiesNumber(Book& book, int new_amount){
	// Validation - check if the new number of copies is greater than 0
	if (new_amount > 0) {
		book.setAvailableCopies(new_amount); // Update the number of copies
		std::cout << "Update book: " << book.getTitle() << " copies amount to: " << new_amount << std::endl;
	}
	else {
		// Display an error message if the number of copies is invalid
		std::cout << "It is not possible to have less than 0 copies!" << std::endl;
	}
}

bool Admin::addBook(std::vector<Book>& books, const Book& book){
	// Check if the book already exists in the vector
	for (const auto& existingBook : books) {
		if (existingBook == book){
			std::cout << "Book: " << book.getTitle() << "already exists." << std::endl;
			return false; // Return false if the book already exists
		}
	}
	// Add the book to the library and display a message
	books.push_back(book);
	std::cout << "Book " << book.getTitle() << " added do library." << std::endl;
	return true; // Return true if the book was successfully added
}

bool Admin::deleteBook(std::vector<Book>& books, const Book& book){
	for (const auto& existingBook : books) {
		if (book == existingBook) {
			books.erase(std::remove(books.begin(), books.end(), book), books.end());
			std::cout << "Book: " << book.getTitle() << " has been succesfully deleted!" << std::endl;
			return true;
		}
	}
	std::cout << "Book: " << book.getTitle() << " can't be deleted. We don't have such book in library." << std::endl;
	return false;
}

bool Admin::addAdmin(std::vector<Admin>& admins, const Admin& admin){
	// Check if an admin with the given login already exists
	for (const auto& existingAdmin : admins) {
		if (existingAdmin == admin) {
			std::cout << "Admin with this login already exists!" << std::endl;
			return false; // Return false if the admin already exists
		}
	}
	// Add the new admin and display a message
	admins.push_back(admin);
	std::cout << "Reader " << admin.getNick() << " was added!" << std::endl;
	return true; // Return true if the admin was successfully added
}

bool Admin::deleteAdmin(std::vector<Admin>& admins, const Admin& admin){
	// Search for the admin in the vector
	for (const auto& existingAdmin : admins) {
		if (admin == existingAdmin) {
			// Remove the admin using std::remove and display a message
			admins.erase(std::remove(admins.begin(), admins.end(), admin), admins.end());
			std::cout << "Admin " << admin.getNick() << " has been succesfully deleted!" << std::endl;
			return true; // Return true if the admin was successfully deleted
		}
	}
	// If the admin is not found, display an error message
	std::cout << "User " << admin.getNick() << " can't be deleted. Are you sure you gave good data, because this admin doesn't exit in our databse." << std::endl;
	return false; // Return false if the admin does not exist
}

void Admin::displayInfo(){
	std::cout << "Admin. Login: " << login << ", nickname: " << nickname << std::endl;
}

// Operator to compare admins based on their login or nickname
bool Admin::operator==(const Admin& other) const{
	return login == other.login || nickname == other.nickname;
}
