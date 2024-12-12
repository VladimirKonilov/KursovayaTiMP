/**
 * @file UserInterface.cpp
 * @brief Реализация класса UserInterface, отвечающего за работу с командной строкой.
 * 
 * Этот файл содержит реализацию класса UserInterface, который анализирует параметры командной строки,
 * обрабатывает ошибки и отображает справочную информацию.
 * 
 * @author Конилов В.В.
 * @date 2024-12-12
 */

#include "UserInterface.h"
#include <iostream>
#include <cstdlib>  // Для std::exit
#include <getopt.h> // Для getopt

/**
 * @class UserInterface
 * @brief Класс, который обрабатывает параметры командной строки для клиента.
 * 
 * Класс UserInterface анализирует переданные в командной строке аргументы, такие как адрес сервера, 
 * порт, входной и выходной файлы, а также путь к конфигурационному файлу. Также предусмотрены методы 
 * для вывода справки и обработки ошибок.
 */

/**
 * @brief Конструктор класса UserInterface.
 * 
 * Этот конструктор обрабатывает параметры командной строки и инициализирует поля класса. Он использует 
 * функцию getopt для парсинга аргументов командной строки и сохраняет значения в соответствующие переменные.
 * В случае возникновения ошибки (например, если обязательные параметры не заданы), вызывается handleError.
 * 
 * @param argc Количество аргументов командной строки.
 * @param argv Массив строк с аргументами командной строки.
 * 
 * @throws std::exit(1) Если обязательные параметры отсутствуют или присутствуют некорректные аргументы.
 */
UserInterface::UserInterface(int argc, char** argv) : serverPort(33333), configFile("~/.config/vclient.conf") {
    int opt;
    while ((opt = getopt(argc, argv, "a:p:i:o:c:h")) != -1) {
        switch (opt) {
            case 'a':
                serverAddress = optarg;
                break;
            case 'p':
                serverPort = std::stoi(optarg);
                break;
            case 'i':
                inputFile = optarg;
                break;
            case 'o':
                outputFile = optarg;
                break;
            case 'c':
                configFile = optarg;
                break;
            case 'h':
                printHelp();
                std::exit(0);
            default:
                handleError("Invalid option provided.");
                break;
        }
    }

    if (serverAddress.empty() || inputFile.empty() || outputFile.empty()) {
        handleError("Missing required parameters.");
    }
}

/**
 * @brief Выводит справку по использованию программы.
 * 
 * Этот метод выводит информацию о том, как использовать программу, с описанием всех доступных опций командной строки.
 */
void UserInterface::printHelp() {
    std::cout << "Usage: client -a <server_address> -p <server_
