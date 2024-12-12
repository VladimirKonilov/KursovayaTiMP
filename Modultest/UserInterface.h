#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <string>
#include <iostream>
#include <stdexcept>
#include <cstdlib>  // для getenv
#include <getopt.h> // для парсинга командной строки

class UserInterface {
public:
    std::string serverAddress;  // Сетевой адрес сервера
    int serverPort;             // Порт сервера
    std::string inputFile;      // Имя файла с исходными данными
    std::string outputFile;     // Имя файла для сохранения результатов
    std::string configFile;     // Имя файла с LOGIN и PASSWORD

    UserInterface(int argc, char** argv);
    static void printHelp();
    static void handleError(const std::string& message);
};

#endif // USER_INTERFACE_H
