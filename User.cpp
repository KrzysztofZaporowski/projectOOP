#include "User.h"

// Class constructor
User::User(std::string login, std::string password, std::string nickname) : login(login), password(password), nickname(nickname) {}

// Metothod to display basic info
void User::displayInfo() {
	std::cout << "Login: " << login << ", nickname: " << nickname << std::endl;
}

// Getters for login, nickname and password
std::string User::getLogin() const {
	return login;
}

std::string User::getNick() const {
	return nickname;
}

std::string User::getPassword() const {
	return password;
}