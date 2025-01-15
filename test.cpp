#include "pch.h"
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <cctype>

// Function to trim whitespace from a string
std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    size_t end = str.find_last_not_of(" \t\n\r");

    if (start == std::string::npos || end == std::string::npos) {
        return ""; 
    }

    return str.substr(start, end - start + 1);
}

std::vector<bool> load_data_from_file(const std::string& filePath) {
    std::ifstream file(filePath);
    std::vector<bool> data;

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filePath);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::string trimmed_line = trim(line);

        if (trimmed_line == "true") {
            data.push_back(true);
        }
        else if (trimmed_line == "false") {
            data.push_back(false);
        }
    }

    file.close();
    return data;
}

std::vector<bool> data = load_data_from_file("C:\\Users\\krzys\\source\\repos\\OOP_project\\OOP_project\\Tests.txt");

// Adding admin tests
TEST(addAdminTest, addAdminTestSuccess) {
    bool instance = data[0];
    EXPECT_TRUE(instance) << "Add admin test (success) failed.";
}
TEST(addAdminTest, addAdminTestFailure) {
    bool instance = data[1];
    EXPECT_FALSE(instance) << "Add admin test (failure) failed.";
}
// Deleting admin tests
TEST(deleteAdminTest, deleteAdminTestSuccess) {
    bool instance = data[4];
    EXPECT_TRUE(instance) << "Delete admin test (success) failed.";
}
TEST(deleteAdminTest, deleteAdminTestFailure) {
    bool instance = data[5];
    EXPECT_FALSE(instance) << "Delete admin test (failure) failed.";
}
// Adding reader tests
TEST(addReaderTest, addReaderTestSuccess) {
    bool instance = data[2];
    EXPECT_TRUE(instance) << "Add reader test (success) failed.";
}
TEST(addReaderTest, addReaderTestFailure) {
    bool instance = data[3];
    EXPECT_FALSE(instance) << "Add reader test (failure) failed.";
}
// Deleting reader tests
TEST(deleteReaderTest, deleteReaderTestSuccess) {
    bool instance = data[6];
    EXPECT_TRUE(instance) << "Delete reader test (success) failed.";
}
TEST(deleteReaderTest, deleteReaderTestFailure) {
    bool instance = data[7];
    EXPECT_FALSE(instance) << "Delete reader test (failure) failed.";
}
// Setting copies amount tests
TEST(setCopiesTest, setCopiesTestSuccess) {
    bool instance = data[8];
    EXPECT_TRUE(instance) << "Set book copies test (success) failed";
}
TEST(setCopiesTest, setCopiesTestFailure) {
    bool instance = data[9];
    EXPECT_FALSE(instance) << "Set book copies test (failure) failed";
}
// Borrowing book test
TEST(readerBorrowBookTest, readerBorroweBookTestSuccess) {
    bool instance = data[10];
    EXPECT_TRUE(instance) << "Reader borrow book test (success) failed";
}
TEST(readerBorrowBookTest, readerBorrowBookTestFailure) {
    bool instance = data[11];
    EXPECT_FALSE(instance) << "Reader borrow book test (failure) failed.";
}
