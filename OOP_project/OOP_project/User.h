#pragma once
#include <string>
#include <iostream>

class User{
public:
	User(std::string login, std::string password, std::string nickname);
	virtual void displayInfo();
	std::string getLogin() const;
	std::string getNick() const;
	std::string getPassword() const;
protected:
	std::string login;
	std::string password;
	std::string nickname;
};

