#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <array>
#include <random>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>

// Класс для чтения данных из бинарного файла
class DataReader {
public:
    std::vector<std::vector<int64_t>> readDataFromFile(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        std::vector<std::vector<int64_t>> vectors;

        if (!file) {
            std::cerr << "Ошибка открытия файла: " << filename << std::endl;
            return vectors;
        }

        while (!file.eof()) {
            uint32_t vectorSize;
            file.read(reinterpret_cast<char*>(&vectorSize), sizeof(vectorSize));

            std::vector<int64_t> vec(vectorSize);
            for (uint32_t i = 0; i < vectorSize; ++i) {
                file.read(reinterpret_cast<char*>(&vec[i]), sizeof(int64_t));
            }

            if (!file.fail()) {  // Добавляем только если данные корректно считаны
                vectors.push_back(vec);
            }
        }

        return vectors;
    }
};

// Класс для записи данных в файл
class DataWriter {
public:
    void writeDataToFile(const std::string& filename, const std::vector<std::vector<int64_t>>& data) {
        std::ofstream file(filename, std::ios::binary);
        if (!file) {
            std::cerr << "Ошибка открытия файла для записи: " << filename << std::endl;
            return;
        }

        for (const auto& vec : data) {
            uint32_t size = vec.size();
            file.write(reinterpret_cast<const char*>(&size), sizeof(size));

            for (int64_t value : vec) {
                file.write(reinterpret_cast<const char*>(&value), sizeof(value));
            }
        }
    }
};

// Класс для взаимодействия с сервером
class Communicator {
public:
    void sendDataToServer(const std::vector<std::vector<int64_t>>& data) {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            std::cerr << "Ошибка создания сокета" << std::endl;
            return;
        }

        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(12345); // Порт сервера
        inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr); // IP сервера

        if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            std::cerr << "Ошибка подключения к серверу" << std::endl;
            close(sock);
            return;
        }

        // Получаем соль от сервера
        uint8_t salt[16];
        ssize_t bytesReceived = recv(sock, salt, sizeof(salt), 0);
        if (bytesReceived <= 0) {
            std::cerr << "Ошибка получения соли от сервера" << std::endl;
            close(sock);
            return;
        }
        std::cout << "Получена соль от сервера." << std::endl;

        // Отправляем данные с использованием соли
        for (const auto& vec : data) {
            uint32_t size = vec.size();
            send(sock, &size, sizeof(size), 0); // Отправляем размер вектора

            for (int64_t value : vec) {
                int64_t saltedValue = value ^ *reinterpret_cast<int64_t*>(salt); // Применяем соль
                send(sock, &saltedValue, sizeof(saltedValue), 0); // Отправляем элементы вектора
            }
        }

        close(sock);
    }
};

// Класс для взаимодействия с пользователем
class UserInterface {
public:
    std::string getInputFilename() {
        std::string filename;
        std::cout << "Введите имя файла для чтения данных: ";
        std::cin >> filename;
        return filename;
    }

    std::string getOutputFilename() {
        std::string filename;
        std::cout << "Введите имя файла для сохранения данных: ";
        std::cin >> filename;
        return filename;
    }

    void displayMessage(const std::string& message) {
        std::cout << message << std::endl;
    }
};

// Функция для генерации соли на сервере
std::array<uint8_t, 16> generateSalt() {
    std::array<uint8_t, 16> salt;
    std::random_device rd;
    std::uniform_int_distribution<uint8_t> dist(0, 255);

    for (auto& byte : salt) {
        byte = dist(rd); // Заполняем соль случайными байтами
    }

    return salt;
}

// Функция для обработки клиента на сервере
void handleClient(int clientSocket) {
    // Генерируем соль
    auto salt = generateSalt();
    send(clientSocket, salt.data(), salt.size(), 0); // Отправляем соль клиенту
    std::cout << "Соль отправлена клиенту." << std::endl;

    // Закрытие соединения
    close(clientSocket);
}

// Главная функция сервера
void startServer() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Ошибка создания сокета сервера" << std::endl;
        return;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345); // Порт сервера
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Принимаем соединения от любого адреса

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Ошибка привязки сокета" << std::endl;
        return;
    }

    listen(serverSocket, 5);
    std::cout << "Ожидание подключения клиента..." << std::endl;

    while (true) {
        sockaddr_in clientAddr;
        socklen_t clientAddrSize = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);

        if (clientSocket >= 0) {
            std::cout << "Клиент подключен" << std::endl;
            handleClient(clientSocket);
        } else {
            std::cerr << "Ошибка подключения клиента" << std::endl;
        }
    }

    close(serverSocket);
}

// Главная функция для запуска клиента
void runClient() {
    UserInterface ui;
    DataReader dataReader;
    DataWriter dataWriter;
    Communicator communicator;

    // 1. Получаем имя входного файла от пользователя
    std::string inputFilename = ui.getInputFilename();
    std::vector<std::vector<int64_t>> data = dataReader.readDataFromFile(inputFilename);

    if (data.empty()) {
        ui.displayMessage("Ошибка: данные не были прочитаны.");
        return;
    }

    // 2. Отправляем данные на сервер
    communicator.sendDataToServer(data);

    // 3. Сохраняем данные
    std::string outputFilename = ui.getOutputFilename();
    dataWriter.writeDataToFile(outputFilename, data);

    ui.displayMessage("Данные успешно обработаны и сохранены.");
}

// Главная функция
int main() {
    // Создаем поток для сервера
    std::thread serverThread(startServer);

    // Запускаем клиент в основном потоке
    runClient();

    // Дожидаемся завершения работы сервера
    serverThread.join();

    return 0;
}
