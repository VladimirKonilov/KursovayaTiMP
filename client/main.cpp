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

// Установим статические параметры по умолчанию
const std::string dataType = "int64_t";
const std::string hashType = "MD5";
const std::string saltSide = "server";

// Функция для чтения логина и пароля из конфигурационного файла
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
