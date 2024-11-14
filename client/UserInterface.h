#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <string>

class UserInterface {
public:
    void displayMessage(const std::string &message);
    void displayError(const std::string &errorMessage);
};

#endif // USERINTERFACE_H
