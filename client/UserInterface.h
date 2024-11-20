#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <string>
#include <stdexcept>
#include <iostream>
#include <unistd.h>

class UserInterface {
public:
    std::string type;
    std::string hash;
    std::string side;

    UserInterface(int argc, char** argv);
    static void printHelp();

private:
    void validate();
};

#endif // USER_INTERFACE_H
