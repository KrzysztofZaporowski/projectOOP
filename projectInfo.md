# Library Management System: Object-Oriented Programming in C++

In this project, we designed a **Library Management System** using **Object-Oriented Programming (OOP)** principles in **C++**. The system manages users, books, and database connections efficiently while implementing key OOP concepts such as **inheritance**, **polymorphism**, and **design patterns**. Below are the key highlights of the project:

## Key Classes and Inheritance

The system is built around several core classes, each handling specific aspects of library management. Example of class connection below.

### User Class (Base Class)
The `User` class is the base class for all types of users in the system. It contains common attributes and methods for user management.

### Reader Class (Inherits from `User`)
The `Reader` class inherits from the `User` class and represents users who borrow books. It includes methods specific to readers, such as book borrowing and returning.

### Admin Class (Inherits from `User`)
The `Admin` class inherits from the `User` class and represents system administrators. It provides functionalities such as adding, updating, and removing books from the system.

#### Polymorphism
All three classes implement a **virtual method** called `displayInfo()`, showcasing polymorphism. This method displays user-specific information, and depending on whether the user is a **Reader** or **Admin**, different outputs are generated.

### Example of Polymorphism:

```cpp
class User {
public:
    virtual void displayInfo(); //virtual method
};

class Reader : public User {
public:
    void displayInfo() override {
        std::cout << "Reader: Displaying reader information" << std::endl;
    }
};

class Admin : public User {
public:
    void displayInfo() override {
        std::cout << "Admin: Displaying admin information" << std::endl;
    }
};
```
## Database Connection: MySQL & Singleton Pattern
To store data in the project, we use a **MySQL** database for persistent storage. The system connects to the database via the **Singleton Pattern**, ensuring that only a single instance of the database connection exists throughout the application.

### Singleton Pattern
The **Singleton Pattern** ensures that there is only one instance of the database connection, preventing multiple instances from consuming unnecessary resources.

**Example of Singleton Database Connection:**
```cpp
class Database {
private:
    static Database* instance;
    MYSQL* connection;
    
    // Private constructor
    Database() {
        connection = mysql_init(NULL);
        mysql_real_connect(connection, "localhost", "root", "password", "library", 0, NULL, 0);
    }

public:
    static Database* getInstance() {
        if (!instance) {
            instance = new Database();
        }
        return instance;
    }

    MYSQL* getConnection() {
        return connection;
    }
};

// Initialize static member
Database* Database::instance = nullptr;
```
## Unit Testing
To avoid logic mistakes we included in this project **Units test**. We use **Google tests** to do this. Firstly we created class `UnitTests` in main project to check if methods works right. Then tests results are saved in file. Then in second project results are read from file and **Google tests** check if result are as expected.
**Example of setting copies amount test**
```cpp
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
```
This is a part of the code is from `UnitTests.cpp` file.

Results from file are stored in vector and then we check if code pass tests.

**Example of Google tests**
```cpp
// Setting copies amount tests
TEST(setCopiesTest, setCopiesTestSuccess) {
    bool instance = data[8];
    EXPECT_TRUE(instance) << "Set book copies test (success) failed";
}
TEST(setCopiesTest, setCopiesTestFailure) {
    bool instance = data[9];
    EXPECT_FALSE(instance) << "Set book copies test (failure) failed";
}
```
