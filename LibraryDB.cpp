#include "LibraryDB.h"

// Initialize the static members of the LibraryDB class.
std::unique_ptr<LibraryDB> LibraryDB::instance = nullptr;
std::once_flag LibraryDB::initFlag;

// Returns the singleton instance of the LibraryDB class. Initializes the instance if it doesn't already exist.
LibraryDB& LibraryDB::getInstance(const std::string& username,
    const std::string& password,
    const std::string& databaseName,
    const std::string& tableBooksName,
    const std::string& tableReadersName,
    const std::string& tableAdminsName,
    const std::string& rentingHistorytableName,
    const std::string& borrowedBooksTableName) {
    std::call_once(initFlag, [&]() {
        instance.reset(new LibraryDB(username, password, databaseName, tableBooksName, tableReadersName, tableAdminsName, rentingHistorytableName, borrowedBooksTableName));
        });
    return *instance;
}

// Constructor for the LibraryDB class. Initializes member variables with database credentials and table names.
LibraryDB::LibraryDB(const std::string& username,
    const std::string& password,
    const std::string& databaseName,
    const std::string& tableBooksName,
    const std::string& tableReadersName,
    const std::string& tableAdminsName,
    const std::string& rentingHistorytableName,
    const std::string& borrowedBooksTableName)
    : username(username), password(password), databaseName(databaseName),
    tableBooksName(tableBooksName), tableReadersName(tableReadersName),
    tableAdminsName(tableAdminsName), rentingHistorytableName(rentingHistorytableName),
    borrowedBooksTableName(borrowedBooksTableName) {}

// Loads data from the database into in-memory data structures for books, readers, admins, borrowed books, and renting history.
void LibraryDB::loadData() {
    try {
        // Get the MySQL driver instance and establish a connection to the database.
        sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();

        std::unique_ptr<sql::Connection> con(
            driver->connect("tcp://127.0.0.1:3306", username, password)
        );

        con->setSchema(databaseName);

        std::unique_ptr<sql::Statement> stmt(con->createStatement());

        // Load books
        std::unique_ptr<sql::ResultSet> resBooks(stmt->executeQuery("SELECT * FROM " + tableBooksName));
        while (resBooks->next()) {
            std::string isbn = resBooks->getString("isbn");
            std::string title = resBooks->getString("title");
            int publish_year = resBooks->getInt("publish_year");
            std::string author_name = resBooks->getString("author_name");
            std::string author_lastname = resBooks->getString("author_lastname");
            int available_copies = resBooks->getInt("available_copies");

            Book book(isbn, title, author_name, author_lastname, publish_year, available_copies);
            books.push_back(book);
        }

        // Load readers
        std::unique_ptr<sql::ResultSet> resReaders(stmt->executeQuery("SELECT * FROM " + tableReadersName));
        while (resReaders->next()) {
            std::string login = resReaders->getString("login");
            std::string password = resReaders->getString("password");
            std::string nick = resReaders->getString("nick");

            Reader reader(login, password, nick);
            readers.push_back(reader);
        }

        // Load admins
        std::unique_ptr<sql::ResultSet> resAdmins(stmt->executeQuery("SELECT * FROM " + tableAdminsName));
        while (resAdmins->next()) {
            std::string login = resAdmins->getString("login");
            std::string password = resAdmins->getString("password");
            std::string nick = resAdmins->getString("nick");

            Admin admin(login, password, nick);
            admins.push_back(admin);
        }

        // Load borrowed books
        std::unique_ptr<sql::ResultSet> resBorrowed(stmt->executeQuery("SELECT * FROM " + borrowedBooksTableName));
        while (resBorrowed->next()) {
            std::string user_login = resBorrowed->getString("user_login");
            std::string book_isbn = resBorrowed->getString("book_isbn");
            std::string book_title = resBorrowed->getString("book_title");
            int due_day = resBorrowed->getInt("due_day");
            int due_month = resBorrowed->getInt("due_month");
            int due_year = resBorrowed->getInt("due_year");

            borrowedBooks.push_back({ user_login, book_isbn, book_title, due_day, due_month, due_year });
        }

        // Load renting history
        std::unique_ptr<sql::ResultSet> resHistory(stmt->executeQuery("SELECT * FROM " + rentingHistorytableName));
        while (resHistory->next()) {
            std::string user_login = resHistory->getString("user_login");
            std::string book_isbn = resHistory->getString("book_isbn");
            std::string book_title = resHistory->getString("book_title");
            int return_day = resHistory->getInt("return_day");
            int return_month = resHistory->getInt("return_month");
            int return_year = resHistory->getInt("return_year");

            rentingHistory.push_back({ user_login, book_isbn, book_title, return_day, return_month, return_year });
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "Error: " << e.what()
            << " (MySQL error code: " << e.getErrorCode()
            << ", SQLState: " << e.getSQLState() << ")"
            << std::endl;
    }
}

std::vector<Book> LibraryDB::getBooks(){
	return books;
}

std::vector<Admin> LibraryDB::getAdmins(){
	return admins;
}

std::vector<Reader> LibraryDB::getReaders(){
	return readers;
}

std::vector<LibraryDB::Item> LibraryDB::getRentingHistory(){
    return rentingHistory;
}

std::vector<LibraryDB::Item> LibraryDB::getBorrowedBooks(){
    return borrowedBooks;
}

void LibraryDB::saveBooksToDB(std::vector<Book>& books) {
    try {
        sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
        std::unique_ptr<sql::Connection> con(driver->connect("tcp://127.0.0.1:3306", username, password));
        con->setSchema(databaseName);

        const std::string clearTableQuery = "DELETE FROM " + tableBooksName;
        std::unique_ptr<sql::Statement> stmt(con->createStatement());
        stmt->executeUpdate(clearTableQuery);

        const std::string insertQuery =
            "INSERT INTO " + tableBooksName + " (isbn, title, publish_year, author_name, author_lastname, available_copies) "
            "VALUES (?, ?, ?, ?, ?, ?)";

        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(insertQuery));
        for (const auto& book : books) {
            pstmt->setString(1, book.getISBN());
            pstmt->setString(2, book.getTitle());
            pstmt->setInt(3, book.getPublishYear());
            pstmt->setString(4, book.getAuthorName());
            pstmt->setString(5, book.getAuthorLastname());
            pstmt->setInt(6, book.getAvailableCopies());
            pstmt->executeUpdate();
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "Error saving books to DB: " << e.what()
            << " (MySQL error code: " << e.getErrorCode()
            << ", SQLState: " << e.getSQLState() << ")"
            << std::endl;
    }
}


void LibraryDB::saveAdminsToDB(std::vector<Admin>& admins) {
    try {
        sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
        std::unique_ptr<sql::Connection> con(driver->connect("tcp://127.0.0.1:3306", username, password));
        con->setSchema(databaseName);

        const std::string clearTableQuery = "DELETE FROM " + tableAdminsName;
        std::unique_ptr<sql::Statement> stmt(con->createStatement());
        stmt->executeUpdate(clearTableQuery);

        const std::string insertQuery =
            "INSERT INTO " + tableAdminsName + " (login, password, nick) VALUES (?, ?, ?)";

        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(insertQuery));
        for (const auto& admin : admins) {
            pstmt->setString(1, admin.getLogin());
            pstmt->setString(2, admin.getPassword());
            pstmt->setString(3, admin.getNick());
            pstmt->executeUpdate();
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "Error saving admins to DB: " << e.what()
            << " (MySQL error code: " << e.getErrorCode()
            << ", SQLState: " << e.getSQLState() << ")"
            << std::endl;
    }
}


void LibraryDB::saveReadersToDB(std::vector<Reader>& readers) {
    try {
        sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
        std::unique_ptr<sql::Connection> con(driver->connect("tcp://127.0.0.1:3306", username, password));
        con->setSchema(databaseName);

        const std::string clearTableQuery = "DELETE FROM " + tableReadersName;
        std::unique_ptr<sql::Statement> stmt(con->createStatement());
        stmt->executeUpdate(clearTableQuery);

        const std::string insertQuery =
            "INSERT INTO " + tableReadersName + " (login, password, nick) VALUES (?, ?, ?)";

        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(insertQuery));
        for (const auto& reader : readers) {
            pstmt->setString(1, reader.getLogin());
            pstmt->setString(2, reader.getPassword());
            pstmt->setString(3, reader.getNick());
            pstmt->executeUpdate();
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "Error saving readers to DB: " << e.what()
            << " (MySQL error code: " << e.getErrorCode()
            << ", SQLState: " << e.getSQLState() << ")"
            << std::endl;
    }
}

void LibraryDB::saveToRentingHistoryTable(std::vector<Item>& renting) {
    try {
        sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
        std::unique_ptr<sql::Connection> con(driver->connect("tcp://127.0.0.1:3306", username, password));
        con->setSchema(databaseName);

        const std::string clearTableQuery = "DELETE FROM renting_history";
        std::unique_ptr<sql::Statement> stmt(con->createStatement());
        stmt->executeUpdate(clearTableQuery);

        const std::string insertQuery =
            "INSERT INTO renting_history (user_login, book_isbn, book_title, return_day, return_month, return_year) "
            "VALUES (?, ?, ?, ?, ?, ?)";

        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(insertQuery));
        for (const auto& record : renting) {
            pstmt->setString(1, record.login);
            pstmt->setString(2, record.isbn);
            pstmt->setString(3, record.title);
            pstmt->setInt(4, record.day);
            pstmt->setInt(5, record.month);
            pstmt->setInt(6, record.year);
            pstmt->executeUpdate();
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "Error saving to renting_history table: " << e.what()
            << " (MySQL error code: " << e.getErrorCode()
            << ", SQLState: " << e.getSQLState() << ")"
            << std::endl;
    }
}

void LibraryDB::saveToBorrowedBooksTable(std::vector<Item>& borrowed) {
    try {
        sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
        std::unique_ptr<sql::Connection> con(driver->connect("tcp://127.0.0.1:3306", username, password));
        con->setSchema(databaseName);

        const std::string clearTableQuery = "DELETE FROM borrowed_books";
        std::unique_ptr<sql::Statement> stmt(con->createStatement());
        stmt->executeUpdate(clearTableQuery);

        const std::string insertQuery =
            "INSERT INTO borrowed_books (user_login, book_isbn, book_title, due_day, due_month, due_year) "
            "VALUES (?, ?, ?, ?, ?, ?)";

        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(insertQuery));
        for (const auto& record : borrowed) {
            pstmt->setString(1, record.login);
            pstmt->setString(2, record.isbn);
            pstmt->setString(3, record.title);
            pstmt->setInt(4, record.day);
            pstmt->setInt(5, record.month);
            pstmt->setInt(6, record.year);
            pstmt->executeUpdate();
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "Error saving to borrowed_books table: " << e.what()
            << " (MySQL error code: " << e.getErrorCode()
            << ", SQLState: " << e.getSQLState() << ")"
            << std::endl;
    }
}



