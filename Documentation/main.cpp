/**
 * @file main.cpp
 * @brief Основной файл клиентского приложения для взаимодействия с сервером.
 * 
 * Этот файл реализует взаимодействие с сервером, включая аутентификацию, отправку и прием данных,
 * а также запись результатов в файл.
 * 
 * @author Конилов В.В.
 * @date 2024-12-12
 */

#include "UserInterface.h"
#include "Communicator.h"
#include "DataReader.h"
#include "DataWriter.h"
#include <cryptopp/cryptlib.h>
#include <cryptopp/hex.h>
#include <cryptopp/osrng.h>
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <cryptopp/md5.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <iterator>
#include <sstream>
#include <cstring>   // Для std::memcpy

/// Статические параметры по умолчанию
const std::string dataType = "int64_t"; /**< Тип данных для обработки */
const std::string hashType = "MD5";     /**< Алгоритм хеширования */
const std::string saltSide = "server";  /**< Сторона, которая будет генерировать соль */

/**
 * @brief Функция для чтения логина и пароля из конфигурационного файла.
 * 
 * Эта функция открывает файл конфигурации и считывает логин и пароль из первых двух строк.
 * Если файл не может быть открыт или данные некорректны, будет выброшено исключение.
 * 
 * @param configFile Путь к конфигурационному файлу.
 * @param login Строка для хранения логина.
 * @param password Строка для хранения пароля.
 * 
 * @throws std::runtime_error Если не удается открыть файл или данные некорректны.
 */
void readLoginPassword(const std::string& configFile, std::string& login, std::string& password) {
    std::ifstream config(configFile);
    if (!config) {
        throw std::runtime_error("Failed to open config file: " + configFile);
    }

    std::getline(config, login);
    std::getline(config, password);

    if (login.empty() || password.empty()) {
        throw std::runtime_error("Invalid login or password in config file.");
    }
}

/**
 * @brief Аутентификация клиента с сервером.
 * 
 * Клиент отправляет логин на сервер, получает соль, объединяет её с паролем, 
 * затем вычисляет хеш и отправляет его на сервер. Если сервер не подтверждает 
 * успешную аутентификацию, выбрасывается исключение.
 * 
 * @param comm Экземпляр класса Communicator для отправки и получения сообщений.
 * @param password Пароль, который будет использоваться для аутентификации.
 * @param hash Алгоритм хеширования, который используется для вычисления хеша.
 * 
 * @throws std::runtime_error Если аутентификация не удалась.
 */
void authenticateAsClient(Communicator& comm, const std::string& password, CryptoPP::HashTransformation& hash) {
    std::string username = "user";
    comm.sendMessage(username);

    std::string salt(16, '\0');
    comm.receiveMessage(salt.data(), 16);

    std::string combined = salt + password;

    std::string calculatedHash;
    CryptoPP::StringSource(
        combined, true,
        new CryptoPP::HashFilter(hash,
                                 new CryptoPP::HexEncoder(
                                     new CryptoPP::StringSink(calculatedHash))));

    comm.sendMessage(calculatedHash);

    char response[2];
    comm.receiveMessage(response, sizeof(response));
    if (std::string(response, 2) != "OK") {
        throw std::runtime_error("Authentication failed");
    }
}

/**
 * @brief Чтение данных из входного файла.
 * 
 * Функция читает входной файл и преобразует его содержимое в вектор векторов.
 * Каждый элемент файла представляет собой строку чисел, которые затем
 * разбиваются на векторы типа int64_t.
 * 
 * @param inputFile Путь к входному файлу.
 * @return Вектор векторов чисел.
 * 
 * @throws std::runtime_error Если файл не удается открыть.
 */
std::vector<std::vector<int64_t>> readInputFile(const std::string& inputFile) {
    std::ifstream file(inputFile);
    if (!file) {
        throw std::runtime_error("Failed to open input file: " + inputFile);
    }

    std::vector<std::vector<int64_t>> vectors;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        vectors.emplace_back((std::istream_iterator<int64_t>(iss)), std::istream_iterator<int64_t>());
    }

    return vectors;
}

/**
 * @brief Запись результатов в выходной файл.
 * 
 * Эта функция записывает результаты в бинарном формате в файл. 
 * Сначала записывается количество результатов, затем сами результаты.
 * 
 * @param outputFile Путь к выходному файлу.
 * @param results Вектор с результатами.
 * 
 * @throws std::runtime_error Если файл не удается открыть.
 */
void writeResults(const std::string& outputFile, const std::vector<int64_t>& results) {
    std::ofstream file(outputFile, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open output file: " + outputFile);
    }

    uint32_t numResults = results.size();
    file.write(reinterpret_cast<const char*>(&numResults), sizeof(numResults));

    for (const auto& result : results) {
        file.write(reinterpret_cast<const char*>(&result), sizeof(result));
    }
}

/**
 * @brief Основная функция программы.
 * 
 * Главная функция программы, которая выполняет следующие действия:
 * 1. Считывает параметры командной строки.
 * 2. Устанавливает соединение с сервером.
 * 3. Выполняет аутентификацию с сервером.
 * 4. Читает данные из входного файла и отправляет их на сервер.
 * 5. Получает результаты от сервера и записывает их в выходной файл.
 * 
 * @param argc Количество аргументов командной строки.
 * @param argv Массив аргументов командной строки.
 * @return Возвращает 0 в случае успешного завершения, 1 при ошибке.
 */
int main(int argc, char** argv) {
    try {
        // Чтение параметров командной строки
        if (argc < 2) {
            std::cerr << "Error: Missing required parameters.\n";
            UserInterface::printHelp();
            return 1;
        }

        // Чтение параметров из командной строки
        UserInterface ui(argc, argv);
        Communicator comm(ui.serverAddress, ui.serverPort);

        comm.connectToServer();

        // Чтение логина и пароля из файла конфигурации
        std::string login, password;
        readLoginPassword(ui.configFile, login, password);

        // Аутентификация
        CryptoPP::Weak::MD5 md5Hash;
        authenticateAsClient(comm, password, md5Hash);

        // Чтение данных из файла
        auto vectors = readInputFile(ui.inputFile);
        std::vector<int64_t> results;

        uint32_t numVectors = vectors.size();
        comm.sendMessage(reinterpret_cast<const char*>(&numVectors), sizeof(numVectors));

        for (const auto& vec : vectors) {
            uint32_t vectorSize = vec.size();
            comm.sendMessage(reinterpret_cast<const char*>(&vectorSize), sizeof(vectorSize));
            comm.sendMessage(reinterpret_cast<const char*>(vec.data()), vec.size() * sizeof(int64_t));

            int64_t result;
            comm.receiveMessage(reinterpret_cast<char*>(&result), sizeof(result));
            results.push_back(result);
            std::cout << "Received result: " << result << std::endl;
        }

        // Запись результатов в файл
        writeResults(ui.outputFile, results);

    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
