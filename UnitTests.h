#pragma once
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include "Book.h"
#include "User.h"
#include "Reader.h"
#include "Admin.h"

class UnitTests {
public:
	bool addAdminSuccess();
	bool addAdminFailure();
	bool addReaderSuccess();
	bool addReaderFailure();
	bool deleteAdminSuccess();
	bool deleteAdminFailure();
	bool deleteReaderSuccess();
	bool deleteReaderFailure();
	bool adminSetCopiesSuccess();
	bool adminSetCopiesFailure();
	bool readerBorrowBookSuccess();
	bool readerBorrowBookFailure();
	void saveVectorToFile(const std::string& filename, const std::vector<bool>& data);
	void allTests();
private:
	std::vector<bool> testsValues;
};

