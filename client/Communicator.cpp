#include "Communicator.h"

Communicator::Communicator(const std::string& serverAddress, int serverPort)
    : socketFd(-1), serverAddress(serverAddress), serverPort(serverPort) {}

Communicator::~Communicator() {
    if (socketFd != -1) {
        close(socketFd);
    }
}

void Communicator::connectToServer() {
    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd == -1) {
        throw std::runtime_error("Failed to create socket");
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    if (inet_pton(AF_INET, serverAddress.c_str(), &serverAddr.sin_addr) <= 0) {
        throw std::runtime_error("Invalid server address");
    }

    if (connect(socketFd, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1) {
        throw std::runtime_error("Failed to connect to server");
    }
}

void Communicator::sendMessage(const std::string& message) {
    sendMessage(message.c_str(), message.size());
}

void Communicator::sendMessage(const char* data, size_t size) {
    if (send(socketFd, data, size, 0) == -1) {
        throw std::runtime_error("Failed to send data");
    }
}

std::string Communicator::receiveMessage(size_t bufferSize) {
    std::string buffer(bufferSize, '\0');
    ssize_t bytesRead = recv(socketFd, buffer.data(), bufferSize, 0);
    if (bytesRead == -1) {
        throw std::runtime_error("Failed to receive data");
    }
    buffer.resize(bytesRead);
    return buffer;
}

void Communicator::receiveMessage(char* buffer, size_t size) {
    ssize_t bytesRead = recv(socketFd, buffer, size, 0);
    if (bytesRead != static_cast<ssize_t>(size)) {
        throw std::runtime_error("Failed to receive the expected amount of data");
    }
}
