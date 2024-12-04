#include "LibraryDB.h"

void LibraryDB::loadData() {
    try {
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

LibraryDB::LibraryDB(std::string username, std::string password, std::string databaseName, std::string tableBooksName, std::string tableReadersName, std::string tableAdminsName, std::string rentingHistorytableName, std::string borrowedBooksTableName)
    : username(username), password(password), databaseName(databaseName),
    tableBooksName(tableBooksName), tableReadersName(tableReadersName), tableAdminsName(tableAdminsName), rentingHistorytableName(rentingHistorytableName), borrowedBooksTableName(borrowedBooksTableName){}

std::vector<Book> LibraryDB::getBooks(){
	return books;
}

std::vector<Admin> LibraryDB::getAdmins(){
	return admins;
}

std::vector<Reader> LibraryDB::getReaders(){
	return readers;
}

std::vector<Item> LibraryDB::getRentingHistory(){
    return rentingHistory;
}

std::vector<Item> LibraryDB::getBorrowedBooks(){
    return borrowedBooks;
}

void LibraryDB::saveBooksToDB(std::vector<Book>& books){
    try {
        sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
        std::unique_ptr<sql::Connection> con(driver->connect("tcp://127.0.0.1:3306", username, password));
        con->setSchema(databaseName);

        const std::string query =
            "INSERT INTO " + tableBooksName + " (isbn, title, publish_year, author_name, author_lastname, available_copies) "
            "VALUES (?, ?, ?, ?, ?, ?) "
            "ON DUPLICATE KEY UPDATE title = VALUES(title), publish_year = VALUES(publish_year), "
            "author_name = VALUES(author_name), author_lastname = VALUES(author_lastname), available_copies = VALUES(available_copies);";

        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));

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

void LibraryDB::saveAdminsToDB(std::vector<Admin>& admins){
    try {
        sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
        std::unique_ptr<sql::Connection> con(driver->connect("tcp://127.0.0.1:3306", username, password));
        con->setSchema(databaseName);

        const std::string query =
            "INSERT INTO " + tableAdminsName + " (login, password, nick) "
            "VALUES (?, ?, ?) "
            "ON DUPLICATE KEY UPDATE password = VALUES(password), nick = VALUES(nick);";

        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));

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

void LibraryDB::saveReadersToDB(std::vector<Reader>& readers){
    try {
        sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
        std::unique_ptr<sql::Connection> con(driver->connect("tcp://127.0.0.1:3306", username, password));
        con->setSchema(databaseName);

        const std::string query =
            "INSERT INTO " + tableReadersName + " (login, password, nick) "
            "VALUES (?, ?, ?) "
            "ON DUPLICATE KEY UPDATE password = VALUES(password), nick = VALUES(nick);";

        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement(query));

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

        // Query to insert or update rows in the renting_history table
        std::string query =
            "INSERT INTO renting_history (user_login, book_isbn, book_title, return_day, return_month, return_year) "
            "VALUES (?, ?, ?, ?, ?, ?) "
            "ON DUPLICATE KEY UPDATE "
            "return_day = VALUES(return_day), "
            "return_month = VALUES(return_month), "
            "return_year = VALUES(return_year)";

        std::unique_ptr<sql::PreparedStatement> stmt(con->prepareStatement(query));

        for (const auto& record : renting) {
            std::string user_login = record.login;
            std::string book_isbn = record.isbn;
            std::string book_title = record.title;
            int return_day = record.day;
            int return_month = record.month;
            int return_year = record.year;

            stmt->setString(1, user_login);
            stmt->setString(2, book_isbn);
            stmt->setString(3, book_title);
            stmt->setInt(4, return_day);
            stmt->setInt(5, return_month);
            stmt->setInt(6, return_year);

            stmt->executeUpdate();
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

        // Query to insert or update rows in the borrowed_books table
        std::string query =
            "INSERT INTO borrowed_books (user_login, book_isbn, book_title, due_day, due_month, due_year) "
            "VALUES (?, ?, ?, ?, ?, ?) "
            "ON DUPLICATE KEY UPDATE "
            "due_day = VALUES(due_day), "
            "due_month = VALUES(due_month), "
            "due_year = VALUES(due_year)";

        std::unique_ptr<sql::PreparedStatement> stmt(con->prepareStatement(query));

        for (const auto& record : borrowed) {
            std::string user_login = record.login;
            std::string book_isbn = record.isbn;
            std::string book_title = record.title;
            int due_day = record.day;
            int due_month = record.month;
            int due_year = record.year;

            stmt->setString(1, user_login);
            stmt->setString(2, book_isbn);
            stmt->setString(3, book_title);
            stmt->setInt(4, due_day);
            stmt->setInt(5, due_month);
            stmt->setInt(6, due_year);

            stmt->executeUpdate();
        }
    }
    catch (sql::SQLException& e) {
        std::cerr << "Error saving to borrowed_books table: " << e.what()
            << " (MySQL error code: " << e.getErrorCode()
            << ", SQLState: " << e.getSQLState() << ")"
            << std::endl;
    }
}


