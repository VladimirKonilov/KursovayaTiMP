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
#include <stdexcept>
#include <cstring>  // для memcpy

// Функция аутентификации клиента
void authenticateAsClient(Communicator& comm, const std::string& password, CryptoPP::HashTransformation& hash) {
    std::string username = "user"; // Имя пользователя
    comm.sendMessage(username);    // Отправка имени пользователя на сервер

    // Получение соли от сервера
    std::string salt = comm.receiveMessage();
    std::string combined = salt + password; // Конкатенация соли и пароля

    // Вычисление хеша
    std::string calculatedHash;
    CryptoPP::StringSource(
        combined, true,
        new CryptoPP::HashFilter(hash,
                                 new CryptoPP::HexEncoder(
                                     new CryptoPP::StringSink(calculatedHash))));

    // Отправка хеша серверу
    comm.sendMessage(calculatedHash);

    // Проверка ответа от сервера
    std::string response = comm.receiveMessage();
    if (response != "OK") {
        throw std::runtime_error("Authentication failed");
    }
}

// Функция для отправки векторов серверу
void sendVectors(Communicator& comm, const std::vector<std::vector<int64_t>>& vectors) {
    // Шаг 6: Отправка количества векторов
    uint32_t numVectors = vectors.size();
    std::string numVectorsStr(reinterpret_cast<const char*>(&numVectors), sizeof(numVectors));
    comm.sendMessage(numVectorsStr);
    std::cout << "Sent number of vectors: " << numVectors << std::endl;

    // Шаги 7-9: Отправка данных для каждого вектора
    for (size_t i = 0; i < vectors.size(); ++i) {
        const auto& vec = vectors[i];

        // Проверка размера вектора
        if (vec.size() > 1000) { // Ограничение на размер вектора
            throw std::runtime_error("Vector size exceeds the maximum allowed size.");
        }

        // Шаг 7: Отправка размера вектора
        uint32_t vectorSize = vec.size();
        std::string vectorSizeStr(reinterpret_cast<const char*>(&vectorSize), sizeof(vectorSize));
        comm.sendMessage(vectorSizeStr);
        std::cout << "Sent vector " << i << " size: " << vectorSize << std::endl;

        // Шаг 8: Отправка данных вектора в двоичном формате
        std::string vectorData(reinterpret_cast<const char*>(vec.data()), vec.size() * sizeof(int64_t));
        comm.sendMessage(vectorData);
        std::cout << "Sent vector " << i << " data." << std::endl;

        // Шаг 9: Получение результата от сервера
        std::string resultStr = comm.receiveMessage(sizeof(int64_t));
        int64_t result;
        std::memcpy(&result, resultStr.data(), sizeof(result));
        std::cout << "Received result for vector " << i << ": " << result << std::endl;
    }
}

int main(int argc, char** argv) {
    try {
        // Проверка наличия параметров командной строки
        if (argc == 1) {
            std::cerr << "Error: Missing required parameters.\n";
            UserInterface::printHelp();
            return 1;
        }

        // Инициализация пользовательского интерфейса и параметров
        UserInterface ui(argc, argv);
        Communicator comm("127.0.0.1", 33333);

        // Установка соединения с сервером
        comm.connectToServer();

        // Выбор алгоритма хеширования
        if (ui.hash == "MD5") {
            CryptoPP::Weak::MD5 md5Hash;
            authenticateAsClient(comm, "P@ssW0rd", md5Hash);
        } else {
            throw std::runtime_error("Unsupported hash type in client");
        }

        // Создание и отправка векторов серверу
        std::vector<std::vector<int64_t>> vectors = {
            {1, 2, 3},
            {4, 5, 6, 7},
            {8, 9}
        };
        sendVectors(comm, vectors);

        std::cout << "All vectors processed successfully!" << std::endl;

    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
