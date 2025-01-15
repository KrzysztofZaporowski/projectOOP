#include "UnitTests.h"
/// <Admins Tests>
bool UnitTests::addAdminSuccess() {
    //ignore couts
    std::streambuf* originalCoutStream = std::cout.rdbuf();
    std::ostringstream nullStream;
    std::cout.rdbuf(nullStream.rdbuf());

    Admin admin("test", "test", "test");
    Admin newAdmin("new_admin", "admin_password", "admin_nick");
    std::vector<Admin> admins;
    if (admin.addAdmin(admins, newAdmin) && admins.size() == 1 && admins[0].getLogin() == "new_admin") {
        std::cout.rdbuf(originalCoutStream);
        return true;
    }
    return false;
}

bool UnitTests::addAdminFailure() {
    //ignore couts
    std::streambuf* originalCoutStream = std::cout.rdbuf();
    std::ostringstream nullStream;
    std::cout.rdbuf(nullStream.rdbuf());

    Admin admin("test", "test", "test");
    Admin existingAdmin("existing_admin", "existing_admin_password", "exisiting_admin_nick");
    std::vector<Admin> admins = { existingAdmin };
    if (admin.addAdmin(admins, existingAdmin)) {
        std::cout.rdbuf(originalCoutStream);
        return true;
    }
    return false;
}

bool UnitTests::addReaderSuccess() {
    //ignore couts
    std::streambuf* originalCoutStream = std::cout.rdbuf();
    std::ostringstream nullStream;
    std::cout.rdbuf(nullStream.rdbuf());

    Admin admin("test", "test", "test");
    Reader newReader("new_reader", "reader_password", "reader_nick");
    std::vector<Reader> readers;
    if (admin.addUser(readers, newReader) && readers.size() == 1 && readers[0].getLogin() == "new_reader") {
        std::cout.rdbuf(originalCoutStream);
        return true;
    }
    return false;
}

bool UnitTests::addReaderFailure() {
    //ignore couts
    std::streambuf* originalCoutStream = std::cout.rdbuf();
    std::ostringstream nullStream;
    std::cout.rdbuf(nullStream.rdbuf());

    Admin admin("test", "test", "test");
    Reader existingReader("existing_reader", "existing_reader_password", "exisiting_reader_nick");
    std::vector<Reader> readers = { existingReader };
    if (admin.addUser(readers, existingReader)) {
        std::cout.rdbuf(originalCoutStream);
        return true;
    }
    return false;
}

bool UnitTests::deleteAdminSuccess() {
    //ignore couts
    std::streambuf* originalCoutStream = std::cout.rdbuf();
    std::ostringstream nullStream;
    std::cout.rdbuf(nullStream.rdbuf());

    Admin admin("test", "test", "test");
    Admin existingAdmin("existing_admin", "existing_admin_password", "exisiting_admin_nick");
    std::vector<Admin> admins = { existingAdmin };
    if (admin.deleteAdmin(admins, existingAdmin)) {
        std::cout.rdbuf(originalCoutStream);
        return true;
    }
    return false;
}

bool UnitTests::deleteAdminFailure() {
    //ignore couts
    std::streambuf* originalCoutStream = std::cout.rdbuf();
    std::ostringstream nullStream;
    std::cout.rdbuf(nullStream.rdbuf());

    Admin admin("test", "test", "test");
    Admin notExistingAdmin("not_existing_admin", "not_existing_admin_password", "not_exisiting_admin_nick");
    std::vector<Admin> admins;
    if (admin.deleteAdmin(admins, notExistingAdmin)) {
        std::cout.rdbuf(originalCoutStream);
        return true;
    }
    return false;
}

bool UnitTests::deleteReaderSuccess() {
    //ignore couts
    std::streambuf* originalCoutStream = std::cout.rdbuf();
    std::ostringstream nullStream;
    std::cout.rdbuf(nullStream.rdbuf());

    Admin admin("test", "test", "test");
    Reader existingReader("existing_reader", "existing_reader_password", "exisiting_reader_nick");
    std::vector<Reader> readers = { existingReader };
    if (admin.deleteUser(readers, existingReader)) {
        std::cout.rdbuf(originalCoutStream);
        return true;
    }
    return false;
}

bool UnitTests::deleteReaderFailure() {
    //ignore couts
    std::streambuf* originalCoutStream = std::cout.rdbuf();
    std::ostringstream nullStream;
    std::cout.rdbuf(nullStream.rdbuf());

    Admin admin("test", "test", "test");
    Reader existingReader("not_existing_reader", "not_existing_reader_password", "not_exisiting_reader_nick");
    std::vector<Reader> readers;
    if (admin.deleteUser(readers, existingReader)) {
        std::cout.rdbuf(originalCoutStream);
        return true;
    }
    return false;
}

bool UnitTests::adminSetCopiesSuccess() {
    //ignore couts
    std::streambuf* originalCoutStream = std::cout.rdbuf();
    std::ostringstream nullStream;
    std::cout.rdbuf(nullStream.rdbuf());

    Admin admin("test", "test", "test");
    Book book("1234567891234", "Test title", "Name", "Lastname", 2024, 5);
    int amount = 7;
    admin.updateBookCopiesNumber(book, amount);
    std::cout.rdbuf(originalCoutStream);
    if (book.getAvailableCopies() == amount) {
        return true;
    }
    return false;
}

bool UnitTests::adminSetCopiesFailure() {
    //ignore couts
    std::streambuf* originalCoutStream = std::cout.rdbuf();
    std::ostringstream nullStream;
    std::cout.rdbuf(nullStream.rdbuf());

    Admin admin("test", "test", "test");
    Book book("1234567891234", "Test title", "Name", "Lastname", 2024, 5);
    int amount = -3;
    admin.updateBookCopiesNumber(book, amount);
    std::cout.rdbuf(originalCoutStream);
    if (book.getAvailableCopies() == amount) {
        return true;
    }
    return false;
}
/// </Admins Tests>

/// <Readers Tests>
bool UnitTests::readerBorrowBookSuccess(){
    // Redirect std::cout to a null stream
    std::streambuf* originalCoutStream = std::cout.rdbuf();
    std::ostringstream nullStream;
    std::cout.rdbuf(nullStream.rdbuf());

    Book book("1234567890", "Test Book", "Name", "Lastname", 2024, 2);
    Reader reader("test_user", "test_password", "test_nick");
    Reader::OutItem result = reader.borrowBook(book);
    std::cout.rdbuf(originalCoutStream);

    if (!result.title.empty() && !result.login.empty() && !result.ISBN.empty()) {
        return true; 
    }
    return false;
}

bool UnitTests::readerBorrowBookFailure(){
    // Redirect std::cout to a null stream
    std::streambuf* originalCoutStream = std::cout.rdbuf();
    std::ostringstream nullStream;
    std::cout.rdbuf(nullStream.rdbuf());

    Book book("1234567890", "Test Book", "Name", "Lastname", 2024, 0);
    Reader reader("test_user", "test_password", "test_nick");
    Reader::OutItem result = reader.borrowBook(book);
    std::cout.rdbuf(originalCoutStream);

    if (!result.title.empty() && !result.login.empty() && !result.ISBN.empty()) {
        return true;
    }
    return false;
}
/// </Readers Tests>

void UnitTests::saveVectorToFile(const std::string& filename, const std::vector<bool>& data) {
    std::ofstream file(filename, std::ios::out | std::ios::trunc);

    if (!file.is_open()) {
        std::cerr << "Error opening file for writing!" << std::endl;
        return;
    }

    for (bool value : data) {
        file << (value ? "true" : "false") << "\n";
    }

    file.close();
}

void UnitTests::allTests() {
    testsValues.push_back(addAdminSuccess());
    testsValues.push_back(addAdminFailure());
    testsValues.push_back(addReaderSuccess());
    testsValues.push_back(addReaderFailure());
    testsValues.push_back(deleteAdminSuccess());
    testsValues.push_back(deleteAdminFailure());
    testsValues.push_back(deleteReaderSuccess());
    testsValues.push_back(deleteReaderFailure());
    testsValues.push_back(adminSetCopiesSuccess());
    testsValues.push_back(adminSetCopiesFailure());
    testsValues.push_back(readerBorrowBookSuccess());
    testsValues.push_back(readerBorrowBookFailure());
    saveVectorToFile("Tests.txt", testsValues);
}
