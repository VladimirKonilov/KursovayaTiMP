#include "UserInterface.h"
#include <iostream>

void UserInterface::displayMessage(const std::string &message) {
    std::cout << message << std::endl;
}

void UserInterface::displayError(const std::string &errorMessage) {
    std::cerr << "Ошибка: " << errorMessage << std::endl;
}
