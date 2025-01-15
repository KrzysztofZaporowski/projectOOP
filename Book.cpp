#include "Book.h"

// Class constructor
Book::Book(std::string isbn, std::string title, std::string authorName, std::string authorLastName, int publishYear, int availableCopies) : 
	isbn(isbn), title(title), authorName(authorName), authorLastName(authorLastName), publishYear(publishYear), availableCopies(availableCopies){}

// Basic getters (and setter for setting copies)
std::string Book::getISBN() const{
	return isbn;
}

std::string Book::getTitle() const{
	return title;
}

int Book::getPublishYear() const{
	return publishYear;
}

std::string Book::getAuthorName() const{
	return authorName;
}

std::string Book::getAuthorLastname() const{
	return authorLastName;
}

int Book::getAvailableCopies() const{
	return availableCopies;
}

void Book::setAvailableCopies(int amount){
	availableCopies = amount;
}

// Function to display info
void Book::displayBookInfo() const{
	std::cout << "Book info:" << std::endl <<
		"Title: " << title << std::endl <<
		"Author: " << authorName << " " << authorLastName << std::endl <<
		"Publish year: " << publishYear << std::endl <<
		"ISBN: " << isbn << std::endl <<
		"Copies in library: " << availableCopies << std::endl;
}

bool Book::operator==(const Book& other) const{
	return title == other.title || isbn == other.isbn;
}
