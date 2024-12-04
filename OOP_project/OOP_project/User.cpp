#include "User.h"

User::User(std::string login, std::string password, std::string nickname) : login(login), password(password), nickname(nickname){}

void User::displayInfo(){
	std::cout << "Login: " << login << ", nickname: " << nickname << std::endl;
}

std::string User::getLogin() const{
	return login;
}

std::string User::getNick() const{
	return nickname;
}

std::string User::getPassword() const{
	return password;
}
