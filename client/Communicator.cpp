#include "Communicator.h"
#include <stdexcept>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sstream>

Communicator::Communicator(const std::string &host, int port) : serverHost(host), serverPort(port) {}

// Correct implementation of formatVectorsAsJson
std::string Communicator::formatVectorsAsJson(const std::vector<std::vector<int>> &vectors) {
    std::ostringstream jsonStream;
    jsonStream << "[";
    for (size_t i = 0; i < vectors.size(); ++i) {
        jsonStream << "[";
        for (size_t j = 0; j < vectors[i].size(); ++j) {
            jsonStream << vectors[i][j];
            if (j < vectors[i].size() - 1) {
                jsonStream << ",";
            }
        }
        jsonStream << "]";
        if (i < vectors.size() - 1) {
            jsonStream << ",";
        }
    }
    jsonStream << "]";
    return jsonStream.str();
}

std::string Communicator::createHttpRequest(const std::string &jsonData, const std::string &host) {
    std::ostringstream requestStream;
    requestStream << "POST / HTTP/1.1\r\n";
    requestStream << "Host: " << host << "\r\n";
    requestStream << "Content-Type: application/json\r\n";
    requestStream << "Content-Length: " << jsonData.size() << "\r\n";
    requestStream << "\r\n";
    requestStream << jsonData;
    return requestStream.str();
}

bool Communicator::authenticateAndSendVectors(const std::string &credentials, const std::vector<std::vector<int>> &vectors) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        throw std::runtime_error("Ошибка создания сокета");
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);

    if (inet_pton(AF_INET, serverHost.c_str(), &serverAddr.sin_addr) <= 0) {
        close(sock);
        throw std::runtime_error("Неверный адрес сервера");
    }

    if (connect(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        close(sock);
        throw std::runtime_error("Ошибка подключения к серверу");
    }

    // Новый формат JSON для аутентификации
    std::string jsonData = "{\"username\":\"user\",\"password\":\"P@ssW0rd\",\"vectors\":" + formatVectorsAsJson(vectors) + "}";
    std::string request = createHttpRequest(jsonData, serverHost);

    if (send(sock, request.c_str(), request.size(), 0) < 0) {
        close(sock);
        throw std::runtime_error("Ошибка отправки данных");
    }

    char buffer[1024] = {0};
    int bytesRead = recv(sock, buffer, sizeof(buffer) - 1, 0);

    close(sock);

    if (bytesRead > 0) {
        std::string response(buffer);
        return response == "success";
    }

    return false;
}
