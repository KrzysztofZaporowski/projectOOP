#include "MenuHandler.h"

// Constructor for MenuHandler class
MenuHandler::MenuHandler(){}

void MenuHandler::menu(){
	int choice;
	// UPDATE : after adding SINGLETON PATTERN we use method getInstance() instead of normal constructor to inicialize libraryDB
	LibraryDB& libraryDB = LibraryDB::getInstance();
	libraryDB.loadData();
	std::vector<Book> books = libraryDB.getBooks();
	std::vector<Reader> readers = libraryDB.getReaders();
	std::vector<Admin> admins = libraryDB.getAdmins();
	std::vector<LibraryDB::Item> rentingHistory = libraryDB.getRentingHistory();
	std::vector<LibraryDB::Item> borrowedBooks = libraryDB.getBorrowedBooks();

	std::cout << "Welcome in library management system!" << std::endl;
	displayCurrentDate();
	while (true){
		std::cout << "Choose what you want to do:" << std::endl <<
			"1. Log in" << std::endl << "2. Sign in" << std::endl << "3. Quit" << std::endl << "(write 1, 2 or 3)" << std::endl;
		std::cin >> choice;
		// Input validation
		if (std::cin.fail()) {
			std::cout << "Invalid input! Please enter a number." << std::endl;

			std::cin.clear();

			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

			continue;
		}
		std::cout << std::endl;
		// Handling menu choices
		switch (choice) {
		case 1:
			while (true) {
				int userType;
				std::cout << "You want to log as:" << std::endl << "1. Reader" << std::endl << "2. Admin" << std::endl << "3. Quit to menu" << std::endl;
				std::cin >> userType;
				if (std::cin.fail()) {
					std::cout << "Invalid input! Please enter a number." << std::endl;

					std::cin.clear();

					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

					continue;
				}
				std::cout << std::endl;
				switch (userType) {
				case 1:
					readerLogIn(readers, books, borrowedBooks, rentingHistory);
					break;
				case 2:
					adminLogIn(readers, admins, books);
					break;
				case 3:
					break;
				default:
					std::cout << "You have choosen unavailable option." << std::endl;
					break;
				}
				if (userType == 3) {
					break;
				}
			}
			break;
		case 2:
			signIn(readers);
			break;
		case 3:
			libraryDB.saveBooksToDB(books);
			libraryDB.saveReadersToDB(readers);
			libraryDB.saveAdminsToDB(admins);
			libraryDB.saveToBorrowedBooksTable(borrowedBooks);
			libraryDB.saveToRentingHistoryTable(rentingHistory);
			return;
		default:
			std::cout << "You have choosen unavaiable option!" << std::endl;
			break;
		}
	}
}

void MenuHandler::displayCurrentDate(){
	auto now = std::chrono::system_clock::now();
	std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
	std::cout << "Today is: " << std::ctime(&currentTime) << std::endl;;
}

void MenuHandler::signIn(std::vector<Reader>& readers){
	Admin* temp = new Admin("temp", "temp", "temp");
	std::string login, password, nick;
	while (true) {
		std::cout << "Enter new login (or type '3' to exit): " << std::endl;
		std::cin >> login;
		std::cout << std::endl;
		if (login == "3") {
			std::cout << "Exiting login creation." << std::endl;
			delete temp;
			break;
		}

		bool found = std::any_of(readers.begin(), readers.end(), [&](const Reader& reader) {
			return reader.getLogin() == login;
			});

		if (found) {
			std::cout << "This login is already used by someone else, try again." << std::endl;
		}
		else {
			std::cout << "Your login is correct!" << std::endl << "Enter password: " << std::endl;
			std::cin >> password;
			std::cout << std::endl << "Enter nickname: " << std::endl;
			std::cin >> nick;
			std::cout << std::endl;
			std::cout << "Now you can log to your acconut!" << std::endl;
			Reader* newReader = new Reader(login, password, nick);
			temp->addUser(readers, *newReader);
			delete temp;
			delete newReader;
			break;
		}
	}
}

void MenuHandler::readerLogIn(std::vector<Reader>& readers, std::vector<Book>& books, std::vector<LibraryDB::Item>& borrowedBooks, std::vector<LibraryDB::Item>& rentingHistory){
	std::string login, password;
	std::vector<Reader::Item> renting, borrowed;
	while (true) {
		std::cout << "Enter login (or type '3' to quit): " << std::endl;
		std::cin >> login;
		std::cout << std::endl;
		if (login == "3") {
			std::cout << "Exiting admin login process." << std::endl;
			break;
		}
		bool loginFound = std::any_of(readers.begin(), readers.end(), [&](const Reader reader) {
			return reader.getLogin() == login;
			});

		if (!loginFound){
			std::cout << "This login: " << login << " is not registered in our library system. Please try again." << std::endl;
			continue;
		}
		std::cout << "Enter password: " << std::endl;
		std::cin >> password;

		auto reader = std::find_if(readers.begin(), readers.end(), [&](const Reader reader) {
			return reader.getLogin() == login;
			});

		if (reader != readers.end() && password == reader->getPassword()) {
			std::cout << "Login successful! Welcome!" << std::endl;
			reader->displayInfo();
			for (const auto& rent : rentingHistory) {
				if (rent.login == login){
					renting.push_back({ rent.title, rent.day, rent.month, rent.year });
				}
			}
			reader->setRentingHistory(renting);
			for (const auto& borr : borrowedBooks) {
				if (borr.login == login){
					borrowed.push_back({ borr.title, borr.day, borr.month, borr.year });
				}
			}
			reader->setBorrowedBooks(borrowed);
			readerOperations(books, *reader, borrowedBooks, rentingHistory);
			break;
		}
		else {
			std::cout << "The password is incorrect. Try again or type '3' to quit." << std::endl;
		}
	}
}

void MenuHandler::readerOperations(std::vector<Book>& books, Reader& reader, std::vector<LibraryDB::Item>& borrowed, std::vector<LibraryDB::Item>& renting){
	int userType;
	std::string title, isbn, authorName, authorLastname;
	bool found;
	while (true) {
		std::cout << "Available reader's operations: " << std::endl
			<< "1. Borrow book" << std::endl
			<< "2. Return book" << std::endl
			<< "3. Display current borrowed books" << std::endl
			<< "4. Display renting history" << std::endl
			<< "5. Display all books" << std::endl
			<< "6. Search for book" << std::endl
			<< "To quit enter '7'" << std::endl
			<< "Your choice: " << std::endl;
		std::cin >> userType;
		std::cout << std::endl;
		switch (userType){
		case 1:
			int type;
			std::cout << "Available options: " << std::endl
				<< "1. By title" << std::endl
				<< "2. BY ISBN" << std::endl
				<< "To quit press '3'" << std::endl;
			std::cin >> type;
			switch (type){
			case 1:
				found = false;
				std::cout << "Enter title of book you want to rent: " << std::endl;
				std::cin.ignore();
				std::getline(std::cin, title);
				for (auto& b : books) {
					if (b.getTitle() == title){
						found = true;
						Reader::OutItem item = reader.borrowBook(b);
						LibraryDB::Item save;
						save.login = item.login;
						save.isbn = item.ISBN;
						save.title = item.title;
						save.day = item.day;
						save.month = item.month;
						save.year = item.year;
						borrowed.push_back(save);
						break;
					}
				}
				if (!found){
					std::cout << "This book is unavailable. We are sorry." << std::endl;
				}
				break;
			case 2:
				found = false;
				std::cout << "Enter ISBN of book you want to rent: " << std::endl;
				std::cin >> isbn;
				std::cout << std::endl;
				for (auto& b : books) {
					if (b.getISBN() == isbn) {
						found = true;
						Reader::OutItem item = reader.borrowBook(b);
						LibraryDB::Item save;
						save.login = item.login;
						save.isbn = item.ISBN;
						save.title = item.title;
						save.day = item.day;
						save.month = item.month;
						save.year = item.year;
						borrowed.push_back(save);
						break;
					}
				}
				if (!found){
					std::cout << "This book is unavailable. We are sorry" << std::endl;
				}
				break;
			case 3:
				std::cout << "Exiting" << std::endl;
				return;
			default:
				std::cout << "You have choosen anavailable option" << std::endl;
				break;
			}
			break;
		case 2:
			std::cout << "Enter title of book you want to return: " << std::endl;
			std::cin.ignore();
			std::getline(std::cin, title);
			for (auto& b : books) {
				if (b.getTitle() == title) {
					found = true;
					std::pair<Reader::OutItem, Reader::OutItem> toSave = reader.returnBook(b);
					Reader::OutItem addToBorrowedBook = toSave.first;
					Reader::OutItem addToRentingHistory = toSave.second;
					LibraryDB::Item item = { addToBorrowedBook.login, addToBorrowedBook.ISBN, addToBorrowedBook.title, addToBorrowedBook.day, addToBorrowedBook.month, addToBorrowedBook.year };
					auto it = std::find(borrowed.begin(), borrowed.end(), item);
					if (it != borrowed.end()) {
						borrowed.erase(it); 
						std::cout << "Book returned successfully!" << std::endl;
						LibraryDB::Item history = { addToRentingHistory.login, addToRentingHistory.ISBN, addToRentingHistory.title, addToRentingHistory.day, addToRentingHistory.month, addToRentingHistory.year };
						renting.push_back(history);
					}
					else {
						std::cout << "Book not found in borrowed list!" << std::endl;
					}
					break;
				}
			}
			if (!found) {
				std::cout << "Book " << title << "wasn't found. Check if title was write right." << std::endl;
			}
			break;
		case 3:
			reader.displayCurrentBorrowedBooks();
			break;
		case 4:
			reader.displayBorrowHistory();
			break;
		case 5:
			for (const auto& b : books) {
				b.displayBookInfo();
			}
			break;
		case 6:
			int searchType;
			std::cout << "Available search options: " << std::endl
				<< "1. Search by title" << std::endl
				<< "2. Search by ISBN" << std::endl
				<< "3. Search by author name" << std::endl
				<< "4. Search by author lastname" << std::endl
				<< "To quit enter '5'" << std::endl
				<< "Your choice: ";
			std::cin >> searchType;
			if (std::cin.fail()) {
				std::cout << "Invalid input! Please enter a number." << std::endl;

				std::cin.clear();

				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

				continue;
			}
			std::cout << std::endl;

			switch (searchType) {
			case 1:
				std::cout << "Enter title of book you want to search: " << std::endl;
				std::cin.ignore();
				std::getline(std::cin, title);
				found = false;
				for (const auto& b : books){
					if (b.getTitle() == title){
						std::cout << "Book is found." << std::endl;
						b.displayBookInfo();
						found = true;
						break;
					}
				}
				if (!found){
					std::cout << "Book wasn't found" << std::endl;
				}
				break;
			case 2:
				std::cout << "Enter ISBN of book you want to search: " << std::endl;
				std::cin >> isbn;
				found = false;
				for (const auto& b : books) {
					if (b.getISBN() == isbn) {
						std::cout << "Book is found." << std::endl;
						b.displayBookInfo();
						found = true;
						break;
					}
				}
				if (!found){
					std::cout << "Book wasn't found" << std::endl;
				}
				break;
			case 3:
				std::cout << "Enter author name of book you want to search: " << std::endl;
				std::cin.ignore();
				std::getline(std::cin, authorName);
				found = false;
				for (const auto& b : books) {
					if (b.getAuthorName() == authorName){
						std::cout << "Book is found." << std::endl;
						b.displayBookInfo();
						found = true;
					}
				}
				if (!found){
					std::cout << "Book wasn't found" << std::endl;
				}
				break;
			case 4:
				std::cout << "Enter author lastname of book you want to search: " << std::endl;
				std::cin.ignore();
				std::getline(std::cin, authorLastname);
				found = false;
				for (const auto& b : books) {
					if (b.getAuthorName() == authorLastname) {
						std::cout << "Book is found." << std::endl;
						b.displayBookInfo();
						found = true;
					}
				}
				if (!found) {
					std::cout << "Book wasn't found" << std::endl;
				}
				break;
			case 5:
				std::cout << "Exiting search menu." << std::endl;
				break;
			default:
				std::cout << "You have chosen an unavailable option: " << searchType << std::endl;
				break;
			}
			break;
		case 7:
			std::cout << "Exiting reader's operations menu." << std::endl;
			return;
		default:
			std::cout << "You have choosen unavailable option. Please try again." << std::endl;
			break;
		}
	}
}

void MenuHandler::adminLogIn(std::vector<Reader>& readers, std::vector<Admin>& admins, std::vector<Book>& books){
	std::string login, password;

	while (true) {
		std::cout << "Enter login (or type '3' to quit): " << std::endl;
		std::cin >> login;
		if (login == "3") {
			std::cout << "Exiting admin login process." << std::endl;
			break;
		}

		bool loginFound = std::any_of(admins.begin(), admins.end(), [&](const Admin& admin) {
			return admin.getLogin() == login;
			});

		if (!loginFound) {
			std::cout << "This login: " << login << " is not registered in our library system. Please try again." << std::endl;
			continue; 
		}

		std::cout << "Enter password: " << std::endl;
		std::cin >> password;

		auto admin = std::find_if(admins.begin(), admins.end(), [&](const Admin& admin) {
			return admin.getLogin() == login;
			});

		if (admin != admins.end() && admin->getPassword() == password) {
			std::cout << "Login successful! Welcome!" << std::endl;
			admin->displayInfo();
			adminOperations(readers, admins, books, *admin);
			break;
		}
		else {
			std::cout << "The password is incorrect. Try again or type '3' to quit." << std::endl;
		}
	}
}

void MenuHandler::adminOperations(std::vector<Reader>& readers, std::vector<Admin>& admins, std::vector<Book>& books, Admin& admin){
	while (true) {
		std::cout << "Available admin's operations:" << std::endl
			<< "1. Add reader" << std::endl
			<< "2. Delete reader" << std::endl
			<< "3. Add admin" << std::endl
			<< "4. Delete admin" << std::endl
			<< "5. Add book" << std::endl
			<< "6. Delete book" << std::endl
			<< "7. Set available book copies" << std::endl
			<< "8. Display readers" << std::endl
			<< "9. Display admins" << std::endl
			<< "10. Display books" << std::endl
			<< "To quit, enter '11'" << std::endl
			<< "Your choice: " << std::endl;

		int userType, amount, publishYear, availableCopies;
		std::cin >> userType;
		std::cout << std::endl;
		std::string login, password, nick, isbn, title, authorName, authorLastName;

		switch (userType) {
		case 1:
			int youSure;
			std::cout << "Are you sure you want to add new reader? (1 - Yes / 2 - No)" << std::endl;
			std::cin >> youSure;
			if (std::cin.fail()) {
				std::cout << "Invalid input! Please enter a number." << std::endl;

				std::cin.clear();

				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

				continue;
			}
			if (youSure == 1) {
				std::cout << "Enter reader login, passoword and nick of new reader (separated by space)" << std::endl;
				std::cin >> login >> password >> nick;
				std::cout << std::endl;
				{
					Reader reader(login, password, nick);
					admin.addUser(readers, reader);
				}
			}
			else if (youSure == 2){
				std::cout << "Operation cancelled." << std::endl;
			}
			else {
				std::cout << "Invalid input. Please enter 1 to confirm or 2 to cancel." << std::endl;
			}
			break;  
		case 2:
			std::cout << "Are you sure you want to delete reader? (1 - Yes / 2 - No)" << std::endl;
			std::cin >> youSure;
			if (std::cin.fail()) {
				std::cout << "Invalid input! Please enter a number." << std::endl;

				std::cin.clear();

				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

				continue;
			}
			if (youSure == 1) {
				std::cout << "Enter reader login, passoword and nick of reader you want to delete (separated by space)" << std::endl;
				std::cin >> login >> password >> nick;
				std::cout << std::endl;
				{
					Reader reader(login, password, nick);
					admin.deleteUser(readers, reader);
				}
			}
			else if (youSure == 2) {
				std::cout << "Operation cancelled." << std::endl;
			}
			else {
				std::cout << "Invalid input. Please enter 1 to confirm or 2 to cancel." << std::endl;
			}
			break;  
		case 3:
			std::cout << "Are you sure you want to add new admin? (1 - Yes / 2 - No)" << std::endl;
			std::cin >> youSure;
			if (std::cin.fail()) {
				std::cout << "Invalid input! Please enter a number." << std::endl;

				std::cin.clear();

				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

				continue;
			}
			if (youSure == 1) {
				std::cout << "Enter reader login, passoword and nick of new admin (separated by space)" << std::endl;
				std::cin >> login >> password >> nick;
				std::cout << std::endl;
				{
					Admin newAdmin(login, password, nick);
					admin.addAdmin(admins, newAdmin);
				}
			}
			else if (youSure == 2) {
				std::cout << "Operation cancelled." << std::endl;
			}
			else {
				std::cout << "Invalid input. Please enter 1 to confirm or 2 to cancel." << std::endl;
			}
			break;  
		case 4:
			std::cout << "Are you sure you want to delete admin? (1 - Yes / 2 - No)" << std::endl;
			std::cin >> youSure;
			if (std::cin.fail()) {
				std::cout << "Invalid input! Please enter a number." << std::endl;

				std::cin.clear();

				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

				continue;
			}
			if (youSure == 1) {
				std::cout << "Enter reader login, passoword and nick of admin you want to delete (separated by space)" << std::endl;
				std::cin >> login >> password >> nick;
				std::cout << std::endl;
				{
					Admin deleteAdmin(login, password, nick);
					admin.deleteAdmin(admins, deleteAdmin);
				}
			}
			else if (youSure == 2) {
				std::cout << "Operation cancelled." << std::endl;
			}
			else {
				std::cout << "Invalid input. Please enter 1 to confirm or 2 to cancel." << std::endl;
			}
			break;  
		case 5:
			std::cout << "Enter title of the book: ";
			std::cin.ignore();
			std::getline(std::cin, title);

			std::cout << "Enter author's first name: ";
			std::getline(std::cin, authorName);

			std::cout << "Enter author's last name: ";
			std::getline(std::cin, authorLastName);

			std::cout << "Enter ISBN: ";
			std::getline(std::cin, isbn);

			std::cout << "Enter publish year: ";
			std::cin >> publishYear;

			std::cout << "Enter available copies: ";
			std::cin >> availableCopies;
			std::cout << std::endl;
			{
				Book book(isbn, title, authorName, authorLastName, publishYear, availableCopies);
				admin.addBook(books, book);
			}
			break;  
		case 6:
			std::cout << "Enter title of the book: ";
			std::cin.ignore();
			std::getline(std::cin, title);

			std::cout << "Enter author's first name: ";
			std::getline(std::cin, authorName);

			std::cout << "Enter author's last name: ";
			std::getline(std::cin, authorLastName);

			std::cout << "Enter ISBN: ";
			std::getline(std::cin, isbn);

			std::cout << "Enter publish year: ";
			std::cin >> publishYear;

			std::cout << "Enter available copies: ";
			std::cin >> availableCopies;
			std::cout << std::endl;
			{
				Book book(isbn, title, authorName, authorLastName, publishYear, availableCopies);
				admin.deleteBook(books, book);
			}
			break;  
		case 7:
			std::cout << "Enter title of the book: ";
			std::cin.ignore();
			std::getline(std::cin, title);

			std::cout << "Enter the new number of available copies: ";
			std::cin >> availableCopies;
			std::cout << std::endl;
			{
				auto book_ = std::find_if(books.begin(), books.end(), [&](const Book& book) {
					return book.getTitle() == title;
					});
				if (book_ != books.end()) {
					admin.updateBookCopiesNumber(*book_, availableCopies);
				}
				else {
					std::cout << "Book with title '" << title << "' not found." << std::endl;
				}
			}
			break; 
		case 8:
			for (auto& r : readers) {
				r.displayInfo();
			}
			break;
		case 9:
			for (auto& a : admins) {
				a.displayInfo();
			}
			break;
		case 10:
			for (auto& b : books) {
				b.displayBookInfo();
			}
			break;
		case 11:
			std::cout << "Exiting admin's operations menu." << std::endl;
			return;  

		default:
			std::cout << "You have chosen an unavailable option. Please try again." << std::endl;
			break;  
		}
	}
}



